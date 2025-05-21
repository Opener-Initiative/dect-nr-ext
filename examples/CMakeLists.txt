build_lib_example(
    NAME mac-pdu-creation
    SOURCE_FILES mac-pdu-creation.cc
    LIBRARIES_TO_LINK
        ${libdect-nr}
)