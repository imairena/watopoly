module card_types;

import <iostream>;

import card;
import playerSquareProperty;

using namespace std;



void MoveCard::apply(Player* p) {
  cout << text << endl << endl;
  p->move(val);
}

void MoneyCard::apply(Player* p) {
  cout << text << endl << endl;
  if (val >= 0) p->receive(val);
  else p->pay(0 - val);
}

void MoveToCard::apply(Player* p) {
  cout << text << endl << endl;
  p->setPosition(val);
}

void GoToTimsCard::apply(Player* p) {
  cout << text << endl << endl;
  p->sendToTims();
}
