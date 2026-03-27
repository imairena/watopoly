module academicbuilding;
import <string>;
import playerSquareProperty;
using namespace std;

AcademicBuilding::AcademicBuilding
  (string name, int position, int cost,
   int improvementCost, string monopolyBlock,
   int tlvls[6]) : 
    Property(name, position, cost), numImprovements{0}, improvementCost{improvementCost},
    monopolyBlock{monopolyBlock}
{
  for (int i = 0; i < 6; i++) {
    tuitionLevels[i] = tlvls[i];
  } // loop
} // ctor

// Accessors
int AcademicBuilding::getCost() const {
  return Property::getCost() + (numImprovements * improvementCost);
}
int AcademicBuilding::getNumImprovements() const { return numImprovements; }
int AcademicBuilding::getImprovementCost() const { return improvementCost; }
int AcademicBuilding::getBlockSize() const { return blockSize; }
string AcademicBuilding::getMonopolyBlock() const { return monopolyBlock; }

// Mutator
void AcademicBuilding::setBlockSize(const int size) {
  blockSize = size;
}

// Other methods
int AcademicBuilding::getFee(Player* p) {
  if (getOwner() == nullptr) return 0;
  if (isMortgaged()) return 0;

  // check for monopoly
  if (numImprovements == 0 && hasMonopoly()) {
    return tuitionLevels[0] * 2; // double base tuition
  }
  // Otherwise return corresponding tuition level
  return tuitionLevels[numImprovements];
} // getFee

void AcademicBuilding::addImprovement() { 
  if (numImprovements < 5) numImprovements++;
}
void AcademicBuilding::removeImprovement() {
  if (numImprovements > 0) numImprovements--;
}
bool AcademicBuilding::hasMonopoly() {
  if (getOwner() == nullptr) return false;
  int numSameBlock = 0;
  // Loop through properties
  for (Property* prop : getOwner()->getProperties()) {
    AcademicBuilding* ab = dynamic_cast<AcademicBuilding*>(prop); // check if prop is ab
    if (ab != nullptr && ab->getMonopolyBlock() == monopolyBlock) { // check if same block
      numSameBlock++;
    } // if
  } // loop
  return numSameBlock == blockSize;
} // hasMonopoly
