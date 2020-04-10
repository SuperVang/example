#include <iostream>
#include <string>

#include <gtest/gtest.h>

/**
 * @brief Construct a new TEST object
 * 
 */
TEST(BASIC_CHAR,chinese_string)
{
    std::string str="但是我拒绝";
    std::cout<<str<<std::endl;
    std::cout<<str.length()<<std::endl;
    for(int i=0;i<str.length();i++)
        std::cout<<str[i]<<std::endl;
}