#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mapnik_c_api.h"
#include <fstream>

// https://github.com/philsquared/Catch/wiki/Supplying-your-own-main()
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static const char * plugin_path = "/usr/local/lib/mapnik/input";

TEST_CASE( "map/srs", "should get/set srs string" ) {
      mapnik_map_t * map;
      map = mapnik_map(256,256);
      const char *srs = mapnik_map_get_srs(map);
      REQUIRE( 0==strcmp(srs,"+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs") );
      mapnik_map_set_srs(map,"+init=epsg:4326");
      const char *srs2 = mapnik_map_get_srs(map);
      REQUIRE( 0==strcmp(srs2,"+init=epsg:4326") );
      mapnik_map_free(map);
}

TEST_CASE( "map/load", "should load xml" ) {
      mapnik_map_t * map;
      map = mapnik_map(256,256);
      mapnik_register_datasources(plugin_path);
      REQUIRE_FALSE(mapnik_map_load(map,"sample/stylesheet.xml"));
      mapnik_map_free(map);
}

TEST_CASE( "map/render", "should render png" ) {
      mapnik_map_t * map;
      map = mapnik_map(256,256);
      mapnik_register_datasources(plugin_path);
      REQUIRE_FALSE(mapnik_map_load(map,"sample/stylesheet.xml"));
      mapnik_map_zoom_all(map);
      mapnik_map_render_to_file(map,"/tmp/mapnik-c-api-test-map1.png");
      printf("\x1b[1;32m ✓ (%s)\x1b[0m\n", "rendered to /tmp/mapnik-c-api-test-map1.png");
      mapnik_map_free(map);
}

TEST_CASE( "map/render_to_mem", "should render png in memory" ) {
      mapnik_map_t * map;
      map = mapnik_map(1024,1024);
      mapnik_register_datasources(plugin_path);
      REQUIRE_FALSE(mapnik_map_load(map,"sample/stylesheet.xml"));
      mapnik_map_zoom_to_box(map, mapnik_bbox(0, 0, 5000000, 5000000));
      mapnik_image_t * i = mapnik_map_render_to_image(map);
      mapnik_image_blob_t * b = mapnik_image_to_png_blob(i);

      std::ofstream o("/tmp/mapnik-c-api-test-map2.png");
      o.write(b->ptr, b->len);
      o.close();

      mapnik_image_blob_free(b);
      mapnik_image_free(i);
      mapnik_map_free(map);
}
