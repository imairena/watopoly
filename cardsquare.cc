export module cardsquare;

import <vector>;
import <string>;
import <memory>;

import playerSquareProperty;
import card;



export class CardSquare: public Square {
protected:
  virtual std::vector<std::unique_ptr<Card>>& getCards() = 0;
  virtual int& getCurrentCard() = 0;
public:
  CardSquare(std::string name, int position);
  void landOn(Player* p);
  void shuffleCards();
  int getNumRemainingCards();
};
