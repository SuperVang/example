#ifndef ALLOC_CONTAINER_H
#define ALLOC_CONTAINER_H

#include "AllocType.h"

// -------------------------------------------------------------------------
// default_alloc

#if defined(USE_AUTO_ALLOC)
typedef AutoAlloc default_alloc;
#else
typedef ScopedAlloc default_alloc;
#endif

// -------------------------------------------------------------------------
// function swap_object

inline void swap(void *a, void *b, size_t cb)
{
    void *t = alloca(cb);
    memcpy(t, a, cb);
    memcpy(a, b, cb);
    memcpy(b, t, cb);
}

template <class Type>
void swap_object(Type *a, Type *b)
{
    swap(a, b, sizeof(Type));
}

// -------------------------------------------------------------------------
// class defragment

template <class ContainerT>
class defragment : private ContainerT::alloc_type,
                   public ContainerT
{
private:
    typedef typename ContainerT::alloc_type AllocT;

public:
    AllocT &get_alloc()
    {
        return *static_cast<AllocT *>(this);
    }

    void swap(defragment &o)
    {
        swap_object(this, &o);
    }

    void defrag()
    {
        AllocT alloc2;
        AllocT::swap(alloc2);

        ContainerT data2(get_alloc());
        data2.copy(*static_cast<const ContainerT *>(this));
        ContainerT::swap(data2);
    }

public:
    defragment()
        : ContainerT(get_alloc())
    {
    }
};

// -------------------------------------------------------------------------
// class stl_allocator

template <class Type, class AllocT = default_alloc>
class StlAlloc
{
private:
    AllocT *m_alloc;

public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Type *pointer;
    typedef const Type *const_pointer;
    typedef Type &reference;
    typedef const Type &const_reference;
    typedef Type value_type;

    template <class U>
    struct rebind
    {
        typedef StlAlloc<U, AllocT> other;
    };

public:
    pointer address(reference val) const
    {
        return &val;
    }
    const_pointer address(const_reference val) const
    {
        return &val;
    }

    size_type max_size() const
    {
        size_type count = (size_type)(-1) / sizeof(Type);
        return (0 < count ? count : 1);
    }

public:
    StlAlloc(AllocT &alloc) : m_alloc(&alloc) {}

    template <class U>
    StlAlloc(const StlAlloc<U, AllocT> &o) : m_alloc(&o.get_alloc()) {}

    pointer allocate(size_type count, const void * = NULL)
    {
        return (pointer)m_alloc->allocate(count * sizeof(Type));
    }
    void deallocate(void *p, size_type cb)
    {
        m_alloc->deallocate(p, cb);
    }
    void construct(pointer p, const Type &val)
    {
        new (p) Type(val);
    }
    void destroy(pointer p)
    {
        p->~Type();
    }

public:
    char *_Charalloc(size_type cb)
    {
        return (char *)m_alloc->allocate(cb);
    }

public:
    typedef AllocT alloc_type;

    AllocT &get_alloc() const
    {
        return *m_alloc;
    }

    void swap(StlAlloc &o)
    {
        std::swap(m_alloc, o.m_alloc);
    }
};

template <class AllocT>
class StlAlloc<void, AllocT>
{
public:
    typedef void value_type;
    typedef void *pointer;
    typedef const void *const_pointer;

    template <class U>
    struct rebind
    {
        typedef StlAlloc<U, AllocT> other;
    };
};

template <class Type, class AllocT>
inline bool operator==(const StlAlloc<Type, AllocT> &, const StlAlloc<Type, AllocT> &)
{
    return true;
}

template <class Type, class AllocT>
inline bool operator!=(const StlAlloc<Type, AllocT> &, const StlAlloc<Type, AllocT> &)
{
    return false;
}

// -------------------------------------------------------------------------
// std::swap

namespace std
{

template <class Type, class AllocT>
inline void swap(StlAlloc<Type, AllocT> &a, StlAlloc<Type, AllocT> &b)
{
    a.swap(b);
}

} // namespace std

// -------------------------------------------------------------------------

// #define NO_CONSTRUCTOR(Type) MEMORY_NO_CONSTRUCTOR_(Type)
// #define NO_DESTRUCTOR(Type) MEMORY_NO_DESTRUCTOR_(Type)

// #define NEW(alloc, Type) MEMORY_NEW(alloc, Type)
// #define NEW_ARRAY(alloc, Type, count) MEMORY_NEW_ARRAY(alloc, Type, count)

// #define ALLOC(alloc, Type) MEMORY_ALLOC(alloc, Type)
// #define ALLOC_ARRAY(alloc, Type, count) MEMORY_ALLOC_ARRAY(alloc, Type, count)

#endif