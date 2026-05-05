# build_lib_example(
#     NAME dect2020ext
#     SOURCE_FILES dect2020ext.cc
#     LIBRARIES_TO_LINK
#         ${libdect-nr-ext}
#         ${libnetanim}
# )

build_lib_example(
    NAME dect2020ext-mac-scheduler-example
    SOURCE_FILES dect2020ext-mac-scheduler-example.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)