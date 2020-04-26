#ifndef FAMILY_H_INCLUDED
#define FAMILY_H_INCLUDED

#define FAM_ALL_OF(family, ...) family_mask_set(&family->all_mask, __VA_ARGS__, -1)
#define FAM_ANY_OF(family, ...) family_mask_set(&family->any_mask, __VA_ARGS__, -1)
#define FAM_NONE_OF(family, ...) family_mask_set(&family->not_mask, __VA_ARGS__, -1)

typedef struct family
{
    int all_mask;
    int any_mask;
    int not_mask;
} family_t;

family_t*   family_create();
void        family_destroy(family_t*);
void        family_mask_set(int*, ...);
bool        family_entity_is_part_of(family_t*, entity_t*);

#endif // FAMILY_H_INCLUDED
