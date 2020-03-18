//
// Created by books on 19-12-12.
//

#include <iostream>
#include <vector>
#include <string>

#include <boost/any.hpp>

#include <gtest/gtest.h>

TEST(BOOST_ANY, implicit_conversion)
{

    std::vector<boost::any> any_vec;

    boost::any int_con = 10;
    boost::any str_con = "cai";
    boost::any dbl_con = 10.34;

    any_vec.push_back(int_con);
    any_vec.push_back(str_con);
    any_vec.push_back(dbl_con);

    for (const boost::any &any_val : any_vec)
        std::cout << any_val.type().name()<< std::endl;

}