#include <boost/test/unit_test.hpp>
#include <ConfigReader.h>
#include <iostream>
#include <stdexcept>
#include <libgen.h>

std::string filepath() {
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    const char* path;
    if(count != -1)
        path = dirname(result);
    std::string config_filename = "/test-reader.cfg";
    return std::string(path) + config_filename;
}

BOOST_AUTO_TEST_CASE(init_config_reader) {
    ConfigReader *cf;
    BOOST_CHECK_NO_THROW((cf = new ConfigReader(filepath())));
}

BOOST_AUTO_TEST_CASE(throw_exc_when_init_config_reader) {
    ConfigReader *cf;
    BOOST_CHECK_THROW((cf = new ConfigReader("nonexistentfile.txt")), libconfig::FileIOException);
    delete cf;
}

BOOST_AUTO_TEST_CASE(read_string) {
    ConfigReader *cf;
    std::string expected = "192.168.0.1";
    BOOST_CHECK_NO_THROW((cf = new ConfigReader(filepath())));
    std::string ip_read = cf->read_string("sensor.ip");
    BOOST_CHECK_EQUAL(expected, ip_read);
    delete cf;
}

BOOST_AUTO_TEST_CASE(read_int) {
    ConfigReader *cf;
    int expected = 4040;
    BOOST_CHECK_NO_THROW((cf = new ConfigReader(filepath())));
    int port_read = cf->read_integer("sensor.port");
    BOOST_CHECK_EQUAL(expected, port_read);
    delete cf;
}

BOOST_AUTO_TEST_CASE(read_arr) {
    ConfigReader *cf;
    BOOST_CHECK_NO_THROW((cf = new ConfigReader(filepath())));
    std::vector<std::string> arr_read = cf->read_string_arr("sensor.sample_arr", 3);
    BOOST_CHECK_EQUAL("x", arr_read[0]);
    BOOST_CHECK_EQUAL("y", arr_read[1]);
    BOOST_CHECK_EQUAL("z", arr_read[2]);
    delete cf;
}

BOOST_AUTO_TEST_CASE(read_bool) {
    ConfigReader *cf;
    bool expected = false;
    BOOST_CHECK_NO_THROW((cf = new ConfigReader(filepath())));
    bool archived_read = cf->read_bool("sensor.archived");
    BOOST_CHECK_EQUAL(expected, archived_read);
    delete cf;
}

BOOST_AUTO_TEST_CASE(throw_exc_read_nonexistent_field) {
    ConfigReader *cf = new ConfigReader(filepath());
    BOOST_CHECK_THROW(cf->read_integer("asdf"), std::runtime_error);
    delete cf;
}
