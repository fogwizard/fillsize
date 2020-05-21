#include "iostream"
#include "stdio.h"
#include "stdint.h"
#include <unistd.h>
#include <string.h>

using namespace std;

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
    printf("input=%s len=[0x%x]%d\n", input_bin_file, total_len, total_len);
    return total_len;
}

void usage(void)
{
	cout << "Usage: fillsize <filename>" << endl;
}

void fill_uint32_at(uint32_t at, uint32_t length, const char *input_bin_file)
{
    union {
	unsigned char c[4];
	uint32_t ui;
    }val = {0};

    val.ui = length;
    FILE *in  = fopen(input_bin_file,"r+");
    fseek(in, at,SEEK_SET);
    fwrite(val.c, sizeof(uint32_t), 1, in);
    fclose(in);
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

int main(int argc, char *argv[])
{
	uint32_t length  = 0;
	uint32_t revsion = 0;

	if(2 != argc){
		usage();
	}
	if (-1 == access(argv[1], 0)) {
		cout << "file:" << argv[1]  << " not exist" << endl;
		return 0;
	}

	length  = input_bin_file_length(argv[1]);

	const char * aname = "../platform/include/rev_integer.h";
	if (-1 != access(aname, 0)) {
		revsion = get_version_integer(aname);
	} else {
		cout << "version file not found" << endl;
	}

	fill_uint32_at(0x1C,        length, argv[1]);
	fill_uint32_at(0x1C + 0x04, revsion,argv[1]);

	return 0;
}

