module card_types;

import <iostream>;

import card;
import playerSquareProperty;

using namespace std;



void MoveCard::apply(Player* p) {
  cout << text << endl << endl;
  p->move(val);
}
