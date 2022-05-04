#pragma once
/*
���ж�����Ļ���
*/
#include "datastruct.h"
#include <string>
#include <vector>
#include <sstream>
#include <memory>
class base
{
public:
	base();
	virtual ~base();
	std::string errorString();
	void setErrorString(std::string);
	virtual bool init() = 0;
	virtual std::string  message();
private:
	std::string m_errorString;
};

