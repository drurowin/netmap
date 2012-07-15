#include "config.h"

// This file defines the default values for the netmap configuration options

// dotty output file
string outfile = "./netmap.dot";

// directory with makelist output (see makelist/config.h)
string datadirName = "../netdata/";

// try to resolv the ip-address, runs 30 times faster when false :) 
bool resolv_hosts = true;

// max number of hops on the drawing, 0 for inf
int max_hops = 0;

//presents the graph in a tree structure, better for large graphs 
bool make_digraph = false;

int verbose_lvl = 0;

string hostcache_file = "./netmap.hostcache";
string failedip_file  = "./netmap.failedip";

list<string> extra_hostcaches;

bool keep_reading = true;
