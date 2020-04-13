#pragma once

#include "IRegistry.h"
#include <fmt/format.h>
#include <fmt/string.h>

namespace aai_heart {

class Error : public std::runtime_error
{
public:
	template<typename... Args>
	explicit Error(const char * format, Args&& ... args)
		: std::runtime_error(fmt::format(format, std::forward<Args>(args)...))
	{
	}
};


/**
 * @brief interface class, used to implement the detail of IHeart service beacuse
 * each function in this class will be called in IHeart service
 */
struct IEvents
{
	virtual ~IEvents() = default;

	virtual std::string onReport(const std::string& query) { return {}; }
	virtual void onInit(const std::string& config) {}
	virtual void onStart() {}
	virtual bool onStep(std::chrono::milliseconds stamp) { return true; }
	virtual void onStop() {}
};

/**
 * @brief interface class, define the public interface if heart service
 * 
 */
struct IHeart
{
	virtual ~IHeart() = default;

	virtual std::future<void> run(const std::string& address) = 0;
	virtual IRegistry& getRegistry() = 0;
	virtual void shutdown() = 0;
};

std::shared_ptr<IHeart> makeHeart(IEvents& events);

}
