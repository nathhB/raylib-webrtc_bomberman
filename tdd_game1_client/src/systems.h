#ifndef TDD_GAME1_CLIENT_SYSTEMS_H_INCLUDED
#define TDD_GAME1_CLIENT_SYSTEMS_H_INCLUDED

void update_transforms_entity_system(entity_system_t*, world_t*, float, entity_t*);
void render_entity_system(entity_system_t*, world_t*, float, entity_t*);
void world_register_animate_entity_system(world_t*, tdd_timer_t*);
void world_register_blink_rendering_entity_system(world_t*, tdd_timer_t*);

#endif // TDD_GAME1_CLIENT_SYSTEMS_H_INCLUDED