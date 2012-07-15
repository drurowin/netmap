#ifndef _hostclass_h_
#define _hostclass_h_

#include <string>
#include <list>

#include "misc.h"

class HostClass {
    friend ostream& operator<<(ostream&, const HostClass&);
public:
    HostClass() : unused(false), unknown(true), unknown_hosts(1), resolved(false)
	{ ip = "unknown_" + int_to_str(++unknown_count); }
    HostClass(const string ipstr, int fn=0)
	: ip(ipstr), unused(false), unknown(false), resolved(false),
	  firstnode(fn), nr_ping(0), tot_ping(0) 
	{ resolv_hostname(); }
    string name() const { return (resolved?hostname:ip); }

    int host_count() const;

    void add_ping(const float& fl) { tot_ping += fl; nr_ping++; }
    float ping() const { return tot_ping / nr_ping; }
	
    string ip;
    string hostname;

    bool unused;
    bool unknown;
    int  unknown_hosts;
private:
    static int unknown_count; //unique identifier for each invaild host
    
    bool resolved;
    bool firstnode;

    int nr_ping;    
    float tot_ping;
    
    void resolv_hostname();
public:
    bool operator==(const HostClass& hc) { return (ip == hc.ip); }
};



struct HostLink {
    HostClass* a;
    HostClass* b;

    bool operator==(HostLink& hl)
	{ return a == hl.a && b == hl.b; }

    bool operator <(HostLink& hl)
	{ return a==hl.a ? b < hl.b : a < hl.a; }
};

ostream& operator<<(ostream&, const HostLink&);

typedef list<HostClass> HostList;
typedef list<HostClass>::iterator HostList_iter;
typedef list<HostClass>::const_iterator CHostList_iter;

typedef list<HostLink> LinkList;
typedef list<HostLink>::iterator LinkList_iter;
typedef list<HostLink>::const_iterator CLinkList_iter;

const string inv_ip = "<<invalid-ip>>";

HostClass* get_hostclass(const string&, int=0);
void addtonetwork(const string&, const string&, int=0);
string create_unknown_node(const string&);
void clean_up_unknown();

extern HostList hostlist;
extern LinkList network;

#endif
