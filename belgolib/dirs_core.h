#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#ifndef __make_dep__
#include <string>
#endif

#ifndef __dirs_core_h__
#define __dirs_core_h__

//this class can be used in two modes
//   1. light mode -> st_mode is used (stat_info=0)
//   2. full  mode -> st_mode & stat_info is used

class Oinfo {
public:
    Oinfo(const string&, int=0);

    bool is_dir() const { return S_ISDIR(st_mode); }
    bool is_ord_file() const { return S_ISREG(st_mode); }
    bool is_link() const { return S_ISLNK(st_mode); }

    const struct stat* ret_stat_info() const { return stat_info; }
private:
    struct stat* stat_info;
    int st_mode;
};

#endif
