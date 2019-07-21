#include "structure_composition.h"

StructureComposition::StructureComposition()
{

}

bool StructureComposition::Init(BaseModuleManager manager)
{
    XtMotor *x,*y,*z,*a,*b,*c;
    XtVcMotor *z_v;
    XtVacuum *v;
    x = manager.GetMotorByName("LUT_X");
    y = manager.GetMotorByName("LUT_Y");
    z_v = manager.GetVcMotorByName("LUT_Z");
    v = manager.GetVacuumByName("LUT_V");
    if(x == nullptr||y == nullptr||z_v == nullptr||v == nullptr)return false;
    lut = new MaterialCarrier("LUT",x,y,z_v,v);
    x = manager.GetMotorByName("SUT_X");
    y = manager.GetMotorByName("SUT_Y");
    z_v = manager.GetVcMotorByName("SUT_Z");
    v = manager.GetVacuumByName("SUT_V");
    if(x == nullptr||y == nullptr||z_v == nullptr||v == nullptr)return false;
    sut =new MaterialCarrier("SUT",x,y,z_v,v);
    x = manager.GetMotorByName("AA_X");
    y = manager.GetMotorByName("AA_Y");
    z = manager.GetMotorByName("AA_Z");
    a = manager.GetMotorByName("AA_A");
    b = manager.GetMotorByName("AA_B");
    c = manager.GetMotorByName("AA_C");
    v = manager.GetVacuumByName("AA_V");
    if(x == nullptr||y == nullptr||z == nullptr||a == nullptr||b == nullptr||c == nullptr||v == nullptr)return false;
    aa_head = new AAHeadModule("AAHead",x,y,z,a,b,c,v);

    return true;
}
