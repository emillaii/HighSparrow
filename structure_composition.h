#ifndef STRUCTURE_COMPOSITION_H
#define STRUCTURE_COMPOSITION_H

#include "aaHeadModule/aaheadmodule.h"
#include "basemodulemanager.h"
#include "material_carrier.h"



class StructureComposition
{
public:
    StructureComposition();
    bool Init(BaseModuleManager manager);
public:
    AAHeadModule* aa_head;
    MaterialCarrier* lut;
    MaterialCarrier* sut;
};

#endif // STRUCTURE_COMPOSITION_H
