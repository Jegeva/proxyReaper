/*
 * proxyReaperHost.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: jg
 */

#include "proxyReaperHost.h"

proxyReaperHost::proxyReaperHost() {
	int i;
		for(i=0;i<4;i++)
			this->Ipv4[i]=0;
		for(i=0;i<16;i++)
			this->Ipv6[i]=0;

}

proxyReaperHost::~proxyReaperHost() {
	// TODO Auto-generated destructor stub
}

proxyReaperHost::proxyReaperHost(string str) {
	int i;
	for(i=0;i<4;i++)
		this->Ipv4[i]=0;
	for(i=0;i<16;i++)
		this->Ipv6[i]=0;

	bool parsed = false;
	if(this->validateIpv4(str)){
		this->parseIpv4(str);
		parsed=true;
		this->Fqdn = string();
	}
	if(this->validateIpv6(str) && (!parsed)){
		this->parseIpv6(str);
		parsed=true;
		this->Fqdn = string();
	}

	if(!parsed){
		this->setFqdn(str);
	}

}

proxyReaperHost::proxyReaperHost(vector<unsigned char> numip) {
	switch(numip.size()){
	case 4 :
		this->setIpv4(numip);

		break;
	case 16 :
		this->setIpv6(numip);
		break;
	default:
		throw(proxyReaperMalformedIpException(string("Host Constructor")));
	}
}

const string& proxyReaperHost::getFqdn() const {
	return this->Fqdn;
}
const unsigned char* proxyReaperHost::getIpv4() const {
	return this->Ipv4;
}
const unsigned char* proxyReaperHost::getIpv6() const {
	return this->Ipv6;
}

const string proxyReaperHost::getIpv4asString() const {
	stringstream tmpstr;
	tmpstr << +(this->Ipv4[0]);
	for(int i=1;i<4;i++)
		tmpstr << "." << +(this->Ipv4[i]);
	return tmpstr.str();


}
const string proxyReaperHost::getIpv6asString() const {
	stringstream tmpstr;
	string str;
	int i=0;
	int j=i;
	int tmp;
	bool compress1stzerochain = true;
	for(int i=0;i<16;i+=2){
		tmp = (((this->Ipv6[i] << 8) + this->Ipv6[i+1]));
		j=i;
		if(!tmp){
			while((j<14) && compress1stzerochain && (!((this->Ipv6[j] << 8) + this->Ipv6[j+1])) ){
				j+=2;
			}
			if(compress1stzerochain && (j!=i)){
				compress1stzerochain = false;
				i=j;
				tmpstr << ":";
			}

			tmp = (((this->Ipv6[i] << 8) + this->Ipv6[i+1]));
		}
		if(i)
			tmpstr << ":";
		if(! (i==14 && !tmp))
			tmpstr << boost::format("%x") % +tmp;
	}
	str = tmpstr.str();
	tmpstr.clear();
	return str;
}

bool proxyReaperHost::validateIpv4(string Ipv4) {
	vector<string> splitted;
	boost::split(splitted, Ipv4, boost::is_any_of("."));
	bool val = true;
	if(splitted.size() == 4){
		for (vector<string>::iterator it = splitted.begin();
				it != splitted.end(); ++it) {
			try {
				int x = boost::lexical_cast<int>(*it);
				if ((x > 255) || (x < 0))
					val = false;
			} catch (boost::bad_lexical_cast const&) {
				val = false;
			}
		}
	}else{
		val = false;
	}
	return val;

}

bool proxyReaperHost::validateIpv4(vector<unsigned char> Ipv4) {
	return (Ipv4.size() == 4) ? true : false;
}

void proxyReaperHost::parseIpv4(string Ipv4) {
	vector<string> splitted;
	boost::split(splitted, Ipv4, boost::is_any_of("."));
	int i=0;
	unsigned char x;
	for (vector<string>::iterator it = splitted.begin();
			it != splitted.end(); ++it) {
		x = boost::lexical_cast<int>(*it);
		this->Ipv4[i++]=x;
	}

}

