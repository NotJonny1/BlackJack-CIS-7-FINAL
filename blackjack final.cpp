#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

const int N_SUITS = 4;
const int N_RANKS = 13;
const float ADVANTAGE[N_RANKS] = { 9.8f, 13.4f, 18.0f, 23.2f, 23.9f, 14.3f, 5.4f, -4.3f,
    -16.9f, -16.9f, -16.9f, -16.9f, -16.0f };
const int VALUES[N_RANKS] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
const string RANK_NAMES[N_RANKS] = { "ACE", "2", "3", "4", "5", "6", "7", "8", "9", "10", "JACK", "QUEEN", "KING" };
const string SUIT_NAMES[N_SUITS] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
const string COLORS[2] = { "RED", "BLACK" };


struct Card {
    int value;
    string name;
    string suit;
    string color;
};

struct Player {
    string name;
    int count = 0;
    string status[2] = { "waiting...", "playing..." };
    float bet = 0;
    Card my_cards[10];
};


class Deck {
private:
    Card cards[N_SUITS][N_RANKS];

public:
    Deck() {
        srand(static_cast<int> (time(0)));
        for (int i = 0; i < N_SUITS; ++i) {
            for (int j = 0; j < N_RANKS; ++j) {
                cards[i][j].suit = SUIT_NAMES[i];
                cards[i][j].value = VALUES[j];
                cards[i][j].name = RANK_NAMES[j];
                if ((cards[i][j].suit == "DIAMONDS") || (cards[i][j].suit == "HEARTS")) cards[i][j].color = COLORS[0];
                else if ((cards[i][j].suit == "CLUBS") || (cards[i][j].suit == "SPADES")) cards[i][j].color = COLORS[1];
            }
        }
    }

    ~Deck() {
    }

    Card deal() {
        Card hand;
        hand = cards[rand() % N_SUITS][rand() % N_RANKS];
        return hand;
    }
};


class Blackjack {
private:
    Player players[2];

public:
    Blackjack(string name, float bet) {
        this->players[0].name = "DEALER";
        this->players[1].name = name;
        this->players[0].bet = 100000.0f;
        this->players[1].bet = bet;
    }

    ~Blackjack() {
    }

    void main_menu(Deck new_deck) {
        short int choice = 0;
        cout << "\n\n\t\t\t\tBLACK JACK\n\n\n\n\tPress 1 and enter to play or press any other number and enter to quit:   ";
        cin >> choice;
        switch (choice) {
        case 1: play(new_deck);
            break;
        default: cout << "\n\n\tOK, BYE...";
            break;
        }
    }

    void play(Deck new_deck) {
        int choice = 1;
        int lucky = 0;
        int showing = 3;
        int num_of_lucky = 0;
        float probability = 0;
        switch (choice) {
        case 1:
            players[0].my_cards[0] = new_deck.deal();
            players[0].my_cards[1] = new_deck.deal();
            players[0].count = players[0].my_cards[0].value + players[0].my_cards[1].value;
            cout << "\n\nPlayer: " << players[0].name << "\nCount: ***" << "\nStatus: " << players[0].status[1] << "\nBank: $" << fixed << std::setprecision(2) << players[0].bet;
            cout << "\n\n" << players[0].name << "\'s hand: " << players[0].my_cards[1].name << " of " << players[0].my_cards[1].suit << " and [HIDDEN CARD]";
            cout << "\n\n\tPress 1 and enter to play this hand or press any other number and enter to quit:  ";
            cin >> choice;
            if (choice != 1) {
                cout << "\n\n\tOK, BYE...";
                break;
            }
            players[1].my_cards[0] = new_deck.deal();
            players[1].my_cards[1] = new_deck.deal();
            players[1].count = players[1].my_cards[0].value + players[1].my_cards[1].value;
            cout << "\n\nPlayer: " << players[1].name << "(YOU)\nCount: " << players[1].count << "\nStatus: " << players[1].status[1] << "\nBank: $" << fixed << std::setprecision(2) << players[1].bet;
            cout << "\n\n" << players[1].name << "\'s hand: " << players[1].my_cards[0].name << " of " << players[1].my_cards[0].suit << " and " << players[1].my_cards[1].name << " of " << players[1].my_cards[1].suit;
            lucky = 21 - players[1].count;
            if (lucky > 10) {
                lucky = 10;
            }
            for (int i = 0; i < 2; ++i) {
                if ((players[0].my_cards[i].value == lucky) || (players[1].my_cards[i].value == lucky)) {
                    num_of_lucky++;
                }
            }

            probability = 4 - num_of_lucky;
            probability /= (52 - showing);
            cout << "\n\n\t\tYour advantage vs. the dealer based on the dealer's up card:  " << fixed << std::setprecision(2) << player_advantage(players[0].my_cards[1].value) << "%.";
            cout << "\n\n\t\tThe probability of you getting " << lucky << " as your next card is " << fixed << std::setprecision(3) << (probability * 100) << "%.";
            cout << "\n\n\tPress 1 and enter to play this hand or press any other number and enter to quit:  ";
            cin >> choice;
            if (choice == 1) {
                handle_play(new_deck, 2);
            }
            else {
                cout << "\n\n\tOK, BYE...";
                break;
            }
        }
    }

    float player_advantage(int card) {
        float adv = 0;
        for (int i = 0; i < N_SUITS; ++i) {
            if (card == VALUES[i]) {
                if (VALUES[i] == 1) {
                    adv = -16.0f;
                }
                else if (VALUES[i] == 8) {
                    adv = -16.9f;
                }
                else {
                    adv = ADVANTAGE[i - 1];
                }
            }
        }
        return adv;
    }

