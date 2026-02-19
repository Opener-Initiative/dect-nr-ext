# Check if stdint library is available
# include(CheckIncludeFile)
# check_include_file(stdint.h HAVE_STDINT_H)
# if(NOT HAVE_STDINT_H)
#     message(FATAL_ERROR "stdint.h not found. This is required for the dect-nr-ext module.")
# endif()
###########
## TYPES ##
###########

set(dect2020ext_types_h
    model/dect-types/dect2020ext-types.h)

############
## HELPER ##
############

set(dect2020ext_mac_pdu_helper_src
    helper/mac-pdu/dect2020ext-mac-pdu-helper.cc)

set(dect2020ext_mac_pdu_helper_h
    helper/mac-pdu/dect2020ext-mac-pdu-helper.h)

###########
## MODEL ##
###########

# DECT-2020 EXT Utilities
set(dect2020ext_utilities_src
    model/utilities/dect2020ext-utilities.cc)

set(dect2020ext_utilities_h
    model/utilities/dect2020ext-utilities.h)

# DECT-2020 EXT Configuration
set(dect2020ext_configuration_src
    model/config/dect2020ext-config.cc)

set(dect2020ext_configuration_h
    model/config/dect2020ext-config.h
    model/config/dect2020ext-config-values.h)

# DECT-2020 EXT Mac Scheduler
set(dect2020ext_mac_scheduler_src
    model/dect-mac/dect2020ext-mac-scheduler.cc)

set(dect2020ext_mac_scheduler_h
    model/dect-mac/dect2020ext-mac-scheduler.h)

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
    model/dect-channel-manager/dect2020ext-channel-manager.cc)

set(dect2020ext_channel_manager_h
    model/dect-channel-manager/dect2020ext-channel-manager.h)

# DECT-2020 EXT MAC
set(dect2020ext_mac_src
    model/dect-mac/dect2020ext-mac.cc)

set(dect2020ext_mac_h
    model/dect-mac/dect2020ext-mac.h)

# DECT-2020 EXT PHY
set(dect2020ext_phy_src
    model/dect-phy/dect2020ext-phy.cc
    model/dect-phy/dect2020ext-interference.cc
    model/dect-phy/dect2020ext-spectrum-signal-parameters.cc)

set(dect2020ext_phy_h
    model/dect-phy/dect2020ext-phy.h
    model/dect-phy/dect2020ext-interference.h
    model/dect-phy/dect2020ext-spectrum-signal-parameters.h)

# DECT-2020 EXT Physical Header Field
set(dect2020ext_physical_header_field_src
    model/mac-pdu/physical-header-field/dect2020ext-phy-control-field-type-1.cc)

set(dect2020ext_physical_header_field_h
    model/mac-pdu/physical-header-field/dect2020ext-phy-control-field-type-1.h)

# DECT-2020 EXT MAC Header Type
set(dect2020ext_mac_header_type_src
    model/mac-pdu/mac-header-type/dect2020ext-mac-header-type.cc)

set(dect2020ext_mac_header_type_h
    model/mac-pdu/mac-header-type/dect2020ext-mac-header-type.h)

# DECT-2020 EXT MAC Common Headers
set(dect2020ext_mac_common_headers_src
    model/mac-pdu/mac-common-headers/beacon-header/dect2020ext-beacon-header.cc
    model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020ext-data-mac-pdu-header.cc
    model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020ext-rd-broadcasting-header.cc
    model/mac-pdu/mac-common-headers/unicast-header/dect2020ext-unicast-header.cc)

set(dect2020ext_mac_common_headers_h
    model/mac-pdu/mac-common-headers/beacon-header/dect2020ext-beacon-header.h
    model/mac-pdu/mac-common-headers/data-mac-pdu-header/dect2020ext-data-mac-pdu-header.h
    model/mac-pdu/mac-common-headers/rd-broadcasting-header/dect2020ext-rd-broadcasting-header.h
    model/mac-pdu/mac-common-headers/unicast-header/dect2020ext-unicast-header.h)

# Mac Multiplexing Header
set(dect2020ext_mac_multiplexing_header_src
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a.cc)

set(dect2020ext_mac_multiplexing_header_h
    model/mac-pdu/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a.h)

# Information Elements
set(dect2020ext_information_elements_src
    model/mac-pdu/information-elements/network-beacon-message-ie/dect2020ext-network-beacon-message-ie.cc
    model/mac-pdu/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie.cc
    model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie.cc
    model/mac-pdu/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie.cc
    model/mac-pdu/information-elements/association-request-message-ie/dect2020ext-association-request-message-ie.cc
    model/mac-pdu/information-elements/association-response-message-ie/dect2020ext-association-response-message-ie.cc
    model/mac-pdu/information-elements/rd-capability-ie/dect2020ext-rd-capability-ie.cc)

