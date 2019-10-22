#ifndef SENSITIVEDETECTOR_H
#define SENSITIVEDETECTOR_H

#include "G4VSensitiveDetector.hh"
#include "Hit.h"
#include <vector>

class G4Step;
class G4HCofThisEvent;

/// Sensitive detector class
///
/// In Initialize(), it creates one hit for each cell.
///

class SensitiveDetector : public G4VSensitiveDetector {
public:
  SensitiveDetector(const G4String& name, const G4String& hitsCollectionName, G4int nofCells);
  virtual ~SensitiveDetector();

  // methods from base class
  virtual void Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
  void SetNbOfLayers(G4int numLayers);
  inline G4int GetNbOfLayers() const {return fNofCells;};

private:
  HitsCollection* fHitsCollection;
  G4int  fNofCells;
};

#endif
