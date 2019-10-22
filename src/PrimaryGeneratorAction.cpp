#include "PrimaryGeneratorAction.h"
#include "G4ParticleGun.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr) {
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleEnergy(100*MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
