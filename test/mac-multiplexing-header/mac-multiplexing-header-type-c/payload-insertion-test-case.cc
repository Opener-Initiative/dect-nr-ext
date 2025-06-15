#include "payload-insertion-test-case.h"
#include "ns3/log.h"

#include "ns3/dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/dect2020-cluster-beacon-message-ie.h"

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase");

namespace ns3
{

Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase::Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase()
	: TestCase("Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase")
{
}

void
Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase::DoSetup()
{
}

void
Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase::DoRun()
{
	Dect2020MacMultiplexingHeaderTypeC mxHeader;
	mxHeader.AddPayload<Dect2020ClusterBeaconMessageIE>();
	mxHeader.RemovePayload();
}

} // namespace ns3