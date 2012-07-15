#include <fstream>

using namespace std;

#ifndef __make_dep__
#include <string>  //makedepend bug
#endif

//TODO :
//
// - define class IOfile
// - overload constructor to enable modes (append a.s.)

#ifndef _files_h_
#define _files_h_

class Infile : public ifstream  {
public:
    Infile();
    Infile(const string &, int mode=ios::in, bool=false);
    int Open(const string &, int mode=ios::in, bool=false);

protected:
    int check_open() const;
};

class Outfile : public ofstream {
public:
    Outfile();
    Outfile(const string &, int mode=ios::out, bool=true);
    int Open(const string &, int mode=ios::out, bool=true);
protected:
    int check_open() const;
    
};

class Dbgfile : public Outfile {
public:
    Dbgfile();
    Dbgfile(const string &);
private:
    int sopen(const string &){return 0;}

};

#endif

