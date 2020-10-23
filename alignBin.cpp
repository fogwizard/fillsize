#include "common.h"
#include "alignbin.h"
#include <fstream>

#define INC(n) (n+sizeof(n))

using namespace std;

alignBin::alignBin(const char *file_name, const char *align_value)
{
    in_file = file_name;
    length  = input_bin_file_length(file_name);
    align   = get_integer_from_string(align_value);
}

void alignBin::do_align(void)
{
    uint32_t len = length + (align - 1);
    len = (~(align -1))& len;
    int append = len - length;

    if(0 != (align%2)) {
        cout << "align: couldn't align to=" << align << endl;
    }

    if(append) {
        /* open,write,close */
        ofstream of;
        of.open(in_file, std::ios_base::app);
        if(of.is_open()) {
            char buf[1] = { 0xFF};
            for(int i = 0; i < append; i++) {
                of.write(buf, sizeof(buf));
            }
            cout << "align: write=" << append << endl;
        }
        of.close();
    }

    cout << "align: append=" << append << " byte to meet align=" << align << endl;
}

uint32_t alignBin::get_integer_from_string(const char *str)
{
    uint32_t val;
    if((str[0] == '0') && ((str[1] == 'x') || str[1] == 'X')) {
        val = strtol(str, NULL, 16);
    } else {
        val = std::atoi(str);
    }
    return val;
}

uint32_t alignBin::input_bin_file_length(const char *input_bin_file)
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

