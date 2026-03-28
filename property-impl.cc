module playerSquareProperty;

import <iostream>;
import <string>;

using namespace std;

Property::Property(string name, int position, int cost) :
  Square(name, position), owner{nullptr}, cost{cost}, mortgaged{false} {}

// Accessors
Player* Property::getOwner() const { return owner; }
int Property::getCost() const { return cost; }
bool Property::isMortgaged() const { return mortgaged; }

void Property::setOwner(Player* p) { 
  if (owner != nullptr) { // remove property from old owner
    owner->removeProperty(this);
  }
  // change owner
  owner = p;

  if (owner != nullptr) { // add property to new owner
    owner->addProperty(this);
  }
} // setOwner

void Property::buyProperty(Player* p) {
  p->pay(cost);
  setOwner(p);
  cout << p->getName() << " bought " << getName() << "!" << endl;
} // buyProperty

void Property::mortgage() {
  if (owner == nullptr) {
    cout << "Cannot mortgage unowned properties." << endl;
    return;
  }

  if (mortgaged) {
    cout << "Property is already mortgaged." << endl;
    return;
  }

  // No errors
  mortgaged = true;
  int amount = cost / 2; 
  owner->receive(amount);
  cout << getName() << " was successfully mortgaged." << endl;

} // mortgage

void Property::unmortgage() {
  if (owner == nullptr) {
    cout << "Cannot unmortgage unowned properties." << endl;
    return;
  }

  if (!mortgaged) {
    cout << "Property is not mortgaged!" << endl;
    return;
  }

  // No errors
  mortgaged = false;
  int amount = (cost / 2) * 1.10; 
  owner->pay(amount);
  cout << getName() << " was successfully unmortgaged." << endl;

} // unmortgage

void Property::landOn(Player* p) {
  // if property is unowned, give choice to buy
  if (owner == nullptr) {
    cout << getName() << " is unowned.";
    cout << " Would you like to buy it for $" << cost << "?";
    cout << " (y/n)" << endl;
    
    char choice;
    bool validChoice = false;
    while (validChoice == false) {
      cin >> choice;
      if (!cin) {
        cin.clear();
        cin.ignore();
      }

      if (choice == 'y' || choice == 'Y') {
        validChoice = true;
        buyProperty(p);
        return;

      } else if (choice == 'n' || choice == 'N') {
        validChoice = true;
        return;
      }

      cout << "Invalid input. Please enter 'y' to buy or 'n' to not buy" << endl;
    } // while
  } // if
  else if (owner == p) {
    cout << "You already own " << getName() << "." << endl;
  }
  else if (mortgaged) {
    cout << getName() << " is mortgaged by " << owner->getName() << ".";
    cout << " No tuition is owed." << endl;
  }
  else { // Need to pay owner
    int amountOwed = getFee(p);
    if (p->getMoney() >= amountOwed) { // check if enough money to pay
      p->pay(amountOwed);
      owner->receive(amountOwed);
      cout << owner->getName() << " owns this property. You paid $" << amountOwed;
      cout << "." << endl;
    }
    else { // not enough to pay
      cout << owner->getName() << " owns this property. You owe $" << amountOwed
        << ", but only have $" << p->getMoney() << ". Use 'bankrupt' command to try to pay." << endl; 
      p->setDebt(amountOwed);
      p->setCreditor(owner);
    }
  }
} // landOn
