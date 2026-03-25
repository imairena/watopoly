module gym;
import player;
using namespace std;

Gym::Gym(string name, int position, int cost) : Property(name, position, cost) {}
int Gym::getFee(Player* p) {
 return 0;
}
