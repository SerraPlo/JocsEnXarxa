#include <cstdlib>
#include <ctime>

#include "EntityManager.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"

#define PRINTLN(p) std::cout << (p) << std::endl

int main() {
	EntityManager entityManager;
	entityManager.LoadJson<Item>("items.json");
	entityManager.LoadJson<Weapon>("weapons.json");
	entityManager.LoadJson<Armor>("armors.json");

	std::srand(unsigned(std::time(nullptr)));

	/*Weapon* sawassWeapon = entityManager.getEntity<Weapon>("weapon_sawass");
	std::cout << "Name: " << sawassWeapon->name << std::endl;
	std::cout << "Description: " << sawassWeapon->description << std::endl;
	std::cout << "Damage: " << sawassWeapon->damage << std::endl;*/
	Item* test = entityManager.GetRandEntity<Item>();
	PRINTLN(test->id);

	system("pause");

	return EXIT_SUCCESS;
}
