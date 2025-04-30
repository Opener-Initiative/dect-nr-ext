build_lib_example(
    NAME dect2020-simulation
    SOURCE_FILES dect2020-simulation.cc
    LIBRARIES_TO_LINK 
            ${libspectrum}
            ${spectrum}
            ${mobility}
            ${network}
            netanim
)