#ifndef  __LOG_H_
#define __LOG_H_
#include <glog/logging.h>
#include <mutex>
#include <memory>
#include <cstdarg>

class Log
{
public:
	Log(void);
	~Log(void);
 
		int32_t init_glog(const char *argv0, 
				const char * log_file = "./Log/log", 
				int32_t log_warning = google::GLOG_WARNING, 
				int32_t log_info = google::GLOG_INFO);

	void glog_msg(uint32_t log_level, const char *format, ...);
 
public:
	static std::shared_ptr<Log> instance();
 
private:
	static std::shared_ptr<Log> m_pinstance;
	static std::mutex m_mutex;
	char log_buff[2048];
};
 
 
#define LOGINIT(...)	Log::instance()->init_glog(__VA_ARGS__);
#define LOGINFO(...)	Log::instance()->glog_msg(0, __VA_ARGS__);
#define LOGWARNING(...) Log::instance()->glog_msg(1,__VA_ARGS__);
#define LOGERROR(...)	Log::instance()->glog_msg(2, __VA_ARGS__);


#endif
