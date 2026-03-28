module cardsquare;

import <iostream>;
import <vector>;
import <string>;
import <algorithm>;
import <memory>;

import playerSquareProperty;
import card;
import tools;

using namespace std;



CardSquare::CardSquare(string name, int position):
  Square{name, position} {}

void CardSquare::landOn(Player* p) {
  vector<unique_ptr<Card>>& cards = getCards();
  cards[currentCard]->apply(p);
  currentCard = (currentCard + 1) % cards.size();
  if (currentCard == 0) {
    cout << "\nEnd of " << getName() << " deck." << endl;
    cout << "Now reshuffling cards." << endl << endl;
    shuffleCards();
  }
}

void CardSquare::shuffleCards() {
  vector<unique_ptr<Card>>& cards = getCards();
  shuffleVector(cards);
}
