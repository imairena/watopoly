export module playerAndSquare;

import <string>;
import <vector>;
//import property;

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

 public:
    // Constructor
    Player(const std::string &name, char token);

    // Movement
    void move(int steps);
    int getPosition() const;

    // Money
    void pay(int amount);
    void receive(int amount);
    int getMoney() const;

    // Properties
    void addProperty(Property* p);
    void removeProperty(Property* p);
    const std::vector<Property*>& getProperties() const;

    // Tims Line
    void sendToTims();
    void leaveTims();
    bool getInTims() const;
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

    // Assets display
    void assets() const;
};
