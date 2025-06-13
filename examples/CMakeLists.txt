build_lib_example(
    NAME mac-pdu-creation
    SOURCE_FILES mac-pdu-creation.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)

build_lib_example(
    NAME network-beacon-message
    SOURCE_FILES network-beacon-message.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)   

build_lib_example(
    NAME buffer-experiment
    SOURCE_FILES buffer-experiment.cc
    LIBRARIES_TO_LINK
        ${libnetwork}
)

build_lib_example(
    NAME mac-mux-header-type-a
    SOURCE_FILES mac-mux-header-type-a.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)