#include "dirs_core.h"

Oinfo::Oinfo(const string& fname, int fmode/*=0*/) 
{
    stat_info = new struct stat;

    lstat(fname.c_str(), stat_info);
    
    st_mode = stat_info->st_mode;

    if(!fmode)
	delete stat_info;

    stat_info = 0;
}
