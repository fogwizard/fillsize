#include "common.h"
#include "mergebin.h"
#include <fstream>

#define INC(n) (n+sizeof(n))

using namespace std;

mergeBin::mergeBin(const char *file_name):in_file(file_name)
{
    length  = input_bin_file_length(file_name);
    default_addr = 0x1C;
}

mergeBin::mergeBin(const char *file_name, const char *addr):in_file(file_name)
{
    length  = input_bin_file_length(file_name);

    /* check default address */
    default_addr = get_integer_from_string(addr);
    if(default_addr >= length) {
        cout << "default_addr bigger than file size, bypass fill size" << endl;
    }
}

mergeBin::mergeBin(const char *file_name, const char *addr, const char *app_name):in_file(file_name), app_file(app_name)
{
    length     = input_bin_file_length(file_name);
    app_length = input_bin_file_length(app_name);

    /* check default address */
    default_addr = get_integer_from_string(addr);
    if(default_addr >= (128*1024)) {
        cout << "default_addr bigger than 128K, load 0x8000" << endl;
        default_addr = 0x8000;
    }

    total_out_length = default_addr + app_length + 512 + 4;

    user_ptr = new char[total_out_length];
    memset(user_ptr, 0xFF, total_out_length);

    cout << "loader_len=0x" << hex << length\
         << ",dec=" << dec << length << endl;
    cout << "app_len=0x"   << hex << app_length\
         << ",dec=" << dec << app_length  << endl;
    cout << "offset_len=0x" << hex << default_addr\
         << ",dec=" << dec << default_addr << endl;
    cout << "out_len=0x"   << hex << total_out_length\
         << ",dec=" << dec << total_out_length << endl;
}

void mergeBin::fill_loader(void)
{
    int len;
    char *dat = static_cast<char *>(user_ptr);

    int expect_read = length;

    std::ifstream ifs(in_file, std::ios::binary | std::ios::in);
    ifs.read(dat, (size_t)expect_read);
    if(ifs.peek() == EOF) len = expect_read;
    ifs.close();

    cout << "loader: fill=" << len << endl;
}

void mergeBin::fill_app(void)
{
    int len;
    char *dat = &user_ptr[default_addr];

    int expect_read = app_length;

    std::ifstream ifs(app_file, std::ios::binary | std::ios::in);
    ifs.read(dat, expect_read);
    if(ifs.peek() == EOF) len = expect_read;
    ifs.close();

    cout << "app: fill=" << len << endl;
}

void mergeBin::fill_magic(void)
{
    uint32_t real_length;
    uint32_t read_size;
    char *dat;

    read_size = *((uint32_t *)(&user_ptr[default_addr + 0x194]));
    if((read_size > (32*1024)) && (read_size < (app_length))) {
        real_length = read_size;
        cout << "valid size in app offset 0x194, app real size=" << real_length << endl;
    } else {
        real_length = app_length;
        cout << "invalid size in app offset 0x194,load " << app_length <<  endl;
    }

    int offset =  default_addr + real_length + 512;
    cout << "magic: fill_offset=" << offset << endl;
    dat = &user_ptr[offset];
    dat[0] = 'a';
    dat[1] = 'b';
    dat[2] = 'l';
    dat[3] = 'e';

    ofstream fout("merge.bin", ios::binary);
    fout.write(user_ptr, offset + 4);
    fout.close();

    cout << "gerarate merge.bin done, length=" << offset + 4 <<  endl;
}

uint32_t mergeBin::input_bin_file_length(const char *input_bin_file)
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

void mergeBin::fill_uint32_at(uint32_t at, uint32_t wval, const char *input_bin_file)
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

uint32_t mergeBin::get_version_integer(const char *path)
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

uint32_t mergeBin::get_integer_from_string(const char *str)
{
    uint32_t val;
    if((str[0] == '0') && ((str[1] == 'x') || str[1] == 'X')) {
        val = strtol(str, NULL, 16);
    } else {
        val = std::atoi(str);
    }
    return val;
}

