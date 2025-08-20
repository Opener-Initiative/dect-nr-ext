#include "dect2020ext-phy.h"

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhy");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhy);

TypeId
Dect2020ExtPhy::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhy")
                            .SetParent<Dect2020Phy>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhy>();
    return tid;
}

Dect2020ExtPhy::Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

} // namespace ns3