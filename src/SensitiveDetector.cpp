#include "SensitiveDetector.h"
#include "G4HCofThisEvent.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName, G4int nofCells)
 : G4VSensitiveDetector(name), fHitsCollection(nullptr), fNofCells(nofCells) {
  collectionName.insert(hitsCollectionName);
  G4cout<<"1NUM LAYERS:    " << fNofCells << G4endl;
}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  fHitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

void SensitiveDetector::SetNbOfLayers(G4int aNumLayers) {
  fNofCells = aNumLayers;
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( edep==0. && stepLength == 0. ) return false;

  auto touchable = (step->GetPreStepPoint()->GetTouchable());

  // Get calorimeter cell id
  auto layerNumber = touchable->GetReplicaNumber(0);
  if ( layerNumber >= fHitsCollection->entries() ) {
    for (int i=fHitsCollection->entries(); i<layerNumber+1; i++)
      fHitsCollection->insert(new ::Hit());
  }
  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber;
    G4Exception("SensitiveDetector::ProcessHits()", "MyCode0004", FatalException, msg);
  }

  // Add values
  hit->Add(edep, stepLength);

  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
  if ( verboseLevel > 1 ) {
     auto nofHits = fHitsCollection->entries();
     // G4cout
     //   << G4endl
     //   << "-------->Hits Collection: in this event they are " << nofHits
     //   << " hits in the detector: " << G4endl;
     G4double sumE = 0;
     //for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
     for ( G4int i=0; i<nofHits; i++ ) sumE += (*fHitsCollection)[i]->GetEdep();
     G4cout << "Deposited " << G4BestUnit(sumE,"Energy") << G4endl;
  }
}
