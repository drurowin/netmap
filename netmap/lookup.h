// Implements a class that:
//		- can do lookups
//		- caches all lookups
//		- read cache files on construction
//		- write cahce files on destruction

#ifndef _lookup_h_
#define _lookup_h_

#include <string>
#include <list>

struct HostRec {
    string ip;
    string hostname;

    bool operator==(const HostRec& hr)
	{ return ip == hr.ip && hostname == hr.hostname; }

    bool operator<(const HostRec& hr)
	{ return ip==hr.ip ? hostname<hr.hostname : ip<hr.ip; }
};

typedef list<HostRec> HostCache;
typedef list<HostRec>::iterator HostCache_iter;
typedef list<HostRec>::const_iterator CHostCache_iter;

typedef list<string> IpList;
typedef list<string>::iterator IpList_iter;
typedef list<string>::const_iterator CIpList_iter;

class LookUp {
public:
    LookUp();
    ~LookUp();

    void init_cache();
    void write_cache();
    
    string lookup(const string&);
private:
    void read_hostfile(const string&);
    void write_hostfile(const string&);
    
    string resolv(const string&);
    HostRec* ip_cache_lookup(const string&);
    HostRec* is_falseduplicate(const HostRec&);
    bool ip_failed_lookup(const string&);

    void read_failedip(const string&);
    void write_failedip(const string&);
    
    HostCache hc;
    IpList failed;
};

extern LookUp lu;

#endif
