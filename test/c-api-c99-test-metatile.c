
#include <stdio.h>
#include "mapnik_c_api.h"
#include "test/c-api-test-cfg.h"

int main() {
    mapnik_map_t * map;
    mapnik_image_t * image;
    mapnik_image_blob_t * metatile;
    FILE * mf;
    map = mapnik_map(8*256,8*256);
    mapnik_register_datasources(MAPNIK_PLUGINDIR, NULL);
    mapnik_map_load(map,"../sample/stylesheet.xml");
    mapnik_map_zoom_all(map);

    image = mapnik_map_render_to_image(map);
    metatile = mapnik_image_to_metatile(map, image, 3, 2, 1, 8);
    if( metatile == NULL || metatile->ptr == NULL) {
        printf("error: %s\n", mapnik_map_last_error(map));
        return -1;
    }
    mf = fopen("./mapnik-c-api-c99-test-metatile.meta", "wb");
    fwrite(metatile->ptr, sizeof(char), metatile->len, mf);
    fclose(mf);
    printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "./mapnik-c-api-c99-test-metatile.meta");
    mapnik_image_free(image);
    mapnik_image_blob_free(metatile);
    mapnik_map_free(map);
    printf("c99 test works\n");
    return 0;
}
