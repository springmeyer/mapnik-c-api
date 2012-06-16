#include <mapnik/graphics.hpp>
#include <mapnik/color.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/datasource_cache.hpp>

#include "mapnik_c_api.h"

int mapnik_register_datasources(const char* path) {
    try {
        mapnik::datasource_cache::instance()->register_datasources(path);
        return 0;
    } catch (std::exception const& ex) {
        printf("%s\n",ex.what());
        return -1;
    }
}

mapnik_map_t mapnik_map( unsigned width, unsigned height ) {
    return new mapnik::Map(width,height);
}

void mapnik_map_free(mapnik_map_t m) {
    mapnik::Map * _m = static_cast<mapnik::Map*>(m);
    delete _m;
}

const char * mapnik_map_get_srs(mapnik_map_t m) {
    mapnik::Map * _m = static_cast<mapnik::Map*>(m);
    return _m->srs().c_str();
}

void mapnik_map_set_srs(mapnik_map_t m, const char* srs) {
    mapnik::Map * _m = static_cast<mapnik::Map*>(m);
    _m->set_srs(srs);
}

int mapnik_map_load(mapnik_map_t m, const char* stylesheet) {
    mapnik::Map * _m = static_cast<mapnik::Map*>(m);
    try {
        mapnik::load_map(*_m,stylesheet);
    } catch (std::exception const& ex) {
        printf("%s\n",ex.what());
        return -1;
    }
    return 0;
}