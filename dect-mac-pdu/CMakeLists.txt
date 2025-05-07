set(source_files
    model/dect2020-beacon-header.cc
    model/dect2020-beacon-message.cc
    model/dect2020-mac-header-type.cc
    model/dect2020-mac-pdu.cc
    model/dect2020-route-info.cc
    model/dect2020-unicast-header.cc)

set(header_files
    model/dect2020-beacon-header.h
    model/dect2020-beacon-message.h
    model/dect2020-mac-header-type.h
    model/dect2020-mac-pdu.h
    model/dect2020-route-info.h
    model/dect2020-unicast-header.h)

set(test_source_files
    test/dect2020-beacon-header-test.cc
    test/dect2020-beacon-header-test-case.cc
    test/dect2020-beacon-message-test.cc
    test/dect2020-beacon-message-test-case.cc
    test/dect2020-mac-header-type-test.cc
    test/dect2020-mac-header-type-test-case.cc
    test/dect2020-route-info-test.cc
    test/dect2020-route-info-test-case.cc)

build_lib(
    LIBNAME dect-mac-pdu
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    TEST_SOURCES ${test_source_files}
)