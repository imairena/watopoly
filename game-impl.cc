module game;

import <iostream>;
import <fstream>;
import <vector>;
import <string>;

import playerSquareProperty;
import board;
import academicbuilding;
import tools;

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
  // get number of players
  gameOut << players.size() << endl;

  // write each player's state
  for (const auto& p : players) {
    gameOut 
      << p.getName() << " "
      << p.getToken() << " "
      << p.getCups() << " "
      << p.getMoney() << " "
      << p.getPosition(); // no endl since need to check if player is in Tims

    if (p.getPosition() == 10) {
      gameOut << " " << (p.getInTims() ? 1 : 0) << " " << p.getTimsTurns();
    }

    gameOut << endl;
  } // loop
  
  // save the state of the board
  board.saveBoard(gameOut);

} // saveGame

void Game::addPlayer(const string name, const char token) {
  players.emplace_back(name, token);
}

void Game::playTurn() {
  Player& currPlayer = players[currentPlayer];
  string command;
  bool hasRolled = false;

  cout << currPlayer.getName() << ", enter command:" << endl;
  while (cin >> command) {
    if (command == "roll") {
      handleRoll(currPlayer, hasRolled);
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
      string filename;
      cin >> filename;

      ofstream out{filename};
      saveGame(out);
      cout << "Game saved successfully to " << filename << endl;
    } // if save
    else if (command == "bankrupt") {
      handleBankrupt(currPlayer);
      return; // turn is over if player declares bankruptcy
    }
    else if (command == "trade") {
      handleTrade(currPlayer);
    } 
    else if (command == "mortgage") {
      handleMortgage(currPlayer);
    } 
    else if (command == "unmortgage") {
      handleUnmortgage(currPlayer);
    } 
    else if (command == "improve") {
      handleImprove(currPlayer);
    }
    else {
      cout << "Invalid command." << endl;
    }
  } // while
} // playTurn


// HELPERS
void Game::handleRoll(Player& currPlayer, bool& hasRolled) {
  if (hasRolled) { cout << "You already rolled this turn." << endl; }
  else {
    // generate random numbers for dice
    int die1 = generateRandom(1, 6);
    int die2 = generateRandom(1, 6);
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
    board.getSquare(newPos).landOn(&currPlayer);

    // check if player rolled doubles
    if (die1 == die2) {
      cout << "You rolled doubles! You get to roll again." << endl;
    } 
    else {
      hasRolled = true;
    }
  } // else (hasRolled == false)
} // handleRoll

void Game::handleBankrupt(Player& currPlayer) {}
void Game::handleTrade(Player& currPlayer) {}
void Game::handleImprove(Player& currPlayer) {}

void Game::handleMortgage(Player& currPlayer) {
  string propName;
  cin >> propName;
  Property* prop = board.getProperty(propName);

  // check if property exists and currPlayer owns it
  if (prop == nullptr) { cout << "Invalid Property" << endl; }
  else if (prop->getOwner() != &currPlayer) {
    cout << "You can only mortgage your own properties." << endl;
  } else {
    // check property for improvements
    AcademicBuilding* ab = dynamic_cast<AcademicBuilding*>(prop);
    if (ab != nullptr && ab->getNumImprovements() > 0) {
      cout << "You must sell all improvements before mortgaging." << endl;
    } else {
      prop->mortgage();
    }
  }
} // handleMortgage

void Game::handleUnmortgage(Player& currPlayer) {
  string propName;
  cin >> propName;
  Property* prop = board.getProperty(propName);

  // check if property exists and currPlayer owns it
  if (prop == nullptr) { cout << "Invalid Property" << endl; }
  else if (prop->getOwner() != &currPlayer) {
    cout << "You can only unmortgage your own properties." << endl;
  } else {
    int unmortgageCost = (prop->getCost() / 2) * 1.10;
    if (currPlayer.getMoney() < unmortgageCost) {
      cout << "Not enough money :(" << prop->getName() << " costs $"
        << unmortgageCost << " to unmortgage." << endl;
    } else {
      prop->unmortgage();
    }
  }
} // handleUnmortgage

