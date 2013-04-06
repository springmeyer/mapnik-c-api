var mapnik = require('..').libmapnik_c;
mapnik.mapnik_register_datasources("/usr/local/lib/mapnik/input");
var map = mapnik.mapnik_map(256,256);
mapnik.mapnik_map_load(map,"sample/stylesheet.xml");
mapnik.mapnik_map_zoom_all(map);
mapnik.mapnik_map_render_to_file(map,"node-test.png");
mapnik.mapnik_map_free(map);
