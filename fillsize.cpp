#include "common.h"
#include "fillsize.h"

#define INC(n) (n+sizeof(n))

using namespace std;

fillVal::fillVal(const char *file_name):in_file(file_name)
{
    length  = input_bin_file_length(file_name);
    default_addr = 0x1C;

    check_versionfile();
}

fillVal::fillVal(const char *file_name, const char *addr):in_file(file_name)
{
    length  = input_bin_file_length(file_name);

    /* check default address */
    default_addr = get_integer_from_string(addr);
    if(default_addr >= length) {
        cout << "default_addr bigger than file size, bypass fill size" << endl;
    }

    check_versionfile();
}

void fillVal::check_versionfile(void)
{
    const char * aname = "../platform/include/rev_integer.h";
    if (-1 != access(aname, 0)) {
        revsion = get_version_integer(aname);
        write_revsion_flag = 1;
        cout << "write version info at:" << default_addr << endl;
    } else {
        write_revsion_flag = 0;
        cout << "version file not found, bypass write version number" << endl;
    }
}


void fillVal::fill_size(void)
{
    if(default_addr < length) {
        fill_uint32_at(default_addr, length, in_file);
    }
}

void fillVal::fill_version(void)
{
    if(write_revsion_flag) {
        fill_uint32_at(INC(default_addr), revsion, in_file);
    }
}

uint32_t fillVal::input_bin_file_length(const char *input_bin_file)
{
    uint32_t total_len = 0;
    int rc = 0;
    char buff[64] = {0};

    FILE *in  = fopen(input_bin_file,"rb");
    while( (rc = fread(buff,sizeof(unsigned char),64,in)) != 0) {
        total_len += rc;
    }
    fclose(in);
    return total_len;
}

void fillVal::fill_uint32_at(uint32_t at, uint32_t wval, const char *input_bin_file)
{
    union {
        unsigned char c[4];
        uint32_t ui;
    } val = {0};

    val.ui = wval;
    FILE *in  = fopen(input_bin_file,"r+");
    fseek(in, at,SEEK_SET);
    fwrite(val.c, sizeof(uint32_t), 1, in);
    fclose(in);

    cout << "Fill val " << wval << " at addr " << at << endl;
}

uint32_t fillVal::get_version_integer(const char *path)
{
    int len;
    char buf[16];
    FILE *in  = fopen(path,"rb");
    fseek(in, 0,SEEK_SET);
    memset(buf, 0x00, sizeof(buf));
    len = fread(buf, sizeof(buf), 1, in);
    fclose(in);

    return std::atoi(buf);
}

uint32_t fillVal::get_integer_from_string(const char *str)
{
    uint32_t val;
    if((str[0] == '0') && ((str[1] == 'x') || str[1] == 'X')) {
        val = strtol(str, NULL, 16);
    } else {
        val = std::atoi(str);
    }
    return val;
}

