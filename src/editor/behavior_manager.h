/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "editor/game_object.h"
#include "behavior/solid_block.h"
#include "behavior/breakable_block.h"
#include "behavior/item_box.h"
#include "behavior/player.h"
#include "behavior/goomba.h"
#include "behavior/turtle.h"
#include "behavior/mushroom.h"
#include "behavior/fire_flower.h"
#include "behavior/pipe.h"

namespace Editor {

    namespace GameObject {
        struct game_object;
    }

    namespace BehaviorManager {

        struct behavior_manager {
            int id;
            Editor::GameObject::game_object *game_object;

            SolidBlock::solid_block solid_block;
            BreakableBlock::breakable_block breakable_block;
            ItemBox::item_box item_box;
            Player::player player;
            Goomba::goomba goomba;
            Turtle::turtle turtle;
            Mushroom::mushroom mushroom;
            FireFlower::fire_flower fire_flower;
            Pipe::pipe pipe;
            bool is_solid_block;
            bool is_breakable_block;
            bool is_item_box;
            bool is_player;
            bool is_goomba;
            bool is_turtle;
            bool is_mushroom;
            bool is_fire_flower;
            bool is_pipe;
        };

        void init(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_solid_block(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_breakable_block(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_item_box(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_player(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_goomba(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_turtle(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_mushroom(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_fire_flower(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_pipe(Editor::BehaviorManager::behavior_manager *bh_manager);

    }
}
