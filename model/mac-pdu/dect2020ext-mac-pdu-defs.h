#ifndef DECT2020EXT_MAC_PDU_DEFS_H
#define DECT2020EXT_MAC_PDU_DEFS_H

#define DECT2020EXT_MAC_PDU_FATAL(message, outputValue) \
    NS_FATAL_ERROR("Error in " \
                        << std::string(__func__) \
                        << " - " << message \
                        << ", got " << outputValue)

#endif