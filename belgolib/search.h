/*
  -- Search.h
  --
  -- This class offers the possibility to search any number of directories for
  -- files
  --
  -- Directories to be searched can be added with the add member
  -- function. Optional argument is the max search depth. The default value (0)
  -- is inf.
  --
  -- These directories can then be searched with the find_file() functions
  --
*/

#include <list>

#ifndef __make_dep__
#include <string>
#endif

#include "dirs.h"

#ifndef _search_h_
#define _search_h_


class SearchPath {
public:
    SearchPath(){ }
    void add_dir(const string& dname, int depth=0)
	{ sb.push_back(new SearchBlock(dname, depth)); }
    void add_dir(const list<string>&, int=0);
    
    list<string> find_file(const string&) ;
    list<string> find_file(bool (*f) (const string&)) const;
    
    list<Filec*> fc_find_file(const string&) const;
    list<Filec*> fc_find_file(bool (*f) (const string&)) const;    
private:
    typedef list<SearchBlock*>::iterator Sbiter;
    typedef list<SearchBlock*>::const_iterator CSbiter;

    list<SearchBlock*> sb;

    list<Filec*> all_files() const;
};

#endif
