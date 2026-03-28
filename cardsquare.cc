export module cardsquare;

import <vector>;
import <string>;
import <memory>;

import playerSquareProperty;
import card;



export class CardSquare: public Square {
  int currentCard = 0;
protected:
  virtual std::vector<std::unique_ptr<Card>>& getCards() = 0;
public:
  CardSquare(std::string name, int position);
  void landOn(Player* p);
  void shuffleCards();
};
