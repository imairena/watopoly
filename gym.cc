export module gym;
import <string>;
import property;

export class Gym : public Property {
  public:
  Gym(std::string name, int position, int cost);
  int getFee(Player* p) override;
};
