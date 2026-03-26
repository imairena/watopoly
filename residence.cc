export module residence;
import <string>;
import playerSquareProperty;

export class Residence : public Property {
  public:
  Residence(std::string name, int position, int cost);
  int getFee(Player* p) override;
};
