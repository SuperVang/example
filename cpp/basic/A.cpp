
#include "A.h"

namespace UNIQUE
{
    class B
    {
    public:
        B()
        {
            std::cout << "Init B" << std::endl;
        };
        ~B()
        {
            std::cout << "Release B" << std::endl;
        };
    };

    A::A() : m_impl(new B())
    {
        std::cout << "Init A" << std::endl;
    }

    A::~A()
    {
        std::cout << "Release A" << std::endl;
    }

} // namespace UNIQUE
