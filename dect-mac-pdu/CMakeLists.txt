set(source_files
    model/headers/dect2020-beacon-header.cc
    model/headers/dect2020-mac-header-type.cc
    model/headers/dect2020-unicast-header.cc
    model/headers/dect2020-physical-header-field.cc
    model/messages/dect2020-beacon-message.cc
    model/information-elements/dect2020-route-info.cc
    model/dect2020-mac-pdu.cc)

set(header_files
    model/headers/dect2020-beacon-header.h
    model/headers/dect2020-mac-header-type.h
    model/headers/dect2020-unicast-header.h
    model/headers/dect2020-physical-header-field.h
    model/messages/dect2020-beacon-message.h
    model/information-elements/dect2020-route-info.h
    model/dect2020-mac-pdu.h)

set(test_source_files
    test/headers/dect2020-beacon-header-test.cc
    test/headers/dect2020-beacon-header-test-case.cc
    test/headers/dect2020-mac-header-type-test.cc
    test/headers/dect2020-mac-header-type-test-case.cc
    test/headers/dect2020-unicast-header-test.cc
    test/headers/dect2020-unicast-header-test-case.cc
    test/headers/dect2020-physical-header-field-test.cc
    test/headers/dect2020-physical-header-field-test-case.cc
    test/messages/dect2020-beacon-message-test.cc
    test/messages/dect2020-beacon-message-test-case.cc
    test/information-elements/dect2020-route-info-test.cc
    test/information-elements/dect2020-route-info-test-case.cc)

build_lib(
    LIBNAME dect-mac-pdu
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    TEST_SOURCES ${test_source_files}
)