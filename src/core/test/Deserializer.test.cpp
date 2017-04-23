#include <boost/test/unit_test.hpp>
#include <Deserializer.h>

BOOST_AUTO_TEST_CASE(values_should_be_correctly_deserialized) {
    uint8_t buffer[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 28, // block size
            0x11,
            0x11, 0x22,
            0x11, 0x22, 0x33, 0x44,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            4, // length of the string
            'a', 'b', 'c', 'd',
    };
    Deserializer deserializer(buffer, sizeof(buffer));

    int8_t int8_value;
    int16_t int16_value;
    int32_t int32_value;
    int64_t int64_value;
    std::string string_value;

    deserializer
            .read(int8_value)
            .read(int16_value)
            .read(int32_value)
            .read(int64_value)
            .read(string_value);

    BOOST_CHECK_EQUAL(int8_value, 0x11);
    BOOST_CHECK_EQUAL(int16_value, 0x1122);
    BOOST_CHECK_EQUAL(int32_value, 0x11223344);
    BOOST_CHECK_EQUAL(int64_value, 0x1122334455667788);
    BOOST_CHECK_EQUAL(string_value, std::string("abcd"));
}

BOOST_AUTO_TEST_CASE(multiple_blocks_handled_correctly) {
    uint8_t buffer[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 16, // block size
            0, 0, 0, 1,
            0, 0, 0, 2,
            0, 0, 0, 2, // block type
            0, 0, 0, 20, // block size
            0, 0, 0, 3,
            0, 0, 0, 4,
            0, 0, 0, 5,
    };
    Deserializer deserializer(buffer, sizeof(buffer));

    int32_t value_1;
    int32_t value_2;
    int32_t value_3;
    int32_t value_4;
    int32_t value_5;

    deserializer
            .read(value_1)
            .read(value_2);
    deserializer.next_block();
    deserializer
            .read(value_3)
            .read(value_4)
            .read(value_5);

    BOOST_CHECK_EQUAL(value_1, 1);
    BOOST_CHECK_EQUAL(value_2, 2);
    BOOST_CHECK_EQUAL(value_3, 3);
    BOOST_CHECK_EQUAL(value_4, 4);
    BOOST_CHECK_EQUAL(value_5, 5);
}

BOOST_AUTO_TEST_CASE(insufficient_block_size_should_cause_exception) {
    uint8_t buffer[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 8, // block size
            0, 0, 0, 1,
    };
    Deserializer deserializer(buffer, sizeof(buffer));

    int32_t value;

    BOOST_CHECK_THROW(
            deserializer.read(value),
            std::exception
    );
}

BOOST_AUTO_TEST_CASE(exceeding_block_size_should_cause_exception) {
    uint8_t buffer[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 16, // block size
            0, 0, 0, 1,
    };
    Deserializer deserializer(buffer, sizeof(buffer));

    int32_t value_1;
    int32_t value_2;

    deserializer.read(value_1);

    BOOST_CHECK_THROW(
            deserializer.read(value_2),
            std::exception
    );
}

BOOST_AUTO_TEST_CASE(next_block_should_return_false_after_last_block) {
    uint8_t buffer[] = {
            0, 0, 0, 1, // block type
            0, 0, 0, 8, // block size
            0, 0, 0, 1, // block type
            0, 0, 0, 8, // block size
            0, 0, 0, 1, // block type
            0, 0, 0, 8, // block size
    };
    Deserializer deserializer(buffer, sizeof(buffer));

    BOOST_CHECK_EQUAL(deserializer.next_block(), true);
    BOOST_CHECK_EQUAL(deserializer.next_block(), true);
    BOOST_CHECK_EQUAL(deserializer.next_block(), false);
}