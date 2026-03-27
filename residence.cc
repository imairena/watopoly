export module residence;
import <string>;
import playerSquareProperty;

export class Residence : public Property {
  static const int defaultCost = 200;
  public:
  Residence(std::string name, int position, int cost = defaultCost);
  int getFee(Player* p) override;
};
