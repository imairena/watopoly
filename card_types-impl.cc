module card_types;

import <iostream>;
import <string>;

import card;
import playerSquareProperty;

using namespace std;



// card ctors

MoveCard::MoveCard(string text, int val): Card{text, val} {}

MoneyCard::MoneyCard(string text, int val): Card{text, val} {}

MoveToCard::MoveToCard(string text, int val): Card{text, val} {}

GoToTimsCard::GoToTimsCard(string text, int val): Card{text, val} {}


// card applying

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
