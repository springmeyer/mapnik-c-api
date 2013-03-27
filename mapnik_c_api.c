#include <mapnik/version.hpp>
#include <mapnik/graphics.hpp>
#include <mapnik/color.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/datasource_cache.hpp>

#include "mapnik_c_api.h"


#ifdef __cplusplus
extern "C"
{
#endif

int mapnik_register_datasources(const char* path) {
    try {
#if MAPNIK_VERSION >= 200200
        mapnik::datasource_cache::instance().register_datasources(path);
#else
        mapnik::datasource_cache::instance()->register_datasources(path);
#endif
        return 0;
    } catch (std::exception const& ex) {
        printf("%s\n",ex.what());
        return -1;
    }
}

struct _mapnik_map_t {
    mapnik::Map * m;
};

mapnik_map_t * mapnik_map(unsigned width, unsigned height) {
    mapnik_map_t * map = new mapnik_map_t;
    map->m = new mapnik::Map(width,height);
    return map;
}

void mapnik_map_free(mapnik_map_t * m) {
    if (m->m) delete m->m;
    if (m) delete m;
}

const char * mapnik_map_get_srs(mapnik_map_t * m) {
    if (m && m->m) return m->m->srs().c_str();
    return NULL;
}

int mapnik_map_set_srs(mapnik_map_t * m, const char* srs) {
    if (m) {
        m->m->set_srs(srs);
        return 0;
    }
    return -1;
}

int mapnik_map_load(mapnik_map_t * m, const char* stylesheet) {
    if (m && m->m) {
        try {
            mapnik::load_map(*m->m,stylesheet);
        } catch (std::exception const& ex) {
            printf("%s\n",ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

int mapnik_map_zoom_all(mapnik_map_t * m) {
    if (m && m->m) {
        try {
            m->m->zoom_all();
        } catch (std::exception const& ex) {
            printf("%s\n",ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

int mapnik_map_render_to_file(mapnik_map_t * m, const char* filepath) {
    if (m && m->m) {
        try {
            mapnik::image_32 buf(m->m->width(),m->m->height());
            mapnik::agg_renderer<mapnik::image_32> ren(*m->m,buf);
            ren.apply();
            mapnik::save_to_file(buf,filepath);
        } catch (std::exception const& ex) {
            printf("%s\n",ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

#ifdef __cplusplus
}
#endif
