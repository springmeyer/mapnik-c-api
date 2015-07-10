
#include <stdio.h>
#include "mapnik_c_api.h"
#include "test/c-api-test-cfg.h"

int main() {
    mapnik_map_t * map;
    map = mapnik_map(256,256);
    mapnik_register_datasources(MAPNIK_PLUGINDIR, NULL);
    mapnik_map_load(map,"sample/stylesheet.xml");
    mapnik_map_zoom_all(map);
    mapnik_map_render_to_file(map,"/tmp/mapnik-c-api-c99-test-map1.png");
    printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "rendered to /tmp/mapnik-c-api-c99-test-map1.png");
    mapnik_map_free(map);
    printf("c99 test works\n");
}
