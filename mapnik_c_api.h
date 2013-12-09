#ifndef MAPNIK_C_API_H
#define MAPNIK_C_API_H

#ifdef __cplusplus
extern "C"
{
#endif

int mapnik_register_datasources(const char* path);


// Coord
typedef struct _mapnik_coord_t {
    double x;
    double y;
} mapnik_coord_t;

// Projection
typedef struct _mapnik_projection_t mapnik_projection_t;

void mapnik_projection_free(mapnik_projection_t *p);

mapnik_coord_t mapnik_projection_forward(mapnik_projection_t *p, mapnik_coord_t c);


// Bbox
typedef struct _mapnik_bbox_t mapnik_bbox_t;

mapnik_bbox_t * mapnik_bbox(double minx, double miny, double maxx, double maxy);

void mapnik_bbox_free(mapnik_bbox_t * b);


// Image
typedef struct _mapnik_image_t mapnik_image_t;

void mapnik_image_free(mapnik_image_t * i);

typedef struct _mapnik_image_blob_t {
    void *ptr;
    unsigned int len;
} mapnik_image_blob_t;

mapnik_image_blob_t mapnik_image_to_png_blob(mapnik_image_t * i);



//  Map
typedef struct _mapnik_map_t mapnik_map_t;

mapnik_map_t * mapnik_map( unsigned int width, unsigned int height );

void mapnik_map_free(mapnik_map_t * m);

const char * mapnik_map_get_srs(mapnik_map_t * m);

int mapnik_map_set_srs(mapnik_map_t * m, const char* srs);

int mapnik_map_load(mapnik_map_t * m, const char* stylesheet);

int mapnik_map_zoom_all(mapnik_map_t * m);

int mapnik_map_render_to_file(mapnik_map_t * m, const char* filepath);

void mapnik_map_resize(mapnik_map_t * m, unsigned int width, unsigned int height);

void mapnik_map_zoom_to_box(mapnik_map_t * m, mapnik_bbox_t * b);

mapnik_projection_t * mapnik_map_projection(mapnik_map_t *m);

mapnik_image_t * mapnik_map_render_to_image(mapnik_map_t * m);

#ifdef __cplusplus
}
#endif


#endif // MAPNIK_C_API_H
