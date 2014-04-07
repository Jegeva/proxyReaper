/*
 * proxyReaperListeningServer.h
 *
 *  Created on: Mar 28, 2014
 *      Author: jg
 */

#ifndef PROXYREAPERLISTENINGSERVER_H_
#define PROXYREAPERLISTENINGSERVER_H_

#include "proxyReaperLib.h"

using namespace std ;

class proxyReaperListeningServer {

private:
	static vector<proxyReaperProxy*> registeredProxies;

protected:
	static string publicIPv4;
	static string publicIPv6;
	static int listeningPort;
public:
	string getpublicipv4();
	proxyReaperListeningServer();
	virtual ~proxyReaperListeningServer();
	void measureProxy(proxyReaperProxy*);
};





#endif /* PROXYREAPERLISTENINGSERVER_H_ */