bool proxyReaperHost::validateIpv6(string Ipv6) {
	boost::regex validationRegExp("(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");
	//http://stackoverflow.com/questions/53497/regular-expression-that-matches-valid-ipv6-addresses
	boost::cmatch what;
	if(! regex_match(Ipv6.c_str(),what,validationRegExp))
		return false;
	return true;
}

bool proxyReaperHost::validateIpv6(vector<unsigned char> Ipv6) {
	return (Ipv6.size() == 16) ? true : false;
}

void proxyReaperHost::parseIpv6(string Ipv6) {
	unsigned char mIpv6[16];
	vector<string> splitted;
	boost::split(splitted, Ipv6, boost::is_any_of(":"));
	unsigned int rankbreak = 0;
	unsigned int rrank = 0;
	int i=0;
	for(;i<16;i++) mIpv6[i]=0;
	if(Ipv6.length()>2){
		i=0;

		bool exitloop = false;
		for (vector<string>::iterator it = splitted.begin();it != splitted.end() && (!exitloop); ++it) {
			switch((*it).length()){
			case 4:
				mIpv6[i]=strtol((*it).substr(0,2).c_str(),NULL,16);
				mIpv6[i+1]=strtol((*it).substr(2,2).c_str(),NULL,16);
				break;
			case 3:
				mIpv6[i]=strtol((*it).substr(0,2).c_str(),NULL,16);
				mIpv6[i+1]=strtol((*it).substr(2,2).c_str(),NULL,16);
				break;
			case 2:
				mIpv6[i]=0;
				mIpv6[i+1]=strtol((*it).substr(0,2).c_str(),NULL,16);
				break;
			case 1:
				mIpv6[i]=0;
				mIpv6[i+1]=strtol((*it).c_str(),NULL,16);
				break;
			case 0:

				exitloop=true;
				break;
			}
			i+=2;
			rankbreak++;
		}
		exitloop=false;
		if(i!=16){
			i=14;
			for (vector<string>::reverse_iterator it = splitted.rbegin();it != splitted.rend() && (!exitloop) ; ++it) {
				switch((*it).length()){
				case 4:
					mIpv6[i]=strtol((*it).substr(0,2).c_str(),NULL,16);
					mIpv6[i+1]=strtol((*it).substr(2,2).c_str(),NULL,16);
					break;
				case 3:
					mIpv6[i]=strtol((*it).substr(0,2).c_str(),NULL,16);
					mIpv6[i+1]=strtol((*it).substr(2,2).c_str(),NULL,16);
					break;
				case 2:
					mIpv6[i]=0;
					mIpv6[i+1]=strtol((*it).substr(0,2).c_str(),NULL,16);
					break;
				case 1:
					mIpv6[i]=0;
					mIpv6[i+1]=strtol((*it).c_str(),NULL,16);
					break;
				case 0:
					mIpv6[i+1]=mIpv6[i]=0;

					break;
				}
				rrank++;
				i-=2;
				if(rrank ==(splitted.size()-rankbreak))
					exitloop=true;
			}

		}
	}
	for(i=0;i<16;i++) this->Ipv6[i]=mIpv6[i];

}

void proxyReaperHost::setIpv4(string Ipv4) {
	if (validateIpv4(Ipv4)) {
#ifdef __DEBUG
		cout << "IPV4 VALID "<< Ipv4 << endl;
#endif
		parseIpv4(Ipv4);
	} else {
#ifdef __DEBUG
		cout << "IPV4 INVALID "<< Ipv4 << endl;
#endif
		throw(proxyReaperMalformedIpv4Exception(Ipv4));
	}
}

