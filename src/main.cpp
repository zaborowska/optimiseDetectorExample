#include "DetectorConstruction.h"
#include "ActionInitialization.h"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "G4HadronicProcessStore.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc,char** argv)
{
  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

// Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  auto physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);
  G4HadronicProcessStore::Instance()->SetVerbose(0);

  auto actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);

  // Initialize visualization
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( argc > 1 ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+argv[1]);
  }
  else  {
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
