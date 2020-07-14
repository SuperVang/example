#pragma once

#include <iostream>
#include <memory>

namespace UNIQUE
{

    class B;
    class ABase
    {
    public:
        virtual ~ABase() = default;
    };

    class A : public ABase
    {
    public:
        A();
        ~A();
        // A()=default;
        // ~A()=default;

    private:
        // B * m_impl;
        std::unique_ptr<B> m_impl;
        // std::shared_ptr<B> m_impl;
    };
} // namespace UNIQUE
