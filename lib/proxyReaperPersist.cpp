/*
 * ProxyReaperPersist.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: jg
 */

#include "proxyReaperPersist.h"

using namespace std;

proxyReaperPersist* proxyReaperPersist::controller = NULL;

vector<proxyReaperProxy*> proxyReaperPersist::getProxies(){
	vector<proxyReaperProxy*> ret;
	vector<map<string,string>> * proxData = query("select * from proxyReaper_proxies;");

	for(vector<map<string,string>>::iterator it = proxData->begin();it !=proxData->end();it++){
		/*
"create table proxyReaper_proxies("
					"ipv4 text not null,"
					"ipv6 text,"
					"scheme text not null,"
					"port int, "
					"anon_level text not null,"
					"checknumber int not null, "
					"last_lag real not null, "
					"mean_lag real not null, "
					"lastcheck int not null, "
					"lastworkingcheck int not null, "
					"workingchecknumber int not null, "
					"foreign key(scheme) references proxyReaper_proxy_schemes(proxy_scheme),"
					"foreign key(ipv4,ipv6) references proxyReaper_hosts(ipv4,ipv6),"
					"primary key(scheme,ipv4,ipv6,port)"
					");

	proxyReaperProxy(
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
		 * */
	ret.push_back(new proxyReaperProxy(
			(*it)["ipv4"],
			(*it)["ipv6"],
			strtoll((*it)["port"].c_str(),NULL,10),
			(*it)["scheme"],
			0,
			(*it)["anon_level"],
			strtoll((*it)["lastcheck"].c_str(),NULL,10),
			strtoll((*it)["lastworkingcheck"].c_str(),NULL,10),
			strtold((*it)["last_lag"].c_str(),NULL),
			strtold((*it)["mean_lag"].c_str(),NULL),
			strtoll((*it)["checknumber"].c_str(),NULL,10),
			strtoll((*it)["workingchecknumber"].c_str(),NULL,10)
	));
	}
	delete proxData;
	return ret;

}

vector<map<string,string>> * proxyReaperPersist::query(string query){
	sqlite3_stmt * statement;
	int ret;
	int i;
	vector<map<string,string>> * funcret = new vector<map<string,string>> ;
	map<string,string>* lineobj=NULL;
	ret=sqlite3_prepare_v2(this->db,query.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};

	ret = sqlite3_step(statement);
	while(ret == SQLITE_ROW){
		lineobj = new map<string,string>;
		i= sqlite3_column_count(statement);
		while(i--){
			lineobj->insert(std::pair<string,string>(
					string(reinterpret_cast<const char*>(sqlite3_column_name(statement,i))),
					sqlite3_column_type(statement,i)==SQLITE_NULL?string():string(reinterpret_cast<const char*>(sqlite3_column_text(statement,i)))));
		}
		funcret->push_back(*lineobj);
		ret = sqlite3_step(statement);
		if(lineobj)
					delete lineobj;
	}

	if(ret != SQLITE_DONE)
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	sqlite3_finalize(statement);
	return funcret;
}

