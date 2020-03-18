//
// Created by books on 19-10-23.
//

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "fbs/person_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include "flatbuffers/util.h"

TEST(FLAT_BUFFERS, build_blob)
{
    flatbuffers::FlatBufferBuilder builder;
    for (int i = 0; i < 10; i++)
    {
        books::Vec3 pos(i, i, i);
        flatbuffers::Offset<flatbuffers::String> name = builder.CreateString("books");
        books::Color eye = books::Color_Red;
        flatbuffers::Offset<flatbuffers::String> weaponName = builder.CreateString("TuLong");
        flatbuffers::Offset<books::Weapon> weapon = books::CreateWeapon(builder, weaponName, i);

        books::PersonBuilder personBuilder(builder);
        personBuilder.add_age(i * 3);
        personBuilder.add_pos(&pos);
        personBuilder.add_equipped_type(books::Equipment_Weapon);
        personBuilder.add_equipped(weapon.Union());
        personBuilder.add_name(name);
        personBuilder.add_eye(eye);
        flatbuffers::Offset<books::Person> person = personBuilder.Finish();
        builder.Finish(person);
    }
    int len = builder.GetSize();
    std::cout << len << std::endl;

    uint8_t *buffer = builder.GetBufferPointer();

    for (int i = 0; i < 10; i++)
    {
        const books::Person *person = books::GetPerson(buffer);
        EXPECT_EQ(person->age(), i * 3);
    }
}

TEST(FlAT_BUFFERS, dump_blob)
{
    flatbuffers::FlatBufferBuilder builder;

    books::Vec3 pos(1, 1, 1);
    flatbuffers::Offset<flatbuffers::String> name = builder.CreateString("books");
    books::Color eye = books::Color_Red;
    flatbuffers::Offset<flatbuffers::String> weaponName = builder.CreateString("TuLong");
    flatbuffers::Offset<books::Weapon> weapon = books::CreateWeapon(builder, weaponName, 10);

    books::PersonBuilder personBuilder(builder);
    personBuilder.add_age(25);
    personBuilder.add_pos(&pos);
    personBuilder.add_equipped_type(books::Equipment_Weapon);
    personBuilder.add_equipped(weapon.Union());
    personBuilder.add_name(name);
    personBuilder.add_eye(eye);
    flatbuffers::Offset<books::Person> person = personBuilder.Finish();
    builder.Finish(person);

    const uint8_t *buffer = builder.GetBufferPointer();
    int len = builder.GetSize();

    std::ofstream ofs("person.bin", std::ios::binary | std::ios::trunc);
    ofs.write((char *) buffer, len);
    ofs.close();
}

TEST(FLAT_BUFFERS, read_blob)
{
    std::ifstream ifs("person.bin", std::ios::binary);
    ifs.seekg(0, std::ios::end);
    int len = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    char *buffer = new char[len];
    ifs.read(buffer, len);
    ifs.close();

//    EXPECT_TRUE(books::VerifyPersonBuffer());

    const books::Person *person1 = books::GetPerson(buffer);
    std::cout << person1->name()->c_str() << std::endl;
    std::cout << person1->age() << std::endl;
}