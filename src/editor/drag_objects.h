/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#pragma once

namespace Drag {

    void init();

    void start_frame();

    void start_drag();

    void update_drag();

    void end_drag();

    void snap_to_grid();

    void snap_to_last_position();

    bool is_dragging_objects();

    bool is_invalid_placement();

}
