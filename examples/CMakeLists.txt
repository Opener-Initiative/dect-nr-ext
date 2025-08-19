build_lib_example(
    NAME mac-pdu-transmission
    SOURCE_FILES mac-pdu-transmission.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)