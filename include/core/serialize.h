/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#pragma once

#include <string>

using namespace std;

namespace Serialize {

    void serialize_game_objects(const string& filepath);

    void deserialize_game_objects(const string& filepath);

}