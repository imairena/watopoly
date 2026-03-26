module non_properties;
import <string>;
import <iostream>;
import playerSquareProperty;
using namespace std;

CoopFee::CoopFee(string name, int position) : Square(name, position) {}
void CoopFee::landOn(Player* p) {
  p->pay(150);
  cout << p->getName() << " Paid $150." << endl;
} // CoopFee::landOn

Tuition::Tuition(string name, int position) : Square(name, position) {}
void Tuition::landOn(Player* p) {
  cout << "Choose payment method: (1) $300 (2) 10 percent of total worth" << endl;
  
  int choice;
  bool validChoice = false;

  while (validChoice == false) {
    cin >> choice;
    if (!cin) {
      cin.clear();
      cin.ignore();
    } // if

    if (choice == 1) {
      validChoice = true;
      p->pay(300);
      cout << p->getName() << " paid $300." << endl;
      return;

    } else if (choice == 2) {
      validChoice = true;
      int totalWorth = p->getMoney();
      int owingAmount = totalWorth / 10;
      p->pay(owingAmount);
      cout << p->getName() << " Paid $" << owingAmount;
      cout << ". 10 percent of $" << totalWorth << "." << endl;
      return;
    } // if
    
    cout << "Invalid choice. Don't be greedy!" << endl; 
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
  cout << p->getName() << " was sent to Tims!!!" << endl;
} // GoToTims::landOn

GooseNesting::GooseNesting(string name, int position) : Square(name, position) {}
void GooseNesting::landOn(Player* p) {
  cout << p->getName() << " is safe at Goose Nesting. Honk!" << endl;
} // GooseNesting::landOn
