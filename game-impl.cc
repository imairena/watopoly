module game;

import <iostream>;
import <fstream>;
import <vector>;
import <string>;
import <sstream>;

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

bool Game::playTurn(bool testMode) {
  Player& currPlayer = players[currentPlayer];
  string command;
  bool hasRolled = false;

  cout << "Player " << currPlayer.getName() << ", enter command:" << endl;
  while (cin >> command) {
    cout << endl;
    if (command == "roll") {
      handleRoll(currPlayer, hasRolled, testMode);
    } // if roll
    else if (command == "next") {
      if (!hasRolled) {
        cout << "You must roll before moving to the next player!" << endl;
      } else {
        nextPlayer(cout);
        return true;
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
      return true; // turn is over if player declares bankruptcy
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
    else if (command == "board") {
      board.display();
    }
    else if (command == "exit") {
      cout << "Thank you for playing Watopoly!";
      return false;
    }
    else {
      cout << "Invalid command." << endl;
    }
    // prompt next command
    cout << "Player " << currPlayer.getName() << ", enter command:" << endl;
  } // while
  return false;
} // playTurn


// HELPERS
void Game::handleRoll(Player& currPlayer, bool hasRolled, bool testMode) {
  if (hasRolled) {
    cout << "You already rolled this turn." << endl;
    return;
  }
  int die1, die2;
  if (testMode) {
    cout << "What two (non-negative) numbers would you like to roll?" << endl;
    string dieString1, dieString2;
    
    while (true) {
      cin >> dieString1 >> dieString2;
      istringstream ss1{dieString1};
      istringstream ss2{dieString2};
      if (!(ss1 >> die1) || !(ss2 >> die2) || die1 < 0 || die2 < 0) {
        cout << "Invalid inputs for dice. Try again." << endl;
      } else break;
    }
  } else {
    // generate random numbers for dice
    die1 = generateRandom(1, 6);
    die2 = generateRandom(1, 6);
  }

  board.display();
  
  int rollSum = die1 + die2;
  cout << currPlayer.getName() << " rolled " << die1
       << " + " << die2 << " = " << rollSum << "!" << endl;
      
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
} // handleRoll

void Game::handleBankrupt(Player& currPlayer) {}
void Game::handleTrade(Player& currPlayer) {}
void Game::handleImprove(Player& currPlayer) {
  string propName, action;
  cin >> propName;
  cin >> action;

  Property* prop = board.getProperty(propName);
  
  // check if property exists and currPlayer owns it
  if (prop == nullptr) { cout << "Invalid Property" << endl; }
  else if (prop->getOwner() != &currPlayer) {
    cout << "You can only modify properties you own." << endl;
  } else {
    // check if property is academic building
    AcademicBuilding* ab = dynamic_cast<AcademicBuilding*>(prop);
    if (ab == nullptr) {
      cout << "You can only improve academic buildings." << endl;
    }
    // check if monopoly is owned
    else if (!ab->hasMonopoly()) {
      cout << "Need monopoly to improve " << propName << "." << endl;
    }
    else if (action == "buy") {
      int improvementCost = ab->getImprovementCost();
      // check if reached max number of improvements
      if (ab->getNumImprovements() == 5) {
        cout << propName << " is maxed out." << endl;
      }
      // check if player has enough money
      else if (currPlayer.getMoney() < improvementCost) {
        cout << "You do not have enough money to improve " << propName << "." << endl;
      }
      // buy improvement if no issues
      else {
        currPlayer.pay(improvementCost);
        ab->addImprovement();
        cout << propName << " was improved! Number of improvements: "
          << ab->getNumImprovements() << "." << endl;
      }
    }
    else if (action == "sell") {
      // check if any improvements are owned
      if (ab->getNumImprovements() == 0) {
        cout << propName << " has no improvements." << endl;
      }
      else {
        int sellingCost = ab->getImprovementCost() / 2;
        currPlayer.receive(sellingCost);
        ab->removeImprovement();
        cout << "Sold improvement on " << propName << "for $" << sellingCost
          << ". Improvements left: " << ab->getNumImprovements() << "." <<  endl;
      }
    }
    else {
      cout << "Invalid action" << endl;
    }
  } 
} // handleImprove

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

