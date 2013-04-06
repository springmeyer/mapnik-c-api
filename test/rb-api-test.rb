require 'ffi'

module Mapnik
  extend ::FFI::Library

  ffi_lib('./libmapnik_c.dylib')

  attach_function :mapnik_register_datasources, [:string], :int
  attach_function :mapnik_map, [:int, :int], :pointer
  attach_function :mapnik_map_free, [:pointer], :void
  attach_function :mapnik_map_get_srs, [:pointer], :string
  attach_function :mapnik_map_set_srs, [:pointer, :string], :int
  attach_function :mapnik_map_load, [:pointer, :string], :int
  attach_function :mapnik_map_zoom_all, [:pointer], :int
  attach_function :mapnik_map_render_to_file, [:pointer, :string], :int

  mapnik_register_datasources("/usr/local/lib/mapnik/input")

  map = mapnik_map(256,256)
  mapnik_map_load(map, "./sample/stylesheet.xml")
  mapnik_map_zoom_all(map)
  mapnik_map_render_to_file(map, "rb-test.png")
  mapnik_map_free(map)
end
