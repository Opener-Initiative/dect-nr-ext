#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_SERIALIZATION_TEST_CASE_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeCSerializationTestCase : public TestCase
{
	public:
		Dect2020MacMultiplexingHeaderTypeCSerializationTestCase();
		virtual void DoSetup();
		virtual void DoRun();
};

} // namespace ns3

#endif /* DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_SERIALIZATION_TEST_CASE_H */