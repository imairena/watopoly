module needleshall;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;

using namespace std;



vector<unique_ptr<Card>> NeedlesHall::cards;

vector<unique_ptr<Card>>& NeedlesHall::getCards() {
  return cards;
}

NeedlesHall::NeedlesHall(int position): CardSquare{string("NeedlesHall"), position} {}

void NeedlesHall::addCard(unique_ptr<Card> card) {
  cards.emplace_back(move(card));
}
