module game;

import <iostream>;
import <fstream>;
import <sstream>;
import <vector>;
import <string>;
import <sstream>;

import playerSquareProperty;
import board;
import academicbuilding;
import tools;

using namespace std;



Game::Game(istream& boardIn, istream& squaresIn, istream& cardsIn):
  board{boardIn, squaresIn, cardsIn, &players} {}

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

  board.display();
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
      bool isBankrupt = handleBankrupt(currPlayer, currPlayer.getDebt(), currPlayer.getCreditor());
      if (isBankrupt) return true; // turn is over if player declares bankruptcy
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
void Game::handleRoll(Player& currPlayer, bool& hasRolled, bool testMode) {
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
  
  int rollSum = die1 + die2;
      
  // update player
  currPlayer.setLastRoll(rollSum);
  int oldPos = currPlayer.getPosition();
  int newPos = (oldPos + rollSum) % 40;
  currPlayer.setPosition(newPos);
  
  board.display();
  cout << currPlayer.getName() << " rolled " << die1
       << " + " << die2 << " = " << rollSum << "!" << endl;
     
  // check if passed collectOSAP (check >40 since =40 is handled by 
  // landOn method in CollectOSAP class)
  if (oldPos + rollSum > 40 && newPos > 0) {
    cout << currPlayer.getName() << " collected $200 from OSAP!" << endl;
    currPlayer.receive(200);
  }

    // apply square action
    Square* landedSquare = &board.getSquare(newPos); // getSquare returns by reference
    landedSquare->landOn(&currPlayer);
    cout << currPlayer.getName() << " landed on " << landedSquare->getName() << "." << endl;
    // if the square was a property and was not bought, need to auction,
    // otherwise just move on with the turn
    Property* landedProp = dynamic_cast<Property*>(landedSquare);
    if (landedProp != nullptr && landedProp->getOwner() == nullptr) {
      handleAuction(landedProp);
    }

  // check if player rolled doubles
  if (die1 == die2) {
    cout << "You rolled doubles! You get to roll again." << endl;
  } 
  else {
    hasRolled = true;
  }
} // handleRoll

bool Game::handleBankrupt(Player& currPlayer, int amountOwed = 0, Player* creditor = nullptr) {

  // Step 1: Can already pay — cannot declare bankruptcy
  if (currPlayer.getMoney() >= amountOwed) {
    cout << "You have enough money to pay. Cannot declare bankruptcy." << endl;
    return false;
  }

  cout << "You are in debt. Choose an option:" << endl;
  cout << "1. Declare Bankruptcy" << endl;
  cout << "2. Try to raise money" << endl;
  cout << "Enter either '1' or '2'" << endl;

  int choice;
  cin >> choice;

  bool declaringBankruptcy = (choice == 1);

  // ---- SELLING PHASE ----
  if (!declaringBankruptcy) {
    while (currPlayer.getMoney() < amountOwed) {
      cout << "Your current balance: $" << currPlayer.getMoney() << endl;
      cout << "You owe: $" << amountOwed << endl;
      cout << "Choose action:" << endl;
      cout << "1. Mortgage a property" << endl;
      cout << "2. Sell improvement" << endl;
      cout << "3. Declare Bankruptcy (only if you have no assets remaining)" << endl;
      cout << "Enter '1', '2' or '3'" << endl;

      int action;
      cin >> action;

      // Mortgage a Property
      if (action == 1) {
        cout << "Enter Property Name:" << endl;
        string propName;
        cin >> propName;

        Property* prop = board.getProperty(propName);

        if (prop == nullptr) {
          cout << "Invalid property." << endl;
          continue;
        }

        if (prop->getOwner() != &currPlayer) {
          cout << "You don't own this property." << endl;
          continue;
        }

        if (prop->isMortgaged()) {
          cout << "Property is already mortgaged." << endl;
          continue;
        }

        prop->mortgage();
        cout << "Property mortgaged successfully." << endl;
      }

      // Sell improvement
      else if (action == 2) {
        cout << "Enter Property Name:" << endl;
        string propName;
        cin >> propName;

        Property* prop = board.getProperty(propName);
        AcademicBuilding* ab = dynamic_cast<AcademicBuilding*>(prop);

        if (ab == nullptr) {
          cout << "Not an academic building." << endl;
          continue;
        }

        if (ab->getOwner() != &currPlayer) {
          cout << "You don't own this property." << endl;
          continue;
        }

        if (ab->getNumImprovements() == 0) {
          cout << "No improvements to sell." << endl;
          continue;
        }

        ab->removeImprovement();
        int refund = ab->getImprovementCost() / 2;
        currPlayer.receive(refund);
        cout << "Sold improvement for $" << refund << endl;
      }

      // Declare Bankruptcy (only if truly no assets remain)
      else if (action == 3) {
        bool hasAssets = false;

        for (auto prop : currPlayer.getProperties()) {
          // Unmortgaged property = can still mortgage it
          if (!prop->isMortgaged()) {
            // Unmortgaged property = still has assets (can be mortgaged)
            hasAssets = true;
            break;
          }

          // Already mortgaged — check if it has improvements to sell
          AcademicBuilding* ab = dynamic_cast<AcademicBuilding*>(prop);
          if (ab != nullptr && ab->getNumImprovements() > 0) {
            hasAssets = true;
            break;
          }
        }

        if (hasAssets) {
          cout << "You still have assets to sell or mortgage. You cannot declare bankruptcy yet." << endl;
        } else {
          declaringBankruptcy = true;
          break;
        }
      }

      else {
        cout << "Invalid action." << endl;
      }
    }
  }

  // Successfully raised enough money
  if (!declaringBankruptcy && currPlayer.getMoney() >= amountOwed) {
    cout << "You have successfully recovered from debt!" << endl;
    return false;
  }

  // ---- BANKRUPTCY ----
  cout << currPlayer.getName() << " is bankrupt!" << endl;

  auto props = currPlayer.getProperties();

  if (creditor == nullptr) {
    // ---- BANK CASE: auction all properties as unmortgaged ----
    for (auto prop : props) {
      if (prop->isMortgaged()) {
        prop->unmortgage();  // properties go to auction as unmortgaged
        // Note: This will charge the bankrupt player the unmortage fee, which
        // will add to their debt, but player is already bankrupt so not
        // important
      }
      prop->setOwner(nullptr);
      handleAuction(prop);
    }

    // Destroy all Roll Up the Rim cups
    totalCups -= currPlayer.getCups();
  }
  else {
    // ---- PLAYER CASE: transfer all assets to creditor ----
    for (auto prop : props) {
      prop->setOwner(creditor);

      if (prop->isMortgaged()) {
        // Creditor must immediately pay 10% of original cost to the bank
        int fee = static_cast<int>(prop->getCost() * 0.1);
        creditor->pay(fee);
        cout << creditor->getName() << " paid $" << fee
             << " (10% fee) for mortgaged property: " << prop->getName() << endl;

        //  Offer creditor the option to unmortgage immediately
        cout << "Would you like to unmortgage " << prop->getName()
             << " now for $" << (prop->getCost() / 2) << "? (1 = Yes, 2 = No)" << endl;
        int unmortgageChoice;
        cin >> unmortgageChoice;
        if (unmortgageChoice == 1) {
          creditor->pay(prop->getCost() / 2);
          prop->unmortgage();
          cout << prop->getName() << " has been unmortgaged." << endl;
        } else {
          cout << "Property left mortgaged. Note: an additional 10% fee applies if unmortgaged later." << endl;
        }
      }
    }

    // Transfer Roll Up the Rim cups to creditor
    for (int i = 0; i < currPlayer.getCups(); ++i) {
      creditor->addCup();
    }
  }

  currPlayer.declareBankrupt();

  // Remove player from game
  cout << currPlayer.getName() << " has been removed from the game." << endl;
  players.erase(players.begin() + currentPlayer);

  // Adjust index so we don't skip the next player
  if (currentPlayer >= static_cast<int>(players.size()) && !players.empty()) {
    currentPlayer = 0;
  }
  return true;
} // handleBankrupt

