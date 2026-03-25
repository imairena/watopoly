module property;
import <iostream>;
import player;
using namespace std;

Property::Property(string name, int position, int cost) :
  Square(name, position), owner{nullptr}, cost{cost}, mortgaged{false} {}

// Accessors
Player* Property::getOwner() const { return owner; }
int Property::getCost() const { return cost; }
bool Property::isMortgaged() const { return mortgaged; }

void Property::setOwner(Player* p) { owner = p; }

void Property::buyProperty(Player* p) {
  p->pay(cost);
  setOwner(p);
  p->addProperty(this);
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
  int amount = cost / 2; // based on https://monopoly.fandom.com/wiki/Mortgage
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
  int amount = (cost / 2) * 1.10; // based on https://monopoly.fandom.com/wiki/Mortgage
  owner->pay(amount);
  cout << getName() << " was successfully unmortgaged." << endl;

} // unmortgage

void Property::landOn(player* p) {} // coming soon :)
