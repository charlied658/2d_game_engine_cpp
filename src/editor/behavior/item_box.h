/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include <string>

namespace ItemBox {

    struct item_box {
        std::string type;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(type);
        }
    };

}
