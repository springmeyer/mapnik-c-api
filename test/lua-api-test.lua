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
]]

local clib = ffi.load("./libmapnik_c.dylib")

clib.mapnik_register_datasources("/usr/local/lib/mapnik/input")

local map = clib.mapnik_map(256,256)
clib.mapnik_map_load(map,"./sample/stylesheet.xml")
clib.mapnik_map_zoom_all(map)
clib.mapnik_map_render_to_file(map,"lua-test.png")
clib.mapnik_map_free(map)