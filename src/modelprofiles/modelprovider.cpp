#include "modelprovider.hh"
#include "R18ModelProfile.hh"
#include "RCBenchyProfile.hh"

ModelProvider::ModelProvider()
{
    modelProfile = new RCBenchyProfile();
    this->LoadModel();
}
void ModelProvider::LoadModel() 
{
    this->Groups = modelProfile->LoadModel();
  //TODO:check if local exist or fall back to default

}
std::vector<GroupBlock*> ModelProvider::GetGroups()
{
    return this->Groups;
}
