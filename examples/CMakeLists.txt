build_lib_example(
    NAME dect2020ext
    SOURCE_FILES dect2020ext.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)