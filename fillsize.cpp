#include "iostream"
#include "stdio.h"
#include "stdint.h"
#include <unistd.h>
#include <string.h>

using namespace std;

#define INC(n) (n+sizeof(n))

uint32_t input_bin_file_length(const char *input_bin_file)
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

void usage(void)
{
    cout << "Usage: fillsize <filename> [address]" << endl;
}

void fill_uint32_at(uint32_t at, uint32_t wval, const char *input_bin_file)
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

uint32_t get_version_integer(const char *path)
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

uint32_t get_integer_from_string(const char *str)
{
    uint32_t val;
    if((str[0] == '0') && ((str[1] == 'x') || str[1] == 'X')) {
        val = strtol(str, NULL, 16);
    } else {
        val = std::atoi(str);
    }
    return val;
}

int main(int argc, char *argv[])
{
    uint32_t length  = 0;
    uint32_t revsion = 0;
    uint32_t write_revsion_flag = 0;
    uint32_t default_addr = 0x1C;

    /* check argc */
    if((2 != argc) && (3 != argc)) {
        usage();
    }

    /* check input file */
    if (-1 == access(argv[1], 0)) {
        cout << "file:" << argv[1]  << " not exist" << endl;
        return 0;
    }

    length  = input_bin_file_length(argv[1]);

    /* check default address */
    if(argc == 3) {
        default_addr = get_integer_from_string(argv[2]);
	if(default_addr >= length){
		cout << "default_addr bigger than file size, exit..." << endl;
		return 0;
	}
    }


    const char * aname = "../platform/include/rev_integer.h";
    if (-1 != access(aname, 0)) {
        revsion = get_version_integer(aname);
        write_revsion_flag = 1;
        cout << "write version info at:" << default_addr << endl;
    } else {
        cout << "version file not found, bypass write version number" << endl;
    }

    fill_uint32_at(default_addr,        length, argv[1]);
    if(write_revsion_flag)
        fill_uint32_at(INC(default_addr), revsion,argv[1]);

    return 0;
}

