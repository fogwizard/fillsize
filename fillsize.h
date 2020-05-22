class fillVal
{
public:
    fillVal(const char *file_name, const char *addr);
    fillVal(const char *file_name);
    void fill_size(void);
    void fill_version(void);
    void check_versionfile(void);
    uint32_t input_bin_file_length(const char *input_bin_file);
    void fill_uint32_at(uint32_t at, uint32_t wval, const char *input_bin_file);
    uint32_t get_version_integer(const char *path);
    uint32_t get_integer_from_string(const char *str);
private:
    const char *in_file;
    uint32_t length;
    uint32_t revsion;
    uint32_t default_addr;
    bool write_revsion_flag;
};
