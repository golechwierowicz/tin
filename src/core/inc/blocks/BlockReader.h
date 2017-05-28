#ifndef TIN_BLOCKREADER_H
#define TIN_BLOCKREADER_H

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <memory>
#include "AbstractBlock.h"

class BlockReader {
    const uint8_t *message_buffer;
    size_t message_size;
public:
    std::vector<std::unique_ptr<AbstractBlock>> blocks;

    BlockReader(const uint8_t *message_buffer, size_t message_size);
};


#endif //TIN_BLOCKREADER_H
