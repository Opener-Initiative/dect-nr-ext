build_lib_example(
    NAME dect2020ext-channel-selection
    SOURCE_FILES dect2020ext-channel-selection.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
        ${libnetwork}
)

build_lib_example(
    NAME dect2020ext-simulation1
    SOURCE_FILES dect2020ext-simulation1.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
        ${libdect-nr}
        ${libpoint-to-point}
        ${libinternet}
        ${libnetwork}
        ${libapplications}
)