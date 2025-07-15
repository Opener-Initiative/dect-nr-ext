build_lib_example(
    NAME network-beacon-message
    SOURCE_FILES network-beacon-message.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)

build_lib_example(
    NAME cluster-beacon-message
    SOURCE_FILES cluster-beacon-message.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)   

build_lib_example(
    NAME mac-pdu-creation
    SOURCE_FILES mac-pdu-creation.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)