#pragma once
#include <list>
#include <utility>
#include "EntityManager.hpp"

class Item;
class Weapon;
class Armor;
class Inventory {
	std::list<std::pair<Item*, int>> items; // Item and quantity

	template <typename T>
	void Load(JsonBox::Value& v, EntityManager* mgr) { // Load Ts into storage list
		for (auto item : v.getArray()) {
			std::string itemId = item.getArray()[0].getString();
			int quantity = item.getArray()[1].getInteger();
			this->items.push_back(std::make_pair(mgr->GetEntity<T>(itemId), quantity));
		}
	}

	template <typename T>
	JsonBox::Array JsonArray(); // Return JSON representation of all the items of the type T

public:
	Inventory(JsonBox::Value& v, EntityManager* mgr); // Load the inventory from a JSON value
	Inventory() = default;

	void Add(Item* item, int count);	// Add an item to the inventory
	void Remove(Item* item, int count); // Remove the specified number of items from the inventory
	int Count(Item* item);
	template <typename T>
	int Count(unsigned int n);			// Return the count of the specified item
	template <typename T>
	T* Get(unsigned int n);				// Return the nth item in the storage list
	template <typename T>
	int Print(bool label = false);		// Output a list of the T items onto stdout
	void Clear();						// Remove all items from the inventory
	void Merge(Inventory* inventory);	// Merge the specified inventory with the current one
	int Print(bool label = false);		// Print the entire inventory
	JsonBox::Object GetJson();			// Get a Json object representation of the inventory
};


