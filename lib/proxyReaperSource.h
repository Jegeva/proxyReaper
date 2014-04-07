/*
 * proxyReaperSource.h
 *
 *  Created on: Mar 15, 2014
 *      Author: jg
 */

#ifndef PROXYREAPERSOURCE_H_
#define PROXYREAPERSOURCE_H_
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <istream>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <map>
#include <stdio.h>
#include <vector>
#include "proxyReaperProxy.h"
#include <algorithm>
#include <string>
using namespace std;

class proxyReaperSource {
private:
	string path;
	map<string,string> arguments;
public:
	proxyReaperSource(string,string);
	virtual ~proxyReaperSource();
	vector<proxyReaperProxy*> get_as_proxyvector();
	vector<string> get_as_urlvector();
	vector<vector<string>> get_as_urlfragvector();
};

#endif /* PROXYREAPERSOURCE_H_ */
