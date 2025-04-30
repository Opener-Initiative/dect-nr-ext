set(source_files
    model/dect2020-beacon-header.cc
    model/dect2020-beacon-message.cc
    model/dect2020-mac-header-type.cc
    model/dect2020-mac-pdu.cc
    model/dect2020-route-info.cc)

set(header_files
    model/dect2020-beacon-header.h
    model/dect2020-beacon-message.h
    model/dect2020-mac-header-type.h
    model/dect2020-mac-pdu.h
    model/dect2020-route-info.h)

build_lib(
    LIBNAME dect-mac-pdu
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
)