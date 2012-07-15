#include <strstream>
#include <string>

#include "misc.h"

using namespace std;

string indent(unsigned lvl) 
{
    string result;
    
    while(lvl--)
	result += "   ";

    return result;
}

string int_to_str(int i) 
{
    strstream ost;

    ost << i << char(0);

    return ost.str();
}

int pow(int base, unsigned exp) 
{
    if(!exp) return 1;

    if(exp == 1) return base;

    return base * pow(base, exp - 1);
}

bool str2int(const string& str, int& arg) 
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

    

bool str2float(const string& str, float& fl) 
{
    if(!str.length())
	return false;
    
    bool had_point = false;
    int beyond_point = 0;

    int int_first = 0;
    for(unsigned i = 0; i < str.length(); i++) {
	if(had_point)
	    beyond_point++;
	
	if(str[i] >= '0' && str[i] <= '9')
	    int_first = int_first*10 + (str[i] - '0');
	else if(str[i] == '.') {
	    if(had_point)
		return false; //found two .
	    else
		had_point = true;
	} else
	    return false;
    }

    fl = int_first / ((float) pow(10, beyond_point));

    return true;
}

//helping out for now, should change soon
bool is_validip(const string& ip) 
{
    int nr_p = 0;
    for(unsigned i = 0; i < ip.length(); i++)
	if(ip[i] == '.')
	    nr_p++;
	else if(ip[i] < '0' || ip[i] > '9')
	    return false;

    if(nr_p != 3)
	return false;

    return true;
}

void skiptillendofline(istream& in) 
{
    char ch;
    in.get(ch);
    while(in && !in.eof() && ch != '\n')
	in.get(ch);
}
