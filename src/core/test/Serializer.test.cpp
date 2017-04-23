#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Serializer"

#include <boost/test/unit_test.hpp>
#include <Serializer.h>

BOOST_AUTO_TEST_CASE(buffer_size_should_be_0_if_nothing_was_written) {
    Serializer serializer;
    uint16_t size;

    serializer.get_buffer(size);

    BOOST_CHECK_EQUAL(size, 0);
}

BOOST_AUTO_TEST_CASE(buffer_size_should_not_be_0_if_something_was_written) {
    Serializer serializer;
    uint16_t size;

    serializer.begin_block(0)
            .end_block();
    serializer.get_buffer(size);

    BOOST_CHECK(size != 0);
}