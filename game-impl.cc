module game;

import <iostream>;
import <vector>;
import <random>;
import <string>;

import playerSquareProperty;
import board;

using namespace std;


Game::Game(istream& boardIn, istream& squaresIn, istream& cardsIn):
  board{boardIn, squaresIn, cardsIn} {}

void Game::all() const {
  // loop thorough players using reference to avoid 
  // creating a copy of all players in each function call
  for (auto& p : players) {
    p.assets();
  }
}

void Game::nextPlayer(ostream& out) {
  out << "Player " << players[currentPlayer].getName();
  out << " has finished their turn." << endl;
  currentPlayer = (currentPlayer +1) % players.size();
  out << "It is now player " << players[currentPlayer].getName();
  out << "'s turn." << endl << endl;
}

void Game::loadGame(istream& gameIn) {
  // to do
}

void Game::saveGame(ostream& gameOut) const {
  // to do
}

void Game::addPlayer(const string name, const char token) {
  players.emplace_back(name, token);
}

void Game::playTurn() {
  Player& currPlayer = players[currentPlayer];
  string command;
  bool hasRolled;

  cout << currPlayer.getName() << ", enter command:" << endl;
  while (cin >> command) {
    if (command == "roll") {
      // Searched up "Generating random number in cpp"
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distrib(1,6);
      
      // generate random numbers for dice
      int die1 = distrib(gen);
      int die2 = distrib(gen);
      int rollSum = die1 + die2;
      cout << currPlayer.getName() << " rolled a " << rollSum << "!" << endl;
      
      // update player
      currPlayer.setLastRoll(rollSum);
      int oldPos = currPlayer.getPosition();
      int newPos = (oldPos + rollSum) % 40;
      currPlayer.setPosition(newPos);
      
      // check if passed collectOSAP (check >40 since =40 is handled by 
      // landOn method in CollectOSAP class)
      if (oldPos + rollSum > 40 && newPos > 0) {
        cout << currPlayer.getName() << " collected $200 from OSAP!" << endl;
        currPlayer.receive(200);
      }

      // apply square action
      board.getSquare(newPos)->landOn(&currPlayer);

      // check if player rolled doubles
      if (die1 == die2) {
        cout << " You rolled doubles! You get to roll again." << endl;
      } 
      else {
        hasRolled = true;
      }
    } // if roll
    else if (command == "next") {
      if (!hasRolled) {
        cout << "You must roll before moving to the next player!" << endl;
      } else {
        nextPlayer(cout);
        return;
      }
    } // if next
    else if (command == "assets") {
      currPlayer.assets();
    } 
    else if (command == "all") {
      all();
    } 
    else if (command == "save") {
      // to do
    } 
    else if (command == "trade") {
      // to do
    } 
    else if (command == "mortgage") {
      // to do
    } 
    else if (command == "unmortgage") {
      // to do
    } 
    else if (command == "improve") {
      // to do
    }
    else {
      cout << "Invalid command." << endl;
    }
  } // while
} // playTurn