proxyReaperPersist::proxyReaperPersist() {
	// TODO Auto-generated constructor stub
	if(this->controller){
		return ;
	}else{
		string homePath = string(getenv("HOME"));
		string folderPath = homePath + string("/.proxyReaper");
		string dbPath = folderPath + string("/db.sqlite3");
		vector<map<string,string>> * queryResult;

		mkdir(folderPath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
		int ret = sqlite3_open_v2(dbPath.c_str(),&this->db,
				SQLITE_OPEN_READWRITE |
				SQLITE_OPEN_CREATE |
				SQLITE_OPEN_FULLMUTEX,
				NULL);
		if(SQLITE_OK != ret){
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
		}

		queryResult=this->query(string("SELECT name FROM sqlite_master WHERE type='table' AND name='proxyReaper_hosts';"));

		if(queryResult->size() == 0){
			delete queryResult;
			// DATABASE STRUCTURE NOT CREATED
			queryResult=this->query(string(
					"create table proxyReaper_urls("
					"url text primary key"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_schemes("
					"scheme text primary key"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_proxy_schemes("
					"proxy_scheme text, "
					"foreign key(proxy_scheme) references proxyReaper_schemes(scheme)"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_url_schemes("
					"url_scheme text, "
					"foreign key(url_scheme) references proxyReaper_schemes(scheme)"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_hosts("
					"ipv4 text not null,"
					"ipv6 text,"
					"fqdn text, "
					"primary key(ipv4,ipv6)"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_proxies("
					"ipv4 text not null,"
					"ipv6 text,"
					"scheme text not null,"
					"port int, "
					"anon_level text not null,"
					"checknumber int not null, "
					"last_lag real not null, "
					"mean_lag real not null, "
					"lastcheck int not null, "
					"lastworkingcheck int not null, "
					"workingchecknumber int not null, "
					"foreign key(scheme) references proxyReaper_proxy_schemes(proxy_scheme),"
					"foreign key(ipv4,ipv6) references proxyReaper_hosts(ipv4,ipv6),"
					"primary key(scheme,ipv4,ipv6,port)"
					");"));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_proxies_incoming("
					"ipv4 text not null,"
					"ipv6 text,"
					"scheme text not null,"
					"port int,"
					"primary key(scheme,ipv4,ipv6,port)"
					");"
			));
			delete queryResult;
			queryResult=this->query(string(
					"create table proxyReaper_sources("
					"path text,"
					"args text,"
					"primary key(path,args)"
					");"
			));
			delete queryResult;
		}else{
			delete queryResult;
		}
		fflush(NULL);
	}
}

proxyReaperPersist::~proxyReaperPersist() {
	// TODO Auto-generated destructor stub
	sqlite3_close_v2(this->db);
}


proxyReaperPersist* proxyReaperPersist::get(){
	if(proxyReaperPersist::controller){
		return proxyReaperPersist::controller;
	}else{
		proxyReaperPersist::controller = new proxyReaperPersist();
		return proxyReaperPersist::controller;
	}

}

void proxyReaperPersist::clean_incoming(){
	delete this->query(string("delete from proxyReaper_proxies_incoming"));

}

void proxyReaperPersist::add_incoming_to_proxies(){
	delete this->query("insert into proxyReaper_proxies (ipv4,ipv6,scheme,port,anon_level,checknumber,last_lag,mean_lag,lastcheck,lastworkingcheck,workingchecknumber) select ipv4,ipv6,scheme,port,'ANON_TRANSPARENT',0,0,0,0,0,0 from proxyReaper_proxies_incoming where ipv4 not in (select ipv4 from proxyReaper_proxies) and not port = 0;");
}
int proxyReaperPersist::store_incoming_coll(vector<vector<string>> coll){
	sqlite3_stmt * statement;
	int ret;
	string sql = "insert into proxyReaper_proxies_incoming(ipv4,ipv6,scheme,port) values (?,?,?,?);";
	ret=sqlite3_prepare_v2(this->db,sql.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};

	for(vector<vector<string>>::iterator it =coll.begin();it!=coll.end();it++){
		if((*it)[1].find(":")== string::npos){
			ret=sqlite3_bind_text(statement, 1,(*it)[1].c_str(),(*it)[1].length(), SQLITE_TRANSIENT);
			if(ret){
				// ERROR
				throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
			};
			ret=sqlite3_bind_text(statement, 2,NULL,0, SQLITE_TRANSIENT);
			if(ret){
				// ERROR
				throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
			};
		}else{
			ret=sqlite3_bind_text(statement, 2,(*it)[1].c_str(),(*it)[1].length(), SQLITE_TRANSIENT);
			if(ret){
				// ERROR
				throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
			};
			ret=sqlite3_bind_text(statement, 1,NULL,0, SQLITE_TRANSIENT);
			if(ret){
				// ERROR
				throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
			};
		}
		ret=sqlite3_bind_text(statement, 3,(*it)[0].c_str(),(*it)[0].length(), SQLITE_TRANSIENT);
		if(ret){
			// ERROR
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
		};
		ret=sqlite3_bind_int(statement, 4,strtoll((*it)[2].c_str(),NULL,10));
		if(ret){
			// ERROR
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
		};
		ret = sqlite3_step(statement);
		if(ret != SQLITE_DONE){
			if(ret != SQLITE_CONSTRAINT) //already exists ;)
				throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
		}
		sqlite3_reset(statement);

	}
	sqlite3_finalize(statement);
	return ret;
}

int proxyReaperPersist::store_incoming(string ipv4,string ipv6,string scheme,int port){
	sqlite3_stmt * statement;
	int ret;
	string sql = "insert into proxyReaper_proxies_incoming(ipv4,ipv6,scheme,port) values (?,?,?,?);";
	ret=sqlite3_prepare_v2(this->db,sql.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 1,ipv4.c_str(),ipv4.length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 2,ipv6.c_str(),ipv6.length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 3,scheme.c_str(),scheme.length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_int(statement, 4,port);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret = sqlite3_step(statement);
	if(ret != SQLITE_DONE){
		if(ret != SQLITE_CONSTRAINT) //already exists ;)
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	}
	sqlite3_finalize(statement);
	return ret;
}

int proxyReaperPersist::store(proxyReaperUrl* url){
	sqlite3_stmt * statement;
	int ret;
	string sql = "insert into proxyReaper_urls (url) values(?);";
	ret=sqlite3_prepare_v2(this->db,sql.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 1, url->getUrl().c_str(), url->getUrl().length(), SQLITE_STATIC);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret = sqlite3_step(statement);

	if(ret != SQLITE_DONE){
		if(ret != SQLITE_CONSTRAINT) //already exists ;)
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	}
	sqlite3_finalize(statement);
	return ret;
}


int proxyReaperPersist::store(vector<proxyReaperProxy*> proxylist){
	delete query("delete from proxyReaper_proxies;");
	delete query("delete from proxyReaper_hosts;");
	for(vector<proxyReaperProxy*>::iterator it = proxylist.begin();it != proxylist.end();it++){
		store(*it);
	}
	return 0;
}
int proxyReaperPersist::store(proxyReaperProxy* proxy){
	const proxyReaperHost* host = proxy->getHost();
	this->store((proxyReaperHost*)host);
	sqlite3_stmt * statement;
	int ret;
	string sql = string("insert into proxyReaper_proxies (ipv4,ipv6,scheme,port,anon_level,checknumber,last_lag,mean_lag,lastcheck,lastworkingcheck,workingchecknumber) values(?,?,?,?,?,?,?,?,?,?,?);");
	ret=sqlite3_prepare_v2(this->db,sql.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 1,(proxy->getHost()->getIpv4asString()).c_str(), proxy->getHost()->getIpv4asString().length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 2,(proxy->getHost()->getIpv6asString()).c_str(), proxy->getHost()->getIpv6asString().length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	if(proxy->getUrl() != NULL){
		ret=sqlite3_bind_text(statement, 3,(proxy->getUrl()->getScheme()).c_str(),proxy->getUrl()->getScheme().length()         , SQLITE_TRANSIENT);
	}else{
		ret=sqlite3_bind_text(statement, 3,NULL,0, SQLITE_TRANSIENT);
	}
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	if(proxy->getUrl()->getPort()){
		ret=sqlite3_bind_int(statement, 4,(proxy->getUrl()->getPort()));
	}else{
		int port = proxyReaperUrl::getSchemePort(proxy->getUrl()->getScheme());
		if(port){
			ret=sqlite3_bind_int(statement, 4,port);
		}else{
			throw(proxyReaperProxyNeedsPortException(proxy->getIpv4asString()));
		}
	}
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};

	string anon_string;
	switch(proxy->getAnonymity()){
	case ANON_TRANSPARENT : //forwarded for
		anon_string=string("ANON_TRANSPARENT");
		break;
	case ANON_LOW_ANONYMOUS: //forwarded
		anon_string=string("ANON_LOW_ANONYMOUS");
		break;
	case ANON_MED_ANONYMOUS: // forwarded for other
		anon_string=string("ANON_MED_ANONYMOUS");
		break;
	case ANON_HIG_ANONYMOUS: // :)
		anon_string=string("ANON_HIG_ANONYMOUS");
		break;
	case ANON_TRANSPARENT_KA:
		anon_string=string("ANON_TRANSPARENT_KA");
		break;
	case ANON_LOW_ANONYMOUS_KA: //forwarded + keep alive
		anon_string=string("ANON_LOW_ANONYMOUS_KA");
		break;
	case ANON_MED_ANONYMOUS_KA: // forwarded for other + keep alive
		anon_string=string("ANON_MED_ANONYMOUS_KA");
		break;
	case ANON_HIG_ANONYMOUS_KA: // :) + keep alive
		anon_string=string("ANON_HIG_ANONYMOUS_KA");
		break;
	}

	ret=sqlite3_bind_text(statement, 5,anon_string.c_str(), anon_string.length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};

	ret=sqlite3_bind_int(statement, 6,proxy->getChecknumber());
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};

	ret=sqlite3_bind_double(statement, 7,proxy->getLastLag());
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_double(statement, 8,proxy->getMeanLag());
		if(ret){
			// ERROR
			throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
		};
	ret=sqlite3_bind_int(statement, 9,proxy->getLastcheck());
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_int(statement, 10,proxy->getLastworkingcheck());
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_int(statement, 11,proxy->getChecknumber());
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};


	ret = sqlite3_step(statement);

	if(ret != SQLITE_DONE)
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	sqlite3_finalize(statement);
	return ret;
	return 0;
}




int proxyReaperPersist::store(proxyReaperHost* host){
	sqlite3_stmt * statement;
	int ret;
	string sql = string("insert into proxyReaper_hosts (ipv4,ipv6,fqdn) values( ?1 , ?2 , ?3 );");

	ret=sqlite3_prepare_v2(this->db,sql.c_str(),-1,&statement,NULL);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 1,(host->getIpv4asString()).c_str(), host->getIpv4asString().length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 2,(host->getIpv6asString()).c_str(), host->getIpv6asString().length(), SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret=sqlite3_bind_text(statement, 3,(host->getFqdn().c_str())        ,host->getFqdn().length()         , SQLITE_TRANSIENT);
	if(ret){
		// ERROR
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	};
	ret = sqlite3_step(statement);

	if(ret != SQLITE_DONE)
		throw(proxyReaperSQLLite3Exception(sqllite3Error(ret)));
	sqlite3_finalize(statement);
	return ret;
}



int proxyReaperPersist::store(proxyReaperSource* source){
	return 0;
}


void proxyReaperPersist::cleanOldNotWorkingAndTransparents(){
delete query(string("delete from proxyReaper_proxies where (lastworkingcheck < (strftime('%s','now')-(24*3600))) or anon_level = 'ANON_TRANSPARENT'"));
}

























string proxyReaperPersist::sqllite3Error(int err){
	switch(err){
	case SQLITE_OK:
		return string("SQLite3 : SUCCESS");
	case SQLITE_ERROR : return string("SQLite3:SQL error or missing database ");
	case SQLITE_INTERNAL : return string("SQLite3:Internal logic error in SQLite ");
	case SQLITE_PERM : return string("SQLite3:Access permission denied ");
	case SQLITE_ABORT : return string("SQLite3:Callback routine requested an abort ");
	case SQLITE_BUSY : return string("SQLite3:The database file is locked ");
	case SQLITE_LOCKED : return string("SQLite3:A table in the database is locked ");
	case SQLITE_NOMEM : return string("SQLite3:A malloc() failed ");
	case SQLITE_READONLY : return string("SQLite3:Attempt to write a readonly database ");
	case SQLITE_INTERRUPT : return string("SQLite3:Operation terminated by sqlite3_interrupt()");
	case SQLITE_IOERR : return string("SQLite3:Some kind of disk I/O error occurred ");
	case SQLITE_CORRUPT : return string("SQLite3:The database disk image is malformed ");
	case SQLITE_NOTFOUND : return string("SQLite3:Unknown opcode in sqlite3_file_control() ");
	case SQLITE_FULL : return string("SQLite3:Insertion failed because database is full ");
	case SQLITE_CANTOPEN : return string("SQLite3:Unable to open the database file ");
	case SQLITE_PROTOCOL : return string("SQLite3:Database lock protocol error ");
	case SQLITE_EMPTY : return string("SQLite3:Database is empty ");
	case SQLITE_SCHEMA : return string("SQLite3:The database schema changed ");
	case SQLITE_TOOBIG : return string("SQLite3:String or BLOB exceeds size limit ");
	case SQLITE_CONSTRAINT : return string("SQLite3:Abort due to constraint violation ");
	case SQLITE_MISMATCH : return string("SQLite3:Data type mismatch ");
	case SQLITE_MISUSE : return string("SQLite3:Library used incorrectly ");
	case SQLITE_NOLFS : return string("SQLite3:Uses OS features not supported on host ");
	case SQLITE_AUTH : return string("SQLite3:Authorization denied ");
	case SQLITE_FORMAT : return string("SQLite3:Auxiliary database format error ");
	case SQLITE_RANGE : return string("SQLite3:2nd parameter to sqlite3_bind out of range ");
	case SQLITE_NOTADB : return string("SQLite3:File opened that is not a database file ");
	case SQLITE_NOTICE : return string("SQLite3:Notifications from sqlite3_log() ");
	case SQLITE_WARNING : return string("SQLite3:Warnings from sqlite3_log() ");
	case SQLITE_ROW : return string("SQLite3:sqlite3_step() has another row ready ");
	case SQLITE_DONE : return string("SQLite3:sqlite3_step() has finished executing ");
	case SQLITE_IOERR_SHORT_READ: return string("SQLite3: SQLITE_IOERR_SHORT_READ");
	case SQLITE_IOERR_WRITE: return string("SQLite3: SQLITE_IOERR_WRITE");
	case SQLITE_IOERR_FSYNC: return string("SQLite3: SQLITE_IOERR_FSYNC");
	case SQLITE_IOERR_DIR_FSYNC: return string("SQLite3: SQLITE_IOERR_DIR_FSYNC");
	case SQLITE_IOERR_TRUNCATE: return string("SQLite3: SQLITE_IOERR_TRUNCATE");
	case SQLITE_IOERR_FSTAT: return string("SQLite3: SQLITE_IOERR_FSTAT");
	case SQLITE_IOERR_UNLOCK: return string("SQLite3: SQLITE_IOERR_UNLOCK");
	case SQLITE_IOERR_RDLOCK: return string("SQLite3: SQLITE_IOERR_RDLOCK");
	case SQLITE_IOERR_DELETE: return string("SQLite3: SQLITE_IOERR_DELETE");
	case SQLITE_IOERR_BLOCKED: return string("SQLite3: SQLITE_IOERR_BLOCKED");
	case SQLITE_IOERR_NOMEM: return string("SQLite3: SQLITE_IOERR_NOMEM");
	case SQLITE_IOERR_ACCESS: return string("SQLite3: SQLITE_IOERR_ACCESS");
	case SQLITE_IOERR_CHECKRESERVEDLOCK: return string("SQLite3: SQLITE_IOERR_CHECKRESERVEDLOCK");
	case SQLITE_IOERR_LOCK: return string("SQLite3: SQLITE_IOERR_LOCK");
	case SQLITE_IOERR_CLOSE: return string("SQLite3: SQLITE_IOERR_CLOSE");
	case SQLITE_IOERR_DIR_CLOSE: return string("SQLite3: SQLITE_IOERR_DIR_CLOSE");
	case SQLITE_IOERR_SHMOPEN: return string("SQLite3: SQLITE_IOERR_SHMOPEN");
	case SQLITE_IOERR_SHMSIZE: return string("SQLite3: SQLITE_IOERR_SHMSIZE");
	case SQLITE_IOERR_SHMLOCK: return string("SQLite3: SQLITE_IOERR_SHMLOCK");
	case SQLITE_IOERR_SHMMAP: return string("SQLite3: SQLITE_IOERR_SHMMAP");
	case SQLITE_IOERR_SEEK: return string("SQLite3: SQLITE_IOERR_SEEK");
	case SQLITE_IOERR_DELETE_NOENT: return string("SQLite3: SQLITE_IOERR_DELETE_NOENT");
	case SQLITE_IOERR_MMAP: return string("SQLite3: SQLITE_IOERR_MMAP");
	case SQLITE_IOERR_GETTEMPPATH: return string("SQLite3: SQLITE_IOERR_GETTEMPPATH");
	case SQLITE_IOERR_CONVPATH: return string("SQLite3: SQLITE_IOERR_CONVPATH");
	case SQLITE_LOCKED_SHAREDCACHE: return string("SQLite3: SQLITE_LOCKED_SHAREDCACHE");
	case SQLITE_BUSY_RECOVERY: return string("SQLite3: SQLITE_BUSY_RECOVERY");
	case SQLITE_BUSY_SNAPSHOT: return string("SQLite3: SQLITE_BUSY_SNAPSHOT");
	case SQLITE_CANTOPEN_NOTEMPDIR: return string("SQLite3: SQLITE_CANTOPEN_NOTEMPDIR");
	case SQLITE_CANTOPEN_ISDIR: return string("SQLite3: SQLITE_CANTOPEN_ISDIR");
	case SQLITE_CANTOPEN_FULLPATH: return string("SQLite3: SQLITE_CANTOPEN_FULLPATH");
	case SQLITE_CANTOPEN_CONVPATH: return string("SQLite3: SQLITE_CANTOPEN_CONVPATH");
	case SQLITE_CORRUPT_VTAB: return string("SQLite3: SQLITE_CORRUPT_VTAB");
	case SQLITE_READONLY_RECOVERY: return string("SQLite3: SQLITE_READONLY_RECOVERY");
	case SQLITE_READONLY_CANTLOCK: return string("SQLite3: SQLITE_READONLY_CANTLOCK");
	case SQLITE_READONLY_ROLLBACK: return string("SQLite3: SQLITE_READONLY_ROLLBACK");
	case SQLITE_READONLY_DBMOVED: return string("SQLite3: SQLITE_READONLY_DBMOVED");
	case SQLITE_ABORT_ROLLBACK: return string("SQLite3: SQLITE_ABORT_ROLLBACK");
	case SQLITE_CONSTRAINT_CHECK: return string("SQLite3: SQLITE_CONSTRAINT_CHECK");
	case SQLITE_CONSTRAINT_COMMITHOOK: return string("SQLite3: SQLITE_CONSTRAINT_COMMITHOOK");
	case SQLITE_CONSTRAINT_FOREIGNKEY: return string("SQLite3: SQLITE_CONSTRAINT_FOREIGNKEY");
	case SQLITE_CONSTRAINT_FUNCTION: return string("SQLite3: SQLITE_CONSTRAINT_FUNCTION");
	case SQLITE_CONSTRAINT_NOTNULL: return string("SQLite3: SQLITE_CONSTRAINT_NOTNULL");
	case SQLITE_CONSTRAINT_PRIMARYKEY: return string("SQLite3: SQLITE_CONSTRAINT_PRIMARYKEY");
	case SQLITE_CONSTRAINT_TRIGGER: return string("SQLite3: SQLITE_CONSTRAINT_TRIGGER");
	case SQLITE_CONSTRAINT_UNIQUE: return string("SQLite3: SQLITE_CONSTRAINT_UNIQUE");
	case SQLITE_CONSTRAINT_VTAB: return string("SQLite3: SQLITE_CONSTRAINT_VTAB");
	case SQLITE_CONSTRAINT_ROWID: return string("SQLite3: SQLITE_CONSTRAINT_ROWID");
	case SQLITE_NOTICE_RECOVER_WAL: return string("SQLite3: SQLITE_NOTICE_RECOVER_WAL");
	case SQLITE_NOTICE_RECOVER_ROLLBACK: return string("SQLite3: SQLITE_NOTICE_RECOVER_ROLLBACK");
	case SQLITE_WARNING_AUTOINDEX: return string("SQLite3: SQLITE_WARNING_AUTOINDEX");
	default : return string("--UNKNOWN ERROR-- !!");
	}

}
