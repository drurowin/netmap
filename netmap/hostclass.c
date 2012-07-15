#include <strstream>
#include <map>

//from belgolib
#include <debug.h>

#include "hostclass.h"
#include "misc.h"
#include "lookup.h"
#include "config.h"

HostList hostlist;
LinkList network;

int HostClass::unknown_count = 0;

//////////////////
//
//  HostClass

void HostClass::resolv_hostname() 
{
    
    hostname = lu.lookup(ip);

    if(!hostname.empty())
	resolved = true;
    
}

//gives a weigth for a node
int HostClass::host_count() const
{
    //kick out doubles
    network.sort(); network.unique();

    //run through network and count the number of occurences
    int hit_count = 0;
    for(LinkList_iter li = network.begin(); li != network.end(); li++) 
	if(li->a->ip == ip || li->b->ip == ip)
	    hit_count++;

    return hit_count;
}   

ostream& operator<<(ostream& os, const HostClass& hc) 
{
    if(hc.unused) return os;

    os << indent(2) << '"' << hc.ip << "\" [ ";

    if(hc.unknown) {
	os << "label =\"";
	if(hc.unknown_hosts>1)
	    os << hc.unknown_hosts << " ";
	os << "Unknown\\nhost";
	if(hc.unknown_hosts>1)
	    os << 's';
	os << "\", shape=triangle, color=red]" << endl;
      
	return os;
    }

    os << " label = \"" << hc.name();
    
    if(hc.resolved)
	os << "\\n" << hc.ip;
    if(hc.nr_ping)
	os << "\\n avg_ping: " << hc.ping() << " ms";
    
    os << "\", ";

    const int host_count = hc.host_count();

    os << "shape = ";
    if(hc.firstnode)
	os << "parallelogram, color=gold";
    else {
	if(host_count > 2)
	    os << "box, ";
	else
	    os << "ellipse,";

	os << " color = ";
	if(host_count > 7)
	    os << "maroon";
	else if(host_count > 6)
	    os << "crimson";
	else if(host_count > 5)
	    os << "orangered";
	else if(host_count > 4)
	    os << "orange";
	else if(host_count > 3)
	    os << "lightsalmon";
	else 
	    os << "cornsilk";
	
    
	/*
	  if(host_count > 6)
	  os << "crimson";
	  else if(host_count > 5)
	  os << "coral";
	  else if(host_count > 4)
	  os << "darksalmon";
	  else if(host_count > 3)
	  os << "lightsalmon";
	  else if(host_count > 2)
	  os << "bisque";
	  else 
	  os << "cornsilk";
    	*/
    }
    
    os << "]" << endl;
    
    return os;
}	

//////////////////////
//
// HostLink members

ostream& operator<<(ostream& os, const HostLink& hl)  
{
    os << indent(2) << '"' << hl.a->ip << '"'
       << (make_digraph?" -> ":" -- ") << '"' << hl.b->ip
       << "\";" << endl;
    return os;
}

////////////////////////////////
//
// Misc HostClass functions

static HostClass* find_host(HostList& hl, const string& ip) 
{
    for(HostList_iter hl_iter = hl.begin(); hl_iter != hl.end(); hl_iter++)
	if(hl_iter->ip == ip) 
	    return &(*hl_iter); //a bit weird :)

    return 0;
}

HostClass* get_hostclass(const string& ip, int firstnode) 
{
    if(HostClass* hc = find_host(hostlist, ip)) 
	return hc;

    HostClass hc(ip, firstnode);
    hostlist.push_back(hc);

    return find_host(hostlist, ip);
}

void addtonetwork(const string& ip1, const string& ip2, int firstnode) 
{
    HostClass* h1 = get_hostclass(ip1, firstnode);
    HostClass* h2 = get_hostclass(ip2);

    HostLink hl = { h1, h2 };

    if(verbose_lvl)
	cout << indent(3) << "Added link: " << h1->name() << " --> "
	     << h2->name() << endl;
    
    network.push_back(hl);
}

string create_unknown_node(const string& prev_node) 
{
    static HostClass* prev_res = 0;

    if(prev_res && prev_res->ip == prev_node) {
	
	prev_res->unknown_hosts++;
	return prev_node;
    }
    
    HostClass hu;
    hostlist.push_back(hu);

    prev_res = find_host(hostlist, hu.ip);
    if(!prev_res)
	cout << "BIGMISTAKE" << endl;

    return hu.name();
}


typedef list<HostClass*> HostPList;
typedef list<HostClass*>::iterator HostPList_iter;

typedef map<HostClass*, HostPList> LinkMap;
typedef map<HostClass*, HostPList>::iterator LinkMap_iter;

static void fill_maps(LinkList& hl, LinkMap& to, LinkMap& from) 
{
    for(LinkList_iter hl_iter = hl.begin(); hl_iter != hl.end(); hl_iter++) {
	if(hl_iter->a->unknown)
	    to[hl_iter->b].push_back(hl_iter->a);

	if(hl_iter->b->unknown)
	    from[hl_iter->a].push_back(hl_iter->b);
    }
}


HostPList trash; //nodes that should be deleted later on



//  static void print_hostplist(ostream& os, HostPList& hp) 
//  {
//      os << "{";
//      int first=1;
//      for(HostPList_iter hp_iter = hp.begin(); hp_iter != hp.end(); hp_iter++) {
//  	if(first) first=0;
//  	else os << ", ";
//  	os << (*hp_iter)->name();
//      }
//      os << "}";
//  }


static void equilize_names(LinkList& n, HostPList& hl) 
{
    HostClass* firstinline = 0;

    for(LinkList_iter n_iter = n.begin(); n_iter != n.end(); n_iter++) {
	bool con_a = contains(hl, n_iter->a);
	bool con_b = contains(hl, n_iter->b);
	if(con_a || con_b) {
	    if(!firstinline) 
		firstinline = (con_a?n_iter->a:n_iter->b);
	    else {
		if(con_a && n_iter->a->name() != firstinline->name()) {
		    trash.push_back(n_iter->a);
		    n_iter->a = firstinline;
		}

		if(con_b && n_iter->b->name() != firstinline->name()) {
		    trash.push_back(n_iter->b);
		    n_iter->b = firstinline;
		}
	    }
	}
    }
}

static void remove_from_list(HostList& hl, HostPList& ht) 
{
    for(HostList_iter hl_iter = hl.begin(); hl_iter != hl.end(); hl_iter++) 
	if(contains(ht, &(*hl_iter)))
	    hl_iter->unused = true;
	
}

void clean_up_unknown() 
{
    if(verbose_lvl)
	cout << indent(2) << "Cleaning up invalid hosts." << endl;
    
    // create a list of all links from an unknown to a node, and a list of
    // links from a node to an unkown and store them in a map
    LinkMap to_u;
    LinkMap from_u;
    fill_maps(network, to_u, from_u);
    
    // give all members of an non-empty intersection, of two elements of both
    // maps, the same name
    for(LinkMap_iter to_iter = to_u.begin(); to_iter != to_u.end(); to_iter++)
	for(LinkMap_iter from_iter = from_u.begin(); from_iter != from_u.end();
	    from_iter++)
	    if(to_iter->first->name() != from_iter->first->name()) {
		
		HostPList hl = intersect(to_iter->second, from_iter->second);
		
		if(!hl.empty())
		    equilize_names(network, hl);
	    }
    
    // clean up trash
    remove_from_list(hostlist, trash);

    network.sort();
    network.unique();    
}

