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

#######################################################################################

# Model

# Mac Header Type
set(mac_header_type_src
    model/mac-header-type/dect2020-mac-header-type.cc)

set(mac_header_type_h
    model/mac-header-type/dect2020-mac-header-type.h)

# Common Headers
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

# Mac Multiplexing Header
set(mac_multiplexing_header_src
    model/mac-multiplexing-header/dect2020-mac-multiplexing-header.cc
    model/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020-mac-multiplexing-header-type-a.cc
    model/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020-mac-multiplexing-header-type-b.cc)

set(mac_multiplexing_header_h
    model/mac-multiplexing-header/dect2020-mac-multiplexing-header.h
    model/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020-mac-multiplexing-header-type-a.h
    model/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020-mac-multiplexing-header-type-b.h)

# Information Elements
set(network_beacon_message_ie_src
    model/information-elements/network-beacon-message-ie/dect2020-network-beacon-message-ie.cc)

set(network_beacon_message_ie_h
    model/information-elements/network-beacon-message-ie/dect2020-network-beacon-message-ie.h)

#######################################################################################

# Tests

# MAC Common Headers

# Unicast Header Test
set(unicast_header_test_src
    test/mac-common-headers/unicast-header/test.cc
    test/mac-common-headers/unicast-header/serialization-test-case.cc)

# Rd Broadcasting Header Test
set(rd_broadcasting_header_test_src
    test/mac-common-headers/rd-broadcasting-header/test.cc
    test/mac-common-headers/rd-broadcasting-header/serialization-test-case.cc)

# Data Mac PDU Test
set(data_mac_pdu_test_src
    test/mac-common-headers/data-mac-pdu/test.cc
    test/mac-common-headers/data-mac-pdu/serialization-test-case.cc)

# Beacon Header Test
set(beacon_header_test_src
    test/mac-common-headers/beacon-header/test.cc
    test/mac-common-headers/beacon-header/serialization-test-case.cc)

# Mac Header Type Test
set(mac_header_type_test_src
    test/mac-header-type/test.cc
    test/mac-header-type/serialization-test-case.cc)

# Mac Multiplexing Header Test
set(mac_multiplexing_header_test_src
    test/mac-multiplexing-header/mac-multiplexing-header-type-b/test.cc
    test/mac-multiplexing-header/mac-multiplexing-header-type-b/payload-insertion-test-case.cc
    test/mac-multiplexing-header/mac-multiplexing-header-type-b/serialization-test-case.cc)

#######################################################################################

build_lib(
    LIBNAME dect-nr
    SOURCE_FILES
        ${mac_pdu_helper_src}
        ${mac_header_type_src}
        ${mac_common_headers_src}
        ${network_beacon_message_ie_src}
        ${mac_multiplexing_header_src}
    HEADER_FILES
        ${mac_pdu_helper_h}
        ${mac_header_type_h}
        ${mac_common_headers_h}
        ${network_beacon_message_ie_h}
        ${mac_multiplexing_header_h}
    TEST_SOURCES
        ${mac_header_type_test_src}
        ${unicast_header_test_src}
        ${rd_broadcasting_header_test_src}
        ${data_mac_pdu_test_src}
        ${beacon_header_test_src}
        ${mac_multiplexing_header_test_src}
    LIBRARIES_TO_LINK
        ${libcore}
        ${libnetwork}
)