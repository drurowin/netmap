#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "files.h"
#include "debug.h"
extern int errno;

using namespace std;

/////////////////////
//
// Infile Members

//Infile public members

Infile::Infile() : ifstream() { }

Infile::Infile(const string& file_name, int mode, bool fatal)
    : ifstream(file_name.c_str(), (std::_Ios_Openmode)mode)
{

    if(cdb>d_list)
	dbgs << "Opening input file (" << file_name << " ) : ";
    
    if(int error_nr = check_open()) {
	if(cdb>d_list)
	    dbgs << " FAILED " << error_nr << endl;

	perror(file_name.c_str());
	if(fatal) {
	    cout << "Error opening file " << file_name << endl;
	    exit(1);
	}
    } else if(cdb>d_list)
	dbgs << " SUCCESS" << endl;

}


int Infile::Open(const string & file_name, int mode, bool fatal)
{
    
    if(cdb>d_list)
	dbgs << "Opening input file (" << file_name << ") : ";

    if(!*this)
	//file is open, close it first
	close();

    open(file_name.c_str(), (std::_Ios_Openmode)mode);

    if(int error_nr = check_open())  { 
	if(cdb>d_list)
	    dbgs << " FAILED " << error_nr << endl;
	
	perror(file_name.c_str());

	if(fatal) {
	    dbgs << "Error opening input file, Exit...." << endl;
	    exit(3);
	}
	
	return 0;
    } else if(cdb>d_list) 
	dbgs << " SUCCESS " << endl;


    return 1;
}

//Infile private members

int Infile::check_open() const
{
    if(*this)
	return 0;
    return (errno?errno:-1);
}

///////////////////////
//
// Outfile Members

//Outfile public members

Outfile::Outfile() : ofstream()  { }

Outfile::Outfile(const string& file_name, int mode, bool fatal) :
    ofstream(file_name.c_str())
{
    if(cdb>d_list)
	dbgs << "Opening output file (" << file_name << ") : ";
    
    if(int error_nr = check_open()) {
	if(cdb>d_list)
	    dbgs << " FAILED " << error_nr << endl;

	perror(file_name.c_str());

	if(fatal) {
	    dbgs << "Exit...." << endl;
	    exit(2);
	}
    } else if(cdb>d_list)
	dbgs << " SUCCESS " << endl;
}

int Outfile::Open(const string& file_name, int mode, bool fatal)
{
    if(cdb>d_list)
	dbgs << "Opening output file (" << file_name << ") : ";

    if(!*this)
	//file is open, close it first
	close();
    
    open(file_name.c_str());
    
    if(int error_nr = check_open())  {
	if(cdb>d_list)
	    dbgs << " FAILED " << error_nr << endl;

	perror(file_name.c_str());

	if(fatal) {
	    dbgs << "Error opening output file, Exit...." << endl;
	    exit(2);
	}
	
	return 0;
    } else if(cdb>d_list)
	dbgs << " SUCCESS " << endl;

    return 1;
}

//Outfile protected members

int Outfile::check_open() const
{
    if(*this)
	return 0;
    return (errno?errno:-1);
    
}

Dbgfile::Dbgfile()
{
//      const string def_name = "debug.out";
    
//      open(def_name.c_str());
//      if(check_open())  {
//  	perror("debug.out");
//  	exit(11);
//      }
}
Dbgfile::Dbgfile(const string &filename) {
    open(filename.c_str());
    if(check_open())  {
	perror(filename.c_str());
	cout << "Could not open debug-file " << filename<< endl;
	exit(11);
    }
}
    	

