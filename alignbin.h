class alignBin
{
public:
    alignBin(const char *file_name, const char *align_value);
    alignBin(const char *file_name, const char *align_value, const char* out_size);
    ~alignBin() {}
    void do_align(void);
    uint32_t get_integer_from_string(const char *str);
    uint32_t input_bin_file_length(const char *input_bin_file);
private:
    const char *in_file;
    uint32_t length;
    uint32_t out_len;
    uint32_t align;
};
