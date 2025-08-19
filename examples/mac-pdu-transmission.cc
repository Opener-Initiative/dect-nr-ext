#include "ns3/core-module.h"
#include "ns3/dect2020ext-mac-pdu-helper.h"

using namespace ns3;

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    std::cout << "DECT-NR-Ext MAC PDU Example" << std::endl;
    std::cout << "This example demonstrates the MAC PDU functionality." << std::endl;

    // Create a simple MAC PDU helper instance
    Dect2020ExtMacPduHelper helper;

    std::cout << "MAC PDU Helper created successfully!" << std::endl;
    std::cout << "Example completed successfully." << std::endl;

    return 0;
}
