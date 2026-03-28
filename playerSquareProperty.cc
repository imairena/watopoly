export module playerSquareProperty;

import <string>;
import <vector>;



// forward declaration
export class Player;



// Square defined with Player
export class Square {
  std::string name;
  int position;

public:
  Square(std::string name, int position);
  virtual void landOn(Player* p) = 0;
  virtual ~Square() = default;
  std::string getName() const;
  int getPosition() const;
};



// Property inherited from Square
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
  virtual int getCost() const;
  bool isMortgaged() const;
};



// Player defined with Property
export class Player {
  std::string name;
  char token;
  int money;
  int position;
  int timsCups;
  bool inTims;
  int timsTurns;
  std::vector<Property*> properties;
  bool isBankrupt;
  int lastRoll;
  int debt;
  Player* creditor;

public:
  // Constructor
  Player(const std::string &name, char token);

  // Movement
  void move(int steps);
  int getPosition() const;
  void setPosition(const int pos);

  // Money
  void pay(int amount);
  void receive(int amount);
  int getMoney() const;
  void setMoney(int amt);
  int getDebt() const;
  void setDebt(int amt);
  Player* getCreditor() const;
  void setCreditor(Player* p);

  // Properties
  void addProperty(Property* p);
  void removeProperty(Property* p);
  const std::vector<Property*>& getProperties() const;

  // Tims Line
  void sendToTims();
  void leaveTims();
  bool getInTims() const;
  void setInTims(const bool inTims);
  int getTimsTurns() const;
  void incrementTimsTurns();

  // Cups
  void addCup();
  void useCup();
  int getCups() const;

  // Bankruptcy
  void declareBankrupt();
  bool bankrupt() const;

  // Info
  std::string getName() const;
  char getToken() const;
  int getLastRoll() const;
  void setLastRoll(int roll);

  // Assets display
  void assets() const;
};
