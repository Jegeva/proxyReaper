/*
 * proxyReaperUrl.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */

#include "proxyReaperUrl.h"

proxyReaperUrl::proxyReaperUrl() {
	// TODO Auto-generated constructor stub

}

proxyReaperUrl::proxyReaperUrl(string constructorInfo) {
	if(constructorInfo.find("://") != string::npos){
		// should be an url
		this->setUrl(constructorInfo);
	}else{
		throw(proxyReaperMalformedUrlException(constructorInfo));
	}

}

bool proxyReaperUrl::validateScheme(string scheme) const {
	// TODO TEMP DEV
	return proxyReaperUrl::ProtoPortMap.count(scheme);
	// TEMP DEV
}
bool proxyReaperUrl::validateUser(string user) const {
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}
bool proxyReaperUrl::validatePassword(string password) const {
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}

bool proxyReaperUrl::validatePort(unsigned int port) const {
	return (port>=0 && port<=65535)?true:false;
}

bool proxyReaperUrl::validatepath(string path) const {
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}
bool proxyReaperUrl::validateparams(string params) const {
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}
bool proxyReaperUrl::validatequery(string query) const{
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}
bool proxyReaperUrl::validateFrag(string frag) const{
	// TODO TEMP DEV
	return true;
	// TEMP DEV
}

const string& proxyReaperUrl::getFrag() const {
	return this->Frag;
}

void proxyReaperUrl::setFrag(const string& frag) {
	Frag = frag;
}

const proxyReaperHost* proxyReaperUrl::getHost() const {
	return this->Host;
}

const string& proxyReaperUrl::getParam(string param) {
	return this->paramsmap[param];
}

const string& proxyReaperUrl::getParams(){
	stringstream ret;
	for(std::map<string,string>::iterator iter = this->paramsmap.begin(); iter != paramsmap.end();)
	{
		ret <<  iter->first << "=" << iter->second ;
		++iter;
		if(iter != paramsmap.end())
			ret << "&";
	}
	this->params = ret.str();
	return this->params;
}

void proxyReaperUrl::setParam(const string& param,const string& value) {
	this->paramsmap[param]=value;
}

const string& proxyReaperUrl::getPassword() const {
	return this->Password;
}

void proxyReaperUrl::setPassword(const string& password) {
	Password = password;
}

const string& proxyReaperUrl::getPath() const {
	return this->path;
}

void proxyReaperUrl::setPath(const string& path) {
	this->path = path;
}

const int proxyReaperUrl::getPort() const {
	return this->Port;
}

void proxyReaperUrl::setPort(const unsigned int port) {
	if(this->validatePort(port)){
		Port = port;
	}else{

	}
}

const string& proxyReaperUrl::getQuery() const {
	return this->query;

}

void proxyReaperUrl::setParamsFromQuery(){
	vector<string> splitted;
	string tosplit,key,value;
	int pos;
	if(this->query.find_first_of('?') == std::string::npos){
		// no query like toto.php?a=1...
		this->Frag = this->query;
	}else{
		this->paramsmap.clear();
		pos = this->query.find_first_of('?');
		this->Frag = this->query.substr(0,pos);
		tosplit = this->query.substr(pos+1,string::npos);
		boost::split(splitted, tosplit, boost::is_any_of("&")); // split params
		for (vector<string>::iterator it = splitted.begin();it != splitted.end(); ++it) {
			// key = value
			key = it->substr(0,it->find_first_of('='));
			value = it->substr(it->find_first_of('=')+1,string::npos);
			this->paramsmap[key]=value;
		}
	}
}

void proxyReaperUrl::setQuery(const string& query) {

	this->query = query;
	setParamsFromQuery();


}

const string& proxyReaperUrl::getScheme() const {
	return this->Scheme;
}

void proxyReaperUrl::setScheme(const string& scheme) {
	if(this->validateScheme(scheme)){
		this->Scheme = scheme;
	}else{
		throw(proxyReaperMalformedUrlException(scheme));
	}
}

const string& proxyReaperUrl::getUrl() const {
	return this->Url;
}

