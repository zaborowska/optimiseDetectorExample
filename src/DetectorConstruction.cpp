#include "DetectorConstruction.h"
#include "DetectorMessenger.h"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4SDManager.hh"
#include "SensitiveDetector.h"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
class G4Material;
class DetectorMessenger;

G4ThreadLocal G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction(), fLayerLV(nullptr), fNofLayers(1) {
  fDetectorMessenger= new DetectorMessenger(this);

  // Geometry parameters (default)
  fNofLayers = 10;
  fAbsorThickness = 10.*mm;
  fCalorSizeXY  = 10.*cm;
  SetWorldMaterial("G4_Galactic");
  SetAbsorMaterial("G4_PbWO4");
}

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Define volumes
  return DefineVolumes();
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
  G4bool fCheckOverlaps = false;

  auto calorThickness = fNofLayers * fAbsorThickness;
  auto worldSizeXY = 2 * fCalorSizeXY;
  auto worldSizeZ  = 2 * calorThickness;

  if ( ! fWorldMaterial || ! fAbsorMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  //
  // World
  //
  auto worldS
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ); // its size

  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 fWorldMaterial,  // its material
                 "World");         // its name

  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

  //
  // Calorimeter
  //
  auto calorimeterS
    = new G4Box("Calorimeter",     // its name
                 fCalorSizeXY/2, fCalorSizeXY/2, calorThickness/2); // its size

  auto calorLV
    = new G4LogicalVolume(
                 calorimeterS,     // its solid
                 fWorldMaterial,  // its material
                 "Calorimeter");   // its name

  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0,0,calorThickness/2.),  // at (0,0,0)
                 calorLV,          // its logical volume
                 "Calorimeter",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

  //
  // Layer
  //
  auto layerS
    = new G4Box("Layer",           // its name
                 fCalorSizeXY/2, fCalorSizeXY/2, fAbsorThickness/2); //its size

  fLayerLV
    = new G4LogicalVolume(
                 layerS,           // its solid
                 fAbsorMaterial,  // its material
                 "LayerLV");         // its name

  new G4PVReplica(
                 "Layer",          // its name
                 fLayerLV,          // its logical volume
                 calorLV,          // its mother
                 kZAxis,           // axis of replication
                 fNofLayers,        // number of replica
                 fAbsorThickness);  // witdth of replica

  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(0,1,0));
  simpleBoxVisAtt->SetVisibility(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  auto layerVisAtt= new G4VisAttributes(G4Colour(0,0,1.0));
  layerVisAtt->SetVisibility(true);
  fLayerLV->SetVisAttributes(layerVisAtt);

  auto worldVisAtt= new G4VisAttributes(G4Colour(1,0,0));
  worldVisAtt->SetVisibility(true);
  worldLV->SetVisAttributes(worldVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

void DetectorConstruction::ConstructSDandField() {
  auto absoSD = dynamic_cast<SensitiveDetector*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("Absorber"));
  if(! absoSD) {
    absoSD = new SensitiveDetector("Absorber", "AbsorberHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  }
  SetSensitiveDetector(fLayerLV, absoSD);
  absoSD->SetVerboseLevel(2);
  absoSD->SetNbOfLayers(fNofLayers);

  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintCalorParameters()
{
  //
  // print parameters
  //
  G4cout
    << G4endl
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is " << fNofLayers << " layers of: [ "
    << fAbsorThickness/mm << "mm of " << fAbsorMaterial->GetName() << " ] " << G4endl
    << fCalorSizeXY/mm << "mm in transverse size." << G4endl
    << "------------------------------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String& material)
{
  // search the material by its name
  G4Material* pttoMaterial =
    G4NistManager::Instance()->FindOrBuildMaterial(material);
  if (pttoMaterial) fWorldMaterial = pttoMaterial;
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorMaterial(const G4String& material)
{
  // search the material by its name
  G4Material* pttoMaterial =
    G4NistManager::Instance()->FindOrBuildMaterial(material);
  if (pttoMaterial) fAbsorMaterial = pttoMaterial;
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfLayers(G4int ival)
{
  // set the number of Layers
  //
  if (ival < 1)
    { G4cout << "\n --->warning from SetfNbOfLayers: "
             << ival << " must be at least 1. Command refused" << G4endl;
      return;
    }
  fNofLayers = ival;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorThickness(G4double val)
{
  // change Absorber thickness
  if (val <= DBL_MIN)
    { G4cout << "\n --->warning from SetAbsorThickness: thickness "
             << val  << " out of range. Command refused" << G4endl;
      return;
    }
  fAbsorThickness = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCalorSizeXY(G4double val)
{
  // change the transverse size
  //
  if (val <= DBL_MIN)
    { G4cout << "\n --->warning from SetfCalorSizeYZ: thickness "
             << val  << " out of range. Command refused" << G4endl;
      return;
    }
  fCalorSizeXY = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
