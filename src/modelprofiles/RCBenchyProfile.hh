#pragma once
#include "modelprofile.hh"
#include <vector>
#include "../objects/ControllBlocks/OnOffBlock.hh"

class RCBenchyProfile : public ModelProfile {
public: 
    std::vector<GroupBlock*> LoadModel() override;
};