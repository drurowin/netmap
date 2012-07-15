#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <iostream>


using namespace std;

#include <list>

#include "debug.h"
#include "dirs.h"

//////////////////
//
//  Filec Members



string Filec::full_path() const 
{
    assert(dirp);

    return dirp->full_path() + name;
}


//////////////////
//
//  Dirc Members

list<Filec*> Dirc::all_files() const
{
    list<Filec*> result;

    for(CDiriter subiter = subdirs.begin(); subiter != subdirs.end();
	subiter++)  
	result = result + (*subiter)->all_files();

    result = result + files + links;
    
    return result;
}	

string Dirc::full_path() const
{
    string result;
    if(parent)
	result = parent->full_path();

    return result + name + '/';
}


//private members

void Dirc::read_dir(const string& dirname)
{

    if(cdb>d_list)
	dbgs << "Called for dir : " << dirname << endl;

	cout << "Called for dir : " << dirname << endl;

    //where we are now
    const string curr_path = (parent?parent->full_path():string());
    const string comp_path = curr_path + dirname;
    
    //open & read the directory (to avoid opening too much files)
    DIR* dir;    
    if(!(dir = opendir(comp_path.c_str()))) {
	const string errmsg = "Couldn't open " + comp_path;
	perror(errmsg.c_str());
	return;
	//exit(1);
    }

    //first make a list of all files & subdirs
    list<string> children;
    while(struct dirent *dirptr = readdir(dir)) 
	children.push_back(dirptr->d_name);
    closedir(dir);
    
    for(CSiter i = children.begin();i != children.end(); i++) {
	const string& file_name = *i;
    
	const string& f_comp_path = comp_path + '/' + file_name;
	
	
	if(cdb>d_list)
	    dbgs << "Trying " << comp_path << " / " << file_name << " : ";

	cout << "Trying " << comp_path << " / " << file_name << " : ";

	if(file_name == "." || file_name == "..") {
	    if(cdb>d_list)
		dbgs << "(skipped)" << endl;

		cout << "(skipped)" << endl;

	    continue;
	}
	
	//check if its a file or dir
	Oinfo obj(f_comp_path);

	if(obj.is_link()) {
	    if(cdb>d_list)
		dbgs << " as link" << endl;

		cout << " as link" << endl;

	    Filec* next_file = new Filec(file_name, this);
	    links.push_back(next_file);
	} else if(obj.is_dir()) {
	    if(cdb>d_list)
		dbgs << " as dir" << endl;

		cout  << " as dir" << endl;

	    if(!(depth-1)) continue; //don't cross depth limit
	    
	    Dirc* next_dir = new Dirc(file_name, depth-1,  this);
	    subdirs.push_back(next_dir);
	} else if(obj.is_ord_file()) {
	    if(cdb>d_list)
		dbgs << " as file" << endl;

		cout << " as file" << endl;
	    
	    Filec* next_file = new Filec(file_name, this);
	    files.push_back(next_file);
	} else 	{
	    if(cdb>d_list)
		dbgs << " as trash" << endl;
	
		cout << " as trash" << endl;
	}
	
    }   
}


///////////////////////
//
// SearchBlock memebers


list<string> SearchBlock::find_file(const string& fname) const
{
    list<string> result;
    
    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++)
	if((*i)->fname() == fname)
	    result.push_back((*i)->full_path());
    return result;
}

list<string> SearchBlock::find_file(bool (*f) (const string&)) const 
{
    list<string> result;

    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++) {
	const string fpath = (*i)->full_path();
	if(f(fpath))
	    result.push_back(fpath);
    }

    return result;
}

list<Filec*> SearchBlock::fc_find_file(const string& fname) const 
{
    list<Filec*> result;

    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++)
	if((*i)->fname() == fname)
	    result.push_back(*i);

    return result;
}

list<Filec*> SearchBlock::fc_find_file(bool (*f) (const string&)) const
{

    list<Filec*> result;

    for(CFiliter i = allfiles.begin(); i != allfiles.end(); i++) {
	const string fpath = (*i)->full_path();
	if(f((fpath)))
	    result.push_back(*i);
    }
    

    return result;
}

    
