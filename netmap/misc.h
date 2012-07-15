#ifndef _misc_h_
#define _misc_h_

#include <string>
#include <strstream>
#include <iostream>

#include <list>

using namespace std;

string indent(unsigned);
string int_to_str(int);

bool str2int(const string&, int&);
bool str2float(const string&, float&);

bool is_validip(const string&);
void skiptillendofline(istream&);


template <class T>
bool contains(list<T>& l, const T& t) 
{
    for(typename list<T>::iterator li = l.begin(); li != l.end(); li++)
	if((*li) == t)
	    return true;
    return false;
}

template <class T>
list<T> intersect(list<T>& l1, list<T>& l2) 
{
    list<T> result;
    for(typename list<T>::iterator li = l1.begin(); li != l1.end(); li++)
	if(contains(l2, *li))
	    result.push_back(*li);

    result.sort();
    result.unique();

    return result;
}

#endif
