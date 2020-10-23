#include "common.h"
#include "ddbin.h"
#include <fstream>

#define INC(n) (n+sizeof(n))

using namespace std;

ddBin::ddBin(const char *file_in, const char *file_out)
{

    int length  = input_bin_file_length(file_in);
    char *buffer = static_cast<char *>(malloc(length));

    {
        ifstream in(file_in);
        in.read(buffer,sizeof(length));
        in.close();
    }

    ofstream out(file_out);
    out.write(buffer, length);
    out.close();

    cout << "dd: write count=" << length << endl;

    free(buffer);
}


ddBin::ddBin(const char *file_in, const char *file_out, const char * out_size)
{
    int length  = input_bin_file_length(file_in);
    int out_len = get_integer_from_string(out_size);
    char *buffer = static_cast<char *>(malloc(length));

    {
        ifstream in(file_in);
        in.read(buffer,sizeof(length));
        in.close();
    }

    if(out_len > length) {
        int append = out_len  - length;
        cout << "out_len > length, fill " << append << " byte with 0xFF" << endl;
        ofstream out(file_out);
        out.write(buffer, length);
        buffer[0] = static_cast<char>(0xff);
        for(int i = 0; i < append; i++) {
            out.write(buffer, 1);
        }
        out.close();
        cout << "dd: write count=" << length << " + " << append << endl;
    } else {
        ofstream out(file_out);
        out.write(buffer, out_len);
        out.close();
        cout << "dd: write count=" << out_len << endl;
    }

    free(buffer);
}

ddBin::ddBin(const char *file_in, const char *file_out, const char *skip, const char* out_size)
{
    int length  = input_bin_file_length(file_in);
    int skip_len= get_integer_from_string(skip);
    int out_len = get_integer_from_string(out_size);

    char *buffer = static_cast<char *>(malloc(length));

    if(skip_len >= length){
        cout << "skip_len >=  length, load skip_len = 0" << endl;
	skip_len = 0x00;
    }

    {
        ifstream in(file_in);
        in.read(buffer,skip_len);
        in.read(buffer,length - skip_len);
        in.close();
    }

    if(out_len > (length - skip_len)) {
        int append = out_len  - (length - skip_len);
        cout << "out_len > length, fill " << append << " byte with 0xFF" << endl;
        ofstream out(file_out);
        out.write(buffer, length);
        buffer[0] = static_cast<char>(0xff);
        for(int i = 0; i < append; i++) {
            out.write(buffer, 1);
        }
        out.close();
        cout << "dd: write count=" << length << " + " << append << endl;
    } else {
        ofstream out(file_out);
        out.write(buffer, out_len);
        out.close();
        cout << "dd: write count=" << out_len << endl;
    }

    free(buffer);
}

void ddBin::dd(void)
{
    cout << "dd: done" << endl;
}

uint32_t ddBin::get_integer_from_string(const char *str)
{
    uint32_t val;
    if((str[0] == '0') && ((str[1] == 'x') || str[1] == 'X')) {
        val = strtol(str, NULL, 16);
    } else {
        val = std::atoi(str);
    }
    return val;
}

uint32_t ddBin::input_bin_file_length(const char *input_bin_file)
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

