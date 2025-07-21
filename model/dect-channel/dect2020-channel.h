// Dect2020Channel - wrapper around ns3::SimpleChannel

#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/pointer.h"

namespace ns3
{

class SimpleChannel;
class SimpleNetDevice;

class Dect2020Channel : public SimpleChannel
{
    public:
        Dect2020Channel();
        virtual ~Dect2020Channel();

    private:
        Ptr<SimpleChannel> m_channel;
};

} // namespace ns3