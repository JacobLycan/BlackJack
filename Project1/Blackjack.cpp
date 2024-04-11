#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include "Random.h"

struct Card {

    enum Rank {
        rank_ace,
        rank_2,
        rank_3,
        rank_4,
        rank_5,
        rank_6,
        rank_7,
        rank_8,
        rank_9,
        rank_10,
        rank_jack,
        rank_queen,
        rank_king,
        max_ranks
    };

    enum Suit {
        suit_heart,
        suit_spade,
        suit_diamond,
        suit_club,
        max_suit
    };
    static constexpr std::array allRanks{ rank_ace, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_jack, rank_queen, rank_king };
    static constexpr std::array allSuits{ suit_club, suit_diamond, suit_heart, suit_spade };
    Rank rank{};
    Suit suit{};
    char card() {

    };
    friend std::ostream& operator<<(std::ostream& out, const Card& card)
    {
        static constexpr std::array ranks{ 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
        static constexpr std::array suits{ 'C', 'D', 'H', 'S' };
        out << ranks[card.rank] << suits[card.suit];
            return out;
    }

    int value() const
    {
        static constexpr std::array rankValues{ 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
        return rankValues[rank];
    }
};

class Deck {
private:
    std::array <Card, 52> m_cards{};
    std::size_t m_nextCardIndex{ 0 };
public:
    Deck() {
        std::size_t count{ 0 };
        for (auto suit : Card::allSuits)
            for (auto rank : Card::allRanks)
                m_cards[count++] = Card{ rank, suit };
    }
    void shuffle(){
         std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
         m_nextCardIndex = 0;
    }

    Card dealCard()
    {
        assert(m_nextCardIndex != 52 && "Deck::dealCard ran out of cards");
        return m_cards[m_nextCardIndex++];
    }
};

struct Player {
    int score{0};
    int numberOfAces{0};
};

namespace Settings {
    int bust{ 21 };
    int dealerStop{ 17 };
   
}


bool playerChoice()
{
    while (true)
    {
        std::cout << "(h) to hit, or (s) to stand: ";

        char ch{};
        std::cin >> ch;

        switch (ch)
        {
        case 'H':
        case 'h':
            return true;
        case 'S':
        case 's':
            return false;
        default:
            std::cout << "Please enter a valid input.";
        }
    }
}
bool playAgain()
{
    while (true)
    {
        std::cout << "Do you want to play again? (Y/N): ";

        char ch{};
        std::cin >> ch;

        switch (ch)
        {
        case 'Y':
        case 'y':
            return true;
        case 'N':
        case 'n':
            return false;
        default:
            std::cout << "Please enter a valid input.";
        }
    }
}

void playBlackJack() {
    Deck deck{};
    deck.shuffle();
    Card dealerCard = deck.dealCard();
    Card playerCard1 = deck.dealCard();
    Card playerCard2 = deck.dealCard();

    Player dealer{ dealerCard.value() };
    Player player{ playerCard1.value() + playerCard2.value() };
    if (dealerCard.value() == 11) {
        dealer.numberOfAces++;
    }
    if (playerCard1.value() == 11) {
        player.numberOfAces++;
    }
    if (playerCard2.value() == 11) {
        player.numberOfAces++;
    }

    std::cout << "The dealer is showing " << dealerCard <<  " (" << dealer.score << ")" << "\n"; 
    std::cout << "You are showing " << playerCard1 << " " << playerCard2 << " (" << player.score << ")" << "\n";
    
    while (player.score <= Settings::bust && playerChoice()) {
        Card flip{ deck.dealCard() };
        if (flip.value() == 11) {
            player.numberOfAces++;
        }
        player.score += flip.value();
        if (player.score > Settings::bust && player.numberOfAces > 0) {
            player.score-= 10;
            player.numberOfAces --;
        }
        std::cout << "You were dealt " << flip << ".   You now have: " <<player.score << "\n";

    }

    if (player.score > Settings::bust) {
        std::cout << "You went bust!\n";
        std::cout << "You Lose!\n";
        return;
    }

    while (dealer.score < Settings::dealerStop) {
        Card flip{ deck.dealCard() };
        if (flip.value() == 11) {
            dealer.numberOfAces++;
        }
        dealer.score += flip.value();
        if (dealer.score > Settings::bust && dealer.numberOfAces > 0) {
            dealer.score-= 10;
            dealer.numberOfAces --;
        }
        std::cout << "The dealer flips a " << flip << ".   They now have: " << dealer.score << "\n";
    }

    if (dealer.score > Settings::bust) {
        std::cout << "The dealer went bust!\n";
        std::cout << "You Win!\n";
    }
    else if (player.score == dealer.score) {
        std::cout << "Push!\n";
    }
    else if (player.score > dealer.score) {
        std::cout << "You win! \n";
    }
    else {
        std::cout << "You lose!\n";
    }

}
int main()
{
    playBlackJack();

    while (playAgain()) {
        playBlackJack();
    }
   


    return 0;
}