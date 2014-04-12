#include <string>
#include <sstream>
#include <iostream>

#include "lib/proxyReaperLib.h"
#include <vector>
#include <thread>
#include <sys/types.h>
#include <dirent.h>
#include <getopt.h>

#define __PROXYREAPER_TEST_URL__ "http://greynet.eu/div/headers.php"
#define __PROXYREAPER_VERSION__ "0.1a"

using namespace std;


string ipv4;
map<proxyReaperProxy*,timeval*> mapMeasure;
proxyReaperUrl * testURL = new proxyReaperUrl(string( __PROXYREAPER_TEST_URL__ ) );
int timeout=5;
int detector_local = false;
int verbose = false;



string printtv(timeval* tv){
	stringstream * ss = new stringstream();
	string str;
	*ss << +(tv->tv_sec*1.0 + (tv->tv_usec/1000000.0)) ;
	str = ss->str();;
	delete ss;
	return str;
} 

string getpublicipv4(){
	proxyReaperUrl * ipcheckUrl = new proxyReaperUrl(string("http://greynet.eu/div/ipcheck.php"));
	
	string ipstring = ipcheckUrl->getContent();
	ipstring = ipstring.substr(ipstring.find(": ")+2,ipstring.length()-ipstring.find("</body")-2);
	delete ipcheckUrl;
	return ipstring ;
}





void threadedCheck(proxyReaperProxy* mProx,char* thread_surv){
	string content;
	timeval * tv = (timeval *)calloc(1,sizeof(timeval));
	string seen;
	string seenip;
	string seenvia;
	string seenproxid;
	string seenxff;
	string seenforw;
	int strfound;
	try{
		content=mProx->getContentTimed(testURL,tv,timeout);
		if(verbose)
			cout << mProx->getUrl()->getUrl() << endl;
		//<< content << endl;
		mapMeasure[mProx]=tv;
		mProx->setLastcheck(time(NULL));
		mProx->setChecknumber(mProx->getChecknumber()+1);
		if(content.find("proxyReaper;") == string::npos){
			free(tv);
			mapMeasure[mProx]=NULL;
		}else{
			mProx->setLastLag((long double) (tv->tv_sec*1.0 + (tv->tv_usec/1000000.0)) );
			mProx->setMeanLag((long double) (mProx->getMeanLag()+mProx->getLastLag()) / 2.0);
			mProx->setLastworkingcheck(mProx->getLastcheck());
			mProx->setWorkingchecknumber(mProx->getWorkingchecknumber()+1);

			seen = string("IP;");
			strfound = content.find(seen);
			seenip = content.substr(strfound+seen.length(),content.find("\n",strfound)-(strfound+seen.length()));

			seen = string("VIA;");
			strfound = content.find(seen);
			seenvia = content.substr(strfound+seen.length(),content.find("\n",strfound)-(strfound+seen.length()));


			seen = string("proxID;");
			strfound = content.find(seen);
			seenproxid = content.substr(strfound+seen.length(),content.find("\n",strfound)-(strfound+seen.length()));


			seen = string("xff;");
			strfound = content.find(seen);
			seenxff = content.substr(strfound+seen.length(),content.find("\n",strfound)-(strfound+seen.length()));


			seen = string("forw;");
			strfound = content.find(seen);
			seenforw = content.substr(strfound+seen.length(),content.find("\n",strfound)-(strfound+seen.length()));

			if(seenip.compare(ipv4)==0 ||seenforw.compare(ipv4)==0 || seenxff.compare(ipv4)==0 ){
				mProx->setAnonymity(ANON_TRANSPARENT);
			}else if(seenvia.length()>0 || seenproxid.length()>0){
				mProx->setAnonymity(ANON_LOW_ANONYMOUS);
			}else{
				mProx->setAnonymity(ANON_HIG_ANONYMOUS);
			}

			fflush(NULL);
		}
	}catch(curlpp::LibcurlRuntimeError & e){
		free(tv);
		mapMeasure[mProx]=NULL;
	}
	*thread_surv = true;
}


