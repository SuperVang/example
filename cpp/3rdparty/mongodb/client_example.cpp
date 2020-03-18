//
// Created by books on 2019/6/26.
//

#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <gtest/gtest.h>


// This should be done only once.
mongocxx::instance &instance=mongocxx::instance::current();
mongocxx::uri uri("mongodb://localhost:27017");
mongocxx::client client(uri);
//mongocxx::client client{mongocxx::uri{}};

mongocxx::database db = client["mydb"];
mongocxx::collection coll = db["test"];

// void insert_example(mongocxx::collection &coll,bsoncxx::builder::stream::document &doc)
TEST(MONGODB,insert)
{	
	bsoncxx::builder::stream::document doc{};

	//Using the stream builder interface to construct this document
	//Use bsoncxx::builder::stream::finalize to obtain a bsoncxx::document::value instance.
	bsoncxx::document::value doc_value = doc
			<< "name" << "MongoDB"
			<< "type" << "database"
			<< "count" << 1
			<< "versions" << bsoncxx::builder::stream::open_array
			<< "v3.2" << "v3.0" << "v2.6"
			<< bsoncxx::builder::stream::close_array
			<< "info" << bsoncxx::builder::stream::open_document
			<< "x" << 203
			<< "y" << 102
			<< bsoncxx::builder::stream::close_document
			<< bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(doc_value) << std::endl;
	
	// insert a document into a collection
	bsoncxx::document::view view = doc_value.view();
	bsoncxx::document::element element = view["name"];
	if (element.type() != bsoncxx::type::k_utf8)
	{
		// Error
		std::cerr << "Error" << std::endl;
		exit(-1);
	}
	std::string name = element.get_utf8().value.to_string();
	bsoncxx::stdx::optional<mongocxx::result::insert_one> result = coll.insert_one(view);
	
	// insert many documents into a collection
	std::vector<bsoncxx::document::value> documents;
	for (int i = 0; i < 100; i++)
	{
		documents.push_back(
				bsoncxx::builder::stream::document{} << "i" << i << bsoncxx::builder::stream::finalize);
	}
	bsoncxx::stdx::optional<mongocxx::result::insert_many> results = coll.insert_many(documents);
	
}

// void query_example(mongocxx::collection &coll,bsoncxx::builder::stream::document &doc)
TEST(MONGODB,query)
{
	bsoncxx::builder::stream::document doc{};

	// findone() query a instance
	std::cout << "findone() query a instance" << std::endl;
	bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = coll.find_one({});
	std::cout << bsoncxx::to_json(maybe_result->view()) << std::endl;
	
	// find() query a cursor
	std::cout << "find() query a cursor" << std::endl;
	mongocxx::cursor cursor = coll.find({});
	for (bsoncxx::document::view value : cursor)
	{
		std::cout << bsoncxx::to_json(value) << std::endl;
	}
	
	// specify a equal query filter: {i:71}
	std::cout << "specify a equal query filter: " << std::endl;
	bsoncxx::document::value value_equal = doc << "i" << 71 << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(value_equal.view()) << std::endl;
	maybe_result = coll.find_one(value_equal.view());
	if (maybe_result)
		std::cout << bsoncxx::to_json(maybe_result->view()) << std::endl;
	
	// specify a region query filter: {i:{$gt:10,$lt:30}}
	std::cout << "specify a region query filter: " << std::endl;
	bsoncxx::document::value query_region = doc << "i"
												<< bsoncxx::builder::stream::open_document
												<< "$gt" << 10 << "$lt" << 30
												<< bsoncxx::builder::stream::close_document
												<< bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(query_region.view()) << std::endl;
	
	cursor = coll.find(query_region.view());
	for (bsoncxx::document::view value : cursor)
		std::cout << bsoncxx::to_json(value) << std::endl;
}

// void update_example(mongocxx::collection &coll,bsoncxx::builder::stream::document &doc)
TEST(MONGODB,update)
{



	bsoncxx::builder::stream::document doc{};

	// update one document
	std::cout << "query one : " << std::endl;
	bsoncxx::document::value query_one = doc << "i" << 10 << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(query_one) << std::endl;
	std::cout << "update one : " << std::endl;
	bsoncxx::document::value update_one = doc << "$set" << bsoncxx::builder::stream::open_document << "i" << 110
											  << bsoncxx::builder::stream::close_document
											  << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(update_one) << std::endl;
	bsoncxx::stdx::optional<mongocxx::result::update> result_one = coll.update_one(query_one.view(), update_one.view());
	
	// update many document
	std::cout << "query many : " << std::endl;
	bsoncxx::document::value query_many = doc << "i" << bsoncxx::builder::stream::open_document
											  << "$lt" << 100 << bsoncxx::builder::stream::close_document
											  << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(query_many) << std::endl;
	std::cout << "update many : " << std::endl;
	bsoncxx::document::value update_many = doc << "$inc" << bsoncxx::builder::stream::open_document
											   << "i" << 100 << bsoncxx::builder::stream::close_document
											   << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(update_many) << std::endl;
	bsoncxx::stdx::optional<mongocxx::result::update> result_many = coll.update_many(query_many.view(), update_many.view());
}

// void delete_example(mongocxx::collection &coll,bsoncxx::builder::stream::document &doc)
TEST(MONGODB,delete)
{
	bsoncxx::builder::stream::document doc{};

	std::cout << "delete one : " << std::endl;
	bsoncxx::document::value delete_one = doc << "name" << "MongoDB" << bsoncxx::builder::stream::finalize;
	std::cout << bsoncxx::to_json(delete_one) << std::endl;
	bsoncxx::stdx::optional<mongocxx::result::delete_result> result_one = coll.delete_one(delete_one.view());
	
	std::cout << "delete many : " << std::endl;
	bsoncxx::document::value delete_many = doc << "i" << bsoncxx::builder::stream::open_document
											   << "$gte" << 100 << bsoncxx::builder::stream::close_document
											   << bsoncxx::builder::stream::finalize;
	std::cout<<bsoncxx::to_json(delete_many)<<std::endl;
	bsoncxx::stdx::optional<mongocxx::result::delete_result> result_many=coll.delete_many(delete_many.view());
	
}

// void index_example(mongocxx::collection &coll,bsoncxx::builder::stream::document &doc)
TEST(MONGODB,index)
{
	bsoncxx::builder::stream::document doc{};

	std::cout<<"create index : "<<std::endl;
	bsoncxx::document::value index_query=doc<<"i"<<1<<bsoncxx::builder::stream::finalize;
	std::cout<<bsoncxx::to_json(index_query)<<std::endl;
	bsoncxx::document::value index_result=coll.create_index(index_query.view());
	std::cout<<bsoncxx::to_json(index_result)<<std::endl;
}

// int main()
// {
//	insert_example(coll,doc);

//	query_example(coll,doc);
	
//	update_example(coll,doc);

//	delete_example(coll, doc);

	// index_example(coll,doc);

	// return 1;
// }

