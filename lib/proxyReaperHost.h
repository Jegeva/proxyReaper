/*
 * proxyReaperHost.h
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */
#ifndef PROXYREAPERHOST_H_
#define PROXYREAPERHOST_H_

#include "proxyReaperExceptions.h"

#include <string>
#include <vector>
#include <iostream>
#include  <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unicode/unistr.h>
#include <locale.h>		/* setlocale() */
#include <stringprep.h>		/* stringprep_locale_charset() */
#include <idna.h>		/* idna_to_ascii_lz() */
using namespace std;

//#define __DEBUG

class proxyReaperHost {

private:

	unsigned char Ipv4[4];
	unsigned char Ipv6[16];
	std::string Fqdn;


	bool validateIpv4(string);
	bool validateIpv4(vector<unsigned char>);

	void parseIpv4(string);
	void parseIpv4(vector<unsigned char>);

	bool validateIpv6(string);
	bool validateIpv6(vector<unsigned char>);

	void parseIpv6(string);
	void parseIpv6(vector<unsigned char>);

public:

	friend ostream& operator<< (ostream & strm, proxyReaperHost* & host);
	friend ostream& operator<< (ostream & strm, const proxyReaperHost* & host);
	proxyReaperHost(string);
	proxyReaperHost(vector<unsigned char>);

	proxyReaperHost();
	virtual ~proxyReaperHost();

	const string& getFqdn() const;
	void setFqdn(string);
	const unsigned char* getIpv4() const;
	const unsigned char* getIpv6() const;
	const string getIpv4asString() const;
	const string getIpv6asString() const;
	void setIpv4(string);
	void setIpv6(string);
	void setIpv4(vector<unsigned char>);
	void setIpv6(vector<unsigned char>);
};



#endif /* PROXYREAPERHOST_H_ */
