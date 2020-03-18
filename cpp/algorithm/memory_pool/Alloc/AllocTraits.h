#ifndef ALLOC_TRAITS_H
#define ALLOC_TRAITS_H

#include <new>
#include <cstdio>
#include <malloc.h>

// class constructor_traits
template <class Type>
struct constructor_traits
{
    static void construct(void *data)
    {
        new (data) Type;
    }

    static void constructArray(Type *array, size_t count)
    {
        for (size_t i = 0; i < count; ++i)
            new (array + i) Type;
    }
};

#define MEMORY_NO_CONSTRUCTOR_(Type)                  \
    template <>                                       \
    struct constructor_traits<Type>                   \
    {                                                 \
        static void construct(void *data) {}          \
        static void constructArray(Type *, size_t) {} \
    };

// #define MEMORY_NO_CONSTRUCTOR(Type) MEMORY_NO_CONSTRUCTOR_(Type)

// class destructor_traits
/// 析构函数指针，FNDestrutor代表void *(void*)
typedef void FnDestructor(void *data);
typedef FnDestructor *destructor_t;

template <class Type>
struct destructor_traits
{
    enum
    {
        hasDestructor = 1
    };

    typedef destructor_t destructor_type;

    static void destruct(void *data)
    {
        ((Type *)data)->~Type();
    }

    static void destructArray(Type *array, size_t count)
    {
        for (size_t i = 0; i < count; ++i)
            array[i].~Type();
    }
};

template <class Type>
inline void destroyArray(Type *array, size_t count)
{
    destructor_traits<Type>::destructArray(array, count);
}

#define MEMORY_NO_DESTRUCTOR_(Type)                  \
    template <>                                      \
    struct destructor_traits<Type>                   \
    {                                                \
        typedef int destructor_type;                 \
                                                     \
        enum                                         \
        {                                            \
            hasDestructor = 0                        \
        };                                           \
        enum                                         \
        {                                            \
            destruct = 0                             \
        };                                           \
                                                     \
        static void destructArray(Type *, size_t) {} \
    };

// #define MEMORY_NO_DESTRUCTOR(Type) MEMORY_NO_DESTRUCTOR_(Type)

// class array_factory
template <class Type>
struct array_factory_has_destructor
{
    struct destructor_header
    {
        size_t count;
    };

    static size_t alloc_size(size_t count)
    {
        return sizeof(destructor_header) + sizeof(Type) * count;
    }

    static void destruct(void *data)
    {
        destructor_header *hdr = (destructor_header *)data;
        destructor_traits<Type>::destructArray((Type *)(hdr + 1), hdr->count);
    }

    template <class AllocT>
    static Type *create(AllocT &alloc, size_t count)
    {
        destructor_header *hdr =
            (destructor_header *)alloc.unmanaged_alloc(
                sizeof(destructor_header) + sizeof(Type) * count, destruct);
        Type *array = (Type *)hdr + 1;
        hdr->count = count;
        constructor_traits<Type>::constructArray(array, count);
        return (Type *)alloc.manage(array, destruct);
    }

    static char *buffer(Type *array)
    {
        return (char *)array - sizeof(destructor_header);
    }

    static size_t size(Type *array)
    {
        return ((destructor_header *)array - 1)->count;
    }
};

template <class Type>
struct array_factory_no_destructor
{
    enum
    {
        destruct = 0
    };

    typedef void destructor_header;

    static size_t alloc_size(size_t count)
    {
        return sizeof(Type) * count;
    }

    template <class AllocT>
    static Type *create(AllocT &alloc, size_t count)
    {
        Type *array = (Type *)alloc.allocate(sizeof(Type) * count);
        constructor_traits<Type>::constructArray(array, count);
        return array;
    }

    static char *buffer(Type *array)
    {
        return (char *)array;
    }

    static size_t size(Type *array)
    {
        assert(!"Don't call me!!!");
        return 0;
    }
};

template <class Type, int hasDestructor = destructor_traits<Type>::hasDestructor>
struct array_factory : array_factory_has_destructor<Type>
{
};

template <class Type>
struct array_factory<Type, 0> : array_factory_no_destructor<Type>
{
};

// =========================================================================
// C Standard Types Support

#define MEMORY_DECL_CTYPE_(Type)  \
    MEMORY_NO_CONSTRUCTOR_(Type); \
    MEMORY_NO_DESTRUCTOR_(Type)

// #define MEMORY_DECL_CTYPE(Type)  \
//     MEMORY_NO_CONSTRUCTOR(Type); \
//     MEMORY_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

MEMORY_DECL_CTYPE_(bool);
MEMORY_DECL_CTYPE_(float);
MEMORY_DECL_CTYPE_(double);

MEMORY_DECL_CTYPE_(int);
MEMORY_DECL_CTYPE_(unsigned int);

MEMORY_DECL_CTYPE_(char);
MEMORY_DECL_CTYPE_(unsigned char);

MEMORY_DECL_CTYPE_(short);
MEMORY_DECL_CTYPE_(unsigned short);

MEMORY_DECL_CTYPE_(long);
MEMORY_DECL_CTYPE_(unsigned long);

#pragma pack() // default pack

class _unmanaged
{
public:
    template <class Type>
    inline Type *operator->*(Type *p) const
    {
        return p;
    }
};

template <class AllocT>
class _managed
{
private:
    AllocT &m_alloc;

public:
    explicit _managed(AllocT &alloc) : m_alloc(alloc) {}

    template <class Type>
    inline Type *operator->*(Type *p) const
    {
        m_alloc.manage(p, destructor_traits<Type>::destruct);
        return p;
    }
};

template <class AllocT>
inline _unmanaged _get_managed(AllocT &alloc, int fnZero)
{
    return _unmanaged();
}

template <class AllocT>
inline _managed<AllocT> _get_managed(AllocT &alloc, destructor_t fn)
{
    return _managed<AllocT>(alloc);
}

#define MEMORY_ALLOC(alloc, Type) ((Type *)(alloc).allocate(sizeof(Type)))
#define MEMORY_ALLOC_ARRAY(alloc, Type, count) ((Type *)(alloc).allocate(sizeof(Type) * (count)))

#define MEMORY_NEW_ARRAY(alloc, Type, count) array_factory<Type>::create(alloc, count)
#define MEMORY_DESTRUCTOR(Type) destructor_traits<Type>::destruct

#define MEMORY_NEW(alloc, Type) _get_managed(alloc, MEMORY_DESTRUCTOR(Type))->*::new ((alloc).unmanaged_alloc(sizeof(Type), MEMORY_DESTRUCTOR(Type))) Type

#define MEMORY_UNMANAGED_NEW(alloc, Type) ::new ((alloc).allocate(sizeof(Type))) Type

#endif