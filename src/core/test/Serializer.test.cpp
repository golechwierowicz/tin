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
            .write((int8_t) 0x11)
            .write((int16_t) 0x1122)
            .write((int32_t) 0x11223344)
            .write((int64_t) 0x1122334455667788)
            .write(std::string("aaaa"))
            .end_block();
    uint8_t* buffer = serializer.get_buffer(size);

    uint8_t expected[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 28, // block size
            0x11,
            0x11, 0x22,
            0x11, 0x22, 0x33, 0x44,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            4, // length of the string
            'a', 'a', 'a', 'a',
    };
    BOOST_CHECK_EQUAL(size, sizeof(expected));
    BOOST_CHECK(memcmp(buffer, expected, sizeof(expected)) == 0);
}

BOOST_AUTO_TEST_CASE(buffer_overflow_should_throw_exception) {
    Serializer serializer;

    BOOST_CHECK_THROW(
            {
                serializer.begin_block(1);
                while (true) {
                    serializer.write((uint8_t) 0);
                }
            },
            std::exception
    );
}

BOOST_AUTO_TEST_CASE(buffer_size_should_omit_incomplete_blocks) {
    Serializer serializer;
    uint16_t size;

    serializer.begin_block(1);
    serializer.get_buffer(size);

    BOOST_CHECK_EQUAL(size, 0);
}