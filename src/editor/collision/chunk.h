/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#pragma once

namespace Chunk {

    struct chunk_unit {
        bool solid_block;
    };

    struct chunk {
        int x, y;
        Chunk::chunk_unit **grid;
    };

    void init(Chunk::chunk *chunk, int x, int y, int chunk_size);

    bool is_solid_block(Chunk::chunk *chunk, int x, int y);

    void set_solid_block(Chunk::chunk *chunk, int x, int y, bool state);

}
