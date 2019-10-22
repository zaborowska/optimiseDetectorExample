#include "Hit.h"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<Hit>* HitAllocator = 0;

Hit::Hit() : G4VHit(), fEdep(0.), fTrackLength(0.) {}

Hit::~Hit() {}

Hit::Hit(const Hit& right) : G4VHit() {
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
}

const Hit& Hit::operator=(const Hit& right) {
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  return *this;
}

G4bool Hit::operator==(const Hit& right) const {
  return ( this == &right ) ? true : false;
}

void Hit::Print() {
  G4cout
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     // << " track length: "
     // << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

void Hit::Draw() {}
