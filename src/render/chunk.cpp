/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include <cstdio>
#include "render/chunk.h"

namespace Chunk {

    /**
     * Initialize a new chunk.
     * @param chunk Chunk reference
     * @param x Starting x position
     * @param y Starting y position
     * @param chunk_size Size of chunk
     */
    void init(Chunk::chunk *chunk, int x, int y, int chunk_size) {
        chunk->x = x;
        chunk->y = y;
        chunk->grid = new Chunk::chunk_unit *[chunk_size];
        for (int i = 0; i < chunk_size; i++) {
            chunk->grid[i] = new Chunk::chunk_unit[chunk_size];
            for (int j = 0; j < chunk_size; j++) {
                chunk->grid[i][j].solid_block = false;
            }
        }
    }

    /**
     * Check if a block within the chunk is a solid block.
     * @param chunk Chunk
     * @param x X position
     * @param y Y position
     * @return True if block is solid
     */
    bool is_solid_block(Chunk::chunk *chunk, int x, int y) {
        //printf("Querying block %d, %d\n", x,y);
        return chunk->grid[x][y].solid_block;
    }

    /**
     * Set a block to be solid.
     * @param chunk Chunk
     * @param x X position
     * @param y Y position
     * @param state True if block is to be solid
     */
    void set_solid_block(Chunk::chunk *chunk, int x, int y, bool state) {
        //printf("Placed block %d, %d\n", x,y);
        chunk->grid[x][y].solid_block = state;
    }
}
