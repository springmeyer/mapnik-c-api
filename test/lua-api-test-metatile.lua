local ffi = require("ffi")

ffi.cdef[[
int mapnik_register_datasources(const char* path);

//  Opaque class structure
typedef struct _mapnik_map_t mapnik_map_t;

mapnik_map_t * mapnik_map( unsigned int width, unsigned int height );

void mapnik_map_free(mapnik_map_t * m);

const char * mapnik_map_get_srs(mapnik_map_t * m);

int mapnik_map_set_srs(mapnik_map_t * m, const char* srs);

int mapnik_map_load(mapnik_map_t * m, const char* stylesheet);

int mapnik_map_zoom_all(mapnik_map_t * m);

int mapnik_map_render_to_file(mapnik_map_t * m, const char* filepath);

typedef struct _mapnik_image_t mapnik_image_t;

void mapnik_image_free(mapnik_image_t * i);

typedef struct _mapnik_image_blob_t {
    char *ptr;
    unsigned int len;
} mapnik_image_blob_t;

void mapnik_image_blob_free(mapnik_image_blob_t * b);

mapnik_image_blob_t * mapnik_image_to_png_blob(mapnik_image_t * i);

mapnik_image_blob_t * mapnik_image_view_to_png_blob(mapnik_image_t * i, unsigned int xx, unsigned int yy, unsigned int xlen, unsigned int ylen);

mapnik_image_t * mapnik_map_render_to_image(mapnik_map_t * m);

]]

local m = ffi.load("../libmapnik_c.so")

m.mapnik_register_datasources("/usr/local/lib/mapnik/input")

local map = m.mapnik_map(256*8,256*8)
m.mapnik_map_load(map,"/etc/mapnik-osm-carto-data/moscow.xml")
m.mapnik_map_zoom_all(map)
local image = m.mapnik_map_render_to_image(map)
local png_blob = m.mapnik_image_view_to_png_blob(image, 256*6, 256*4, 256, 256)
--local png_blob = m.mapnik_image_to_png_blob(image)
m.mapnik_image_free(image)
local png_file = io.open("lua-test.png", "w")
png_file:write(ffi.string(png_blob.ptr, png_blob.len))
png_file:close()
m.mapnik_image_blob_free(png_blob)

m.mapnik_map_free(map)