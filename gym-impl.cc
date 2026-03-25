module gym;
using namespace std;

Gym::Gym(string name, int position, int cost) : Property(name, position, cost) {}

// getFee calculates amount owed by multiplying the dice roll that moved the player
// onto the gym square as explained in: https://monopoly.fandom.com/wiki/Utility
int Gym::getFee(Player* p) {
  Player* propOwner = getOwner();
  // check owner is not null for extra safety since landOn already does so
  if (propOwner == nullptr) { return 0; }

  int diceRoll = p->getLastRoll();
  int gymsOwned = 0;

  // Loop through player's properties to count gyms
  for (Property* prop : propOwner->getProperties()) {
    // Check if prop is a gym
    if (dynamic_cast<Gym*>(prop) != nullptr) { gymsOwned++; }
  } // loop
  
  // Calculate fee based on number of gyms owned
  if (gymsOwned == 1) {
    return diceRoll * 4;
  } else if (gymsOwned == 2) {
    return diceRoll * 10;
  } else { // no gyms owned
    return 0;
  }
} // getFee
