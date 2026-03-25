module square;
import <string>;
using namespace std;

Square::Square(string name, int position) :
  name{name}, position{position} {}
 
string Square::getName() const { return name; }
int Square::getPosition() const { return position; }
