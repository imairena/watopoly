export module property;
import <string>;
import square;

export class Property : public Square {
  Player* owner;
  int cost;
  bool mortgaged;

  public:
  Property(std::string name, int position, int cost);
  virtual int getFee(Player* p) = 0;
  virtual ~Property() = default;

  void buyProperty(Player* p);
  void mortgage();
  void unmortgage();
  void setOwner(Player* p);
  void landOn(Player* p) override;

  Player* getOwner() const;
  int getCost() const;
  bool isMortgaged() const;
};
