#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

class ConfigReader {
    public:
        ConfigReader(const std::string& filepath);
        int64_t read_integer(const std::string& path);
        bool read_bool(const std::string& path);
        std::strin read_string(const std::string& path);
}

#endif // _CONFIG_READER_H_
