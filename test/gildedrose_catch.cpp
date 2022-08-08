#include <catch2/catch.hpp>
#include "ApprovalTests.hpp"

#include "GildedRose.h"
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Item& obj)
{
    return os
            << "name: " << obj.name
            << ", sellIn: " << obj.sellIn
            << ", quality: " << obj.quality;
}

void AssertItemUpdated(const Item& beforeUpdate, const Item& toTest)
{
    REQUIRE(toTest.sellIn == (beforeUpdate.sellIn-1));
    REQUIRE(toTest.quality == (beforeUpdate.quality-1));
}

const std::string genericItemName = "foo";
const std::string backstagePassName = "Backstage passes to a TAFKAL80ETC concert";
const std::string sulfurasName = "Sulfuras, Hand of Ragnaros";
const std::string agedBreeName = "Aged Brie";

TEST_CASE("UpdateQuality_SingleItem") {

    vector<Item> items;
    Item genericItem(genericItemName, 1, 1);
    items.push_back(genericItem);
    GildedRose app(items);

    SECTION("Updating item once"){

        app.updateQuality();
        AssertItemUpdated(genericItem, items.front());
    }

}
TEST_CASE("UpdateQuality_SingleItem_QualityLossTwice_AfterSelltime") {

    vector<Item> items;
    Item genericItem(genericItemName, 0, 10);
    items.push_back(genericItem);

    GildedRose app(items);

    SECTION("Updating item once"){

        app.updateQuality();
        const auto updatedItem = items.front();
        REQUIRE(updatedItem.sellIn == -1);
        REQUIRE(updatedItem.quality == 8);

            SECTION("Updating item again"){

                app.updateQuality();
                const auto updatedItem = items.front();
                REQUIRE(updatedItem.sellIn == -2);
                REQUIRE(updatedItem.quality == 6);
            }
    }
}

TEST_CASE("UpdateQuality_SingleItem_UpdateZeroQuality") {

    vector<Item> items;
    Item genericItem(genericItemName, 0, 0);
    items.push_back(genericItem);

    GildedRose app(items);

    app.updateQuality();

    const auto updatedItem = items.front();
    REQUIRE(updatedItem.sellIn == -1);
    REQUIRE(updatedItem.quality == 0);
}

TEST_CASE("UpdateQuality_AgedBree_IncreaseInQuality") {

    vector<Item> items;
    Item agedBree(agedBreeName, 10, 10);
    items.push_back(agedBree);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 11);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);
}

TEST_CASE("UpdateQuality_AgedBree_IncreaseInQuality_NotGreaterThan50") {

    vector<Item> items;
    Item agedBree(agedBreeName, 10, 50);
    items.push_back(agedBree);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 50);
}

TEST_CASE("UpdateQuality_Sulfuras_NeverChangesValue") {

    vector<Item> items;
    Item sulfuras(sulfurasName, 1, 50);
    items.push_back(sulfuras);

    GildedRose app(items);

    for(int i=0; i < 10; i++)
    {
        app.updateQuality();
    }
    REQUIRE(items.front().quality > 0);
}

TEST_CASE("UpdateQuality_Sulfuras_NeverChangesSelldate") {

    vector<Item> items;
    Item sulfuras(sulfurasName, 1, 50);
    items.push_back(sulfuras);

    GildedRose app(items);

    for(int i=0; i < 10; i++)
    {
        app.updateQuality();
    }
    REQUIRE(items.front().sellIn == sulfuras.sellIn);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality") {

    vector<Item> items;
    Item backstagePass(backstagePassName, 20, 10);
    items.push_back(backstagePass);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 11);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality_10dayOrLess") {

    vector<Item> items;
    Item backstagePass(backstagePassName, 10, 10);
    items.push_back(backstagePass);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);

    app.updateQuality();
    REQUIRE(items.front().quality == 14);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality_5dayOrLess") {

    vector<Item> items;
    Item backstagePass(backstagePassName, 5, 10);
    items.push_back(backstagePass);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 13);

    app.updateQuality();
    REQUIRE(items.front().quality == 16);
        app.updateQuality();
    REQUIRE(items.front().quality == 19);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality_ZeroValueAtExpiration") {

    vector<Item> items;
    Item backstagePass(backstagePassName, 3, 10);
    items.push_back(backstagePass);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 13);

    app.updateQuality();
    REQUIRE(items.front().quality == 16);

    app.updateQuality();
    REQUIRE(items.front().quality == 19);

    app.updateQuality();
    REQUIRE(items.front().quality == 0);
}

TEST_CASE("ItemsInSystem_QualityClampedAt50_Assumption", "[.]") {

    vector<Item> items;
    Item testA("foo", 10, 100);
    items.push_back(testA);

    GildedRose app(items);
    app.updateQuality();
    REQUIRE(items.front().quality == 50);
}