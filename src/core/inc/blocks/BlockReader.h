#ifndef TIN_BLOCKREADER_H
#define TIN_BLOCKREADER_H


#include <cstdint>
#include <cstdlib>
#include <vector>
#include "AbstractBlock.h"

class BlockReader {
    uint8_t *message_buffer;
    size_t message_size;
public:
    std::vector<AbstractBlock*> blocks;

    BlockReader(uint8_t *message_buffer, size_t message_size);
    ~BlockReader();

};


#endif //TIN_BLOCKREADER_H
