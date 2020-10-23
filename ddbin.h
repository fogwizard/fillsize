class ddBin
{
public:
    ddBin(const char *file_in, const char *file_out);
    ddBin(const char *file_in, const char *file_out, const char * out_size);
    ddBin(const char *file_in, const char *file_out, const char *skip, const char* out_size);
    ~ddBin() {}
    void dd(void);
    uint32_t get_integer_from_string(const char *str);
    uint32_t input_bin_file_length(const char *input_bin_file);
};
