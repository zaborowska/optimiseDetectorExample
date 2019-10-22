#include "DetectorMessenger.h"

#include <sstream>

#include "DetectorConstruction.h"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),fDetector(Det),
 fDetDir(0),
 fSizeXYCmd(0),
 fSizeZCmd(0),
 fNbLayersCmd(0),
 fAbsorMaterial(0),
 fWorldMaterial(0)
{
  fDetDir = new G4UIdirectory("/detector/");
  fDetDir->SetGuidance("detector construction commands");

  fSizeXYCmd = new G4UIcmdWithADoubleAndUnit("/detector/setSizeXY",this);
  fSizeXYCmd->SetGuidance("Set tranverse size of the calorimeter");
  fSizeXYCmd->SetParameterName("Size",false);
  fSizeXYCmd->SetRange("Size>0.");
  fSizeXYCmd->SetUnitCategory("Length");
  fSizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeXYCmd->SetToBeBroadcasted(false);

  fSizeZCmd = new G4UIcmdWithADoubleAndUnit("/detector/setSizeZ",this);
  fSizeZCmd->SetGuidance("Set tranverse size of the calorimeter");
  fSizeZCmd->SetParameterName("Size",false);
  fSizeZCmd->SetRange("Size>0.");
  fSizeZCmd->SetUnitCategory("Length");
  fSizeZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeZCmd->SetToBeBroadcasted(false);

  fNbLayersCmd = new G4UIcmdWithAnInteger("/detector/setNbOfLayers",this);
  fNbLayersCmd->SetGuidance("Set number of layers.");
  fNbLayersCmd->SetParameterName("NbLayers",false);
  fNbLayersCmd->SetRange("NbLayers>0");
  fNbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNbLayersCmd->SetToBeBroadcasted(false);

  fAbsorMaterial = new G4UIcmdWithAString("/detector/setDetectorMaterial",this);
  fAbsorMaterial->SetGuidance("Set material of detector.");
  fAbsorMaterial->SetParameterName("AbsorberMaterial",false);
  fAbsorMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  fAbsorMaterial->SetToBeBroadcasted(false);

  fWorldMaterial = new G4UIcmdWithAString("/detector/setWorldMaterial",this);
  fWorldMaterial->SetGuidance("Set material outside of detector (world).");
  fWorldMaterial->SetParameterName("WorldMaterial",false);
  fWorldMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWorldMaterial->SetToBeBroadcasted(false);
}

DetectorMessenger::~DetectorMessenger()
{
  delete fSizeXYCmd;
  delete fSizeZCmd;
  delete fNbLayersCmd;
  delete fDetDir;
  delete fAbsorMaterial;
  delete fWorldMaterial;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fSizeXYCmd )
   { fDetector->SetCalorSizeXY(fSizeXYCmd->GetNewDoubleValue(newValue));}

  if( command == fSizeZCmd )
    { fDetector->SetAbsorThickness(fSizeZCmd->GetNewDoubleValue(newValue));}

  if( command == fNbLayersCmd )
    { fDetector->SetNbOfLayers(fNbLayersCmd->GetNewIntValue(newValue));}

  if( command == fAbsorMaterial )
    { fDetector->SetAbsorMaterial(newValue);}

  if( command == fWorldMaterial )
    { fDetector->SetWorldMaterial(newValue);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