set(dect2020ext_information_elements_h
    model/mac-pdu/information-elements/network-beacon-message-ie/dect2020ext-network-beacon-message-ie.h
    model/mac-pdu/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie.h
    model/mac-pdu/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie.h
    model/mac-pdu/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie.h
    model/mac-pdu/information-elements/association-request-message-ie/dect2020ext-association-request-message-ie.h
    model/mac-pdu/information-elements/association-response-message-ie/dect2020ext-association-response-message-ie.h
    model/mac-pdu/information-elements/rd-capability-ie/dect2020ext-rd-capability-ie.h)

# set(route_info_ie_src
#     model/mac-pdu/information-elements/route-info-ie/dect2020ext-route-info-ie.cc)

# set(route_info_ie_h
#     model/mac-pdu/information-elements/route-info-ie/dect2020ext-route-info-ie.h)

###########
## TESTS ##
###########

# Message Tests
set(dect2020ext_message_test_src
    test/network-beacon-message/dect2020ext-network-beacon-message-serialization-test-case.cc
    test/network-beacon-message/dect2020ext-network-beacon-message-test.cc
    test/cluster-beacon-message/dect2020ext-cluster-beacon-message-serialization-test-case.cc
    test/cluster-beacon-message/dect2020ext-cluster-beacon-message-test.cc)

# Spectrum Management Procedure Test
set(dect2020ext_spectrum_management_procedure_test_src
    test/spectrum-management-procedure/dect2020ext-spectrum-management-procedure-test.cc
    test/spectrum-management-procedure/dect2020ext-scan-measurement-duration-test-case.cc
    test/spectrum-management-procedure/dect2020ext-rssi-measurement-test-case.cc
    test/spectrum-management-procedure/dect2020ext-network-beacon-period-test-case.cc)

# Configuration Test
set(dect2020ext_configuration_test_src
    test/configuration/dect2020ext-configuration-test.cc
    test/configuration/dect2020ext-configuration-test-case.cc)

# Physical Header Field
set(dect2020ext_physical_header_field_test_src
    test/physical-header-field/dect2020ext-physical-header-field-test.cc
    test/physical-header-field/dect2020ext-physical-header-field-serialization-test-case.cc)

# Information Elements Tests

set(dect2020ext_information_elements_test_src
    test/information-elements/network-beacon-message-ie/test.cc
    test/information-elements/network-beacon-message-ie/serialization-test-case.cc
    test/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie-test.cc
    test/information-elements/cluster-beacon-message-ie/dect2020ext-cluster-beacon-message-ie-serialization-test-case.cc
    test/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie-test.cc
    test/information-elements/random-access-resource-ie/dect2020ext-random-access-resource-ie-serialization-test-case.cc)

# MAC Common Headers

set(dect2020ext_mac_common_headers_test_src
    test/mac-common-headers/beacon-header/dect2020ext-beacon-header-test.cc
    test/mac-common-headers/beacon-header/dect2020ext-beacon-header-serialization-test-case.cc)

# Mac Header Type Test
set(dect2020ext_mac_header_type_test_src
    test/mac-header-type/dect2020ext-mac-header-type-test.cc
    test/mac-header-type/dect2020ext-mac-header-type-serialization-test-case.cc)

# Mac Multiplexing Header Test
set(mac_multiplexing_header_test_src
    test/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a-test.cc
    test/mac-multiplexing-header/mac-multiplexing-header-type-a/dect2020ext-mac-multiplexing-header-type-a-serialization-test-case.cc)

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
        ${dect2020ext_mac_pdu_helper_src}
        ${dect2020ext_physical_header_field_src}
        ${dect2020ext_mac_header_type_src}
        ${dect2020ext_mac_common_headers_src}
        ${dect2020ext_information_elements_src}
        ${dect2020ext_mac_multiplexing_header_src}
        ${dect2020ext_spectrum_signal_parameters_src}
        ${dect2020ext_mac_scheduler_src}
        ${dect2020ext_configuration_src}
        ${dect2020ext_utilities_src}
    HEADER_FILES
        ${dect2020ext_types_h}
        ${dect2020ext_net_device_h}
        ${dect2020ext_channel_manager_h}
        ${dect2020ext_channel_h}
        ${dect2020ext_mac_h}
        ${dect2020ext_phy_h}
        ${dect2020ext_mac_pdu_helper_h}
        ${dect2020ext_physical_header_field_h}
        ${dect2020ext_mac_header_type_h}
        ${dect2020ext_mac_common_headers_h}
        ${dect2020ext_information_elements_h}
        ${dect2020ext_mac_multiplexing_header_h}
        ${dect2020ext_spectrum_signal_parameters_h}
        ${dect2020ext_mac_scheduler_h}
        ${dect2020ext_configuration_h}
        ${dect2020ext_utilities_h}
    TEST_SOURCES
        ${dect2020ext_information_elements_test_src}
        ${dect2020ext_physical_header_field_test_src}
        ${dect2020ext_configuration_test_src}
        ${dect2020ext_message_test_src}
        ${mac_multiplexing_header_test_src}
        ${dect2020ext_mac_common_headers_test_src}
        ${dect2020ext_mac_header_type_test_src}
        # ${dect2020ext_spectrum_management_procedure_test_src}
    LIBRARIES_TO_LINK
        ${libnetwork}
        ${libspectrum}
        ${libmobility}
        ${libpropagation}
)