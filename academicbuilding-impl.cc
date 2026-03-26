module academicbuilding;
import <string>;
import playerSquareProperty;
using namespace std;

AcademicBuilding
  (string name, int position, int cost,
   int improvementCost, string monopolyBlock, int blockSize,
   int tlvls[6]) : 
    Property(name, position, cost), improvementCost{improvementCost},
    blockSize{blockSize}, monopolyBlock{monopolyBlock}, 
    numImprovements{0}
{
  for (int i = 0; i < 6; i++) {
    tuitionLevels[i] = tlvls[i];
  } // loop
} // ctor

int AcademicBuilding::getFee(Player* p) {
  if (owner == nullptr) return 0;
  if (isMortgaged()) return 0;

  // check for monopoly
  if (numImprovements == 0 && hasMonopoly()) {
    return tuitionLevels[0] * 2; // double base tuition
  }
  // Otherwise return corresponding tuition level
  return tuitionLevels[numImprovements];
} // getFee

void addImprovement();
void removeImprovement();
bool hasMonopoly();

// Accessors
int getCost() const override;
int getNumImprovements() const;
int getImprovementCost() const;
int getblockSize() const;
string getMonopolyBlock() const;
