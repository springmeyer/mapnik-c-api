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

mapnik_image_blob_t * mapnik_image_to_metatile(mapnik_map_t * m, mapnik_image_t * i, unsigned int z, unsigned int x, unsigned int y, unsigned int ms);

]]

local m = ffi.load("../libmapnik_c.so")

if m.mapnik_register_datasources("/usr/local/lib/mapnik/input") ~= 0 then
    print("mapnik_image_to_metatile error:"..ffi.string(m.mapnik_map_last_error(map)))
else
    local map = m.mapnik_map(256*8,256*8)
    m.mapnik_map_load(map,"../sample/stylesheet.xml")
    m.mapnik_map_zoom_all(map)
    local image = m.mapnik_map_render_to_image(map)
    local metatile = m.mapnik_image_to_metatile(map, image, 3, 0, 0, 8)
    if metatile and metatile.ptr then
        local metatile_file = io.open("lua-test-metatile.meta", "w")
        metatile_file:write(ffi.string(metatile.ptr, metatile.len))
        metatile_file:close()
        m.mapnik_image_blob_free(metatile)
    else
        print("mapnik_image_to_metatile error:"..ffi.string(m.mapnik_map_last_error(map)))
    end
    m.mapnik_image_free(image)
    m.mapnik_map_free(map)
end
