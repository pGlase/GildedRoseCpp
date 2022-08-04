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

TEST_CASE("ItemsInSystem_QualityClampedAt50_Assumption", "[.]") {

    vector<Item> items;
    Item testA("foo", 10, 100);
    items.push_back(testA);

    GildedRose app(items);
    app.updateQuality();
    REQUIRE(items.front().quality == 50);
}