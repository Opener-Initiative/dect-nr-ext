# Check if stdint library is available
include(CheckIncludeFile)
check_include_file(stdint.h HAVE_STDINT_H)
if(NOT HAVE_STDINT_H)
    message(FATAL_ERROR "stdint.h not found. This is required for the dect-mac-pdu module.")
endif()

# Helper
set(mac_pdu_helper_src
    helper/dect2020-mac-pdu-helper.cc)

set(mac_pdu_helper_h
    helper/dect2020-mac-pdu-helper.h)

# Model
set(mac_header_type_src
    model/mac-header-type/dect2020-mac-header-type.cc)

set(mac_header_type_h
    model/mac-header-type/dect2020-mac-header-type.h)

set(mac_common_headers_src
    model/mac-common-headers/beacon-header/dect2020-beacon-header.cc
    model/mac-common-headers/data-mac-pdu-header/dect2020-data-mac-pdu-header.cc
    model/mac-common-headers/rd-broadcasting-header/dect2020-rd-broadcasting-header.cc
    model/mac-common-headers/unicast-header/dect2020-unicast-header.cc)

set(mac_common_headers_h
    model/mac-common-headers/beacon-header/dect2020-beacon-header.h
    model/mac-common-headers/data-mac-pdu-header/dect2020-data-mac-pdu-header.h
    model/mac-common-headers/rd-broadcasting-header/dect2020-rd-broadcasting-header.h
    model/mac-common-headers/unicast-header/dect2020-unicast-header.h)

# Tests
set(mac_header_type_test_src
    test/mac-header-type/test.cc
    test/mac-header-type/test-case.cc)

set(rd_broadcasting_header_test_src
    test/mac-common-headers/rd-broadcasting-header/test.cc
    test/mac-common-headers/rd-broadcasting-header/test-case.cc)

build_lib(
    LIBNAME dect-nr
    SOURCE_FILES
        ${mac_pdu_helper_src}
        ${mac_header_type_src}
        ${mac_common_headers_src}
    HEADER_FILES
        ${mac_pdu_helper_h}
        ${mac_header_type_h}
        ${mac_common_headers_h}
    TEST_SOURCES
        ${mac_header_type_test_src}
        ${rd_broadcasting_header_test_src}
    LIBRARIES_TO_LINK
        ${libcore}
        ${libnetwork}
)