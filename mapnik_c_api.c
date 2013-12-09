#include <mapnik/version.hpp>
#include <mapnik/graphics.hpp>
#include <mapnik/color.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/datasource_cache.hpp>

#include "mapnik_c_api.h"

#include <stdlib.h>

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
    if (m)  {
        if (m->m) delete m->m;
        delete m;
    }
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


void mapnik_map_resize(mapnik_map_t *m, unsigned int width, unsigned int height) {
    if (m&& m->m) {
        m->m->resize(width, height);
    }
}


struct _mapnik_projection_t {
    mapnik::projection * p;
};

mapnik_projection_t * mapnik_map_projection(mapnik_map_t *m) {
    mapnik_projection_t * proj = new mapnik_projection_t;
    if (m && m->m)
        proj->p = new mapnik::projection(m->m->srs());
    else
        proj->p = NULL;
    return proj;
}


void mapnik_projection_free(mapnik_projection_t *p) {
    if (p) {
        if (p->p) delete p->p;
        delete p;
    }
}


mapnik_coord_t mapnik_projection_forward(mapnik_projection_t *p, mapnik_coord_t c) {
    if (p && p->p) {
        p->p->forward(c.x, c.y);
    }
    return c;
}

struct _mapnik_bbox_t {
    mapnik::box2d<double> b;
};

mapnik_bbox_t * mapnik_bbox(double minx, double miny, double maxx, double maxy) {
    mapnik_bbox_t * b = new mapnik_bbox_t;
    b->b = mapnik::box2d<double>(minx, miny, maxx, maxy);
    return b;
}

void mapnik_bbox_free(mapnik_bbox_t * b) {
    if (b)
        delete b;
}

void mapnik_map_zoom_to_box(mapnik_map_t * m, mapnik_bbox_t * b) {
    if (m && m->m && b) {
        m->m->zoom_to_box(b->b);
    }
}

struct _mapnik_image_t {
    mapnik::image_32 *i;
};

void mapnik_image_free(mapnik_image_t * i) {
    if (i) {
        if (i->i) delete i->i;
        delete i;
    }
}

mapnik_image_t * mapnik_map_render_to_image(mapnik_map_t * m) {
    mapnik::image_32 * im = new mapnik::image_32(m->m->width(), m->m->height());
    if (m && m->m) {
        try {
            mapnik::agg_renderer<mapnik::image_32> ren(*m->m,*im);
            ren.apply();
        } catch (std::exception const& ex) {
            delete im;
            printf("%s\n",ex.what());
            return NULL;
        }
    }
    mapnik_image_t * i = new mapnik_image_t;
    i->i = im;
    return i;
}

mapnik_image_blob_t mapnik_image_to_png_blob(mapnik_image_t * i) {
    mapnik_image_blob_t blob;
    blob.ptr = NULL;
    blob.len = 0;
    if (i && i->i) {
        std::string s = save_to_string(*(i->i), "png256");
        blob.len = s.length();
        blob.ptr = malloc(blob.len); // malloc is important here, as it's the reponsibility of
                                     // the C-API user to free() the allocated memory later
                                     // and the delete-operator of C++ might not be available
        memcpy(blob.ptr, s.c_str(), blob.len);
    }
    return blob;
}


#ifdef __cplusplus
}
#endif
