#include "search.h"

//////////////////////
//
// SearchPath members


void SearchPath::add_dir(const list<string>& dlist, int depth /*= 0*/) 
{
    for(CSiter diter = dlist.begin(); diter != dlist.end(); diter++) 
	add_dir( *diter, depth);    
}

list<string> SearchPath::find_file(const string& fname) //const
{
//    Why does this doens't work ?????????
    
//      list<string> result;
//      for(CSbiter sbiter = sb.begin(); sbiter != sb.end(); sbiter++) {
//  	list<string> ores = (*sbiter)->find_files(fname.c_str());
//  	result = result + ores; //(*sb)->find_files(fname);
//      }

    list<string> result;
    
    const list<Filec*> allfiles = all_files();
    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++)
	if((*i)->fname() == fname)
	    result.push_back((*i)->full_path());  

    result.unique();
    
    return result;
}

list<string> SearchPath::find_file(bool (*f)(const string&) ) const 
{

    list<string> result;

    const list<Filec*> allfiles = all_files();
    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++) {
	const string fpath = (*i)->full_path();
	if(f(fpath))
	    result.push_back(fpath);
    }

    result.unique();

    return result;
}

list<Filec*> SearchPath::fc_find_file(const string& fname) const 
{
    list<Filec*> result;

    const list<Filec*> allfiles = all_files();
    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++)
	if((*i)->fname() == fname)
	    result.push_back(*i);
    
    result.unique();

    return result;
}

list<Filec*> SearchPath::fc_find_file(bool (*f) (const string&) ) const
{
    list<Filec*> result;

    const list<Filec*> allfiles = all_files();
    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++) {
	const string fpath = (*i)->full_path();
	if(f(fpath))
	    result.push_back(*i);
    }

    result.unique();

    return result;
}


// private members

list<Filec*> SearchPath::all_files() const 
{
    list<Filec*> result;
    
    for(CSbiter sbiter = sb.begin(); sbiter != sb.end(); sbiter++)
	result = result + (*sbiter)->ret_all_files();

    return result;
}

	
    
    
