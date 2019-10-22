#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  EventAction(G4int);
  virtual ~EventAction() {};

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
  std::vector<G4double>& GetCalEdep() { return fCalEdep; }

private:
  G4int fHID;
  G4int fCellNo;
  std::vector<G4double> fCalEdep;
};

#endif
