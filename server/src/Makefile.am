bin_PROGRAMS = server
server_SOURCES = \
	account.cpp account.h \
	configfile.cpp configfile.h \
	database.cpp database.h \
	dbsqlite3.cpp dbsqlite3.h \
	packet.cpp packet.h \
	player.cpp player.h \
	protocol.cpp protocol.h \
	protspec.h \
	server.cpp \
	serversocket.cpp serversocket.h \
	usermanager.cpp usermanager.h

server_CPPFLAGS = -Wall -Wno-sign-compare -g -I/usr/include/libxml2
server_LDADD = -lxml2 -lboost_thread -lsqlite3
