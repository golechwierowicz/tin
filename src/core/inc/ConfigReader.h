#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include <libconfig.h++>
#include <vector>

class ConfigReader {
    public:
        ConfigReader(const std::string& filename);
        int read_integer(const std::string& path);
        bool read_bool(const std::string& path);
        std::string read_string(const std::string& path);
        std::vector<std::string> read_string_arr(const std::string& path, size_t size);
    private:
        libconfig::Config cf;
};

#endif // _CONFIG_READER_H_
