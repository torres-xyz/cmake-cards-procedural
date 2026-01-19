#include <catch2/catch_test_macros.hpp>
#include "../src/card.hpp"
#include <algorithm>

TEST_CASE("All Cards In Database have unique IDs")
{
    std::vector<Card> cardDbSUT = GetCardDB();

    auto greaterID = [](const Card &a, const Card &b) -> auto {
        return a.cardID < b.cardID;
    };

    auto sameID = [](const Card &a, const Card &b) -> auto {
        return a.cardID == b.cardID;
    };

    // Sort vector in descending order
    std::ranges::sort(cardDbSUT.begin(), cardDbSUT.end(), greaterID);

    REQUIRE(std::ranges::adjacent_find(cardDbSUT.begin(), cardDbSUT.end(), sameID) == cardDbSUT.end());
}

TEST_CASE("All cards from GetCardDB have unique uids")
{
    std::vector<Card> cardDbSUT = GetCardDB();

    auto greaterUid = [](const Card &a, const Card &b) -> auto {
        return a.uid < b.uid;
    };

    auto sameUid = [](const Card &a, const Card &b) -> auto {
        return a.uid == b.uid;
    };

    // Sort vector in descending order
    std::ranges::sort(cardDbSUT.begin(), cardDbSUT.end(), greaterUid);

    REQUIRE(std::ranges::adjacent_find(cardDbSUT.begin(), cardDbSUT.end(), sameUid) == cardDbSUT.end());
}


TEST_CASE("All cards created by GetCardFromDB have unique uids")
{
    //Get all card ids available
    std::vector<int> allCardIDs;
    std::vector<Card> cardDb = GetCardDB();
    for (const Card &card: cardDb)
    {
        allCardIDs.push_back(card.cardID);
    }

    std::vector<Card> allCardsSUT;
    for (const int id: allCardIDs)
    {
        allCardsSUT.push_back(GetCardFromDB(id));
    }

    auto greaterUid = [](const Card &a, const Card &b) -> auto {
        return a.uid < b.uid;
    };
    // Sort allCardsSUT in descending order of uid
    std::ranges::sort(allCardsSUT.begin(), allCardsSUT.end(), greaterUid);

    auto sameUid = [](const Card &a, const Card &b) -> auto {
        return a.uid == b.uid;
    };
    REQUIRE(std::ranges::adjacent_find(allCardsSUT.begin(), allCardsSUT.end(), sameUid) == allCardsSUT.end());
}
