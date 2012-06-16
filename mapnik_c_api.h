#ifndef MAPNIK_C_API_H
#define MAPNIK_C_API_H

#ifdef __cplusplus
extern "C"
{
#endif

int mapnik_register_datasources(const char* path);

typedef void *mapnik_map_t;

mapnik_map_t mapnik_map( unsigned int width, unsigned int height );

void mapnik_map_free(mapnik_map_t m);

const char * mapnik_map_get_srs(mapnik_map_t m);

void mapnik_map_set_srs(mapnik_map_t m, const char* srs);

int mapnik_map_load(mapnik_map_t m, const char* stylesheet);

void mapnik_map_zoom_all(mapnik_map_t m);

void mapnik_map_render_to_file(mapnik_map_t m, const char* filepath);

#ifdef __cplusplus
}
#endif


#endif // MAPNIK_C_API_H