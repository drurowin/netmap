#include <iostream>
#include <stdlib.h>

//from belgolib
#include "debug.h"
#include "files.h"

#include "args.h"
#include "config.h"

Dbgfile dbgs;
int cdb = 0;

int main(int argc, char* argv[]) 
{
    if(cdb)
	dbgs.Open("dbgout.txt");
    
    if(argc > 1)
	process_args(argc, argv);
    
    Infile inf(infile);
    
    string ip;
    int par_launched = 0;
    while(inf && !inf.eof()) {
	inf >> ip;

	string outfile = outdir + "/" + ip + ".txt";
	string cmdstring;
	if(use_mtr)
	    cmdstring = "mtr -r -c 3 -n " + ip
		+ " | grep -v HOST "
		+ " | awk ' { print $1 \" \" $6 } ' > " + outfile;
	else
	    cmdstring = string("traceroute -n ") + (traceroute_I?"-I ":"") + ip 
	    + " | awk ' { print $2, $3, $5, $7 } ' > " + outfile;


	if(par_launched < par_traceroute) {
	    cmdstring += "&";
	    par_launched++;
	} else
	    par_launched = 0;

	if(cdb>d_list)
	    dbgs << "Executing command: " << cmdstring << endl;


	if(be_verbose)
	    cout << "Tracing: " << ip << endl;
	
	system(cmdstring.c_str());
    }

    return 0;
}
