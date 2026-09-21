#pragma once

#include <vector>
#include <Print.h> 

#include "../config/env.hh"
#include "../objects/BasicBlock.hh"
#include "../objects/GroupBlock.hh"
#include "../enums/BlockTypeEnum.hh"

class Renderer
{
  public:
    void drawBlock(GroupBlock *bl,Print &client);
    void drawHead(Print &client);
    void drawHeader(Print &client);
#if REM_ENABLE_OFFLINE_MAP
    void drawOLMJS(Print &client);
#endif
    void drawNew(std::vector<GroupBlock *>blocks,Print &client);
  private:
    
    void drawSmallBlock(BasicBlock* cb,Print &client,BlockTypeEnum type);
};
