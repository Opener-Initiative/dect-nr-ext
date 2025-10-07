# Check if stdint library is available
include(CheckIncludeFile)
check_include_file(stdint.h HAVE_STDINT_H)
if(NOT HAVE_STDINT_H)
    message(FATAL_ERROR "stdint.h not found. This is required for the dect-nr-ext module.")
endif()

##########
##HELPER##
##########

# set(mac_pdu_helper_src
#     helper/mac-pdu/dect2020ext-mac-pdu-helper.cc)

# set(mac_pdu_helper_h
#     helper/mac-pdu/dect2020ext-mac-pdu-helper.h)

#########
##MODEL##
#########

# DECT-2020 EXT NetDevice
set(dect2020ext_net_device_src
    model/dect-net-device/dect2020ext-net-device.cc)

set(dect2020ext_net_device_h
    model/dect-net-device/dect2020ext-net-device.h)

# DECT-2020 EXT Channel
set(dect2020ext_channel_src
    model/dect-channel/dect2020ext-channel.cc)
    
set(dect2020ext_channel_h
    model/dect-channel/dect2020ext-channel.h)

# DECT-2020 EXT Channel Manager
set(dect2020ext_channel_manager_src
    model/dect-channel/dect2020ext-channel-manager.cc)

set(dect2020ext_channel_manager_h
    model/dect-channel/dect2020ext-channel-manager.h)

# DECT-2020 EXT MAC
set(dect2020ext_mac_src
    model/dect-mac/dect2020ext-mac.cc)

set(dect2020ext_mac_h
    model/dect-mac/dect2020ext-mac.h)

# DECT-2020 EXT PHY
set(dect2020ext_phy_src
    model/dect-phy/dect2020ext-phy.cc)

set(dect2020ext_phy_h
    model/dect-phy/dect2020ext-phy.h)

# # Physical Header Field
# set(physical_header_field_src
#     model/mac-pdu/physical-header-field/dect2020ext-physical-header-field.cc)

# set(physical_header_field_h
#     model/mac-pdu/physical-header-field/dect2020ext-physical-header-field.h)

# # Mac Header Type
# set(mac_header_type_src
#     model/mac-pdu/mac-header-type/dect2020ext-mac-header-type.cc)

# set(mac_header_type_h
#     model/mac-pdu/mac-header-type/dect2020ext-mac-header-type.h)

# # Common Headers
# set(mac_common_headers_src
#     model/mac-pdu/mac-common-headers/beacon-header/dect2020ext-beacon-header.cc
#     model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020ext-data-mac-pdu-header.cc
#     model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020ext-rd-broadcasting-header.cc
#     model/mac-pdu/mac-common-headers/unicast-header/dect2020ext-unicast-header.cc)

# set(mac_common_headers_h
#     model/mac-pdu/mac-common-headers/beacon-header/dect2020ext-beacon-header.h
#     model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020ext-data-mac-pdu-header.h
#     model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020ext-rd-broadcasting-header.h
#     model/mac-pdu/mac-common-headers/unicast-header/dect2020ext-unicast-header.h)

# # Mac Multiplexing Header
# set(mac_multiplexing_header_src
#     model/mac-pdu/mac-multiplexing-header/dect2020ext-mac-multiplexing-header.cc
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a.cc
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020ext-mac-multiplexing-header-type-b.cc
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-c/dect2020ext-mac-multiplexing-header-type-c.cc)

# set(mac_multiplexing_header_h
#     model/mac-pdu/mac-multiplexing-header/dect2020ext-mac-multiplexing-header.h
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a.h
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-b/dect2020ext-mac-multiplexing-header-type-b.h
#     model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-c/dect2020ext-mac-multiplexing-header-type-c.h)

# # Information Elements
# set(network_beacon_message_ie_src
#     model/mac-pdu/information-elements/network-beacon-message-ie/dect2020ext-network-beacon-message-ie.cc)

# set(network_beacon_message_ie_h
#     model/mac-pdu/information-elements/network-beacon-message-ie/dect2020ext-network-beacon-message-ie.h)

# set(cluster_beacon_message_ie_src
#     model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie.cc)

# set(cluster_beacon_message_ie_h
#     model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie.h)

# set(route_info_ie_src
#     model/mac-pdu/information-elements/route-info-ie/dect2020ext-route-info-ie.cc)

# set(route_info_ie_h
#     model/mac-pdu/information-elements/route-info-ie/dect2020ext-route-info-ie.h)

# set(association_request_message_ie_src
#     model/mac-pdu/information-elements/association-request-message-ie/dect2020ext-association-request-message-ie.cc)

# set(association_request_message_ie_h
#     model/mac-pdu/information-elements/association-request-message-ie/dect2020ext-association-request-message-ie.h)

#########
##Tests##
#########

# # Physical Header Field
# set(physical_header_field_test_src
#     test/physical-header-field/test.cc
#     test/physical-header-field/serialization-test-case-1.cc
#     test/physical-header-field/serialization-test-case-2.cc)

# # MAC Common Headers

# # Unicast Header Test
# set(unicast_header_test_src
#     test/mac-common-headers/unicast-header/test.cc
#     test/mac-common-headers/unicast-header/serialization-test-case.cc)

# # Rd Broadcasting Header Test
# set(rd_broadcasting_header_test_src
#     test/mac-common-headers/rd-broadcasting-header/test.cc
#     test/mac-common-headers/rd-broadcasting-header/serialization-test-case.cc)

# # Data Mac PDU Test
# set(data_mac_pdu_test_src
#     test/mac-common-headers/data-mac-pdu/test.cc
#     test/mac-common-headers/data-mac-pdu/serialization-test-case.cc)

# # Beacon Header Test
# set(beacon_header_test_src
#     test/mac-common-headers/beacon-header/test.cc
#     test/mac-common-headers/beacon-header/serialization-test-case.cc)

# # Mac Header Type Test
# set(mac_header_type_test_src
#     test/mac-header-type/test.cc
#     test/mac-header-type/serialization-test-case.cc)

# # Mac Multiplexing Header Test
# set(mac_multiplexing_header_test_src
#     test/mac-multiplexing-header/mac-multiplexing-header-type-c/test.cc
#     test/mac-multiplexing-header/mac-multiplexing-header-type-c/payload-insertion-test-case.cc
#     test/mac-multiplexing-header/mac-multiplexing-header-type-c/serialization-test-case.cc)

# # Information Elements

# set(network_beacon_message_ie_test_src
#     test/information-elements/network-beacon-message-ie/test.cc
#     test/information-elements/network-beacon-message-ie/serialization-test-case.cc)

# set(route_info_ie_test_src
#     test/information-elements/route-info-ie/test.cc
#     test/information-elements/route-info-ie/serialization-test-case.cc)

build_lib(
    LIBNAME dect-nr-ext
    SOURCE_FILES
        ${dect2020ext_net_device_src}
        ${dect2020ext_channel_manager_src}
        ${dect2020ext_channel_src}
        ${dect2020ext_mac_src}
        ${dect2020ext_phy_src}
    HEADER_FILES
        ${dect2020ext_net_device_h}
        ${dect2020ext_channel_manager_h}
        ${dect2020ext_channel_h}
        ${dect2020ext_mac_h}
        ${dect2020ext_phy_h}
    LIBRARIES_TO_LINK
        ${libnetwork}
        ${libdect-nr}
)