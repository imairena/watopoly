module player;

import <iostream>;
import property; // assuming that we will create a module for Property
using namespace std;

// Constructor
Player::Player(const string &name, char token)
    : name{name}, token{token}, money{1500}, position{0}, timsCups{0},
      inTims{false}, timsTurns{0}, isBankrupt{false} {}

// Movement
void Player::move(int steps) {
    if (inTims) return; // cannot move if in Tims

    position = (position + steps) % 40;

    if (position < 0) {
        position +=40; // handle negative movement
    }
}

int PLayer::getPosition() const {
    return position;
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

// Properties
void Player::addProperty(Property* p) {
    properties.emplace_back(p);
}

void Player::removeProperty(Property* p) {
    for (auto it = properties.begin; it != properties.end(); ++it) {
        if (*it == p) {
	    properties.erase(it);
	    return;
	}
    }
}

const vector<Property*>& Player::getProperties() const {
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
    return; timsCups;
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

// Assets
void Player::assets() const {
    cout << "Player: " << name << endl;
    cout << "Money: $" << money << endl;
    cout << "Position: " << position << endl;
    cout << "Tims Cups: " << timsCups << endl;
    cout << "Properties:" << name << endl;
    for (auto p : properties) {
        cout << " - Property at " << p << endl;
    }
}
