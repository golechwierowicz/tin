#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Deserializer"

#include <boost/test/unit_test.hpp>
#include <Deserializer.h>

BOOST_AUTO_TEST_CASE(is_1_1) {
    BOOST_CHECK_EQUAL(1, 1);
}