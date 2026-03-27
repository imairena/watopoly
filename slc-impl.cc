module slc;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;

using namespace std;



vector<unique_ptr<Card>>& SLC::getCards() {
  return cards;
}

SLC::SLC(int position): CardSquare{string("SLC"), position} {}

void SLC::addCard(unique_ptr<Card> card) {
  cards.emplace_back(move(card));
}
