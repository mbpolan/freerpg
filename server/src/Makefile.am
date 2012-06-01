bin_PROGRAMS = server
server_SOURCES = \
	account.cpp account.h \
	configfile.cpp configfile.h \
	database.cpp database.h \
	dbsqlite3.cpp dbsqlite3.h \
	engine.cpp engine.h \
	map.cpp map.h \
	maploader.h \
	maploaderxml.cpp maploader.h \
	packet.cpp packet.h \
	player.cpp player.h \
	protocol.cpp protocol.h \
	protspec.h \
	server.cpp \
	serversocket.cpp serversocket.h \
	tile.cpp tile.h \
	tilesetloader.cpp tilesetloader.h 

server_CPPFLAGS = -Wall -Wno-sign-compare -g -I/usr/include/libxml2
server_LDADD = -lxml2 -lboost_thread -lsqlite3
