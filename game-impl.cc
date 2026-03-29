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

int Game::getActivePlayers() const {
  int numActivePlayers = 0;
  for (const auto& p : players) {
    if (!p.bankrupt()) { numActivePlayers++; }
  }
  return numActivePlayers;
}


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
  // Load players
  string numPlayersString;
  int numPlayers;
  
  if (!getline(gameIn, numPlayersString)) {
    cout << "Invalid file" << endl;
    return;
  }
  stringstream ss{numPlayersString};
  ss >> numPlayers;
  
  // Valid file
  for (int i = 0; i < numPlayers; i++) {
    string line, name;
    char token;
    int cups, money, position;

    getline(gameIn, line);
    stringstream ss{line};
    
    // read player info
    ss >> name >> token >> cups >> money >> position;
    // add the player
    addPlayer(name, token);
    // Update player.s stats
    Player& p = players.back(); // return last player we added by reference
    p.setMoney(money);
    for (int j = 0; j < cups; j++) { p.addCup(); }
    p.setPosition(position);
    // Edge case: Player was in Tims
    if (position == 10) {
      int inTims, turns;
      ss >> inTims >> turns;

      // Update player's status
      p.setInTims(inTims == 1);
      for (int k = 0; k < turns; ++k) { p.incrementTimsTurns(); }
    }
  } // loop to load players

  // Load board
  board.loadBoard(gameIn);
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
  cout << endl;

  if (currPlayer.getInTims()) {
    handleTimsTurn(currPlayer);
    // check if player still in Tims after handleTimsTurn
    if (!currPlayer.getInTims()) {
      cout << endl << "Now out of Tims, you may "
           << "continue your turn as usual." << endl;
    } else hasRolled = true;
  }
  
  cout << "Player " << currPlayer.getName() << ", enter command:" << endl
       << "(Type \"help\" to see available commands.)" << endl;
  while (cin >> command) {
    cout << endl;
    if (command == "roll") {
      if (currPlayer.getDebt() > 0) {
        cout << "You must pay your debt before rolling!" << endl;
      } else {
        handleRoll(currPlayer, hasRolled, testMode);
      }
    } // if roll
    else if (command == "next") {
      if (currPlayer.getDebt() > 0) {
        cout << "You must pay your debt before moving to the next player!" << endl;
      }
      else if (!hasRolled) {
        cout << "You must roll before moving to the next player!" << endl;
      }	else {
        currPlayer.setNumRolls(0);
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
      cout << "Please enter the name of the file you wish to save to:" << endl;
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
      cout << "Now exiting game." << endl
           << "Thank you for playing Watopoly!" << endl;
      return false;
    }
    else if (command == "help") {
      cout << "The available commands are:" << endl
           << "roll, next, board, assets, all, trade, mortgage," << endl
           << "unmortgage, improve, bankrupt, save, exit" << endl;
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
  currPlayer.incrementNumRolls();
  
  // update player
  currPlayer.setLastRoll(rollSum);
  int oldPos = currPlayer.getPosition();
  
  // Can't roll more than 2 doubles
  bool triple_double = (die1 == die2 && currPlayer.getNumRolls() >= 3) ? true : false;
  int new_position = triple_double ? 30 : oldPos + rollSum; // Go to Tims (pos 30) if you rolled 3 doubles

  int newPos = (new_position) % 40;
  currPlayer.setPosition(newPos);
  
  board.display();
  cout << currPlayer.getName() << " rolled " << die1
       << " + " << die2 << " = " << rollSum << "!" << endl;

  // check if player rolled doubles
  if (die1 == die2) {
    if (currPlayer.getNumRolls() < 3) {
      cout << "You rolled doubles! You get to roll again this turn." << endl << endl;
    }
    else {
      cout << "Oh No! You just rolled your third triple." << endl;
      hasRolled = true;
    }
  } else {
    hasRolled = true;
  }
     
  // check if passed collectOSAP (check >40 since =40 is handled by 
  // landOn method in CollectOSAP class)
  if (new_position > 40 && newPos > 0) {
    cout << currPlayer.getName() << " collected $200 from OSAP!" << endl << endl;
    currPlayer.receive(200);
  }

  handleLandOn(currPlayer, newPos);

  // If landOn() moved the player, then treat as if player landed on new square
  if (newPos != currPlayer.getPosition()) {
    newPos = currPlayer.getPosition();
    handleLandOn(currPlayer, newPos);
  }

  // Limit of two moves in a row
  if (newPos != currPlayer.getPosition()) {
    cout << "Max number of moves per turn reached." << endl << endl;
  }
}// handleRoll

void Game::handleLandOn(Player& currPlayer, int newPos) {
  // apply square action
  Square* landedSquare = &board.getSquare(newPos); // getSquare returns by reference
  cout << currPlayer.getName() << " landed on " << landedSquare->getName() << "." << endl;
  
  // ---- ROLL UP THE RIM LOGIC ----
  bool isCardSquare =
    (landedSquare->getName() == "SLC" ||
     landedSquare->getName() == "NEEDLES HALL");

  if (isCardSquare && totalCups < maxCups) {
    int chance = generateRandom(1, 100);

    if (chance == 67) {  // 1% chance
      cout << "Congratulations! You won a Roll Up the Rim cup!" << endl;

      currPlayer.addCup();
      totalCups++;

      cout << "You now have: " << currPlayer.getCups() << " cup(s)." << endl << endl;
      // Skip normal square effect
      return;
    }
  }

  // ---- NORMAL SQUARE EFFECT ----
  landedSquare->landOn(&currPlayer);

  // if the square was a property and was not bought, need to auction,
  // otherwise just move on with the turn
  Property* landedProp = dynamic_cast<Property*>(landedSquare);
  if (landedProp != nullptr && landedProp->getOwner() == nullptr) {
    handleAuction(landedProp);
  }
} // handleLandOn

void Game::handleTimsTurn(Player& currPlayer) {
  cout << currPlayer.getName() << " is in the DC Tims Line." << endl;
  cout << "Turns in Tims: " << currPlayer.getTimsTurns() << "/3" << endl;
  cout << "Current Tims cup(s): " << currPlayer.getCups() << endl;
  
  if (currPlayer.getTimsTurns() >= 3) {
    cout << "This is your third turn in Tims. You must leave." << endl;

    if (currPlayer.getCups() == 0 && currPlayer.getMoney() < 50) {
      cout << "Unfortunately, you do not have the means necessary to leave." << endl
           << "Therefore, you have gone bankrupt and lost the game." << endl;
      currPlayer.declareBankrupt();
      return;
    }
    
    while (true) {
      cout << "Choose an option:" << endl;
      cout << "1. Pay $50" << endl;
      if (currPlayer.getCups() > 0) {
        cout << "2. Use a Roll Up the Rim cup" << endl;
      }
      
      int choice;
      cin >> choice;
      
      // Pay $50 and leave Tims
      if (choice == 1) {
        currPlayer.pay(50);
        currPlayer.leaveTims();
        cout << "Paid $50 and left Tims." << endl;
        return;
      }
      
      // Use Tims Cup and leave Tims
      if (choice == 3) {
        if (currPlayer.getCups() <= 0) {
          cout << "You do not have any cups to use" << endl;
          continue;
        }
        currPlayer.useCup();
        totalCups--;
        cout << "Used a Roll Up the Rim cup and left Tims." << endl;
        return;
      }
    }
  }
      
  while (true) {
  
    cout << "Choose an option:" << endl;
    cout << "1. Roll for doubles" << endl;
    cout << "2. Pay $50" << endl;
    if (currPlayer.getCups() > 0) {
      cout << "3. Use a Roll Up the Rim cup" << endl;
    }

    int choice;
    cin >> choice;

    // Pay $50 and leave Tims
    if (choice == 2) {
      currPlayer.pay(50);
      currPlayer.leaveTims();
      cout << "Paid $50 and left Tims." << endl;
      return;
    }

    // Use Tims Cup and leave Tims
    if (choice == 3) {
      if (currPlayer.getCups() <= 0) {
        cout << "You do not have any cups to use" << endl;
        continue;
      }
      currPlayer.useCup();
      totalCups--;
      cout << "Used a Roll Up the Rim cup and left Tims." << endl;
      return;
    }
    
    // Try to roll for doubles
    if (choice == 1) {

      int die1 = generateRandom(1, 6);
      int die2 = generateRandom(1, 6);

      if (die1 == die2) {
        cout << "You rolled doubles (" << die1
             << " and " << die2 << ") and left Tims!" << endl;
        currPlayer.leaveTims();
      } else {
        cout << "You did not roll doubles. (" << die1
             << " and " << die2 << ")" << endl;
        currPlayer.incrementTimsTurns();
      }
      
      return;
    }
  }
} // handleTimsTurn

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
    currPlayer.pay(amountOwed);
    currPlayer.setDebt(0);
    if (creditor) creditor->receive(amountOwed);
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
  cout << "The property " << prop->getName() << " is up for auction!" << endl
       << "Everyone, place your bids." << endl << endl;
  while (activePlayers.size() > 1) {
    // Ask bidder for bid
    Player* currBidder = activePlayers[currBidderIdx];
    cout << endl << currBidder->getName() << " is up. Enter a bid greater than $"
         << highestBid << ", or type 'leave'" << endl;
    string input;
    cin >> input;

    // Check input
    if (input == "leave") {
      cout << currBidder->getName() << " has left the auction." << endl;
      activePlayers.erase(activePlayers.begin() + currBidderIdx); // Note players are shifted

      // if removed last player, then need to reset index
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
  cout << "\nGoing once, going twice, sold! " << highestBidder->getName()
    << " bought " << prop->getName() << " for $" << highestBid << "!!" << endl;
  highestBidder->pay(highestBid);
  prop->setOwner(highestBidder);
} // handleAuction

void Game::handleTrade(Player& currPlayer) {
  string otherPlayerName;
  Player *otherPlayer;

  // get other player
  while (true) {
    cout << "Which player would you like to trade with?" << endl;
    cin >> otherPlayerName;
    cout << endl;
    otherPlayer = board.getPlayer(otherPlayerName);
    if (!otherPlayer) {
      cout << "There is no player named " << otherPlayerName << "." << endl
           << "Please try again." << endl;
    } else if (&currPlayer == otherPlayer) {
      cout << "You cannot trade with yourself. Try again." << endl;
    }
    else break;
  }

  // determine the type of trade
  string receive, give;
  int intReceive, intGive;
  Property *propReceive = nullptr;
  Property *propGive = nullptr;

  // determine what to give
  while (true) {
    cout << "Please enter the amount of money/property you would like to GIVE:" << endl;
    cin >> give;
    cout << endl;
    stringstream ss{give};

    // give is an amount of money
    if (ss >> intGive) {
      if (intGive < 0) {
        cout << "You cannot trade a negative amount of money. try again." << endl;
        continue;
      }
      if (intGive > currPlayer.getMoney()) {
        cout << "You do not have enough money to make this trade. Try again." << endl;
        continue;
      }
      break;
    }
    // give is a property
    propGive = board.getProperty(give);
    if (!propGive) {
      cout << "Invalid property/amount of money. Try again." << endl;
      continue;
    }
    if (propGive->getOwner() != &currPlayer) {
      cout << "You do not own this property. Try again." << endl;
      continue;
    }
    break;
  }

  // determine what to receive
  while (true) {
    cout << "Please enter the amount of money/property you would like to RECEIVE:" << endl;
    cin >> receive;
    cout << endl;
    stringstream ss{receive};

    // receive is an amount of money
    if (ss >> intReceive) {
      if (intReceive < 0) {
        cout << "You cannot trade a negative amount of money. try again." << endl;
        continue;
      }
      if (intReceive > otherPlayer->getMoney()) {
        cout << otherPlayer->getName()
             << " does not have enough money to make this trade. Try again." << endl;
        continue;
      }
      break;
    }
    // give is a property
    propReceive = board.getProperty(receive);
    if (!propReceive) {
      cout << "Invalid property/amount of money. Try again." << endl;
      continue;
    }
    if (propReceive->getOwner() != otherPlayer) {
      cout << otherPlayer->getName()
           << " does not own this property. Try again." << endl;
      continue;
    }
    break;
  }

  // can't trade money for money
  if (!propGive && !propReceive) {
    cout << "You may not trade money for money. Aborting trade." << endl << endl;
    return;
  }

  // display trade details
  cout << "Trade summary:" << endl << "Player " << currPlayer.getName()
       << " would GIVE " << give << " to player " << otherPlayer->getName() << endl
       << "and RECEIVE " << receive << " in return." << endl << endl;
  
  // get response
  string response;
  while (true) {
    cout << "Player " << otherPlayer->getName()
         << ", please enter your response to the proposed trade:" << endl
         << "(\"accept\" or \"reject\")" << endl;
    cin >> response;
    cout << endl;
    if (response == "accept") break;
    else if (response == "reject") {
      cout << "Trade has been rejected. Player " << currPlayer.getName()
           << " may continue their turn as usual." << endl << endl;
      return;
    } else {
      cout << "Invalid response. Try again." << endl;
      continue;
    }
  }
  
  // complete give part of trade
  if (propGive) {
    propGive->setOwner(otherPlayer);
  } else {
    currPlayer.pay(intGive);
    otherPlayer->receive(intGive);
  }

  // complete receive part of trade
  if (propReceive) {
    propReceive->setOwner(&currPlayer);
  } else {
    currPlayer.receive(intReceive);
    otherPlayer->pay(intReceive);
  }

  cout << "Trade completed successfully!" << endl << endl;
} // handleTrade

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
      cout << "Need monopoly to buy/sell improvements for " << propName << "." << endl;
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

