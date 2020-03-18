#include <iostream>
#include <numeric>

#include <gtest/gtest.h>

TEST(CPP_NUMBER, max_number) {
    std::cout << "Max (uint8_t) = " << std::numeric_limits<uint8_t>::max()
              << std::endl;
    std::cout << "Max (int8_t) = " << std::numeric_limits<int8_t>::max()
              << std::endl;
    std::cout << "Max (uint16_t) = " << std::numeric_limits<uint16_t>::max()
              << std::endl;
    std::cout << "Max (int16_t) = " << std::numeric_limits<int16_t>::max()
              << std::endl;
    std::cout << "Max (uint32_t) = " << std::numeric_limits<uint32_t>::max()
              << std::endl;
    std::cout << "Max (int32_t) = " << std::numeric_limits<int32_t>::max()
              << std::endl;
    std::cout << "Max (uint64_t) = " << std::numeric_limits<uint64_t>::max()
              << std::endl;
    std::cout << "Max (int64_t) = " << std::numeric_limits<int64_t>::max()
              << std::endl;
    std::cout << "Max (float) = " << std::numeric_limits<float>::max()
              << std::endl;
    std::cout << "Max (double) = " << std::numeric_limits<double>::min()
              << std::endl;
}

TEST(CPP_NUMBER, inf_number) {
    std::cout << "Infinity (uint8_t) = "
              << std::numeric_limits<uint8_t>::infinity() << std::endl;
    std::cout << "Infinity (int8_t) = "
              << std::numeric_limits<int8_t>::infinity() << std::endl;
    std::cout << "Infinity (uint16_t) = "
              << std::numeric_limits<uint16_t>::infinity() << std::endl;
    std::cout << "Infinity (int16_t) = "
              << std::numeric_limits<int16_t>::infinity() << std::endl;
    std::cout << "Infinity (uint32_t) = "
              << std::numeric_limits<uint32_t>::infinity() << std::endl;
    std::cout << "Infinity (int32_t) = "
              << std::numeric_limits<int32_t>::infinity() << std::endl;
    std::cout << "Infinity (uint64_t) = "
              << std::numeric_limits<uint64_t>::infinity() << std::endl;
    std::cout << "Infinity (int64_t) = "
              << std::numeric_limits<int64_t>::infinity() << std::endl;
    std::cout << "Infinity (float) = " << std::numeric_limits<float>::infinity()
              << std::endl;
    std::cout << "Infinity (double) = "
              << std::numeric_limits<double>::infinity() << std::endl;
}

TEST(CPP_NUMBER, eps_number) {
    std::cout << "Epsilon (uint8_t) = "
              << std::numeric_limits<uint8_t>::epsilon() << std::endl;
    std::cout << "Epsilon (int8_t) = "
              << std::numeric_limits<int8_t>::epsilon() << std::endl;
    std::cout << "Epsilon (uint16_t) = "
              << std::numeric_limits<uint16_t>::epsilon() << std::endl;
    std::cout << "Epsilon (int16_t) = "
              << std::numeric_limits<int16_t>::epsilon() << std::endl;
    std::cout << "Epsilon (uint32_t) = "
              << std::numeric_limits<uint32_t>::epsilon() << std::endl;
    std::cout << "Epsilon (int32_t) = "
              << std::numeric_limits<int32_t>::epsilon() << std::endl;
    std::cout << "Epsilon (uint64_t) = "
              << std::numeric_limits<uint64_t>::epsilon() << std::endl;
    std::cout << "Epsilon (int64_t) = "
              << std::numeric_limits<int64_t>::epsilon() << std::endl;
    std::cout << "Epsilon (float) = " << std::numeric_limits<float>::epsilon()
              << std::endl;
    std::cout << "Epsilon (double) = "
              << std::numeric_limits<double>::epsilon() << std::endl;
}