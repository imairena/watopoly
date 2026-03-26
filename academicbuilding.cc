export module academicbuilding;
import <string>;
import playerSquareProperty;

export class AcademicBuilding : public Property {
  int numImprovements;
  int improvementCost;
  int blockSize;
  std::string monopolyBlock;
  int tuitionLevels[6];
public:
  AcademicBuilding
    (std::string name, int position, int cost,
     int improvementCost, std::string monopolyBlock,
     int tlvls[6]);
  int getFee(Player* p) override;
  void addImprovement();
  void removeImprovement();
  bool hasMonopoly();

  // Accessors
  int getCost() const override;
  int getNumImprovements() const;
  int getImprovementCost() const;
  int getblockSize() const;
  std::string getMonopolyBlock() const;
};
