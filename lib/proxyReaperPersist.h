/*
 * ProxyReaperPersist.h
 *
 *  Created on: Mar 15, 2014
 *      Author: jg
 */

#ifndef PROXYREAPERPERSIST_H_
#define PROXYREAPERPERSIST_H_

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include "proxyReaperLib.h"

using namespace std;

class proxyReaperPersist {
private:
	string dbpath;
	vector<map<string,string>> * query(string);
	sqlite3 * db;
	static proxyReaperPersist* controller;
	string sqllite3Error(int err);

public:
	static proxyReaperPersist* get();
	proxyReaperPersist();
	virtual ~proxyReaperPersist();
	int store( proxyReaperUrl*);
	int store( proxyReaperHost*);
	int store( proxyReaperProxy*);
	int store( proxyReaperSource*);
	int store(vector<proxyReaperProxy*>);
	void clean_incoming();
	int store_incoming(string ipv4,string ipv6,string scheme,int port);
	int store_incoming_coll(vector<vector<string>>);
	void add_incoming_to_proxies();
	void cleanOldNotWorkingAndTransparents();
	vector<proxyReaperProxy*> getProxies();
};



#endif /* PROXYREAPERPERSIST_H_ */
