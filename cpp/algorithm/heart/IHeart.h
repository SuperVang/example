#pragma once

#include <Registry.h>


namespace aai_heart {

class Error : public std::runtime_error
{
public:
	template<typename... Args>
	explicit Error(fmt::string_view format, Args&& ... args)
		: std::runtime_error(fmt::format(format, std::forward<Args>(args)...))
	{
	}
};

struct IEvents
{
	virtual ~IEvents() = default;

	virtual std::string onReport(const std::string& query) { return {}; }
	virtual void onInit(const std::string& config) {}
	virtual void onStart() {}
	virtual bool onStep(std::chrono::milliseconds stamp) { return true; }
	virtual void onStop() {}
};

struct IHeart
{
	virtual ~IHeart() = default;

	virtual std::future<void> run(const std::string& address) = 0;
	virtual IRegistry& getRegistry() = 0;
	virtual void shutdown() = 0;
};

std::shared_ptr<IHeart> makeHeart(IEvents& events);

}
