CPP = g++
CC = gcc
CPPOPT = -std=c++0x -O3 -Wall
CPPLIBOPT = -std=c++0x -fPIC -O3 -Wall -c
CCLIBOPT = -fPIC -O0 -g3 -pg -Wall -c
CPPSHAREDOPT = -shared -pg
CPPLIB =  -lproxyReaperlib -lcurlpp -lpthread
CPPLIBLIB = -lcurl -lsqlite3 -lidn -lcurlpp -lboost_regex -lboost_iostreams

proxyReaper : main.cpp libproxyReaperlib.so
	$(CPP) $(CPPOPT) -c main.cpp -o main.o
	$(CPP) $(CPPOPT) -L"./" -o proxyReaper main.o -Wl,-rpath '-Wl,$$ORIGIN' $(CPPLIB)

libproxyReaperlib.so : proxyReaperHost.o proxyReaperExceptions.o proxyReaperPersist.o proxyReaperProxy.o proxyReaperSource.o proxyReaperUrl.o nproc.o
	$(CPP) $(CPPSHAREDOPT) -o libproxyReaperlib.so ./nproc.o ./proxyReaperExceptions.o ./proxyReaperHost.o ./proxyReaperPersist.o ./proxyReaperProxy.o ./proxyReaperSource.o ./proxyReaperUrl.o $(CPPLIBLIB)  

proxyReaperHost.o : lib/proxyReaperHost.h lib/proxyReaperHost.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperHost.o lib/proxyReaperHost.cpp

proxyReaperExceptions.o : lib/proxyReaperExceptions.h lib/proxyReaperExceptions.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperExceptions.o lib/proxyReaperExceptions.cpp

proxyReaperPersist.o : lib/proxyReaperPersist.h lib/proxyReaperPersist.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperPersist.o lib/proxyReaperPersist.cpp

proxyReaperProxy.o : lib/proxyReaperProxy.h lib/proxyReaperProxy.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperProxy.o  lib/proxyReaperProxy.cpp

proxyReaperSource.o : lib/proxyReaperSource.h lib/proxyReaperSource.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperSource.o lib/proxyReaperSource.cpp

proxyReaperUrl.o : lib/proxyReaperUrl.h lib/proxyReaperUrl.cpp
	$(CPP)  $(CPPLIBOPT) -o proxyReaperUrl.o lib/proxyReaperUrl.cpp

nproc.o : lib/nproc.h lib/nproc.c
	$(CC) $(CCLIBOPT) -o nproc.o lib/nproc.c

clean:
	rm proxyReaper libproxyReaperlib.so *.o
