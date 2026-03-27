module cardsquare;

import <iostream>;
import <vector>;
import <string>;
import <algorithm>;
import <memory>;

import playerSquareProperty;
import card;

using namespace std;



CardSquare::CardSquare(string name, int position):
  Square{name, position} {}

void CardSquare::landOn(Player* p) {
  vector<unique_ptr<Card>>& cards = getCards();
  currentCard = (currentCard + 1) % cards.size();
  if (currentCard == 0) {
    cout << "End of " << getName() << " deck." << endl;
    cout << "Now reshuffling deck." << endl << endl;
    // ============================================================= add shuffle here!!!
  }
  cards[currentCard]->apply(p);
}
