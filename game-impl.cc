module game;

import <iostream>;
import <vector>;
import <string>;

import playerSquareProperty;
import board;

using namespace std;



Game::Game(istream& boardIn, istream& squaresIn, istream& cardsIn):
  board{boardIn, squaresIn, cardsIn} {}

void Game::playTurn() {
  // to do
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

void Game::all() const {
  for (auto p : players) {
    p.assets();
  }
}