void proxyReaperUrl::setUrl(const string& url) {
	// Regexp from @diegoperini
	// some mods for capturing and url case sensitivity
	// modded also to support icu in urls
	unsigned int port;
	string upcombo;
	vector<string> splitted;
	string tosplit,key,value;

	UnicodeString validationRegExpStr(
			"^("
			// protocol identifier
			"([^:]+)://"
			// user:pass authentication
			"(?:([^:@]+(?::[^@]*)?)@)?"
			"("
			// IP address exclusion
			// private & local networks
			"(?!(?:10|127)(?:\\.\\d{1,3}){3})"
			"(?!(?:169\\.254|192\\.168)(?:\\.\\d{1,3}){2})"
			"(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})"
			// IP address dotted notation octets
			// excludes loopback network 0.0.0.0
			// excludes reserved space >= 224.0.0.0
			// excludes network & broacast addresses
			// (first & last IP address of each class)
			"(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])"
			"(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}"
			"(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))"
			"|"
			// host name
			"(?:(?:[[:L*:][:N*:]]+-?)*?[[:L*:][:N*:]]+)"
			// domain name
			"(?:\\.(?:[[:L*:][:N*:]]+-?)*?[[:L*:][:N*:]]+)*"
			// TLD identifier
			"(\\.[[:L*:][:N*:]]{2,})"
			")"
			// port number
			"(?::(\\d{2,5}))?"
			// resource path
			"(/[^\\s]*)?"
			")$"

	)	;
	boost::u32regex validationRegExp = boost::make_u32regex(validationRegExpStr);
	boost::cmatch what;
	if(! u32regex_match(url.c_str(),what,validationRegExp)){
		throw(proxyReaperMalformedUrlException(url));
	}else{
		/*
		 *
		 *  std::cout <<  "full :" << std::string(what[1].first, what[1].second)<< std::endl;;
		 *  std::cout <<  "proto:" << std::string(what[2].first, what[2].second)<< std::endl;;
		 *  std::cout <<  "pass :" << std::string(what[3].first, what[3].second)<< std::endl;;
		 *  std::cout <<  "dom  :" << std::string(what[4].first, what[4].second)<< std::endl;;
		 *  std::cout <<  "tld  :" << std::string(what[5].first, what[5].second)<< std::endl;;
		 *  std::cout <<  "port :" << std::string(what[6].first, what[6].second)<< std::endl;;
		 *  std::cout <<  "ress :" << std::string(what[7].first, what[7].second)<< std::endl;;
		 *
		 *  */


		/*
		 * Scheme
		 * */

		this->setScheme ( std::string(what[2].first, what[2].second));

		if(! proxyReaperUrl::ProtoPortMap.count(this->Scheme)){
			throw(proxyReaperUrlUnsupportedSchemeException(this->Scheme));
		}

		/*
		 * User / Password
		 * */
		upcombo = std::string(what[3].first, what[3].second);
		if(upcombo.length()>0){
			unsigned int pos = upcombo.find_first_of(':')-1;
			if (pos != std::string::npos){
				this->User = upcombo.substr(0,pos);
				this->Password = upcombo.substr(pos,std::string::npos);
			}
		}

		/*
		 * Host
		 * */

		this->Host = new proxyReaperHost(std::string(what[4].first, what[4].second));


		/*
		 * Port
		 * */
		if(std::string(what[6].first, what[6].second).length() >0){
			port = strtol(std::string(what[6].first, what[6].second).c_str(),NULL,10);

			if((port == LONG_MIN || port == LONG_MAX) && (errno == ERANGE))
				throw(proxyReaperMalformedUrlException(url));
			if(!validatePort(port)){
				throw(proxyReaperMalformedUrlException(url));
			}
			this->setPort(port);
		}


		/*
		 * Query like : toto.php?a=1&b=2&....
		 * Also sets the fragment and the parameters map
		 *
		 * */


		this->query = std::string(what[7].first, what[7].second);
		this->setParamsFromQuery();
		this->syncUrl();


	}


}

void proxyReaperUrl::syncUrl() {
	stringstream urlstream;
	string temp;
	urlstream << this->Scheme << "://";
	if(this->User.length()>0){
		urlstream << this->User;
	}
	if(this->Password.length()>0){
		urlstream << ":" <<this->Password <<"@";
	}
	temp = this->Host->getFqdn();
	if(temp.length()){
		urlstream << temp;
	}else{
		temp = this->Host->getIpv4asString();
		if(temp.length()){
			urlstream << temp;
		}else{
			urlstream <<this->Host->getIpv6asString();
		}
	}

	if(this->Port)
		urlstream << ":" <<this->Port;
	if(this->Frag.length()>0){
		urlstream << this->Frag;
		if(this->paramsmap.size()>0){
			urlstream << "?" << this->getParams();
		}
	}
	this->Url = urlstream.str();
}


const string& proxyReaperUrl::getUser() const {
	return this->User;
}

void proxyReaperUrl::setUser(const string& user) {
	User = user;
}


const string proxyReaperUrl::getContentTimed(timeval* tres){
	timeval t2,t1;
	stringstream * ss = new stringstream();
		curlpp::Cleanup myCleanup;
		curlpp::Easy myRequest;

		curlpp::options::Url curlppOptUrl(this->Url);
		myRequest.setOpt(curlpp::options::FollowLocation(1));
		myRequest.setOpt(curlpp::options::MaxRedirs(5));
		myRequest.setOpt(curlpp::options::WriteStream(ss));
		myRequest.setOpt(curlpp::options::Timeout(1));
		myRequest.setOpt(curlpp::options::ConnectTimeout(2));
		myRequest.setOpt(curlppOptUrl);

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

		this->content = ss->str();
		delete ss;
		return this->content;
}

const string proxyReaperUrl::getContent(){
	return getContentTimed(NULL);
}

proxyReaperUrl::~proxyReaperUrl() {
	// TODO Auto-generated destructor stub
	delete this->Host;
}

map<string,int> proxyReaperUrl::ProtoPortMap = {
		{"http", 80},
		{"https", 443},
		{"ftp", 21},
		{"ftps", 990},
		{"sftp", 115},
		{"pop2", 109},
		{"pop3", 110},
		{"pop3s", 995},
		{"ssh", 22},
		{"telnet", 23},
		{"smtp", 25},
		{"domain", 53},
		{"dns", 53},
		{"socks4", 0},
		{"socks4a", 0},
		{"socks5", 0},
		{"socks5h", 0},
};

int proxyReaperUrl::getSchemePort(const string& scheme){
	return proxyReaperUrl::ProtoPortMap[scheme];
}

ostream& operator<< (ostream &strm, proxyReaperUrl*& url) {
	strm << "proxyReaperUrl(" << url->getUrl() <<endl << url->Host<<endl
			<< ")";
	return strm;
}
