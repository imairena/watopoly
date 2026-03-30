export module needleshall;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;



export class NeedlesHall: public CardSquare {
  static std::vector<std::unique_ptr<Card>> cards;
  static int currentCard;
  std::vector<std::unique_ptr<Card>>& getCards() override;
  int& getCurrentCard() override;
public:
  NeedlesHall(int position);
  void addCard(std::unique_ptr<Card> card);
};
