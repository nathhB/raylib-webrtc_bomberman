#ifndef TDD_GAME1_SERVER_SYSTEMS_H_INCLUDED
#define TDD_GAME1_SERVER_SYSTEMS_H_INCLUDED

void spawn_players_system(system_t*, world_t*, float);
void explode_players_system(system_t*, world_t*, float);
void broadcast_players_positions_system(system_t*, world_t*, float);

#endif // TDD_GAME1_SERVER_SYSTEMS_H_INCLUDED