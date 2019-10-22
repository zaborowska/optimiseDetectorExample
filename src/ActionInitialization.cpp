#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "EventAction.h"
#include "RunAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::ActionInitialization(DetectorConstruction* aDetector) : G4VUserActionInitialization(), fDetector(aDetector) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
  auto event = new EventAction(fDetector->GetNOfLayers());
  SetUserAction(event);
  SetUserAction(new RunAction(event));
}
