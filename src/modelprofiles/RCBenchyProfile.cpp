#include "RCBenchyProfile.hh"
#include "../actions/LinearBlinkAction.cpp"
#include "../actions/BlinkAction.cpp"
#include "../actions/RandomBlinkAction.cpp"
#include "../objects/ControllBlocks/ActionBlock.hh"
#include "../objects/ControllBlocks/OnOffBlock.hh"
#include "../objects/ControllBlocks/SliderBlock.hh"
#include "../objects/ControllBlocks/InputSliderBlock.hh"

std::vector<GroupBlock*> RCBenchyProfile::LoadModel() 
{
  std::vector<GroupBlock*> Groups;
  const std::vector<int> allBlockPins = {26,27,14,12};


  Groups.push_back(new GroupBlock(0,controll));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0, {12},"Levá"));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 1, {14},"Kabina"));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0, {27},"Pravá"));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0, {12,14,27},"Kabina vše"));
  Groups.push_back(new GroupBlock(1,controll));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0,{12,27},"Pozičaky"));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0, allBlockPins,"Vše"));
  Groups.push_back(new GroupBlock(2,controll));
  Groups.back()->blocks.push_back(new OnOffBlock(0, 0, {26},"Reflektor"));
  Groups.push_back(new GroupBlock(3,inputSlider));
  Groups.back()->blocks.push_back(new InputSliderBlock(6,0,{26},"Reflektor slider"));
  Groups.push_back(new GroupBlock(4,action));
  Groups.back()->blocks.push_back(new ActionBlock(3, 0, {12,14,27},"Action Random Kabina", new RandomBlinkAction()));
  Groups.back()->blocks.push_back(new ActionBlock(3, 0, allBlockPins,"Action Random Vše", new RandomBlinkAction()));




  return Groups;
}