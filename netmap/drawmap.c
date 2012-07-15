#include <strstream>
#include <iostream>

#include <fstream>

//belgolib
#include <files.h>


#include "drawmap.h"
#include "hostclass.h"
#include "misc.h"
#include "config.h"


void init_dot(Outfile& os) 
{
    os << (make_digraph?"digraph":"graph") << " \"NetMap\" { " << endl;
    os << indent(1) << "node [ color = red, shape=triangle, "
	"label=\"Invalid\\nhost\", style=filled ];" << endl;
    //os << indent(1) << "node [ color = black," << endl;
    //os << indent(2) << "style = filled ];" << endl;
}

void close_dot(Outfile& os) 
{
    os << "}" << endl;
}


void draw_map(const string& fname) 
{
    cout << indent(1) << "Generating dotty output." << endl;
    
    Outfile os(fname);

    init_dot(os);

    //kick out duplicates
    if(verbose_lvl)
	cout << indent(2) << "Removing duplicates from network." << endl;
    
    network.sort();
    network.unique();

    if(keep_reading)
	clean_up_unknown();

    if(verbose_lvl)
	cout << indent(2) << "Drawing map." << endl;
    
    for(HostList_iter hiter = hostlist.begin(); hiter != hostlist.end();
	hiter++) 
	os << (*hiter);
	
    
    for(LinkList_iter niter = network.begin(); niter != network.end(); niter++)
	os << (*niter);

    close_dot(os);
}
