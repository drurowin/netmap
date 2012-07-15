#include <strstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "args.h"
#include "config.h"

static string indent(unsigned lvl) 
{
    string result;
    
    while(lvl--)
	result += "   ";

    return result;
}

static void disp_usage() 
{
    cout << "Usage: makelist <options>" << endl << endl;
    cout << "  Options:" << endl;
    cout << indent(1) << "-p n\t\t: Specify number of traceroutes to run in "
	"parallel (" << par_traceroute << ")" << endl;
    cout << indent(1) << "-d <datadir>\t: Specify the output-directory ("
	 << outdir << ")" << endl; 
    cout << indent(1) << "-i <infile>\t: Specify the input-file (" << infile
	 << ")" << endl; 
    cout << indent(1) << "-I\t\t: Execute traceroute -I" << endl;
    cout << indent(1) << "-m\t\t: Use mtr instead of traceroute " << endl;
    cout << indent(1) << "-v\t\t: Be verbose" << endl;
    cout << endl;
    
    exit(-1);
}    

static void set_string_arg(strstream& ss, string& val) 
{
    string argvalue;
    ss >> argvalue;

    if(!argvalue.length())
	disp_usage();
    
    val = argvalue;
}

static bool str2int(const string& str, int& arg) 
{
    if(!str.length())
	return false;
    arg = 0;
    for(unsigned i = 0; i < str.length(); i++)
	if(str[i] >= '0' && str[i] <= '9')
	    arg = arg * 10 + (str[i] - '0');
	else
	    return false;

    return true;
}

static void process(const string& argline) 
{
    strstream ss;
    ss << argline;

    string arg;
    ss >> arg;
    while(arg.length() && !ss.eof()) {
	if(arg == "-p") {
	    string argval;
	    set_string_arg(ss, argval);
	    if(!str2int(argval, par_traceroute))
		disp_usage();
	} else if(arg == "-d")
	    set_string_arg(ss, outdir);
	else if(arg == "-i")
	    set_string_arg(ss, infile);
	else if(arg == "-I")
	    traceroute_I = true;
	else if(arg == "-m")
	    use_mtr = true;
	else if(arg == "-v")
	    be_verbose = true;
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
