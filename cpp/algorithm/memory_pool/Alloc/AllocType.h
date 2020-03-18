
#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <iostream>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <algorithm>
#include <memory>
#include <cstring>
#include <malloc.h>
#include <new>

#include "AllocTraits.h"

/* 16k */
const int MemBlockBytes = 16384;
/* must >= sizeof(AllocateHeader) */
const int Padding = 32;
/// 每个内存块的大小
const int MemBlockSize = MemBlockBytes - Padding;
/// 内存块结构中头指针的大小
const int HeaderSize = sizeof(void *);
/// 内存块中去除头指针后的数据块大小
const int BlockSize = MemBlockSize - HeaderSize;

/// 对系统函数malloc和free进行封装
class StdAlloc
{
public:
    void *allocate(size_t cb) { return malloc(cb); }

    void *reallocate(void *p, size_t oldSize, size_t newSize) { return realloc(p, newSize); }

    void deallocate(void *p) { free(p); }
    void deallocate(void *p, size_t) { free(p); }

    void swap(StdAlloc &o) {}

    size_t alloc_size(void *p) { return malloc_usable_size(p); }
};

/// 内存池
class BlockPool
{
private:
#pragma pack(1)
    /// 内存池中闲置的数据块
    struct Block
    {
        Block *next;
    };
#pragma pack()

    /// 指向当前闲置的数据块
    Block *m_freeList;
    /// 当前闲置的数据块的数量
    int m_nFree;
    /// 当前闲置数据块的最大容量
    const int m_nFreeLimit;

    BlockPool(const BlockPool &);
    void operator=(const BlockPool &);

public:
    BlockPool(int cbFreeLimit = INT_MAX)
        : m_freeList(NULL), m_nFree(0),
          m_nFreeLimit(cbFreeLimit / MemBlockSize + 1)
    {
    }

    ~BlockPool()
    {
        clear();
    }

    void *allocate(size_t cb)
    {
        /// 确保申请的容量不小于每个数据块的大小
        assert(cb >= (size_t)MemBlockSize);

        /// 如果申请的容量大于数据块
        if (cb > (size_t)MemBlockSize)
        {
            /// 直接调用系统函数申请一块合适的内存
            return malloc(cb);
        }
        /// 如果申请的容量等于数据块
        else
        {
            /// 如果当前池中有闲置的内存块
            if (m_freeList)
            {
                /// 确保当前闲置内存块的大小合适
                assert(malloc_usable_size(m_freeList) >= cb);
                /// 取下当前闲置内存块节点
                Block *blk = m_freeList;
                m_freeList = blk->next;
                --m_nFree;
                return blk;
            }
            /// 如果当前池中没有闲置的内存块，申请一块合适的内存
            return malloc(MemBlockSize);
        }
    }

    void deallocate(void *p)
    {
        /// 如果当前容量已超过限制
        if (m_nFree >= m_nFreeLimit)
        {
            /// 直接将内存返还给系统
            free(p);
        }
        else
        {
            /// 将该内存块标记为闲置，挂到闲置链表中
            Block *blk = (Block *)p;
            blk->next = m_freeList;
            m_freeList = blk;
            ++m_nFree;
        }
    }

    static size_t alloc_size(void *p)
    {
        return malloc_usable_size(p);
    }

    void clear()
    {
        /// 释放所有闲置内存到系统
        while (m_freeList)
        {
            Block *blk = m_freeList;
            m_freeList = blk->next;
            free(blk);
        }
        m_nFree = 0;
    }
};

/// 内存池的代理类，目的是维护内存池的唯一性
class ProxyAlloc
{
private:
    BlockPool *m_alloc;

public:
    ProxyAlloc(BlockPool &alloc) : m_alloc(&alloc) {}
    ProxyAlloc() : m_alloc(new BlockPool()) {}

    void *allocate(size_t cb) { return m_alloc->allocate(cb); }
    void deallocate(void *p) { m_alloc->deallocate(p); }
    void swap(ProxyAlloc &o) { std::swap(m_alloc, o.m_alloc); }
    size_t alloc_size(void *p) const { return m_alloc->alloc_size(p); }
};

template <class AllocT>
class RegionAlloc
{
private:
#pragma pack(1)

    struct MemBlock;
    friend struct MemBlock;
    /// 内存块
    struct MemBlock
    {
        /// 指向链表中前一个内存块的指针
        MemBlock *pPrev;
        /// 内存块中的实际数据区
        char buffer[BlockSize];
    };
    /// 用于对象的自动析构
    struct DestroyNode
    {
        /// 指向前一个对象的指针
        DestroyNode *pPrev;
        /// 对象的析构函数指针
        destructor_t fnDestroy;
    };
#pragma pack()

    /// 指向当前内存块中未用部分的起点
    char *m_begin;
    /// 指向当前内存块中未用部分的终点
    char *m_end;
    /// 内存构造函数
    AllocT m_alloc;
    ///指向当前对象的析构节点
    DestroyNode *m_destroyChain;

    const RegionAlloc &operator=(const RegionAlloc &);

