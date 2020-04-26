#ifndef TDD_GAME1_CLIENT_GAME_SYSTEMS_H_INCLUDED
#define TDD_GAME1_CLIENT_GAME_SYSTEMS_H_INCLUDED

void inputs_system(system_t*, world_t*, float);
void process_inputs_entity_system(entity_system_t*, world_t*, float, entity_t*);
void movement_entity_system(entity_system_t*, world_t*, float, entity_t*);
void world_register_collision_entity_system(world_t*);
void render_map_system(system_t*, world_t*, float);
void debug_render_colliders_entity_system(entity_system_t*, world_t*, float, entity_t*);
void update_player_position_system(system_t*, world_t*, float);
void position_interpolation_entity_system(entity_system_t*, world_t*, float, entity_t*);
void send_local_player_position_system(system_t* system, world_t* world, float delta);
void spawn_bombs_system(system_t*, world_t*, float);
void lifetime_entity_system(entity_system_t*, world_t*, float, entity_t*);
void explode_players_system(system_t*, world_t*, float);
void update_state_machines_entity_system(entity_system_t*, world_t*, float, entity_t*);
void spawn_bonuses_system(system_t*, world_t*, float);
void apply_bonuses_system(system_t*, world_t*, float);
void world_register_update_bonuses_effects_entity_system(world_t*);
void player_kick_bomb_system(system_t*, world_t*, float);
void world_register_render_ui_system(world_t*);
// void update_players_map_positions_system(system_t*, world_t*, float);
void update_bombs_colliders_system(system_t*, world_t*, float);

#endif // TDD_GAME1_CLIENT_GAME_SYSTEMS_H_INCLUDED