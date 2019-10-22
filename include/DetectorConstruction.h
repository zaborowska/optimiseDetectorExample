#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class G4Material;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.
/// A single box is created, made of one material, can be divided into cells.
/// SD is attached to the box. It is also attached to the outer volume to
/// keep track of the leakage on the sides, as well as behind the box.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  void SetAbsorMaterial (const G4String&);
  void SetAbsorThickness(G4double);
  void SetWorldMaterial (const G4String&);
  void SetCalorSizeXY   (G4double);
  void SetNbOfLayers    (G4int);
  void PrintCalorParameters();
  inline const G4Material*        GetWorldMaterial()     {return fWorldMaterial;};
  inline const G4Material* GetAbsorMaterial() const {return fAbsorMaterial;};
  inline G4double GetAbsorThickness() const {return fAbsorThickness;};
  inline G4int GetNOfLayers() const {return fNofLayers;};
  inline G4double GetCalorSizeXY() const {return fCalorSizeXY;};

private:
  G4VPhysicalVolume* DefineVolumes();

  static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
                                      // magnetic field messenger
  G4LogicalVolume* fLayerLV;
  G4int fNofLayers;
  G4double fAbsorThickness;
  G4double fCalorSizeXY;
  G4Material*        fAbsorMaterial;
  G4Material*        fWorldMaterial;
  DetectorMessenger* fDetectorMessenger;
};

#endif
