#pragma once

#ifdef _DEBUG
#define LOG_DEBUG(msg, LevelError) Log::Debug(msg, __FILE__, __LINE__, LevelError)
#define GL_ATTEMPT(func) func; \
	if(Log::CheckGLError(__FILE__, __LINE__)) __debugbreak()
#define CHECK_GL_ERROR() Log::CheckGLError(__FILE__, __LINE__)
#else //shouldn't really be outputting to console if this is a release build
#define LOG_DEBUG(msg)
#define GL_ATTEMPT(func) func
#define CHECK_GL_ERROR()
#endif


class Log
{
private:
	Log();
public:
	//different log levels
	enum LevelLog
	{
		Error,
		Trace,
		Warning,
	};

	static void Debug(const std::string& msg, const char* file, int line, LevelLog);
	static bool CheckGLError(const char* file, int line);
};