    /// 寻找当前当前内存块的头地址
    MemBlock *chainHeader_() const
    {
        /// m_begin一直指向当前内存块可用内存的初始位置
        return (MemBlock *)(m_begin - HeaderSize);
    }

    void init_()
    {
        /// 申请一个内存块，并将其前节点置为空
        MemBlock *pNew = (MemBlock *)m_alloc.allocate(sizeof(MemBlock));
        pNew->pPrev = NULL;
        /// 初始化内存起始位置与结束位置
        m_begin = pNew->buffer;
        m_end = (char *)pNew + m_alloc.alloc_size(pNew);
    }

public:
    RegionAlloc() : m_destroyChain(NULL)
    {
        init_();
    }
    explicit RegionAlloc(const AllocT &alloc) : m_alloc(alloc), m_destroyChain(NULL)
    {
        init_();
    }
    explicit RegionAlloc(RegionAlloc &owner)
        : m_alloc(owner.m_alloc), m_destroyChain(NULL)
    {
        init_();
    }

    ~RegionAlloc()
    {
        clear();
    }

    void swap(RegionAlloc &o)
    {
        std::swap(m_begin, o.m_begin);
        std::swap(m_end, o.m_end);
        std::swap(m_destroyChain, o.m_destroyChain);
        m_alloc.swap(o.m_alloc);
    }

    void clear()
    {
        /// 首先调用所有的析构函数
        while (m_destroyChain)
        {
            DestroyNode *curr = m_destroyChain;
            m_destroyChain = m_destroyChain->pPrev;
            curr->fnDestroy(curr + 1);
        }
        /// 销毁所有申请的内存块
        MemBlock *pHeader = chainHeader_();
        while (pHeader)
        {
            MemBlock *curr = pHeader;
            pHeader = pHeader->pPrev;
            m_alloc.deallocate(curr);
        }
        /// 内存的初始和结束位置置空
        m_begin = m_end = (char *)HeaderSize;
    }

    inline void *allocate(size_t cb)
    {
        /// 如果申请的数量小于当前可用的数量
        if ((size_t)(m_end - m_begin) >= cb)
        {
            return m_end -= cb;
        }

        /// 如果申请的数量大于一个内存块的容量
        if (cb >= BlockSize)
        {
            MemBlock *pHeader = chainHeader_();
            /// 申请一片足够的数量的内存
            MemBlock *pNew = (MemBlock *)m_alloc.allocate(HeaderSize + cb);
            /// 如果当前内存块存在，将新的内存块插入到当前内存块的前面
            if (pHeader)
            {
                pNew->pPrev = pHeader->pPrev;
                pHeader->pPrev = pNew;
            }
            /// 如果当前内存块不存在，则将新的内存块当做第一个内存块
            else
            {
                m_end = m_begin = pNew->buffer;
                pNew->pPrev = NULL;
            }
            return pNew->buffer;
        }
        /// 如果申请的数量小于一个内存块的容量
        else
        {
            /// 申请一个内存块，将其插到当前内存块的后面
            MemBlock *pNew = (MemBlock *)m_alloc.allocate(sizeof(MemBlock));
            pNew->pPrev = chainHeader_();
            /// 当前内存块剩下的部分舍弃不用，更新m_begin和m_end让其指向新的内存块
            m_begin = pNew->buffer;
            m_end = (char *)pNew + m_alloc.alloc_size(pNew);
            return m_end -= cb;
        }
    }

    /// 存在析构函数时
    inline void *unmanaged_alloc(size_t cb, destructor_t fn)
    {
        /// 申请一块容纳目标数量及析构节点的内存，析构节点指向此内存起点
        DestroyNode *pNode = (DestroyNode *)allocate(sizeof(DestroyNode) + cb);
        pNode->fnDestroy = fn;
        return pNode + 1;
    }
    /// 存在析构函数时
    inline void manage(void *p, destructor_t fn)
    {
        /// 验证当前节点的析构函数是否正确
        DestroyNode *pNode = (DestroyNode *)p - 1;
        assert(pNode->fnDestroy == fn);

        pNode->pPrev = m_destroyChain;
        m_destroyChain = pNode;
    }
    /// 不存在析构函数时
    inline void *unmanaged_alloc(size_t cb, int fnZero)
    {
        return allocate(cb);
    }
    /// 不存在析构函数时
    inline void manage(void *p, int fnZero)
    {
        // no action
    }

    void *reallocate(void *p, size_t oldSize, size_t newSize)
    {
        if (oldSize >= newSize)
            return p;
        void *p2 = allocate(newSize);
        memcpy(p2, p, oldSize);
        return p2;
    }

    void deallocate(void *p, size_t cb)
    {
        // no action
    }

    template <class Type>
    void destroy(Type *obj)
    {
        // no action
    }

    template <class Type>
    void destroyArray(Type *array, size_t count)
    {
        // no action
    }
};

typedef RegionAlloc<StdAlloc> AutoAlloc;
typedef RegionAlloc<ProxyAlloc> ScopedAlloc;

#endif