#include "base.h"


base::base()
{
}


base::~base()
{
}

std::string base::errorString()
{
	return m_errorString;
}

void base::setErrorString(std::string errStr)
{
	m_errorString = errStr;
}

std::string base::message()
{
	std::stringstream ss;
	ss << "��������������" << std::endl;
	return ss.str();
}

