#include <strstream>
#include <iostream>
#include <string>

#include <stdlib.h>

#include "args.h"
#include "misc.h"
#include "config.h"

static void disp_usage() 
{
    cout << "Usage: netmap <options>"  << endl << endl;
    cout << "    Options:" << endl;
    cout << indent(1) << "-V\t\t: Display version and exit" << endl;
    cout << indent(1) << "-v\t\t: Be verbose" << endl;
    cout << indent(1) << "-o <outfile.dot>: Name for dotty output file ("
	 << outfile << ")" << endl;
    cout << indent(1) << "-d <datadir>\t: Specify the directory containing the "
	 << "makelist output" << endl
	 << "\t\t  (" << datadirName << ")" << endl;
    cout << indent(1) << "-m <n>\t: Specify the maximum number of hops (inf)"
	 << endl;
    cout << indent(1) << "-i\t\t: Generate directional graph" << endl;    
    cout << indent(1) << "-n\t\t: Don't resolve hostnames" << endl;
    cout << indent(1) << "-I <hostfile>: Add <hostfile> to hostcache. " 
	"Multiple files, require" << endl;
    cout << "\t\t  multiple -I's (See " << hostcache_file
	 << " for layout)" << endl;
    cout << indent(1) << "-C <cachedipfile>: File used as hostcache ("
	 << hostcache_file << ")" << endl;
    cout << indent(1) << "-F <failedipfile>: File used for failed ips ("
	 << failedip_file << ")" << endl;
    cout << indent(1) << "-s\t\t: Stop reading traceroute output when an "
	"invalid host" << endl << "\t\t  is encountered." << endl;
    cout << endl;
    cout << "    Note: The defaults values can be edited in config.c"
	 << endl << endl;
    
    
    exit(-1);
}

static void disp_version() 
{
    cout << "  2001 - Wouter Godefroy" << endl;
    exit(0);
}

static void set_string_arg(strstream& ss, string& val) 
{
    string argvalue;
    ss >> argvalue;

    if(!argvalue.length())
	disp_usage();
    
    val = argvalue;
}

static void process(const string& argline) 
{
    strstream ss;
    ss << argline;

    string arg;
    ss >> arg;
    while(arg.length() && !ss.eof()) {
	if(arg == "-V")
	    disp_version();
	else if(arg == "-v")
	    verbose_lvl += 1;
	else if(arg == "-o")
	    set_string_arg(ss, outfile);
	else if(arg == "-d")
	    set_string_arg(ss, datadirName);
	else if(arg == "-m") {
	    string argval;
	    set_string_arg(ss, argval);
	    if(!str2int(argval, max_hops))
		disp_usage();
	} else if(arg == "-i")
	    make_digraph = true;
	else if(arg == "-n") 
	    resolv_hosts = false;
	else if(arg == "-I") {
	    string hostfile;
	    set_string_arg(ss, hostfile);
	    extra_hostcaches.push_back(hostfile);
	} else if(arg == "-s")
	    keep_reading = false;
	else if(arg == "-C") 
	    set_string_arg(ss, hostcache_file);
	else if(arg == "-F")
	    set_string_arg(ss, failedip_file);
	else
	    disp_usage();
	
	arg.empty();
	ss >> arg;
    }

}


void process_args(int argc, char* argv[]) 
{
    if(argc < 2)
	return;

    string argline;
    for(int i = 1; i < argc; i++) 
	argline += argv[i] + string(" ");

    process(argline);
}
