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


# Model --------------------------------------------------------------------------------

# Physical Header Field
set(physical_header_field_src
    model/mac-pdu/physical-header-field/dect2020-physical-header-field.cc)

set(physical_header_field_h
    model/mac-pdu/physical-header-field/dect2020-physical-header-field.h)

# Mac Header Type
set(mac_header_type_src
    model/mac-pdu/mac-header-type/dect2020-mac-header-type.cc)

set(mac_header_type_h
    model/mac-pdu/mac-header-type/dect2020-mac-header-type.h)

# Common Headers
set(mac_common_headers_src
    model/mac-pdu/mac-common-headers/beacon-header/dect2020-beacon-header.cc
    model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020-data-mac-pdu-header.cc
    model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020-rd-broadcasting-header.cc
    model/mac-pdu/mac-common-headers/unicast-header/dect2020-unicast-header.cc)

set(mac_common_headers_h
    model/mac-pdu/mac-common-headers/beacon-header/dect2020-beacon-header.h
    model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020-data-mac-pdu-header.h
    model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020-rd-broadcasting-header.h
    model/mac-pdu/mac-common-headers/unicast-header/dect2020-unicast-header.h)

# Mac Multiplexing Header
set(mac_multiplexing_header_src
    model/mac-pdu/mac-multiplexing-header/dect2020-mac-multiplexing-header.cc
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020-mac-multiplexing-header-type-a.cc
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020-mac-multiplexing-header-type-b.cc
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-c/dect2020-mac-multiplexing-header-type-c.cc)

set(mac_multiplexing_header_h
    model/mac-pdu/mac-multiplexing-header/dect2020-mac-multiplexing-header.h
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020-mac-multiplexing-header-type-a.h
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020-mac-multiplexing-header-type-b.h
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-c/dect2020-mac-multiplexing-header-type-c.h)

# Information Elements
set(network_beacon_message_ie_src
    model/mac-pdu/information-elements/network-beacon-message-ie/dect2020-network-beacon-message-ie.cc)

set(network_beacon_message_ie_h
    model/mac-pdu/information-elements/network-beacon-message-ie/dect2020-network-beacon-message-ie.h)

set(cluster_beacon_message_ie_src
    model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020-cluster-beacon-message-ie.cc)

set(cluster_beacon_message_ie_h
    model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020-cluster-beacon-message-ie.h)

set(route_info_ie_src
    model/mac-pdu/information-elements/route-info-ie/dect2020-route-info-ie.cc)

set(route_info_ie_h
    model/mac-pdu/information-elements/route-info-ie/dect2020-route-info-ie.h)

set(association_request_message_ie_src
    model/mac-pdu/information-elements/association-request-message-ie/dect2020-association-request-message-ie.cc)

set(association_request_message_ie_h
    model/mac-pdu/information-elements/association-request-message-ie/dect2020-association-request-message-ie.h)

# Tests --------------------------------------------------------------------------------

# Physical Header Field
set(physical_header_field_test_src
    test/physical-header-field/test.cc
    test/physical-header-field/serialization-test-case-1.cc
    test/physical-header-field/serialization-test-case-2.cc)

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
    test/mac-multiplexing-header/mac-multiplexing-header-type-c/test.cc
    test/mac-multiplexing-header/mac-multiplexing-header-type-c/payload-insertion-test-case.cc
    test/mac-multiplexing-header/mac-multiplexing-header-type-c/serialization-test-case.cc)

# Information Elements

set(network_beacon_message_ie_test_src
    test/information-elements/network-beacon-message-ie/test.cc
    test/information-elements/network-beacon-message-ie/serialization-test-case.cc)

set(route_info_ie_test_src
    test/information-elements/route-info-ie/test.cc
    test/information-elements/route-info-ie/serialization-test-case.cc)

# Network Beacon Message
set(network_beacon_message_example_src
    examples/network-beacon-message.cc)

build_lib(
    LIBNAME dect-nr
    SOURCE_FILES
        ${mac_pdu_helper_src}
        ${mac_header_type_src}
        ${mac_common_headers_src}
        ${network_beacon_message_ie_src}
        ${cluster_beacon_message_ie_src}
        ${mac_multiplexing_header_src}
        ${physical_header_field_src}
        ${route_info_ie_src}
        ${association_request_message_ie_src}
    HEADER_FILES
        ${mac_pdu_helper_h}
        ${mac_header_type_h}
        ${mac_common_headers_h}
        ${network_beacon_message_ie_h}
        ${cluster_beacon_message_ie_h}
        ${mac_multiplexing_header_h}
        ${physical_header_field_h}
        ${route_info_ie_h}
        ${association_request_message_ie_h}
    TEST_SOURCES
        ${mac_header_type_test_src}
        ${unicast_header_test_src}
        ${rd_broadcasting_header_test_src}
        ${data_mac_pdu_test_src}
        ${beacon_header_test_src}
        ${mac_multiplexing_header_test_src}
        ${physical_header_field_test_src}
        ${network_beacon_message_ie_test_src}
        ${route_info_ie_test_src}
    LIBRARIES_TO_LINK
        ${libcore}
        ${libnetwork}
)