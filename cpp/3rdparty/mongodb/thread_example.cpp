//
// Created by books on 2019/6/26.
//

#include <cstdint>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

#include <gtest/gtest.h>

TEST(MONGODB,thread_pool)
{
	mongocxx::instance &instance=mongocxx::instance::current();
	mongocxx::pool pool{mongocxx::uri{}};
	
	auto threadfunc = [](mongocxx::client& client, std::string dbname) {
		auto col = client[dbname]["col"].insert_one({});
	};
	
	// Great! Using the pool allows the clients to be synchronized while sharing only one
	// background monitoring thread.
	std::thread t1 ([&]() {
		auto c = pool.acquire();
		threadfunc(*c, "db1");
		threadfunc(*c, "db2");
	});
	
	std::thread t2 ([&]() {
		auto c = pool.acquire();
		threadfunc(*c, "db2");
		threadfunc(*c, "db1");
	});
	
	t1.join();
	t2.join();
}