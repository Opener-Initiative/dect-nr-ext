build_lib_example(
    NAME dect2020ext
    SOURCE_FILES dect2020ext.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)

build_lib_example(
    NAME spectrum-management-example-1
    SOURCE_FILES spectrum-management/spectrum-management-example-1.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)