class mergeBin
{
public:
    mergeBin(const char *file_name, const char *addr, const char *app_name);
    mergeBin(const char *file_name, const char *addr);
    mergeBin(const char *file_name);
    ~mergeBin(){if(user_ptr)delete user_ptr;}
    void fill_loader(void);
    void fill_app(void);
    void fill_magic(void);
    uint32_t input_bin_file_length(const char *input_bin_file);
    void fill_uint32_at(uint32_t at, uint32_t wval, const char *input_bin_file);
    uint32_t get_version_integer(const char *path);
    uint32_t get_integer_from_string(const char *str);
private:
    const char *in_file;
    const char *app_file;
    char *user_ptr;
    uint32_t length;
    uint32_t app_length;
    uint32_t total_out_length;
    uint32_t revsion;
    uint32_t default_addr;
    bool write_revsion_flag;
};
