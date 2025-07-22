#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/pointer.h"

namespace ns3
{

class SimpleNetDevice;
class SimpleChannel;

class Dect2020NetDevice : public Object
{
    public:
        Dect2020NetDevice();
        ~Dect2020NetDevice();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        // Setters and Getters

        Ptr<SimpleNetDevice> GetDevice(void) const;

    private:
        Ptr<SimpleNetDevice> m_device;
};

} // namespace ns3