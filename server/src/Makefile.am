bin_PROGRAMS = server
server_SOURCES = \
	configfile.cpp configfile.h \
	packet.cpp packet.h \
	protocol.cpp protocol.h \
	server.cpp \
	serversocket.cpp serversocket.h \
	usermanager.cpp usermanager.h

server_CPPFLAGS = -Wall -Wno-sign-compare -g -I/usr/include/libxml2
server_LDADD = -lxml2 -lboost_thread


