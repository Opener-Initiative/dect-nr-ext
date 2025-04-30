check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(source_files
    # model/mac-pdu/dect2020-mesh-header.cc
    # model/mac-pdu/dect2020-mac-pdu.cc
    # model/mac-pdu/dect2020-beacon-header.cc
    # model/mac-pdu/dect2020-beacon-message.cc
    # model/mac-pdu/dect2020-mac-header-type.cc
)

set(header_files
    # model/mac-pdu/dect2020-mesh-header.h
    # model/mac-pdu/dect2020-mac-pdu.h
    # model/mac-pdu/dect2020-beacon-header.h
    # model/mac-pdu/dect2020-beacon-message.h
    # model/mac-pdu/dect2020-mac-header-type.h
)

# build_lib(
#     LIBNAME dect_nr
#     SOURCE_FILES ${source_files}
#     HEADER_FILES ${header_files}
#     LIBRARIES_TO_LINK
#             ${spectrum}
#             ${mobility}
#             ${network}
# )