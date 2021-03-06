/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#pragma once

#include <string>

namespace Serializer {

    void serialize_game_objects(const std::string& filepath);

    void deserialize_game_objects(const std::string& filepath);

}