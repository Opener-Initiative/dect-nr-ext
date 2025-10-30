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

build_lib_example(
    NAME dect2020ext-frame-timer
    SOURCE_FILES dect2020ext-frame-timer.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)

build_lib_example(
    NAME dect2020ext-rssi-test
    SOURCE_FILES dect2020ext-rssi-test.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
        ${libdect-nr}
        ${libpoint-to-point}
        ${libinternet}
        ${libnetwork}
        ${libapplications}
)

build_lib_example(
    NAME dect2020ext-spectrum-management-procedure
    SOURCE_FILES dect2020ext-spectrum-management-procedure.cc
    LIBRARIES_TO_LINK
        ${libdect-nr-ext}
)