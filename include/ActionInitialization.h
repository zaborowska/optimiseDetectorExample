#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.h"

/// Action initialization class.
/// Creates Primary particles and analysis (histograms)

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization();
  ActionInitialization(DetectorConstruction* aDetector);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  DetectorConstruction* fDetector;
};

#endif
