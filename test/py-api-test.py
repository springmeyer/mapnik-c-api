import sys
sys.path.append('./py-api')
from mapnik_c import *

mapnik_register_datasources("/usr/local/lib/mapnik/input")
m = mapnik_map(256,256)
mapnik_map_load(m,"sample/stylesheet.xml")
mapnik_map_zoom_all(m)
mapnik_map_render_to_file(m,"py-test.png")
mapnik_map_free(m)