void Game::handleAuction(Property* prop) {
  // track highest bidder and highest bid
  int highestBid = 0;
  Player* highestBidder = nullptr;
  size_t currBidderIdx = 0;

  // track players who are still bidding
  std::vector<Player*> activePlayers;
  for (auto& p : players) { activePlayers.emplace_back(&p); }

  // auction
  while (activePlayers.size() > 1) {
    // Ask bidder for bid
    Player* currBidder = activePlayers[currBidderIdx];
    cout << currBidder->getName() << " is up. Enter a bid greater than $" <<
      highestBid << ", or type 'leave'" << endl;
    string input;
    cin >> input;

    // Check input
    if (input == "leave") {
      cout << currBidder->getName() << " has left the auction." << endl;
      activePlayers.erase(activePlayers.begin() + currBidderIdx); // Note players are shifted

      // if deleted last player, then need to reset index
      // Note: This comparison is why currBidderIdx is size_t
      if (currBidderIdx >= activePlayers.size()) { currBidderIdx = 0; } 
    }
    else {
      // check that input is: an int, within player's budget, and higher than previous' bid
      istringstream iss{input};
      int amount;
      if (iss >> amount && iss.eof()) {
        if (amount <= highestBid) {
          cout << "Bid must be higher than $" << highestBid << "." << endl;
        }
        else if (currBidder->getMoney() < amount) {
          cout << "You cannot bid this much money, you only have $"
            << currBidder->getMoney() << " cash." << endl;
        }
        else {
          // no issues
          highestBid = amount;
          highestBidder = currBidder;
          cout << highestBidder->getName() << " bid $" << highestBid << "!" << endl;
          currBidderIdx = (currBidderIdx + 1) % activePlayers.size(); // next player
        }
      } else {
        cout << "Invalid input." << endl;
      } 
    } // input was not 'leave'
  } // while

  // Declare winner
  if (highestBidder == nullptr) { highestBidder = activePlayers[0]; }
  cout << "Going once, going twice, sold! " << highestBidder->getName()
    << " bought " << prop->getName() << " for $" << highestBid << "!!" << endl;
  highestBidder->pay(highestBid);
  prop->setOwner(highestBidder);
} // handleAuction

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

