
#include <cereal/archives/xml.hpp>
#include <fstream>

#include <gtest/gtest.h>

struct MyDATA
{
    int x, y;
    double z;

    /**
     * @brief 必须在该类中声明此函数
     */
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(x, y, z);
    }
};

TEST(CEREAL_XML, write_xml)
{

    std::ofstream ofs("data.xml");
    cereal::XMLOutputArchive archive(ofs);

    MyDATA d1;
    d1.x = 3;
    d1.y = 1;
    d1.z = 9.9;
    int men = 5;
    double age = 2;

    archive(CEREAL_NVP(d1),
            men,
            cereal::make_nvp("age", age));
}

TEST(CEREAL_XML, read_xml)
{
    std::ifstream ifs("data.xml");
    if (!ifs.is_open())
        return;

    cereal::XMLInputArchive archive(ifs);

    MyDATA m1;
    int someInt;
    double d;

    archive(m1, someInt, d); // NVPs not strictly necessary when loading
                             // but could be used (even out of order)

    EXPECT_EQ(m1.x, 3);
    EXPECT_EQ(m1.y, 1);
    EXPECT_EQ(m1.z, 9.9);
    EXPECT_EQ(someInt, 5);
    EXPECT_EQ(d, 2);
}