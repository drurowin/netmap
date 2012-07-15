// TODO :
//
//   - Consider using vector instead of list<T*>'s 

#include <list>

#ifndef __make_dep__
#include <string>
#endif

#include "dirs_core.h"

#ifndef _dirs_h_
#define _dirs_h_

class Dirc; //forward declaration
class Filec;

typedef list<Dirc*> DirList;
typedef list<Dirc*>::iterator Diriter;
typedef list<Dirc*>::const_iterator CDiriter;

typedef list<Filec*> FileList;
typedef list<Filec*>::iterator Filiter;
typedef list<Filec*>::const_iterator CFiliter;

typedef list<string>::iterator Siter;
typedef list<string>::const_iterator CSiter;

class Filec {
public:
    Filec(const string& fname, Dirc* dir) : name(fname), dirp(dir) { };

    const string& fname() const { return name; }
    string full_path() const;
private:
    string name;  // filename    
    Dirc* dirp;   // directory this file belongs to
};

class Dirc {
public:
    Dirc(const string& fname, int lvl=0, Dirc* parentp=0)
	: name(fname), parent(parentp), depth(lvl)  { read_dir(fname); } 
    
//      Dirc(const string&, Dirc*, int);
//      Dirc(const string& fname) : parent(0), name(fname), depth(0) {
//  	read_dir(fname); }

    const list<Dirc*>&  all_subdirs()   const { return subdirs; }
    const list<Filec*>& all_loc_files() const { return files; }
    const list<Filec*>& all_loc_links() const { return links; }

    list<Filec*> all_files() const;
    
    //string full_path(const string& ) const;
    string full_path() const;

    void read_dir(const string&);    
private:

    
    string name;   //dirname    
    Dirc* parent;  //parent directory
    int depth;

    list<Dirc*> subdirs;
    list<Filec*> files;
    list<Filec*> links;
};


class SearchBlock : public Dirc {
public:
    SearchBlock(const string& fname, int depth=0) : Dirc(fname, depth)
	{ allfiles = all_files(); }

    list<string> find_file(const string&) const;
    list<string> find_file(bool (*f)(const string&)) const;

    list<Filec*> fc_find_file(const string&) const;
    list<Filec*> fc_find_file(bool (*f)(const string&)) const;

    list<Filec*> ret_all_files() { return allfiles; }
private:
    list<Filec*> allfiles;
};

//maybe crappy performance (should be checked :)
template<class T>
list<T*> operator+(const list<T*>& ld1, const list<T*>& ld2) 
{
    list<T*> result=ld1;

    for(typename list<T*>::const_iterator i = ld2.begin(); i != ld2.end(); i++)
	result.push_back(*i);

    return result;
}


#endif

