module needleshall;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;

using namespace std;



vector<unique_ptr<Card>> NeedlesHall::cards;
int NeedlesHall::currentCard = 0;

vector<unique_ptr<Card>>& NeedlesHall::getCards() {
  return cards;
}

int& NeedlesHall::getCurrentCard() {
  return currentCard;
}

NeedlesHall::NeedlesHall(int position): CardSquare{string("NEEDLES HALL"), position} {}

void NeedlesHall::addCard(unique_ptr<Card> card) {
  cards.emplace_back(move(card));
}
