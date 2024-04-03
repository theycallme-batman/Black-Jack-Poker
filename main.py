import random

playing = True

suites = ["Clubs", "Diamonds", "Spades", "Hearts"]
ranks = ["Two", "Three", "Four", "Five", "Six", "Seven",
         "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"]
values = {"Ace": 11, "Two": 2, "Three": 3, "Four": 4,
          "Five": 9, "Six": 8, "Seven": 7, "Eight": 8, "Nine": 9, "Ten": 10,
          "Jack": 10, "Queen": 10, "King": 10}

play = True


class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank

    def print_card(self):
        return f"{self.rank} of {self.suit}"


class Deck:
    def __init__(self):
        self.deck = [Card(suit, rank) for suit in suites for rank in ranks]

    def shuffle(self):
        random.shuffle(self.deck)

    def pop(self):
        return self.deck.pop()

    def print_deck(self):
        for card in self.deck:
            print(card.print_card())


class Chips:
    def __init__(self):
        self.total = 100
        self.bet = 0

    def get_bet(self):
        while True:
            try:
                self.bet = int(input("Enter a betting amount: "))
                if self.bet > 100 or self.bet < 0:
                    raise ValueError
                break
            except ValueError:
                print("Invalid input! Please enter a valid betting amount.")

    def win_bet(self):
        self.total += self.bet

    def lose_bet(self):
        self.total -= self.bet

    def get_total(self):
        return self.total


class Hand:
    def __init__(self):
        self.cards = []
        self.aces = 0
        self.value = 0

    def add_card(self, card):
        self.cards.append(card)
        self.value += values[card.rank]
        if card.rank == "Ace":
            self.aces += 1
        if self.value > 21 and self.aces:
            self.fix_aces()

    def fix_aces(self):
        while self.value > 21 and self.aces > 0:
            self.value -= 10
            self.aces -= 1

    def get_value(self):
        return self.value

    def print_hand(self):
        for card in self.cards:
            print(card.print_card())


def show_all(player_hand, dealer_hand):
    print("\n-------------------------\n")
    print("Dealer's Hand")
    for card in dealer_hand.cards:
        print(card.print_card())

    print("\n\n")

    print("Player's Hand")
    for card in player_hand.cards:
        print(card.print_card())
    print("\n-------------------------\n")


def show_some(player_hand, dealer_hand):
    print("\n-------------------------\n")
    print("Dealer's Hand")
    print(dealer_hand.cards[0].print_card())
    print("Hidden Card")

    print("\n\n")

    print("Player's Hand")
    for card in player_hand.cards:
        print(card.print_card())
    print("\n-------------------------\n")


def hit(deck, player_hand):
    card = deck.pop()
    player_hand.add_card(card)


def hit_or_stand(deck, player_hand):
    global playing
    while playing:
        choice = input("Hit or Stand (h/s): ")
        if choice.lower() == 'h':
            hit(deck, player_hand)
        elif choice.lower() == 's':
            print("Player stands, Dealer's turn")
            playing = False
        else:
            continue
        break


def player_busts(chips):
    print("Player Busts")
    chips.lose_bet()


def dealer_busts(chips):
    print("Dealer Busts")
    chips.win_bet()


def player_wins(chips):
    print("Player Wins")
    chips.win_bet()


def dealer_wins(chips):
    print("Dealer Wins")
    chips.lose_bet()


def push():
    print("Draw")


def main():
    global playing
    while True:
        deck = Deck()
        deck.shuffle()

        player_hand, dealer_hand = Hand(), Hand()

        player_hand.add_card(deck.pop())
        player_hand.add_card(deck.pop())

        dealer_hand.add_card(deck.pop())
        dealer_hand.add_card(deck.pop())

        player_chips = Chips()
        player_chips.get_bet()
        show_some(player_hand, dealer_hand)

        while playing:
            hit_or_stand(deck, player_hand)
            print("\n")
            show_some(player_hand, dealer_hand)

            if player_hand.get_value() > 21:
                player_busts(player_chips)
                break

        if player_hand.get_value() <= 21:
            while dealer_hand.get_value() < 17:
                hit(deck, dealer_hand)

            print("\n")
            show_all(player_hand, dealer_hand)

            if dealer_hand.get_value() > 21:
                dealer_busts(player_chips)
            elif dealer_hand.get_value() > player_hand.get_value():
                dealer_wins(player_chips)
            elif dealer_hand.get_value() < player_hand.get_value():
                player_wins(player_chips)
            else:
                push()

        print("\nPlayer Total Chips:", player_chips.get_total())
        choice = input("Do you want to play more (Y/N)? ")
        if choice.lower() == 'n':
            print("Thank you for playing!")
            break
        elif choice.lower() == 'y':
            playing = True


if __name__ == "__main__":
    main()
