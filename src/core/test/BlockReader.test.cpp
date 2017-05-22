#include <boost/test/unit_test.hpp>
#include <blocks/BlockReader.h>
#include <blocks/DebugBlock.h>

BOOST_AUTO_TEST_CASE(blocks_read_correctly) {
    uint8_t buffer[] = {
            0, 0, 0, 0, // block type
            0, 0, 0, 22, // block size
            0x69,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            4, // length of the string
            'a', 'b', 'c', 'd',

            0, 0, 0, 0, // block type
            0, 0, 0, 24, // block size
            0x96,
            0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
            6, // length of the string
            'a', 'b', 'c', 'd', 'e', 'f'
    };
    BlockReader reader(buffer, sizeof(buffer));

    BOOST_CHECK_EQUAL(reader.blocks.size(), 2);

    DebugBlock* block1 = (DebugBlock*) reader.blocks[0];

    BOOST_CHECK_EQUAL(block1->type, bt_debug);
    BOOST_CHECK_EQUAL(block1->u8_value, 0x69);
    BOOST_CHECK_EQUAL(block1->i64_value, 0x1122334455667788);
    BOOST_CHECK_EQUAL(block1->str_value, "abcd");

    DebugBlock* block2 = (DebugBlock*) reader.blocks[1];

    BOOST_CHECK_EQUAL(block2->type, bt_debug);
    BOOST_CHECK_EQUAL(block2->u8_value, 0x96);
    BOOST_CHECK_EQUAL(block2->i64_value, 0x8877665544332211);
    BOOST_CHECK_EQUAL(block2->str_value, "abcdef");
}