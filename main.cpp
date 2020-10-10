#include "Header.h"


bool playing = true;

std::vector<std::string> suites = { "Clubs","Diamonds","Spades","Hearts" };
std::vector<std::string> ranks = { "Two","Three", "Four", "Five", "Six", "Seven",
				"Eight","Nine", "Ten", "Jack", "Queen", "King", "Ace" };
std::map<std::string, int> values = { {"Ace",11},{"Two",2},{"Three",3},{"Four",4},
				{"Five",9},{"Six",8},{"Seven",7},{"Eight",8},{"Nine",9},{"Ten",10},
				{"Jack",10},{"Queen",10},{"King",10} };


bool play = true;



class Card {
	std::string suit, rank;
public:
	Card(std::string suit, std::string rank) : suit(suit), rank(rank) {};
	std::string print() { return(rank + " of " + suit); }

	std::string getrank() { return rank; }
	std::string getsuit() { return suit; }
};

class Deck {
	std::vector<Card> deck;
public:
	Deck() {
		for (auto s : suites)
			for (auto r : ranks)
				deck.push_back(Card(s, r));
	}

	void shuffle() {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
	}

	Card pop() {
		Card temp = deck[deck.size() - 1];
		deck.pop_back();
		return temp;
	}


	void print() {
		for (auto c : deck)
			std::cout << c.print() << std::endl;
	}
};



class Chips {
	int total = 100;
	int bet = 0;

public:
	void getbet() {
		std::cout << "Enter a Betting amount: ";
		std::cin >> bet;
		if (bet > 100 || bet < 0) getbet();
	}

	void winbet() {
		total += bet;
	}

	void losebet() {
		total -= bet;
	}


	int gettotal() {
		return total;
	}
};

class Hand {
	std::vector<Card> cards;
	int aces = 0;
	int value = 0;

public:
	void add_card(Card card) {
		std::string rank = card.getrank();
		if (rank == "Ace") aces++;
		value += values[rank];
		cards.push_back(card);
		if (value > 21 && aces) fixaces();
	}

	void fixaces() {
		while (value > 21 && aces > 0) {
			value -= 10;
			aces--;
		}
	}

	int getValue() {
		return value;
	}

	void print() {
		for (auto it : cards) std::cout << it.print() << std::endl;
	}

	std::vector<Card>& hands() { return cards; }
};

void showall(Hand& playerhand, Hand& dealerhand) {
	std::cout << "\n-------------------------\n";
	std::cout << "Dealer's Hand" << std::endl;
	for (auto it : dealerhand.hands())
		std::cout << it.print() << std::endl;

	std::cout << "\n\n";

	std::cout << "Player's Hand" << std::endl;
	for (auto it : playerhand.hands())
		std::cout << it.print() << std::endl;
	std::cout << "\n-------------------------\n";
}


void showsome(Hand& playerhand, Hand& dealerhand) {
	std::vector<Card> dcards = dealerhand.hands();
	std::vector<Card> pcards = playerhand.hands();

	std::cout << "\n-------------------------\n";
	std::cout << "Dealer's Hand" << std::endl;
	std::cout << dcards[0].print() << std::endl;
	std::cout << "Hidden Card" << std::endl;

	std::cout << "\n\n";

	std::cout << "Player's Hand" << std::endl;
	for (auto it : playerhand.hands())
		std::cout << it.print() << std::endl;
	std::cout << "\n-------------------------\n";
}

void hit(Deck& d, Hand& playerhand) {
	Card singlecard = d.pop();
	playerhand.add_card(singlecard);
}


void hit_or_stand(Deck& d, Hand& playerhand) {
	char c;
	while (play) {
		std::cout << "Hit or Stand(h/s) " << std::endl;
		std::cin >> c;
		if (c == 'h') hit(d, playerhand);
		else if (c == 's') {
			std::cout << "Player stands, Dealer's turn" << std::endl;
			play = false;
		}
		else continue;
		break;
	}
}


void playerbusts(Chips& chips) {
	std::cout << "Player Busts" << std::endl;
	chips.losebet();
}

void dealerbusts(Chips& chips) {
	std::cout << "Dealer Busts" << std::endl;
	chips.winbet();
}

void playerwins(Chips& chips) {
	std::cout << "Player Wins" << std::endl;
	chips.winbet();
}

void dealerwins(Chips& chips) {
	std::cout << "Dealer Wins" << std::endl;
	chips.losebet();
}

void push() {
	std::cout << "Draw" << std::endl;
}

int main() {
	while (true) {
		Deck d;
		d.shuffle();

		Hand playerhand, dealerhand;

		playerhand.add_card(d.pop());
		playerhand.add_card(d.pop());

		dealerhand.add_card(d.pop());
		dealerhand.add_card(d.pop());

		Chips playerchips;
		playerchips.getbet();
		showsome(playerhand, dealerhand);


		while (play) {

			hit_or_stand(d, playerhand);
			std::cout << "\n";
			showsome(playerhand, dealerhand);

			if (playerhand.getValue() > 21) {
				playerbusts(playerchips);
				break;
			}

		}

		if (playerhand.getValue() <= 21) {
			while (dealerhand.getValue() < 17) {
				hit(d, dealerhand);
			}

			std::cout << "\n";
			showall(playerhand, dealerhand);


			if (dealerhand.getValue() > 21) dealerbusts(playerchips);
			else if (dealerhand.getValue() > playerhand.getValue()) dealerwins(playerchips);
			else if (dealerhand.getValue() < playerhand.getValue()) playerwins(playerchips);
			else push();


		}

		std::cout << "\nPlayer Total Chips " << playerchips.gettotal() << std::endl;

		char c;
		std::cout << "DO you want to play more(Y/N) ";
		std::cin >> c;


		if (c == 'N' || c == 'n') {
			std::cout << "Thankyou for playing" << std::endl;
			break;
		}
		else if (c == 'Y' || c == 'y') play = true;


	}

	return 0;
}

