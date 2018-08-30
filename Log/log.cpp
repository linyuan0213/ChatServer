#include "log.h"

std::shared_ptr<Log> Log::m_pinstance = nullptr;
std::mutex Log::m_mutex;

Log::Log(void)
{
	memset(log_buff, 0, 2048);
}

Log::~Log(void)
{

}


// 初始化日志
int32_t Log::init_glog(const char *argv0, 
		const char * log_file, 
		int32_t log_warning, 
		int32_t log_info)
{
	CHECK_NOTNULL(log_file);

	google::InitGoogleLogging(argv0);

	google::SetLogDestination(log_info, log_file);

	google::SetStderrLogging(log_warning);

	return 0;
}

//glog日志函数调用
void Log::glog_msg(uint32_t log_level, const char *format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsprintf(log_buff, format, arg_ptr);
	switch(log_level)
	{
		case 0:
			DLOG(INFO) << log_buff;
			break;
		case 1:
			DLOG(WARNING) << log_buff;
			break;
		case 2:
			DLOG(ERROR) << log_buff;
			break;
		case 3:
			DLOG(FATAL) << log_buff;
			break;
		default:
			break;
	}		
	va_end(arg_ptr);

	memset(log_buff, 0, 2048);		
}


std::shared_ptr<Log> Log::instance()
{
	if (nullptr == m_pinstance)
	{
		std::lock_guard<std::mutex> lock(m_mutex); 
		m_pinstance = std::make_shared<Log>();
	}
	return m_pinstance;
}