    void handle_play(Deck new_deck, int c)
    {
        int choice = 0;
        int cards = c;
        int lucky = 0;
        int showing = (c * 2) - 1;
        int num_of_lucky = 0;
        float probability = 0;
        cout << "\n\n1. To Take another card from the dealer.\n2. To stay and show cards.\n5. To surrender press any number and enter.\n\tCHOICE:  ";
        cin >> choice;
        switch (choice) {
        case 1:
        {
            players[0].my_cards[cards] = new_deck.deal();
            players[1].my_cards[cards] = new_deck.deal();
            players[0].count += players[0].my_cards[cards].value;
            players[1].count += players[1].my_cards[cards].value;
            ++cards;
            cout << "\n\nPlayer: " << players[1].name << "(YOU)\nCount: " << players[1].count << "\nBank: $" << fixed << std::setprecision(2) << players[1].bet;
            cout << "\n\n" << players[1].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[1].my_cards[i].name << " of " << players[1].my_cards[i].suit << "\n";
            }
            lucky = 21 - players[1].count;
            if (lucky > 10) {
                lucky = 10;
            }
            for (int i = 0; i < cards; ++i) {
                if ((players[0].my_cards[i].value == lucky) || (players[1].my_cards[i].value == lucky)) {
                    num_of_lucky++;
                }
            }

            probability = 4 - num_of_lucky;
            probability = probability / (52 - showing);
            cout << "\n\n\t\tYour advantage vs. the dealer based on the dealer's up card:  " << fixed << std::setprecision(2) << player_advantage(players[0].my_cards[1].value) << "%.";
            cout << "\n\n\t\tThe probability of you getting " << lucky << " as your next card is " << fixed << std::setprecision(3) << (probability * 100) << "%.";
            if (players[0].count >= 21 || players[1].count >= 21) break;
            else handle_play(new_deck, cards);
        }
        break;

        case 2:
        {
            cout << "\n\n\tOK...";
        }
        break;

        default: main_menu(new_deck);
        }
        if (((players[0].count <= 21) && (players[0].count > players[1].count)) || (players[1].count > 21)) {
            players[0].bet += players[1].bet;
            players[1].bet -= players[1].bet;
            cout << "\n\n\t THE HOUSE WINS!! SORRY!";
            cout << "\n\nPlayer: " << players[0].name << "\nCount: " << players[0].count << "\nBank: $" << fixed << std::setprecision(2) << players[0].bet;
            cout << "\n\n" << players[0].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[0].my_cards[i].name << " of " << players[0].my_cards[i].suit << "\n";
            }
            cout << "\n\nPlayer: " << players[1].name << "(YOU)\nCount: " << players[1].count << "\nBank: $" << fixed << std::setprecision(2) << players[1].bet;
            cout << "\n\n" << players[1].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[1].my_cards[i].name << " of " << players[1].my_cards[i].suit << "\n";
            }
        }
        else if (((players[1].count <= 21) && (players[1].count > players[0].count)) || (players[0].count > 21)) {
            players[0].bet -= players[1].bet;
            players[1].bet += players[1].bet;
            cout << "\n\n\tCONGRATULATIONS YOU WIN!!";
            cout << "\n\nPlayer: " << players[0].name << "\nCount: " << players[0].count << "\nBank: $" << fixed << std::setprecision(2) << players[0].bet;
            cout << "\n\n" << players[0].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[0].my_cards[i].name << " of " << players[0].my_cards[i].suit << "\n";
            }
            cout << "\n\nPlayer: " << players[1].name << "(YOU)\nCount: " << players[1].count << "\nBank: $" << fixed << std::setprecision(2) << players[1].bet;
            cout << "\n\n" << players[1].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[1].my_cards[i].name << " of " << players[1].my_cards[i].suit << "\n";
            }
        }
        else {
            cout << "\n\n\tNOBODY WINS!!";
            cout << "\n\nPlayer: " << players[0].name << "\nCount: " << players[0].count << "\nBank: $" << fixed << std::setprecision(2) << players[0].bet;
            cout << "\n\n" << players[0].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[0].my_cards[i].name << " of " << players[0].my_cards[i].suit << "\n";
            }
            cout << "\n\nPlayer: " << players[1].name << "(YOU)\nCount: " << players[1].count << "\nBank: $" << fixed << std::setprecision(2) << players[1].bet;
            cout << "\n\n" << players[1].name << "\'s hand: \n";
            for (int i = 0; i < cards; ++i) {
                cout << players[1].my_cards[i].name << " of " << players[1].my_cards[i].suit << "\n";
            }
        }
        cout << "\n\n";
        if (players[0].bet == 0) {
            cout << "\n\n\tWe're sorry, but the house is bankrupt.";
        }
        while (players[1].bet == 0) {
            cout << "\n\n\tHow much would you like to add to your bank:  $";
            cin >> players[1].bet;
        }
        main_menu(new_deck);
    }
};

int main(int argc, char** argv)
{
    string name;
    float bet = 0.0f;
    Deck newDeck = Deck();
    cout << "\nWhat is your name:  ";
    cin >> name;
    cout << "\nHow much are you betting:  ";
    cin >> bet;
    Blackjack newBlackJack = Blackjack(name, bet);
    newBlackJack.main_menu(newDeck);

    return 0;
}