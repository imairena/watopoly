module non_properties;
import <string>;
import <iostream>;
import playerSquareProperty;
using namespace std;

CoopFee::CoopFee(string name, int position) : Square(name, position) {}
void CoopFee::landOn(Player* p) {
  int amountOwed = 150;
  if (p->getMoney() >= amountOwed) { // check if enough money to pay
    p->pay(150);
    cout << p->getName() << " Paid $150." << endl;
  }
  else { // not enough to pay
    cout << "The Coop Fee costs $150, but you only have $" << p->getMoney()
	 << ". Use 'bankrupt' command to try to pay" << endl;
    p->setDebt(amountOwed);
    p->setCreditor(nullptr);
  }
} // CoopFee::landOn

Tuition::Tuition(string name, int position) : Square(name, position) {}
void Tuition::landOn(Player* p) {

  string input;
  if (p->getMoney() < 300) {
    cout << "You do not have $300, so you must pay 10% of your total worth" << endl;
    input = "2";
  }
  
  cout << "Choose payment method: (1) $300 (2) 10 percent of total worth" << endl;
  
  bool validChoice = false;

  while (input == "2" || (validChoice == false && cin >> input)) {
    if (input == "assets" || input == "all") {
      cout << "You cannot view assets at this moment." << endl;
    }
    else if (input == "1") {
      validChoice = true;
      p->pay(300);
      cout << p->getName() << " paid $300." << endl;
      return;
    } 
    else if (input == "2") {
      validChoice = true;
      // Calculate total worth of player
      int totalWorth = p->getMoney(); 
      // Calculate total worth of properties owned
      for (Property* prop : p->getProperties()) {
        totalWorth += prop->getCost();
      }

      int owingAmount = static_cast<int>(totalWorth / 10);
      p->pay(owingAmount);
      cout << p->getName() << " paid $" << owingAmount;
      cout << ". 10 percent of $" << totalWorth << "." << endl;
      return;
    }
    else {
      cout << "Invalid choice. Please enter 1 or 2" << endl; 
    }
  } // while
} // Tuition::landOn

CollectOSAP::CollectOSAP(string name, int position) : Square(name, position) {}
void CollectOSAP::landOn(Player* p) {
  p->receive(200);
  cout << p->getName() << " gets $200!" << endl;
} // CollectOSAP::landOn

DCTimsLine::DCTimsLine(string name, int position) : Square(name, position) {}
void DCTimsLine::landOn(Player* p) {
  if (!p->getInTims()) { cout << p->getName() << " is visiting the DC Tims Line!" << endl; }
} // DCTimsLine::landOn

GoToTims::GoToTims(string name, int position) : Square(name, position) {}
void GoToTims::landOn(Player* p) {
  p->setPosition(10); // Tims in the 10th square on the board
  p->setInTims(true);
  p->incrementTimsTurns();
  cout << p->getName() << " was sent to Tims!!!" << endl;
} // GoToTims::landOn

GooseNesting::GooseNesting(string name, int position) : Square(name, position) {}
void GooseNesting::landOn(Player* p) {
  cout << p->getName() << " is safe at Goose Nesting. Honk!" << endl;
} // GooseNesting::landOn
