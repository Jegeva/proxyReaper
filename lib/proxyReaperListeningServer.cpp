/*
 * proxyReaperListeningServer.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: jg
 */

#include "proxyReaperListeningServer.h"

using namespace std;

string proxyReaperListeningServer::publicIPv4 ;
string proxyReaperListeningServer::publicIPv6 ;
int proxyReaperListeningServer::listeningPort = 65534;

string proxyReaperListeningServer::getpublicipv4(){
		//proxyReaperUrl * ipcheckUrl = new proxyReaperUrl(string("http://checkip.dyndns.org/"));
		//proxyReaperUrl * ipcheckUrl = new proxyReaperUrl(string("http://greynet.eu/div/headers.php"));
	proxyReaperUrl * ipcheckUrl = new proxyReaperUrl(string("http://greynet.eu/div/ipcheck.php"));

	string ipstring = ipcheckUrl->getContent();
	//	ipstring = ipstring.substr(ipstring.find(";")+1,ipstring.find("\n")-2);
		ipstring = ipstring.substr(ipstring.find(": ")+2,ipstring.length()-ipstring.find("</body")-2);
		this ->publicIPv4 = ipstring;
		return this ->publicIPv4 ;
}

proxyReaperListeningServer::proxyReaperListeningServer() {
	// TODO Auto-generated constructor stub
	if(proxyReaperListeningServer::publicIPv4.length() == 0)
	cout << "publicIP:" << proxyReaperListeningServer::getpublicipv4() << endl;

}

proxyReaperListeningServer::~proxyReaperListeningServer() {
	// TODO Auto-generated destructor stub
}

