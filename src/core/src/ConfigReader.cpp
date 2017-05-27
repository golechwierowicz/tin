#include <ConfigReader.h>
#include <stdexcept>

ConfigReader::ConfigReader(const std::string& filename) {
   cf.readFile(filename.c_str());
}

int ConfigReader::read_integer(const std::string& path) {
    int result;
    
    if(!cf.lookupValue(path, result)) 
        throw std::runtime_error("Cannot read from path: " + path);

    return result;
}

bool ConfigReader::read_bool(const std::string& path) {
    bool result;

    if(!cf.lookupValue(path, result)) 
        throw std::runtime_error("Cannot read from path: " + path);
    
    return result;
}

std::string ConfigReader::read_string(const std::string& path) {
    std::string result;
   
    if(!cf.lookupValue(path, result)) 
        throw std::runtime_error("Cannot read from path: " + path);

    return result;
}
