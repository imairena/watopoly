module playerSquareProperty;

import <iostream>;
import <string>;

using namespace std;

// Constructor
Player::Player(const string &name, char token)
  : name{name}, token{token}, money{1500}, position{0}, timsCups{0},
    inTims{false}, timsTurns{0}, isBankrupt{false}, lastRoll{0},
    debt{0}, creditor{nullptr}
{}

// Movement
void Player::move(int steps) {
  if (inTims) return; // cannot move if in Tims

  position = (position + steps) % 40;

  if (position < 0) {
    position += 40; // handle negative movement
  }
}

int Player::getPosition() const {
  return position;
}

void Player::setPosition(const int pos) {
  position = pos;
}

// Money
void Player::pay(int amount) {
  money -= amount;
}

void Player::receive(int amount) {
  money += amount;
}

int Player::getMoney() const {
  return money;
}

void Player::setMoney(int amt) {
  money = amt;
}

int Player::getDebt() const {
  return debt;
}

void Player::setDebt(int amt) {
  debt = amt;
}

Player* Player::getCreditor() const {
  return creditor;
}

void Player::setCreditor(Player* p) {
  creditor = p;
}

// Properties
void Player::addProperty(Property* p) {
  properties.emplace_back(p);
}

void Player::removeProperty(Property* p) {
  for (auto it = properties.begin(); it != properties.end(); ++it) {
    if (*it == p) {
	    properties.erase(it);
	    return;
    }
  }
}

const std::vector<Property*>& Player::getProperties() const {
  return properties;
}

// Tims Line
void Player::sendToTims() {
  inTims = true;
  position = 10; // Tims location
  timsTurns = 0;
}

void Player::leaveTims() {
  inTims = false;
  timsTurns = 0;
}

void Player::setInTims(const bool inTims) {
  if (inTims) sendToTims();
  else leaveTims();
}

bool Player::getInTims() const {
  return inTims;
}

int Player::getTimsTurns() const {
  return timsTurns;
}

void Player::incrementTimsTurns() {
  ++timsTurns;
}

// Cups
void Player::addCup() {
  ++timsCups;
}

void Player::useCup() {
  if (timsCups > 0) {
    --timsCups;
    leaveTims();
  }
}

int Player::getCups() const {
  return timsCups;
}

// Bankruptcy
void Player::declareBankrupt() {
  isBankrupt = true;
}

bool Player::bankrupt() const {
  return isBankrupt;
}

// Info
string Player::getName() const {
  return name;
}

char Player::getToken() const {
  return token;
}

int Player::getLastRoll() const {
  return lastRoll;
}

void Player::setLastRoll(int roll) {
  lastRoll = roll;
}

// Assets
void Player::assets() const {
  cout << "Player: " << name << endl;
  cout << "Money: $" << money << endl;
  cout << "Position: " << position << endl;
  cout << "Tims Cups: " << timsCups << endl;
  cout << "Properties: " << endl;
  for (auto p : properties) {
    cout << " - " << p->getName();
    if (p->isMortgaged()) { cout << " (Mortgaged)"; }
    cout << endl << endl;
  }
}
