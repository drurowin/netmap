#include <strstream>
#include <iostream>

#include <netdb.h> 	//hostent def

//from belgolib
#include <debug.h>
#include <files.h>


#include "lookup.h"
#include "misc.h"
#include "config.h"

LookUp lu;

/////////////////////////
//
// public lookup members

LookUp::LookUp() {}

void LookUp::init_cache() 
{
    //init cache

    cout << indent(1) << "Reading lookup cache" << endl;
    
    // TODO: iter over a list that the user can add to with a command line
    // switch
    read_hostfile(hostcache_file);

    for(list<string>::iterator e_iter = extra_hostcaches.begin();
	e_iter != extra_hostcaches.end(); e_iter++)
	read_hostfile(*e_iter);

    read_failedip(failedip_file);
}

LookUp::~LookUp() {}

void LookUp::write_cache() 
{
    
    //write cache
    cout << indent(1) << "Writing lookup cache" << endl;

    write_hostfile(hostcache_file);

    write_failedip(failedip_file);
}


string LookUp::lookup(const string& ip) 
{
    //search in chache first ...
    if(HostRec *hr = ip_cache_lookup(ip))
	return hr->hostname;

    if(ip_failed_lookup(ip))
	return string();

    //... nothing there, do lookup and add it to the cache
    HostRec hr; hr.ip = ip;

    hr.hostname = resolv(ip);

    if(hr.hostname.empty())
	failed.push_back(hr.ip);
    else
	hc.push_back(hr);

    return hr.hostname;
}


/////////////////////////
//
// private lookup members

void LookUp::read_hostfile(const string& fname) 
{
    if(verbose_lvl)
	cout << indent(2) << "Reading " << fname << endl;
    
    Infile cfile(fname, ios::binary | ios::in);

    while(cfile && !cfile.eof()) {
	HostRec hr;
	cfile >> hr.ip;
	if(!is_validip(hr.ip)) {
	    skiptillendofline(cfile);
	    continue;
	}

	cfile >> hr.hostname;

	if(hr.hostname.empty()) {
	    skiptillendofline(cfile);
	    continue;
	}

	if(HostRec* hrp = is_falseduplicate(hr)) {
	    if(cdb) {
		dbgs << "************************************************"
		     << endl;
		dbgs << "WARNING: Found false duplicate in " << fname << endl;
		dbgs << "File says: (" << hr.ip << ", " << hr.hostname << ")"
		     << endl;
		dbgs << "Cache says: (" << hrp->ip << ", " << hr.hostname
		     << ")" << endl;
		dbgs << "************************************************"
		     << endl;
	    }
	    
	    cout << "Found false duplicate in " << fname << ": (" << hr.ip
		 << ", " << hr.hostname << ")" << endl;
	    cout << "   while cache says: " << hrp->ip << ", " << hrp->hostname
		 << ")" << endl;
	} else
	    hc.push_back(hr);
	
	skiptillendofline(cfile);
    }

    hc.sort();
    hc.unique();
}

void LookUp::write_hostfile(const string& hostfile) 
{
    hc.sort();
    hc.unique();
    
    Outfile os(hostfile);

    for(HostCache_iter hc_iter = hc.begin(); hc_iter != hc.end(); hc_iter++)
	os << hc_iter->ip << "\t" << hc_iter->hostname << endl;
}	

HostRec* LookUp::ip_cache_lookup(const string& ip) 
{
    for(HostCache_iter hc_iter = hc.begin(); hc_iter != hc.end(); hc_iter++)
	if(hc_iter->ip == ip)
	    return &(*hc_iter);

    return 0;
}

HostRec* LookUp::is_falseduplicate(const HostRec& hr) 
{
    if(HostRec* hrp = ip_cache_lookup(hr.ip))
	if(hrp->hostname != hr.hostname)
	    return hrp;
    
    return 0;
}

bool LookUp::ip_failed_lookup(const string& ip) 
{
    for(IpList_iter f_iter = failed.begin(); f_iter != failed.end(); f_iter++)
	if(*f_iter == ip)
	    return true;
    return false;
}

static void transform_ip(const string& ip, char result[4]) 
{
    for(int i=0; i<4; i++)
	result[i] = 0;    

    int currpos=0;
    for(unsigned i=0; i < ip.length(); i++)
	if(ip[i] >= '0'  && ip[i] <= '9')
	    result[currpos] = result[currpos] * 10 + (ip[i] - '0');
	else if(ip[i] == '.')
	    currpos++;
	else
	    //chicken out
	    return;
}


string LookUp::resolv(const string& ip) 
{    
    if(!resolv_hosts) return string();
    
    static int first_run = 1;
    if(first_run) {
	first_run = 0;
	sethostent(1);
    }

    char res_ip[4];
    transform_ip(ip, res_ip);

    if(verbose_lvl) {
	cout << indent(4) << "Resolving " << ip << " : ";
	cout.flush();
    }
    
    if(cdb>d_detail) {
	dbgs << " * Resolving " << ip << " : ";
	dbgs.flush();
    }

    hostent* he = gethostbyaddr(res_ip, 4, AF_INET);

    if(!he) {
	if(verbose_lvl)
	    cout << " lookup FAILED !" << endl;
	
	if(cdb>d_detail)
	    dbgs << "FAILED!" << endl;
	return string();
    }

    string result;
    result = he->h_name;
    
    if(verbose_lvl) 
	cout << result << endl;
    
    if(cdb>d_detail)
	dbgs << result << endl;

    return result;
}

void LookUp::read_failedip(const string& fname) 
{
    Infile cfile(fname, ios::binary | ios::in);

    while(cfile && !cfile.eof()) {
	string ip;
	cfile >> ip;
	if(!is_validip(ip)) {
	    skiptillendofline(cfile);
	    continue;
	}

	failed.push_back(ip);
	skiptillendofline(cfile);
    }

    failed.sort();
    failed.unique();
}

void LookUp::write_failedip(const string& fname) 
{
    failed.sort();
    failed.unique();
    
    Outfile os(fname);

    for(IpList_iter f_iter = failed.begin(); f_iter != failed.end(); f_iter++)
	os << *f_iter << endl;
}

	
	
	
