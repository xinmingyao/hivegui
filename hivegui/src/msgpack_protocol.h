#if !defined(MSPACK_PROTOCOL__)
#define MSPACK_PROTOCOL__

#include "stdafx.h"
using namespace std ;
class User {
public:
	User(){}
	User(string n,string p) : name(n),pwd(p) 
	{ }
	int from; // hive will send msg to this handle
	int tag;    // message tag
	string name;
    string pwd;
	msgpack::type::raw_ref r;
	MSGPACK_DEFINE(from,tag,name,pwd,r);
};

#endif

