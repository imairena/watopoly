export module residence;
import <string>;
import square;
import property;

export class Residence : public Property {
  public:
  Residence(std::string name, int position, int cost);
  int getFee(Player* p) override;
};
