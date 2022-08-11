#pragma once

#include <iostream>
#include <string>
#include <ctime>

#define EXCEPCION_ARGS __FILE__, typeid(*this).name(), __LINE__, time(nullptr)

// For ctime() func unsafe warnings (Visual Studio)
#pragma warning(disable : 4996)

class BaseException : public std::exception
{
public:
	BaseException(std::string file, std::string classname, int line, time_t time, std::string msg)
	{
		error_msg = "File name: " + file + "\nException in " + classname + " in line " + std::to_string(line)
			+ "\nTime: " + ctime(&time) + "Info: " + msg + "\n";
	}

	virtual const char* what(void) const noexcept override { return (this->error_msg).c_str(); } 

protected:
	std::string error_msg;
};


class EmptyException : public BaseException
{
public:
	EmptyException(std::string file, std::string classname, int line, time_t time, std::string msg)
		: BaseException(file, classname, line, time, msg) { } ;
	const char* what(void) const noexcept { return this->error_msg.c_str(); }
};

class FileOpenException : public BaseException
{
public:
	FileOpenException(std::string file, std::string classname, int line, time_t time, std::string msg)
		: BaseException(file, classname, line, time, msg) { };
	const char* what(void) const noexcept { return this->error_msg.c_str(); }
};

class FileFormatException : public BaseException
{
public:
	FileFormatException(std::string file, std::string classname, int line, time_t time, std::string msg)
		: BaseException(file, classname, line, time, msg) { };
	const char* what(void) const noexcept { return this->error_msg.c_str(); }
};
