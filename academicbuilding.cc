export module academicbuilding;
import <string>;
import playerSquareProperty;

export class AcademicBuilding : public Property {
  int numImprovements;
  int improvementCost;
  // blocksize will be changed after initialization
  int blockSize = 3;
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
  int getBlockSize() const;
  std::string getMonopolyBlock() const;

  // Mutators
  void setBlockSize(const int size);
  void setNumImprovements(const int num);
};
