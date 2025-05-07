set(base_examples
    pdu-creation
    serialization-deserialization
)

foreach(
    example
    ${base_examples}
)
    build_lib_example(
        NAME ${example}
        SOURCE_FILES ${example}.cc
        LIBRARIES_TO_LINK
            ${libdect-mac-pdu}
            ${libcore}
            ${libinternet}
    )

endforeach()