//
// Created by books on 2020/5/8.
//

#include <iostream>
#include <memory>

#include <gtest/gtest.h>

namespace AA{
	class A;

	class B{
	public:
		B()=default;
		~B()=default;
		std::shared_ptr<A> a;
	};
}

TEST(CPP_SHARED_PTR,forward_declare)
{
	AA::B b;
}

namespace AA{
	class A{
	public:
		A(){
			std::cout<<"Initilize A"<<std::endl;
		}
		~A(){
			std::cout<<"Release A"<<std::endl;
		}
	};
}