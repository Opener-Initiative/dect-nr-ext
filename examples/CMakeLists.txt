build_lib_example(
    NAME dect2020ext-frame-timer
    SOURCE_FILES dect2020ext-frame-timer.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)

build_lib_example(
    NAME dect2020ext-spectrum-management-procedure
    SOURCE_FILES dect2020ext-spectrum-management-procedure.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
        ${libnetanim}
)