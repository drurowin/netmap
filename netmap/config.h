#ifndef _config_h_
#define _config_h_

#include <list>
#include <string>

using namespace std;

const string version = "NetMap 0.1.3 20100106";


extern string outfile;
extern string datadirName;
extern bool resolv_hosts;
extern int max_hops;
extern bool make_digraph;

extern int verbose_lvl;

extern string hostcache_file;
extern string failedip_file;

extern list<string> extra_hostcaches;

extern bool keep_reading;

#endif
