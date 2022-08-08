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

TEST_CASE("UpdateQuality_SingleItem") {

    vector<Item> items;
    Item testA("foo", 1, 1);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();

    AssertItemUpdated(testA, items.front());
}

TEST_CASE("UpdateQuality_SingleItem_QualityLossTwice_AfterSelltime") {

    vector<Item> items;
    Item testA("foo", 0, 10);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 8);
    app.updateQuality();
    REQUIRE(items.front().quality == 6);
}

TEST_CASE("UpdateQuality_SingleItem_UpdateZeroQuality") {

    vector<Item> items;
    Item testA("foo", 0, 0);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();

    REQUIRE(items.front().quality >= 0);
}

TEST_CASE("UpdateQuality_AgedBree_IncreaseInQuality") {

    vector<Item> items;
    Item testA("Aged Brie", 10, 10);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 11);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);
}

TEST_CASE("UpdateQuality_AgedBree_IncreaseInQuality_NotGreaterThan50") {

    vector<Item> items;
    Item testA("Aged Brie", 10, 50);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 50);
}

TEST_CASE("UpdateQuality_Sulfuras_NeverChangesValue") {

    vector<Item> items;
    Item sulfuras("Sulfuras, Hand of Ragnaros", 1, 50);
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
    Item sulfuras("Sulfuras, Hand of Ragnaros", 1, 50);
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
    Item testA("Backstage passes to a TAFKAL80ETC concert", 20, 10);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 11);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality_10dayOrLess") {

    vector<Item> items;
    Item testA("Backstage passes to a TAFKAL80ETC concert", 10, 10);
    items.push_back(testA);

    GildedRose app(items);

    app.updateQuality();
    REQUIRE(items.front().quality == 12);

    app.updateQuality();
    REQUIRE(items.front().quality == 14);
}

TEST_CASE("UpdateQuality_BackstagePass_IncreaseInQuality_5dayOrLess") {

    vector<Item> items;
    Item testA("Backstage passes to a TAFKAL80ETC concert", 5, 10);
    items.push_back(testA);

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
    Item testA("Backstage passes to a TAFKAL80ETC concert", 3, 10);
    items.push_back(testA);

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