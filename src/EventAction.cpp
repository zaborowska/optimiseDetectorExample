#include "EventAction.h"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include "Hit.h"
#include "Analysis.hh"
#include "SensitiveDetector.h"


EventAction::EventAction()
: G4UserEventAction(),
  fHID(-1),
  fCellNo(10),
  fCalEdep{ std::vector<G4double>(10, 0.)}
{
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  fCellNo = dynamic_cast<SensitiveDetector*>(sdManager->FindSensitiveDetector("Absorber"))->GetNbOfLayers();
}

EventAction::EventAction(G4int aCellNo)
  : G4UserEventAction(),
    fHID(-1),
    fCellNo(aCellNo),
    fCalEdep{ std::vector<G4double>(aCellNo, 0.)}
{
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

void EventAction::BeginOfEventAction(const G4Event* event) {
  // Get hits collections
  if (fHID==-1) {
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce)
      {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl;
        G4Exception("EventAction::EndOfEventAction()",
                    "B5Code001", JustWarning, msg);
        return;
      }
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fHID = sdManager->GetCollectionID("AbsorberHitsCollection");
  }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  HitsCollection* hcHC = static_cast<HitsCollection*>(hce->GetHC(fHID));

  if ( (!hcHC) )
    {
      G4ExceptionDescription msg;
      msg << "Some of hits collections of this event not found." << G4endl;
      G4Exception("EventAction::EndOfEventAction()",
                  "B5Code001", JustWarning, msg);
      return;
    }
    double primary_energy;
    int hitId;
    primary_energy = G4EventManager::GetEventManager()->GetConstCurrentEvent()
      ->GetPrimaryVertex()->GetPrimary(0)->GetTotalEnergy();

    //G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fCellNo = G4RunManager::GetRunManager()->GetCurrentEvent()->GetHCofThisEvent()->GetHC(0)->GetSize(); //dynamic_cast<SensitiveDetector*>(sdManager->FindSensitiveDetector("Absorber"))->GetNbOfLayers();
    fCalEdep.resize(fCellNo);
    double sum_deposited = 0;
    for (G4int ix=0;ix<fCellNo;ix++) {
            hitId = ix;
            Hit* hit = (*hcHC)[hitId];
            G4double eDep = hit->GetEdep();
            fCalEdep[ix] = eDep;
            sum_deposited += eDep;
    }
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, primary_energy);
    man->FillNtupleDColumn(1, sum_deposited);
    man->AddNtupleRow();
}
