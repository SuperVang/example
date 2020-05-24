#include <iostream>
#include <fstream>
#include "SearchRequest.pb.h"
#include <gtest/gtest.h>
#include "Test.pb.h"

TEST(PROTOBUF, simple_io)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    books::SearchRequest searchRequest;
    searchRequest.set_query("books");
    searchRequest.set_page_number(100);
    searchRequest.set_result_per_page(100);
    searchRequest.add_ips(4);
    searchRequest.add_ips(1);

    std::string strQuest;
    searchRequest.SerializeToString(&strQuest);
    std::cout << searchRequest.DebugString() << std::endl;

    std::ofstream ofs("test.txt", std::ios::trunc);
    searchRequest.SerializeToOstream(&ofs);
    ofs.close();

    books::SearchRequest searchRequest2;
    std::ifstream ifs("test.txt");
    // searchRequest2.ParseFromString(strQuest);
    searchRequest2.ParseFromIstream(&ifs);
    std::cout << searchRequest2.query() << std::endl;
    std::cout << searchRequest2.page_number() << std::endl;
    std::cout << searchRequest2.result_per_page() << std::endl;
    std::cout << searchRequest2.ips_size() << std::endl;
    //    google::protobuf::ShutdownProtobufLibrary();
}

TEST(PROTOBUF, repeated_msg)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    books::ResponseMsg respMsg;
    for (int i = 0; i < 10; i++)
    {
        books::Pt *pt = respMsg.add_pts();
        pt->set_x(i * 2);
        pt->set_y(i * 4);
    }
    for(const books::Pt &pt : respMsg.pts())
    {
        std::cout<<pt.x()<<std::endl;
    }
    respMsg.PrintDebugString();
    //    google::protobuf::ShutdownProtobufLibrary();
}

TEST(PROTOBUF, oneof_msg)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    books::ObjList list;
    for (int i = 0; i < 10; i++)
    {
        books::Obj *tmpObj = list.add_objs();
        int id = rand() % 2;
        tmpObj->set_id(id);
        if (rand() % 2 == 0)
        {
            books::ObjA *a = new books::ObjA();
            a->set_x(1.0);
            a->set_y(2.0);
            tmpObj->set_allocated_a(a);
        }
        else
        {
            books::ObjB *b = new books::ObjB();
            b->set_lon(10);
            b->set_lat(20);
            tmpObj->set_allocated_b(b);
        }
    }

    std::string buffer = list.SerializeAsString();

    books::ObjList list2;
    list2.ParseFromString(buffer);
    for (int i = 0; i < list2.objs_size(); i++)
    {
        books::Obj tmpObj = list2.objs(i);
        std::cout << "ID " << tmpObj.id() << std::endl;
        std::cout << "Has Obj A ? " << tmpObj.has_a() << std::endl;
        std::cout << "Has Obj B ? " << tmpObj.has_b() << std::endl;

        if (tmpObj.has_a())
        {
            books::ObjA a = tmpObj.a();
            std::cout << "Has A " << a.x() << " " << a.y() << std::endl;
        }
        else if (tmpObj.has_b())
        {
            books::ObjB b = tmpObj.b();
            std::cout << "Has B " << b.lon() << " " << b.lat() << std::endl;
        }

        std::cout << std::endl;
    }

    // google::protobuf::ShutdownProtobufLibrary();
}

// TEST(PROTOBUF, build_old_data)
// {
//     books::Person person;
//     person.set_name("books");
//     person.set_age(12);

//     std::string filename = "tmp.txt";
//     std::ofstream ofs(filename, std::ios::trunc);
//     person.SerializeToOstream(&ofs);
//     ofs.flush();
//     ofs.close();
// }

TEST(PROTOBUF, load_old_data)
{
    std::string filename = "tmp.txt";
    std::ifstream ifs(filename);

    books::Person person;
    person.ParseFromIstream(&ifs);
    // ifs.close();
    person.set_sex(1);

    person.PrintDebugString();
}