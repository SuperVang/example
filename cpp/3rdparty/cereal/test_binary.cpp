//
// Created by books on 2020/6/8.
//

#include <iostream>
#include <fstream>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

#include <gtest/gtest.h>

#include "MyData.h"

TEST(CEREAL_BINARY,read_write_binary)
{
	std::string file="data.bin";
	{
		std::ofstream ofs(file,std::ios::trunc);
		cereal::BinaryOutputArchive archive(ofs);
		
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
	
	{
		std::ifstream ifs(file);
		if (!ifs.is_open())
			return;
		
		cereal::BinaryInputArchive archive(ifs);
		
		MyDATA m1;
		int someInt=0;
		double d=0;
		
		archive(m1, someInt, d); // NVPs not strictly necessary when loading
		// but could be used (even out of order)
		
		EXPECT_EQ(m1.x, 3);
		EXPECT_EQ(m1.y, 1);
		EXPECT_EQ(m1.z, 9.9);
		EXPECT_EQ(someInt, 5);
		EXPECT_EQ(d, 2);
	}
}


TEST(CEREAL_BINARY,read_write_binary_append)
{
	std::string file="recurse_data.bin";
	{
		std::ofstream ofs(file,std::ios::trunc);
		cereal::BinaryOutputArchive archive(ofs);
		
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
	
	{
		std::ofstream ofs(file,std::ios::app);
		cereal::BinaryOutputArchive archive(ofs);
		
		MyDATA d1;
		d1.x = 4;
		d1.y = 2;
		d1.z = 10.9;
		int men = 6;
		double age = 3;
		
		archive(CEREAL_NVP(d1),
				men,
				cereal::make_nvp("age", age));
	}
	
	{
		std::ifstream ifs(file);
		if (!ifs.is_open())
			return;
		
		cereal::BinaryInputArchive archive(ifs);
		
		{
			MyDATA m1;
			int someInt=0;
			double d=0;
			
			archive(m1, someInt, d); // NVPs not strictly necessary when loading
			// but could be used (even out of order)
			
			EXPECT_EQ(m1.x, 3);
			EXPECT_EQ(m1.y, 1);
			EXPECT_EQ(m1.z, 9.9);
			EXPECT_EQ(someInt, 5);
			EXPECT_EQ(d, 2);
		}
		
		{
			MyDATA m1;
			int someInt=0;
			double d=0;
			
			archive(m1, someInt, d); // NVPs not strictly necessary when loading
			// but could be used (even out of order)
			
			EXPECT_EQ(m1.x, 4);
			EXPECT_EQ(m1.y, 2);
			EXPECT_EQ(m1.z, 10.9);
			EXPECT_EQ(someInt, 6);
			EXPECT_EQ(d, 3);
		}
		
	}
}


TEST(CEREAL_BINARY,read_write_pointer)
{
	std::string file="data_ptr.bin";
	{
		std::ofstream ofs(file,std::ios::trunc);
		cereal::BinaryOutputArchive archive(ofs);
		
		std::shared_ptr<MyPointer> ptr(new MyPointer(1,2));
		
		archive(CEREAL_NVP(ptr));
	}
	
	{
		std::ifstream ifs(file);
		if (!ifs.is_open())
			return;
		
		cereal::BinaryInputArchive archive(ifs);
		
		std::shared_ptr<MyPointer> ptr;
		archive(ptr);
		
		EXPECT_EQ(ptr->x,1);
		EXPECT_EQ(ptr->y,2);
		
	}
	
}

