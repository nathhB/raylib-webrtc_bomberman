set(SOURCES components.c family.c world.c entity_tag_manager.c entity_group_manager.c tdd_ecs.h)

add_library(tddecs ${SOURCES})

target_link_libraries(tddecs tdd)