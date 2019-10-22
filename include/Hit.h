#ifndef HIT_H
#define HIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


/// Hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class Hit : public G4VHit {
 public:
    Hit();
    Hit(const Hit&);
    virtual ~Hit();

    // operators
    const Hit& operator=(const Hit&);
    G4bool operator==(const Hit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // methods to handle data
    inline void Add(G4double de, G4double dl);

    // get methods
    inline G4double GetEdep() const {return fEdep;};
    inline G4double GetTrackLength() const {return fTrackLength;};

private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
};

using HitsCollection = G4THitsCollection<Hit>;

extern G4ThreadLocal G4Allocator<Hit>* HitAllocator;

inline void* Hit::operator new(size_t) {
  if (!HitAllocator) {
    HitAllocator = new G4Allocator<Hit>;
  }
  void *hit;
  hit = (void *) HitAllocator->MallocSingle();
  return hit;
}

inline void Hit::operator delete(void *hit) {
  if (!HitAllocator) {
    HitAllocator = new G4Allocator<Hit>;
  }
  HitAllocator->FreeSingle((Hit*) hit);
}

inline void Hit::Add(G4double de, G4double dl) {
  fEdep += de;
  fTrackLength += dl;
}

#endif