int main(int argc,char** argv){

	int c,i;
	int available_procs = num_processors(NPROC_CURRENT_OVERRIDABLE);
	std::cout << "ProxyReaper "<< __PROXYREAPER_VERSION__ << std::endl << "on "<< +available_procs << " processing units" << endl;
	char * prox_surv = (char*)calloc(available_procs,sizeof(char));
	vector<std::thread*> threadvector (available_procs);

	while (1) {

		int option_index = 0;
		static struct option long_options[] = {
				{"timeout", 			required_argument, 		0,  0 },
				{"detector-url",  	required_argument,		0,  0 },
				{"detector-local",	no_argument,			0,  0 },
				{"verbose", 			no_argument,			0,  0 },
				{0,         			0,						0,  0 }
		};

		c = getopt_long(argc, argv, "H:t:vlh",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			if(strcmp(long_options[option_index].name,"timeout")==0){
				timeout = strtoll(optarg,NULL,10);
			}
			if(strcmp(long_options[option_index].name,"detector-local")==0){
				detector_local = true;
			}
			if(strcmp(long_options[option_index].name,"detector-url")==0){
				testURL = new proxyReaperUrl(string( optarg ) );
			}
			if(strcmp(long_options[option_index].name,"verbose")==0){
				verbose = true;
			}
			break;
		case 'v':
			verbose = true;
			break;
		case 'H':
			testURL = new proxyReaperUrl(string( optarg ) );
			break;

		case 't':
			timeout = strtoll(optarg,NULL,10);
			break;

		case 'h':
			cout <<
			"--timeout sec\t\t:timeout in seconds (also -t)\n"
			"--detector-host url\t:url to a php script as described in doc.(also -H)\n"
			"--detector-local   \t:use a local server to replace php script.\n"
			"--verbose   \t\t:be verbose. (also -v)\n"
			"-h : this help"
			<< endl;
			break;
		default:
			printf("?? getopt returned character code 0%x %c ??\n",c, c);
		}
	}



	ipv4 = getpublicipv4();

	proxyReaperPersist * persist = proxyReaperPersist::get();

	proxyReaperSource * mSource = new proxyReaperSource("/home/jg/.proxyReaper/sources/hma.pl","");
	persist->clean_incoming();
	persist->store_incoming_coll(mSource->get_as_urlfragvector());
	persist->add_incoming_to_proxies();

	vector<proxyReaperProxy*> sources =persist->getProxies();

	fflush(NULL);
	for(i=0;i<available_procs;i++){
		*(prox_surv+i)=true;
		threadvector[i] = NULL;
	}
	vector<proxyReaperProxy*>::iterator it = sources.begin();
	// 		threadedCheck((*it),prox_surv+i);
	while(1){
		for(i=0;i<available_procs;i++){
			if(*(prox_surv+i)){
				if(threadvector[i]){
					threadvector[i]->join();
					delete threadvector[i];
					*(prox_surv+i) = false;
				}
				threadvector[i] = new std::thread(threadedCheck,(*it),prox_surv+i);

				it++;
				if(it == sources.end())
					break;
			}
		}

		if(it == sources.end())
			break;
		else
			usleep((timeout*1000000)/40);
	}

	sleep(timeout); // by construction all threads finished
	for(i=0;i<available_procs;i++){
		threadvector[i]->join();
		delete threadvector[i];
		*(prox_surv+i) = false;
	}


	fflush(NULL);
	if(verbose)
		for(map<proxyReaperProxy*,timeval*>::iterator iter = mapMeasure.begin(); iter != mapMeasure.end();iter++)
		{
			if(iter->second){
				cout << iter->first->getUrl()->getUrl() << " = " <<	printtv(iter->second) << endl;
				free(iter->second);
			}
			else{
				cout << iter->first->getUrl()->getUrl() << " = " << "Broken" << endl;
			}


		}

	persist->store(sources);

	/*clean proxies not working > 24h or transparent ones*/
	persist->cleanOldNotWorkingAndTransparents();


	for(map<proxyReaperProxy*,timeval*>::iterator iter = mapMeasure.begin(); iter != mapMeasure.end();iter++){
		delete iter->first;
	}

	delete testURL;
	delete persist;
	delete mSource;
	free(prox_surv);
	return 0;
}




