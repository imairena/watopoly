module slc;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;

using namespace std;



vector<unique_ptr<Card>> SLC::cards;
int SLC::currentCard = 0;

vector<unique_ptr<Card>>& SLC::getCards() {
  return cards;
}

int& SLC::getCurrentCard() {
  return currentCard;
}

SLC::SLC(int position): CardSquare{string("SLC"), position} {}

void SLC::addCard(unique_ptr<Card> card) {
  cards.emplace_back(move(card));
}
