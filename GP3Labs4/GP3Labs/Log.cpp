#include "pch.h"
#include "Log.h"

void Log::Debug(const std::string& msg, const char* file, int line, LevelLog level)
{
	switch (level)
	{
		//Track different log types
		case Error:
			std::cout << "LOG ERROR(" << file << ", " << line << "): " << msg << std::endl;
		break;
			
		case Trace:
			std::cout << "LOG TRACE(" << file << ", " << line << "): " << msg << std::endl;
		break;

		case Warning:
			std::cout << "LOG WARNING(" << file << ", " << line << "): " << msg << std::endl;
		break;
	}

	
}
bool Log::CheckGLError(const char* file, int line)
{
	GLenum err;
	bool errorFound = false;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "GL ERROR(" << file << ", " << line << "): " <<
			glewGetErrorString(err) << std::endl;
		errorFound = true;
	}
	return errorFound;
}