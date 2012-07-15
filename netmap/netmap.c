#include <strstream>
#include <iostream>
#include <fstream>

//belgolib
#include <debug.h>
#include <dirs.h>
#include <files.h>


#include "hostclass.h"
#include "drawmap.h"
#include "misc.h"
#include "args.h"
#include "lookup.h"
#include "config.h"

Dbgfile dbgs;
int cdb = 1;

static void copy_line(strstream& os, istream& inf) 
{
    string result;

    char ch;
    while(inf && !inf.eof() && inf.get(ch) && ch != '\n')
	result += ch;

    os << result;
}


int count_invalid_hosts(const string& fname) 
{
    Infile inf(fname, ios::binary | ios::in);

    int invalid_hosts = 0;
    int invalid_left = 0;
    while(inf && !inf.eof()) {
	string ip;
	inf >> ip;
	if(is_validip(ip)) {
	    invalid_hosts += invalid_left;
	    invalid_left = 0;
	} else
	    invalid_left++;

	skiptillendofline(inf);
    }

    if(cdb>d_detail)
	dbgs << "found	 " << invalid_hosts << " invalid hosts in " << fname
	     << endl; 
    
    return invalid_hosts;
}

static void process_filelist(const string& fname) 
{
    const int num_invalid_hosts =
	(keep_reading?count_invalid_hosts(fname):0);
    
    Infile inf(fname, ios::binary | ios::in);

    int mark_first = 1;
    string prev_addr = inv_ip;
    int nr_hops = 0;
    int invalid_hosts_found = 0;
    while(inf && !inf.eof()) {
	strstream line;
	copy_line(line, inf);
	
	if(max_hops && ++nr_hops > max_hops) {
	    if(verbose_lvl)
		cout << indent(3) << "Max hops, skipping rest" << endl;
	    if(cdb>d_list)
		dbgs << "Max hops for " << fname << ", skipping rest of file"
		     << endl;
	    return;
	}
	
	string ip_addr;
	line >> ip_addr;

	bool ip_addr_unknown = false;
	
	if(!is_validip(ip_addr)) {
	    if(!keep_reading) {
		if(verbose_lvl)
		    cout << indent(3) << "Invalid ip, skipping rest of file."
			 << endl;
		if(cdb>d_list)
		    dbgs << "Invalid ip (" << ip_addr << ") in " << fname
			 << endl;
		return;
	    }

	    // check there are still some valid nodes left in the file,
	    // otherwise skip rest of file
	    if(++invalid_hosts_found > num_invalid_hosts) {
		    if(verbose_lvl)
			cout << indent(3) << "No valid ips left in file, skipping "
			    "rest" << endl;
		    if(cdb>d_list)
			dbgs << "No valid ips left in file, skipping rest" << endl;

		    return;
		}
	    
	    // add unknown node for this
	    ip_addr = create_unknown_node(prev_addr);
	    ip_addr_unknown = true;

	    if(verbose_lvl)
		cout << indent(3) << "Invalid ip, added unknown node: "
		     << ip_addr << endl;
	    if(cdb>d_list)
		dbgs << "Added unknown node (" << ip_addr
		     << ") for invalid ip-address in " << fname << endl;
	} else
	    ip_addr_unknown = false;

	if(prev_addr != inv_ip && prev_addr != ip_addr) {
	    if(cdb>d_list)
		dbgs << "Adding connection: " << ip_addr << " --> " << prev_addr
		     << endl;
	    
	    addtonetwork(prev_addr, ip_addr, mark_first);
	    mark_first = 0;
	} else if(cdb>d_list)
	    dbgs << "Skipping link: " << ip_addr << " --> " << prev_addr
		 << endl;
	
	prev_addr = ip_addr; 

	//try to store the ping times for this host
	if(!ip_addr_unknown) {
	    HostClass* hc = get_hostclass(ip_addr, mark_first);
	    mark_first = 0;
	    string time;
	    for(int i=0; i < 3; i++) {
		line >> time;
		float ptime;
		if(str2float(time, ptime))
		    hc->add_ping(ptime);
	    }
	}   
    }
}


int main(int argc, char* argv[])
{

    cout << version << endl;

    if(cdb)
	dbgs.Open("dbgout.txt");
    
    if(argc > 1)
	process_args(argc, argv);

    lu.init_cache();
    
    cout << indent(1) << "Reading datadir: " << datadirName << endl;
    Dirc datadir(datadirName);

	datadir.read_dir(datadirName);
    FileList filelist = datadir.all_files();

    for(CFiliter fl_iter = filelist.begin(); fl_iter != filelist.end();
	fl_iter++) {
	if(verbose_lvl)
	    cout << indent(2) << "Reading: " << (*fl_iter)->fname() << endl;
	
	process_filelist((*fl_iter)->full_path());
    }
    
    draw_map(outfile);
    lu.write_cache();
    
    return 0;
}
