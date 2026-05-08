#ifndef DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtAssociationRequestMessageIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtAssociationRequestMessageIeSerializationTestCase();
    virtual ~Dect2020ExtAssociationRequestMessageIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_SERIALIZATION_TEST_CASE_H */
