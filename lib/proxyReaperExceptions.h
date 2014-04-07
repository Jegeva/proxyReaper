/*
 * proxyReaperExceptions.h
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */
#include <stdexcept>
#include <string>

#ifndef PROXYREAPEREXCEPTIONS_H_
#define PROXYREAPEREXCEPTIONS_H_

using namespace std;

class proxyReaperMalformedIpv4Exception : public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperMalformedIpv4Exception(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperMalformedIpv4Exception() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};


class proxyReaperMalformedIpv6Exception : public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperMalformedIpv6Exception(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperMalformedIpv6Exception() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};


class proxyReaperInvalidHostname : public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperInvalidHostname(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperInvalidHostname() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};


class proxyReaperMalformedIpException: public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperMalformedIpException(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperMalformedIpException() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};

class proxyReaperMalformedUrlException: public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperMalformedUrlException(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperMalformedUrlException() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};

class proxyReaperUrlUnsupportedSchemeException: public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperUrlUnsupportedSchemeException(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperUrlUnsupportedSchemeException() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};

class proxyReaperProxyUnsupportedSchemeException: public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperProxyUnsupportedSchemeException(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperProxyUnsupportedSchemeException() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};


class proxyReaperSQLLite3Exception: public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperSQLLite3Exception(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperSQLLite3Exception() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};


class proxyReaperProxyNeedsPortException : public std::runtime_error {
private:
	std::string what_;
public:
	explicit proxyReaperProxyNeedsPortException(const std::string& error_message)  :
	                std::runtime_error(error_message),what_(error_message){};
	~proxyReaperProxyNeedsPortException() throw(){};
	virtual const char* what() const throw() {return what_.c_str();};
};

#endif /* PROXYREAPEREXCEPTIONS_H_ */
