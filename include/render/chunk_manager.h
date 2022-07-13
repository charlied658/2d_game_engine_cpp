/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#pragma once

#include "chunk.h"

namespace ChunkManager {

    void init();

    void reload();

    bool is_solid_block(int x, int y);

    void set_solid_block(int x, int y, bool state);

}
