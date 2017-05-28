#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include <libconfig.h++>

class ConfigReader {
    public:
        ConfigReader(const std::string& filename);
        int read_integer(const std::string& path);
        bool read_bool(const std::string& path);
        std::string read_string(const std::string& path);
    private:
        libconfig::Config cf;
};

#endif // _CONFIG_READER_H_
