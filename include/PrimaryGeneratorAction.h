#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  inline const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
  G4ParticleGun* fParticleGun;
};

#endif
