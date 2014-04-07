/*
 * proxyReaperProxy.h
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */

#ifndef PROXYREAPERPROXY_H_
#define PROXYREAPERPROXY_H_

#include "proxyReaperHost.h"
#include "proxyReaperUrl.h"

enum anonymity_level {
	ANON_TRANSPARENT, //forwarded for
	ANON_LOW_ANONYMOUS, //forwarded
	ANON_MED_ANONYMOUS, // forwarded for other
	ANON_HIG_ANONYMOUS, // :)
	ANON_TRANSPARENT_KA,
	ANON_LOW_ANONYMOUS_KA, //forwarded + keep alive
	ANON_MED_ANONYMOUS_KA, // forwarded for other + keep alive
	ANON_HIG_ANONYMOUS_KA // :) + keep alive
};

using namespace std;

class proxyReaperProxy: public proxyReaperHost {
private:
	static map<string,bool> UrlAcceptedTypesMap;
	proxyReaperUrl * Url;
	string protocol;
	int protocolversion;
	anonymity_level anonymity;
	time_t lastcheck;
	time_t lastworkingcheck;
	long double mean_lag;
	long double last_lag;
	int checknumber;
	int workingchecknumber;

public:

	friend ostream& operator<< (ostream &, proxyReaperProxy*);
	friend ostream& operator<< (ostream &, proxyReaperProxy);
	proxyReaperProxy();
	proxyReaperProxy(string);
	proxyReaperProxy(
			string ipv4,
			string ipv6,
			int port,
			string scheme,
			int protover,
			string level,
			time_t lastcheck,
			time_t lastworkingcheck,
			long double last_lag,
			long double mean_lag,
			int checknbr,
			int workingnbr);
	virtual ~proxyReaperProxy();
	anonymity_level getAnonymity() const;
	void setAnonymity(anonymity_level anonymity);
	int getChecknumber() const;
	void setChecknumber(int checknumber);
	const proxyReaperHost * getHost() const;
	long double getLastLag() const;
	void setLastLag(long double lastLag);
	time_t getLastcheck() const;
	void setLastcheck(time_t lastcheck);
	time_t getLastworkingcheck() const;
	void setLastworkingcheck(time_t lastworkingcheck);
	long double getMeanLag() const;
	void setMeanLag(long double meanLag);
	const string& getProtocol() const;
	void setProtocol(const string& protocol);
	int getProtocolversion() const;
	void setProtocolversion(int protocolversion);
	int getWorkingchecknumber() const;
	void setWorkingchecknumber(int workingchecknumber);
	void setUrl(string);
	const proxyReaperUrl  * getUrl();
	const string getContent(proxyReaperUrl  *);
	const string getContentTimed(proxyReaperUrl  * ,timeval*,int);
};

#endif /* PROXYREAPERPROXY_H_ */
