#ifndef DECT2020EXT_PHY_H
#define DECT2020EXT_PHY_H

#include "ns3/dect2020-phy.h"

namespace ns3
{

class Dect2020ExtPhy : public Object
{
  public:
    Dect2020ExtPhy();
    virtual ~Dect2020ExtPhy();

    static TypeId GetTypeId();
};

} // namespace ns3

#endif /* DECT2020EXT_PHY_H */