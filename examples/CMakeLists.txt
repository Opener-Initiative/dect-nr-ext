build_lib_example(
    NAME mac-pdu-transmission
    SOURCE_FILES mac-pdu-transmission.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
        ${libdect-nr-ext}
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