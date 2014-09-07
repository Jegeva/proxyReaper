/*
 * proxyReaperProxy.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */

#include "proxyReaperProxy.h"



proxyReaperProxy::proxyReaperProxy() {
	// TODO Auto-generated constructor stub



}

proxyReaperProxy::proxyReaperProxy(string constructorInfo) {
	// TODO Auto-generated constructor stub
	if(constructorInfo.find("://") != string::npos){

		this->setUrl(constructorInfo);

	}else{
		throw(proxyReaperMalformedUrlException(constructorInfo));
	}
}

proxyReaperProxy::proxyReaperProxy(
		string _ipv4,
		string _ipv6,
		int _port,
		string _scheme,
		int _protover,
		string _level,
		time_t _lastcheck,
		time_t _lastworkingcheck,
		long double _last_lag,
		long double _mean_lag,
		int _checknbr,
		int _workingnbr){
	stringstream ss;
	if(_ipv4.length()){
		ss << _scheme << "://" << _ipv4 << ":" << +_port;
	}else{
		ss << _scheme << "://" << _ipv6 << ":" << +_port;
	}
	this->setUrl(ss.str());
	this->anonymity = ANON_TRANSPARENT;
	if( _level.compare(string("ANON_TRANSPARENT"))==0)
		this->anonymity = ANON_TRANSPARENT;
	else if( _level.compare(string("ANON_LOW_ANONYMOUS"))==0)
		this->anonymity= ANON_LOW_ANONYMOUS;
	else if( _level.compare(string("ANON_MED_ANONYMOUS"))==0)
		this->anonymity=ANON_MED_ANONYMOUS;
	else if( _level.compare(string("ANON_HIG_ANONYMOUS"))==0)
		this->anonymity=ANON_HIG_ANONYMOUS;
	else if( _level.compare(string("ANON_TRANSPARENT_KA"))==0)
		this->anonymity=ANON_TRANSPARENT_KA;
	else if( _level.compare(string("ANON_LOW_ANONYMOUS_KA"))==0)
		this->anonymity=ANON_LOW_ANONYMOUS_KA;
	else if( _level.compare(string("ANON_MED_ANONYMOUS_KA"))==0)
		this->anonymity=ANON_MED_ANONYMOUS_KA;
	else if( _level.compare(string("ANON_HIG_ANONYMOUS_KA"))==0)
		this->anonymity=ANON_HIG_ANONYMOUS_KA;

	this->lastcheck = _lastcheck;
	this->lastworkingcheck = _lastworkingcheck;
	this->last_lag = _last_lag;
	this->mean_lag = _mean_lag;
	this->checknumber = _checknbr;
	this->workingchecknumber = _workingnbr;
}

anonymity_level proxyReaperProxy::getAnonymity() const {
	return anonymity;
}

void proxyReaperProxy::setAnonymity(anonymity_level anonymity) {
	this->anonymity = anonymity;
}

int proxyReaperProxy::getChecknumber() const {
	return checknumber;
}

void proxyReaperProxy::setChecknumber(int checknumber) {
	this->checknumber = checknumber;
}

const proxyReaperHost * proxyReaperProxy::getHost() const {
	return this->Url->getHost();
}

long double proxyReaperProxy::getLastLag() const {
	return last_lag;
}

void proxyReaperProxy::setLastLag(long double lastLag) {
	last_lag = lastLag;
}

time_t proxyReaperProxy::getLastcheck() const {
	return lastcheck;
}

void proxyReaperProxy::setLastcheck(time_t lastcheck) {
	this->lastcheck = lastcheck;
}

time_t proxyReaperProxy::getLastworkingcheck() const {
	return lastworkingcheck;
}

void proxyReaperProxy::setLastworkingcheck(time_t lastworkingcheck) {
	this->lastworkingcheck = lastworkingcheck;
}

long double proxyReaperProxy::getMeanLag() const {
	return mean_lag;
}

void proxyReaperProxy::setMeanLag(long double meanLag) {
	mean_lag = meanLag;
}

const string& proxyReaperProxy::getProtocol() const {
	return protocol;
}

void proxyReaperProxy::setProtocol(const string& protocol) {
	this->protocol = protocol;
}

int proxyReaperProxy::getProtocolversion() const {
	return protocolversion;
}

void proxyReaperProxy::setProtocolversion(int protocolversion) {
	this->protocolversion = protocolversion;
}

int proxyReaperProxy::getWorkingchecknumber() const {
	return workingchecknumber;
}

void proxyReaperProxy::setWorkingchecknumber(int workingchecknumber) {
	this->workingchecknumber = workingchecknumber;
}

void proxyReaperProxy::setUrl(string url) {
	try{
	proxyReaperUrl * tmp= new proxyReaperUrl(url);
	if(UrlAcceptedTypesMap.count(tmp->getScheme()) != 0){
		this->Url=tmp;
	}else{
		delete tmp;
		throw(proxyReaperProxyUnsupportedSchemeException(url));
	}}catch(proxyReaperMalformedUrlException e){
		;
	}
}

const proxyReaperUrl * proxyReaperProxy::getUrl() {
	return this->Url;
}

proxyReaperProxy::~proxyReaperProxy() {
	// TODO Auto-generated destructor stub
	delete this->Url;
}

const string proxyReaperProxy::getContentTimed(proxyReaperUrl *url,timeval* tres, int timeout){
	timeval t2,t1;
	stringstream * ss = new stringstream();
	string str;
	curlpp::Cleanup myCleanup;
	curlpp::Easy myRequest;


	myRequest.setOpt(curlpp::options::FollowLocation(1));
	myRequest.setOpt(curlpp::options::MaxRedirs(5));
	myRequest.setOpt(curlpp::options::WriteStream(ss));
	myRequest.setOpt(curlpp::options::Timeout(timeout));
	myRequest.setOpt(curlpp::options::ConnectTimeout(timeout));
	myRequest.setOpt(curlpp::options::Proxy(this->getUrl()->getUrl()));
	myRequest.setOpt(curlpp::options::Url(url->getUrl()));
#define __PROXY_REAPER_USER_AGENT__ "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:24.0) Gecko/20140205 Firefox/24.0 Iceweasel/24.3.0"
#ifdef __PROXY_REAPER_USER_AGENT__
	myRequest.setOpt(curlpp::options::UserAgent(string(__PROXY_REAPER_USER_AGENT__)));
#endif
	if(tres != NULL)
		gettimeofday(&t2,NULL);
	try{
		myRequest.perform();
	}catch(curlpp::LibcurlRuntimeError & e){
		delete ss;
		throw e;
	}
	if(tres != NULL){
		gettimeofday(&t1,NULL);
		timersub(&t1,&t2,tres);
	}
	//cout << ss->str() << endl;
	str = ss->str();
	delete ss;
	return str;
}

const string proxyReaperProxy::getContent(proxyReaperUrl *url){
	return this->getContentTimed(url,NULL,5);
}


ostream& operator<< (ostream & strm, proxyReaperProxy* proxy){
	strm << proxy->getUrl();
	return strm;
}
ostream& operator<< (ostream & strm, proxyReaperProxy proxy){
	strm << proxy.getUrl();
	return strm;
}

map<string,bool> proxyReaperProxy::UrlAcceptedTypesMap = {
		{"http",true},
		{"https",true},
		{"socks4",true},
		{"socks5",true}

};

