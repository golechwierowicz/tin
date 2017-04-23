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

BOOST_AUTO_TEST_CASE(buffer_size_should_not_be_0_if_a_block_was_written) {
    Serializer serializer;
    uint16_t size;

    serializer.begin_block(0)
            .end_block();
    serializer.get_buffer(size);

    BOOST_CHECK(size != 0);
}

BOOST_AUTO_TEST_CASE(buffer_contents_should_be_correct) {
    Serializer serializer;
    uint16_t size;

    serializer.begin_block(1)
            .write((int8_t) 1)
            .write((int16_t) 1)
            .write((int32_t) 1)
            .write((int64_t) 1)
            .end_block();
    uint8_t* buffer = serializer.get_buffer(size);

    uint8_t expected[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 23, // block size
            1,
            0, 1,
            0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1
    };
    BOOST_CHECK_EQUAL(size, sizeof(expected));
    BOOST_CHECK(memcmp(buffer, expected, sizeof(expected)) == 0);
}