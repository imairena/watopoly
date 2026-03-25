export module non-properties;
import square;
import <string>;
// import player;
export class Player;

export class CoopFee : public Square {
  CoopFee(std::string name, int position);
  void landOn(Player* p) override;
};
export class Tuition : public Square {
  Tuition(std::string name, int position);
  void landOn(Player* p) override;
};
export class CollectOSAP : public Square {
  CollectOSAP(std::string name, int position);
  void landOn(Player* p) override;
};
export class DCTimsLine : public Square {
  DCTimsLine(std::string name, int position);
  void landOn(Player* p) override;
};
export class GoToTims : public Square {
  GoToTims(std::string name, int position);
  void landOn(Player* p) override;
};
export class GooseNesting : public Square {
  GooseNesting(std::string name, int position);
  void landOn(Player* p) override;
};
