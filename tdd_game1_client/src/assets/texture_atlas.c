#include <stdio.h>
#include <string.h>

#include "texture_atlas.h"

#define ATLAS_INFO_TOKENS_COUNT 6
#define SPRITE_INFO_TOKENS_COUNT 13
#define ATLAS_FILE_LINE_MAX_SIZE 255
#define ATLAS_TEXTURES_PATH "spritesheets/"

static void     parse_atlas_file(FILE *, texture_atlas_asset_t*);
static char**   parse_atlas_file_line(char*, unsigned int);
static void     unload_texture_atlas(asset_t*);

asset_t* load_texture_atlas(const char* path)
{
    FILE* atlas_file = fopen(path, "r");

    if (atlas_file == NULL)
    {
        TraceLog(LOG_WARNING, "Failed to load atlas file: %s", path);

        return NULL;
    }

    texture_atlas_asset_t* texture_atlas_asset = malloc(sizeof(texture_atlas_asset_t));

    texture_atlas_asset->base.unload_func = unload_texture_atlas;
    texture_atlas_asset->regions = htable_create();

    parse_atlas_file(atlas_file, texture_atlas_asset);
    fclose(atlas_file);

    TraceLog(LOG_INFO, "Done loading texture altas: %s", path);

    return (asset_t*)texture_atlas_asset;
}

static void parse_atlas_file(FILE *atlas_file, texture_atlas_asset_t* atlas)
{
    char line[ATLAS_FILE_LINE_MAX_SIZE];

    while (fgets(line, ATLAS_FILE_LINE_MAX_SIZE, atlas_file))
    {
        char line_type = line[0];
        char* line_data = line + 2; // remove line type and first white space
        char** tokens = NULL;

        if (line_type == 'a')
        {
            tokens = parse_atlas_file_line(line_data, ATLAS_INFO_TOKENS_COUNT);
            char texture_path[255] = {0};

            strcat(strcat(texture_path, ATLAS_TEXTURES_PATH), tokens[0]);

            TraceLog(LOG_INFO, "Parsed atlas description line (texture_path: %s)", texture_path);

            atlas->texture = LoadTexture(texture_path);
        }
        else if (line_type == 's')
        {
            tokens = parse_atlas_file_line(line_data, SPRITE_INFO_TOKENS_COUNT);
            char* region_id = strdup(tokens[0]);
            atlas_region_t* region = malloc(sizeof(atlas_region_t));
            
            region->atlas_texture = &atlas->texture;
            region->rect = (Rectangle){ .x = (float)atoi(tokens[3]), .y = (float)atoi(tokens[4]), .width = (float)atoi(tokens[5]), .height = (float)atoi(tokens[6]) };

            htable_add(atlas->regions, region_id, region);

            TraceLog(LOG_INFO, "Loaded atlas region (id: %s, x: %d, y: %d, w: %d, h: %d)",
                region_id, (int)region->rect.x, (int)region->rect.y, (int)region->rect.width, (int)region->rect.height);
        }

        if (tokens) free(tokens);
    }
}

static char** parse_atlas_file_line(char* line, unsigned int tokens_count)
{
    char** tokens = malloc(sizeof(char*) * tokens_count);
    char* token = strtok(line, " ");
    unsigned int token_id = 0;

    while (token != NULL) {
        tokens[token_id++] = token;
        token = strtok(NULL, " ");
    }

    return tokens;
}

static void clear_atlas_region(void** atlas_region_ptr)
{
    free(atlas_region_ptr);
}

static void unload_texture_atlas(asset_t* asset)
{
    texture_atlas_asset_t* texture_atlas_asset = (texture_atlas_asset_t*)asset;

    htable_destroy(&texture_atlas_asset->regions, true, clear_atlas_region, true);
    UnloadTexture(texture_atlas_asset->texture);
}