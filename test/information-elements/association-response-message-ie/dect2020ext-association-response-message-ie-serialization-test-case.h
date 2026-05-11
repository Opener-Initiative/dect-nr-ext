#ifndef DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtAssociationResponseMessageIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtAssociationResponseMessageIeSerializationTestCase();
    virtual ~Dect2020ExtAssociationResponseMessageIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_SERIALIZATION_TEST_CASE_H */