export module gym;
import <string>;
import playerSquareProperty;

export class Gym : public Property {
  static const int defaultCost = 150;
  public:
  Gym(std::string name, int position, int cost = defaultCost);
  int getFee(Player* p) override;
};
