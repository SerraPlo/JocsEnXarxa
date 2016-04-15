#include <cstdlib>
#include <ctime>

#include "EntityManager.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"

int main() {
	EntityManager entityManager;
	entityManager.loadJson<Item>("items.json");
	entityManager.loadJson<Weapon>("weapons.json");
	entityManager.loadJson<Armor>("armors.json");

	std::srand(unsigned(std::time(nullptr)));

	Weapon* sawassWeapon = entityManager.getEntity<Weapon>("weapon_sawass");
	std::cout << "Name: " << sawassWeapon->name << std::endl;
	std::cout << "Description: " << sawassWeapon->description << std::endl;
	std::cout << "Damage: " << sawassWeapon->damage << std::endl;

	system("pause");

	return EXIT_SUCCESS;
}
