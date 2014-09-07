/*
 * proxyReaperSource.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: jg
 */

#include "proxyReaperSource.h"

using namespace std ;

proxyReaperSource::proxyReaperSource(string path,string args) {
	// TODO Auto-generated constructor stub
	this->path = path;
	vector<string> splitted;
	string item,key,value;
	unsigned int pos;
	boost::split(splitted, args, boost::is_any_of("-")); // split params
	for (vector<string>::iterator it = splitted.begin();it != splitted.end(); ++it) {
		item = *it;
		pos = item.find(' ');
		key = item.substr(0,pos);
		if(pos != std::string::npos){
			value = item.substr(pos+1,item.length());
		}else{
			value = string("");
		}
		this->arguments.insert(std::pair<string,string>(key,value));
	}
}




proxyReaperSource::~proxyReaperSource() {
	// TODO Auto-generated destructor stub
}

vector<vector <string>> proxyReaperSource::get_as_urlfragvector(){
	string command;
	string line;
	FILE * fs;
	int read,i;

	vector<vector<string>> ret ;
	vector<string> * tmpret ;
	char *ipCharPtr,*portCharPtr,*typeCharPtr,*lineCharPtr;
	string ip,port,type;
	command = (this->path+string(" "));
	for(map<string,string>::iterator it = (this->arguments).begin();it != (this->arguments).end();it++){
		command += string("-") + it->first + string(" ") + it->second;
	}
	fprintf(stderr,"execing :");
	fs = popen(command.c_str(),"r");
	if(fs == NULL){
		//	return ret;
		//		fprintf(stderr,"fail\n");
		return ret;
		//	}else{
		//		fprintf(stderr,"success\n");
	}

	do{
		read = fscanf(fs,"%ms",&lineCharPtr);
		if(read && (read != EOF)){
			//printf("%s\n",lineCharPtr);
			ipCharPtr = portCharPtr  = typeCharPtr = 0;
			ipCharPtr = lineCharPtr;
			i=0;
			while(*(ipCharPtr+i)!=';' && *(ipCharPtr+i) ) i++ ;
			if(*(ipCharPtr+i)){
				*(ipCharPtr+i)=0;
				portCharPtr = (ipCharPtr+i+1);
				i++;
				while(*(ipCharPtr+i)!=';' && *(ipCharPtr+i) ) i++ ;
				if(*(ipCharPtr+i)){
					*(ipCharPtr+i)=0;
					typeCharPtr = (ipCharPtr+i+1);
					i++;
					while(*(ipCharPtr+i)!=';' && *(ipCharPtr+i) ) i++ ;
					*(ipCharPtr+i)=0;
				}
			}
			if(portCharPtr && typeCharPtr && ipCharPtr){
				ip=string(ipCharPtr);
				port=string(portCharPtr);
				type=string(typeCharPtr);

				std::transform(type.begin(), type.end(), type.begin(), ::tolower);


				if(type.find("sock") != string::npos){
					if(type.find("5") != string::npos){
						type = string("socks5");
					}else{
						type = string("socks4");
					}
				}

				tmpret = new vector<string>;
				tmpret->push_back(type);
				tmpret->push_back(ip);
				tmpret->push_back(port);
				ret.push_back(*tmpret);
				delete tmpret;
				//cout << "IP=" << ip << " PORT =" << port << " TYPE=" << type << "-> "<< urlprox.str()<< endl;




			}else{

				fprintf(stderr,"caught bad line ! %s\n",this->path.c_str());


			}

		}
		free(lineCharPtr);
	}while(read && (read != EOF));
	fclose(fs);
	fprintf(stderr,"execed\n");
	return ret;
}

vector <string> proxyReaperSource::get_as_urlvector(){
	vector<vector <string>> vvs = get_as_urlfragvector();
	vector <string> ret;
	stringstream urlprox;
	for(vector<vector <string>>::iterator it = vvs.begin();it != vvs.end();it++){
	urlprox << (*it)[0] << "://" << (*it)[1] << ":" << (*it)[2];
	ret.push_back(urlprox.str());
	urlprox.str(std::string());
	}
	return ret;
}

vector<proxyReaperProxy*> proxyReaperSource::get_as_proxyvector(){
	vector<string> stringvec = get_as_urlvector();
	vector<proxyReaperProxy*> ret;
	for(vector<string>::iterator it=stringvec.begin();it != stringvec.end()  ;it++){
		try{
			ret.push_back(new proxyReaperProxy(*it));
			//cout << "IP=" << ip << " PORT =" << port << " TYPE=" << type << "-> "<< urlprox.str()<< endl;
		}catch(proxyReaperMalformedUrlException & e){
			cerr << "caught bad line ! : " << this->path << " "<< e.what() << endl;
		}
	}

	return ret;
}