void proxyReaperHost::setIpv4(vector<unsigned char> Ipv4) {
	vector<unsigned char>::iterator it;
	stringstream tmpstr;
	int i=0;
	it = Ipv4.begin();
	tmpstr << +*it ;++it;
	for (;it != Ipv4.end(); ++it) {
		tmpstr << "."<< +*it ;
	}
	if (validateIpv4(Ipv4)) {
#ifdef __DEBUG
		cout << string("IPV4 VALID ") << tmpstr.str() << endl;
#endif
		it = Ipv4.begin();
		for (;it != Ipv4.end(); ++it) {
			this->Ipv4[i++]=*it;
		}
	} else {
#ifdef __DEBUG
		cout << string("IPV4 INVALID ") << tmpstr.str() << endl;

#endif
		throw(proxyReaperMalformedIpv6Exception(tmpstr.str()));
	}


}

void proxyReaperHost::setIpv6(const string Ipv6) {

	if (validateIpv6(Ipv6)) {
#ifdef __DEBUG
		cout << "IPV6 VALID "<< Ipv6 << endl;
#endif
		parseIpv6(Ipv6);
	} else {
#ifdef __DEBUG
		cout << "IPV6 INVALID "<< Ipv6 << endl;
#endif
		throw(proxyReaperMalformedIpv6Exception(Ipv6));
	}


}

void proxyReaperHost::setIpv6(vector<unsigned char> Ipv6) {
	vector<unsigned char>::iterator it;
	stringstream tmpstr;
	int i=0;

	if (validateIpv6(Ipv6)) {


		for (it = Ipv6.begin();it != Ipv6.end(); ++it) {
			this->Ipv6[i++]=*it;
		}
#ifdef __DEBUG
		cout << string("IPV6 VALID ")<< this->getIpv6asString() << endl;
#endif
	} else {
#ifdef __DEBUG
		tmpstr <<hex<< +*it ;++it;
		for (it = Ipv6.begin();it != Ipv6.end(); ++it) {
			tmpstr << ":"<< +*it ;
		}
		cout << string("IPV6 INVALID ")<< tmpstr.str() << endl;

#endif
		throw(proxyReaperMalformedIpv6Exception(tmpstr.str()));
	}


}

void proxyReaperHost::setFqdn(string hostname){
	bool err = true;
	vector<unsigned char> addr_vector;
	setlocale (LC_ALL, "");
	const char* lochostname = hostname.c_str();
	char* inthostname = 0;
	idna_to_ascii_lz (lochostname, &inthostname, 0);


	struct hostent * host_cstruct = gethostbyname2(inthostname,AF_INET);
	if (host_cstruct){
		err= false;
		for(int i=0;i<4;i++)
			addr_vector.push_back( *((host_cstruct->h_addr)+i) );
		this->setIpv4(addr_vector);

	}
	addr_vector.clear();
	host_cstruct = gethostbyname2(inthostname,AF_INET6);
	if (host_cstruct){
		err = false;
		for(int i=0;i<16;i++)
			addr_vector.push_back( *((host_cstruct->h_addr)+i) );
		this->setIpv6(addr_vector);
	}
	if(err){
		throw(proxyReaperInvalidHostname(inthostname));
	}else{
		this->Fqdn = string(inthostname)		;
	}
	if(inthostname)
	free(inthostname);
}

ostream& operator<< (ostream &strm, proxyReaperHost* host) {
	strm << "proxyReaperHost(" << host->getFqdn() <<
			" ipv4:" << host->getIpv4asString() <<
			" ipv6:" << host->getIpv6asString() <<
			")";
	return strm;
}
ostream& operator<< (ostream &strm, proxyReaperHost*& host) {
	strm << "proxyReaperHost(" << host->getFqdn() <<
			" ipv4:" << host->getIpv4asString() <<
			" ipv6:" << host->getIpv6asString() <<
			")";
	return strm;
}
ostream& operator<< (ostream &strm, const proxyReaperHost*& host) {
	strm << "proxyReaperHost(" << host->getFqdn() <<
			" ipv4:" << host->getIpv4asString() <<
			" ipv6:" << host->getIpv6asString() <<
			")";
	return strm;
}
