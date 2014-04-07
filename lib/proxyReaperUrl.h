/*
 * proxyReaperUrl.h
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */
#ifndef PROXYREAPERURL_H_
#define PROXYREAPERURL_H_

#include <string>
#include <map>
#include "proxyReaperHost.h"
#include "proxyReaperExceptions.h"
#include <errno.h>
#include <boost/regex/icu.hpp>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace std;



class proxyReaperUrl {

private:
	static map<string,int> ProtoPortMap;
	string Url;
	string Scheme;
	string User;
	string Password;
	proxyReaperHost* Host;
	int Port=0;
	string path;
	string params;
	string query;
	string Frag;
	string content;
	map<string,string> paramsmap;
	bool validateUrl(string) const;
	bool validateScheme(string) const;
	bool validateUser(string) const;
	bool validatePassword(string) const;
	bool validatePort(unsigned int) const;
	bool validatepath(string) const;
	bool validateparams(string) const;
	bool validatequery(string) const;
	bool validateFrag(string) const;
	void setParamsFromQuery();
	void syncUrl();
public:

	friend ostream& operator<< (ostream & strm, proxyReaperUrl* & host);
	proxyReaperUrl();
	proxyReaperUrl(string);
	virtual ~proxyReaperUrl();
	const string& getFrag() const;
	void setFrag(const string& frag);
	const proxyReaperHost*  getHost() const;
	const string& getParams() ;
	const string& getParam(string param) ;
	void setParam(const string& param,const string& value);
	const string& getPassword() const;
	void setPassword(const string& password);
	const string& getPath() const;
	void setPath(const string& path);
	const int getPort() const;
	void setPort(const unsigned int port);
	const string& getQuery() const;
	void setQuery(const string& query);
	const string& getScheme() const;
	void setScheme(const string& scheme);
	const string& getUrl() const;
	void setUrl(const string& url);
	const string& getUser() const;
	void setUser(const string& user);
	const string getContent();
	const string getContentTimed(timeval*);
	static int getSchemePort(const string& scheme);


};

#endif /* PROXYREAPERURL_H_ */
