export module card_types;

import card;
import playerSquareProperty;



export class MoveCard: public Card {
public:
  void apply(Player* p) override;
};
