//
// Created by books on 2019/6/26.
//

#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types/value.hpp>

#include <gtest/gtest.h>

TEST(MONGODB,bson)
{
	// { "hello" : "world" }
	bsoncxx::builder::basic::document basic_builder{};
	basic_builder.append(bsoncxx::builder::basic::kvp("hello", "world"));
	basic_builder.append(bsoncxx::builder::basic::kvp("age",1));
	bsoncxx::builder::basic::document recur_builder{};
	recur_builder.append(bsoncxx::builder::basic::kvp("basic",basic_builder));
	bsoncxx::document::value basic_doc = basic_builder.extract();
	bsoncxx::document::value recur_doc=recur_builder.extract();
	std::cout<<bsoncxx::to_json(basic_doc)<<std::endl;
	std::cout<<bsoncxx::to_json(recur_doc)<<std::endl;
	
	bsoncxx::builder::stream::document stream_builder{};
	bsoncxx::document::value stream_doc=stream_builder<<"name"<<"cai"<<bsoncxx::builder::stream::finalize;
	std::cout<<bsoncxx::to_json(stream_doc)<<std::endl;
	
	bsoncxx::document::view stream_view=stream_doc.view();
	for(bsoncxx::document::view::iterator iter=stream_view.begin();iter!=stream_view.end();iter++)
		std::cout<<iter->key()<<std::endl;
}