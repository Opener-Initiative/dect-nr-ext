#ifndef DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtPhyControlFieldType1SerializationTestCase : public TestCase
{
    public:
        Dect2020ExtPhyControlFieldType1SerializationTestCase();
        virtual ~Dect2020ExtPhyControlFieldType1SerializationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif //DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_SERIALIZATION_TEST_CASE_H