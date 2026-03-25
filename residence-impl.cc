module residence;
using namespace std;

Residence::Residence(string name, int position, int cost)
  : Property(name, position, cost) {}
int Residence::getFee(Player* p) {
  Player* propOwner = getOwner();
  // landOn already checks if unowned, check again for more safety
  if (propOwner == nullptr) { return 0; }

  int residencesOwned = 0;
  // Loop through player's properties to count residences
  for (Property* prop : propOwner->getProperties()) {
    // Check if prop is a Residence
    if (dynamic_cast<Residence*>(prop) != nullptr) { residencesOwned++; }
  } // loop
  
  // Calculate fee based on number of residences owned
  switch (residencesOwned) {
    case 1:
      return 25;
    case 2:
      return 50;
    case 3:
      return 100;
    case 4:
      return 200;
    default:
      return 0;
  } // switch
} // getFee
