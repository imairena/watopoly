export module square;
import <string>;
export class Player; // to be changed with import player once it's done

export class Square {
  std::string name;
  int position;

  public:
  Square(std::string name, int position);
  virtual void landOn(Player* p) = 0;
  virtual ~Square() = default;
  std::string getName() const;
  int getPosition() const;
};
