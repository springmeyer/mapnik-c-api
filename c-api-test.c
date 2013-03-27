#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mapnik_c_api.h"

static const char * plugin_path = "/usr/local/lib/mapnik/input";

void test_map() {
    mapnik_map_t * map;
    map = mapnik_map(256,256);

    const char *srs = mapnik_map_get_srs(map);
    if(!strcmp(srs, "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs"))
        printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "get srs");

    mapnik_map_set_srs(map,"+init=epsg:4326");
    const char *srs2 = mapnik_map_get_srs(map);
    if(!strcmp(srs2, "+init=epsg:4326"))
        printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "set srs");

    mapnik_register_datasources(plugin_path);
    if (!mapnik_map_load(map,"sample/stylesheet.xml"))
        printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "load map");

    // render a map
    mapnik_map_zoom_all(map);
    mapnik_map_render_to_file(map,"/tmp/mapnik-c-api-test-map1.png");
    printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "rendered to /tmp/mapnik-c-api-test-map1.png");
    mapnik_map_free(map);
}


int main(int argc, char **argv) {
    test_map();
    return 0;
}