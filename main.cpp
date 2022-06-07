#include <iostream>
#include <vector>
#include <time.h>
#include <conio.h>
#include <functional>

// я думал разделить это все по заголовочным файлам, но подумал, что тогда вам будет тяжелее проверять ахпха
// когда тут пояивлось уже 350 строк я понял, что лучше бы разделил..
// я решил не делать ввод урона пользователем, а разные классы 

// хотелось сделать больше разных классов, но времени нема

// базовые классы
class Unit;

class Dragon 
{
	protected:
		int damage;
		int health;

	public:
		Dragon(int damage, int health);
		~Dragon();

		virtual void attack(Unit* unit) = 0;
		void receiveDamage(int damage);

		virtual void obility(Unit* unit) = 0;

		int getDamage();
		int getHealth();
};

Dragon::Dragon(int damage, int health)
	: damage(damage), health(health)
{

}

Dragon::~Dragon()
{

}

void Dragon::receiveDamage(int daamge)
{
	this->health -= damage;

	if (this->health < 0)
	{
		this->health = 0;
	}
}

int Dragon::getDamage() // вообще 2 одинаковые функции в разных классах которые работают одинаково, не особо приятно
{
	return this->damage;
}

int Dragon::getHealth()
{
	return this->health;
}


class Unit 
{
	protected:
		int damage;
		int health;
		int maxHealth;

		int timeFire;
		int fireDamage;
		bool inFire;

		int timeFrozen;
		bool frozened;

	public:
		Unit(int damage, int health);
		~Unit();

		virtual void attack(Dragon* dragon) = 0;
		virtual void heal(int health);
		virtual void receiveDamage(int damage) = 0; // будем переопределять потому что разные классы по разному получают урон

		virtual void catchFire(int time, int fireDamage) = 0;
		virtual void frozen(int time) = 0;

		int getDamage();
		int getHealth();
		bool getFire();
		int getFireTime();
};

Unit::Unit(int damage, int health)
	: damage(damage), health(health), maxHealth(health)
{

}

Unit::~Unit()
{

}

void Unit::heal(int health)
{
	this->health += health;

	if (this->health > this->maxHealth)
	{
		this->health = this->maxHealth;
	}
}

int Unit::getDamage()
{
	return this->damage;
}

int Unit::getHealth()
{
	return this->health;
}

bool Unit::getFire()
{
	return this->inFire;
}

int Unit::getFireTime()
{
	return this->timeFire;
}


// солдаты
class SwordUnit : public Unit
{
	public:
		SwordUnit(int damage, int health);
		~SwordUnit();

		void attack(Dragon* dragon);
		void receiveDamage(int damage);

		void catchFire(int time, int fireDamage);
		void frozen(int time);

		void update();
};

SwordUnit::SwordUnit(int damage, int health)
	: Unit(damage, health)
{
	this->timeFire = 0;
	this->inFire = false;
}

void SwordUnit::attack(Dragon* dragon)
{
	dragon->receiveDamage(this->damage);
}

void SwordUnit::receiveDamage(int damage)
{
	this->health -= damage;

	if (this->health < 0)
	{
		this->health = 0;
	}
}

void SwordUnit::catchFire(int time, int fireDamage)
{
	this->inFire = true;
	this->timeFire = time;
	this->fireDamage = fireDamage;
}

void SwordUnit::frozen(int time)
{

}

void SwordUnit::update()
{
	if (this->inFire)
	{
		this->timeFire--;
		this->receiveDamage(this->fireDamage);

		if (this->timeFire == 0)
		{
			this->inFire = false;
		}
	}
}


class Wizard : public Unit 
{
	private:
		int selfRegen;
		int timeRegen;

	public:
		Wizard(int damage, int health, int regen);
		~Wizard();

		void attack(Dragon* dragon);
		void heal(int health);
		void receiveDamage(int damage);

		void catchFire(int time, int fireDamage);
		void frozen(int time);

		void update();

		int getSelfRegen();
		int getOtherRegen();
		int getTimeRegen();
};

Wizard::Wizard(int damage, int health, int regen)
	: Unit(damage, health)
{
	this->selfRegen = regen;
	this->timeFire = 0;
	this->inFire = false;
	this->timeRegen = 0;
}

Wizard::~Wizard()
{

}

void Wizard::attack(Dragon* dragon)
{
	if (this->frozened)
	{
		dragon->receiveDamage(this->damage);
	}
}

void Wizard::heal(int health)
{
	if (this->timeFire == 0)
	{
		this->Unit::heal(health);
		this->timeRegen = 3;
	}
}

void Wizard::receiveDamage(int damage)
{
	this->health -= damage;

	if (this->health < 0)
	{
		this->health = 0;
	}
} 

void Wizard::catchFire(int time, int fireDamage) // в 2 раза меньше урона от огня
{
	this->timeFire = time;
	this->fireDamage = fireDamage / 2;
}

void Wizard::frozen(int time)
{
	this->timeFire = time / 2;
	this->frozened = true;
}

void Wizard::update()
{
	if (this->inFire)
	{
		this->timeFire--;
		this->receiveDamage(this->fireDamage);

		if (this->timeFire == 0)
		{
			this->inFire = false;
		}
	}

	if (this->timeRegen > 0)
	{
		this->timeRegen--;
	}

	if (this->frozened)
	{
		this->timeFrozen--;

		if (this->timeFrozen == 0)
		{
			this->frozened = false;
		}
	}
}

int Wizard::getSelfRegen()
{
	return this->selfRegen;
}

int Wizard::getOtherRegen()
{
	return this->selfRegen * 2;
}

int Wizard::getTimeRegen()
{
	return this->timeRegen;
}


// Драконы
class DragonFire : public Dragon
{
	private:
		int fireDamage;

	public:
		DragonFire(int damage, int health, int fireDamage);
		~DragonFire();

		void attack(Unit* unit);

		void obility(Unit* unit); // поджигает огнем
};

DragonFire::DragonFire(int damage, int health, int fireDamage)
	: Dragon(damage, health)
{
	this->fireDamage = fireDamage;
}

DragonFire::~DragonFire()
{

}

void DragonFire::attack(Unit* unit)
{
	unit->receiveDamage(this->damage);
	unit->catchFire(2, 2);
}

void DragonFire::obility(Unit* unit)
{
	unit->catchFire(2, this->fireDamage);
}

class DragonIce : public Dragon
{
	private:
		int timeFrozen;

	public:
		DragonIce(int damage, int health, int timeFrozen);
		~DragonIce();

		void attack(Unit* unit);

		void obility(Unit* unit); // замораживает
};

DragonIce::DragonIce(int damage, int health, int timeFrozen)
	: Dragon(damage, health), timeFrozen(timeFrozen)
{

}

void DragonIce::attack(Unit* unit)
{
	unit->receiveDamage(this->damage);

}

void DragonIce::obility(Unit* unit)
{
	unit->frozen(this->timeFrozen);
}

// Ввод данных пользователем
void init(int& countSword, int& countWizard)
{
	std::cout << "Enter count of sword units: ";
	std::cin >> countSword;

	std::cout << "Enter count of wizard units: ";
	std::cin >> countWizard;
}

template <typename Units>
// Вывод таблицы с войсками
void printTable(std::vector<Units*>& units, std::string title, int attacked = -1)
{
	std::cout << "\n\t\t" << title << " (" << units.size() << ")";

	for (int i = 0; i < units.size(); i++)
	{
		std::cout << "\n" << i + 1 << ". " << title << "[health = " << units[i]->getHealth() << "], [damage = " << units[i]->getDamage() << "]";

		if (units[i]->getHealth() == 0)
		{
			std::cout << " *DIED*";
		}

		else 
		{	
			if (units[i]->getFire())
			{
				std::cout << " | in fire! (" << units[i]->getFireTime() << ")";
			}

			if (attacked == i)
			{
				std::cout << " | attacked!";
			}
		}
	}
}

template <typename Units>
bool isAlive(std::vector<Units*>& units)
{
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i]->getHealth() != 0)
		{
			return true;
		}
	}

	return false;
}

template <typename Units>
void filterAlive(std::vector<Units*>& units)
{
	for (int j = 0; j < units.size(); j++)
	{
		if (units[j]->getHealth() == 0)
		{
			units.erase(units.begin() + j);
		}
	}
}

template <typename Units>
void obilityDragon(std::vector<Units*>& units, Dragon* dragon)
{
	int countOfCatch = rand() % (units.size() - 0) - 0;

	for (int i = 0; i <= countOfCatch; i++)
	{
		dragon->obility(units[rand() % (units.size() - 0) - 0]);
	}
}

int main()
{
	srand(time(0));
	rand(); // фиктивный вызов 

	system("cls");

	int countSword;
	int countWizard;

	init(countSword, countWizard);

	system("cls");

	std::vector<SwordUnit*> swordUnits;
	std::vector<Wizard*> wizards;

	int typeDragon = rand() % (1 - 0 + 1) - 0;
	Dragon* dragon;

	switch (typeDragon)
	{
		case 0:
			std::cout << "Type of dragon - fire!!\n\n";
			dragon = new DragonFire(15, 1000, 5);
			break;

		case 1:
			std::cout << "Type of dragon - ice!\n\n";
			dragon = new DragonIce(10, 1500, 2);
			break;
	}


	for (int i = 0; i < countSword; i++)
	{
		swordUnits.push_back(new SwordUnit(5, 50));
	}

	for (int i = 0; i < countWizard; i++)
	{
		wizards.push_back(new Wizard(20, 10, 2));
	}

	printTable(swordUnits, "Sword unit");
	std::cout << "\n\n";
	printTable(wizards, "Wizard unit");

	std::cout << "\n\nPress enter to start..";
	_getch();

	while ((isAlive(swordUnits) || isAlive(wizards)) && dragon->getHealth() != 0)
	{
		system("cls");

		// атака юнитов
		int choice;

		std::cout << "Your turn!\n\n";

		std::cout << "\n\nEnter choice of unit (1 - swords, 2 - wizards): ";
		std::cin >> choice;

		system("cls");

		if (choice == 1)
		{
			for (int i = 0; i < swordUnits.size(); i++)
			{
				swordUnits[i]->attack(dragon);
			}
		}

		else if (choice == 2)
		{
			int op;

			std::cout << "Enter op (1 - attack, 2 - heal): ";
			std::cin >> op;

			if (op == 1)
			{ 
				for (int i = 0; i < wizards.size(); i++)
				{
					wizards[i]->attack(dragon);
				}
			}

			else if (op == 2)
			{
				int whoHeal;

				while (true) 
				{
					std::cout << "Heal 1 - swords, 2 - self ";
					std::cin >> whoHeal;

					if (wizards[0]->getTimeRegen() == 0)
					{
						if (whoHeal == 1)
						{
							for (int i = 0; i < swordUnits.size(); i++)
							{
								swordUnits[i]->heal(wizards[0]->getOtherRegen());
							}

							break;
						}

						else if (whoHeal == 2)
						{
							for (int i = 0; i < wizards.size(); i++)
							{
								wizards[i]->heal(wizards[0]->getSelfRegen());
							}

							break;
						}
					}

					else
					{
						std::cout << "For regen you have to wait " << wizards[0]->getTimeRegen();
					}
				}
			}
		}

		// атака дракона
		int typeOfUnit = rand() % (2 - 0) - 0;

		int size;
		int unit;

		if (typeOfUnit == 0)
		{
			unit = rand() % (swordUnits.size() - 0) - 0;
			dragon->attack(swordUnits[unit]);
		
			obilityDragon(swordUnits, dragon);
		}

		else if (typeOfUnit == 1)
		{
			unit = rand() % (wizards.size() - 0) - 0;
			dragon->attack(wizards[unit]);
		
			obilityDragon(wizards, dragon);
		}

		// результаты
		for (int j = 0; j < swordUnits.size(); j++)
		{
			swordUnits[j]->update();
		}

		for (int j = 0; j < wizards.size(); j++)
		{
			wizards[j]->update();
		}

		std::cout << "Result of the turn: \n\n";

		printTable(swordUnits, "Sword units", (typeOfUnit == 0 ? unit : -1));
		std::cout << "\n\n";
		printTable(wizards, "Wizard units", (typeOfUnit == 1 ? unit : -1));

		std::cout << "\n\n Dragon health: " << dragon->getHealth();

		_getch();
		system("cls");

		filterAlive(swordUnits);
		filterAlive(wizards);
	}

	if (dragon->getHealth() != 0)
	{
		std::cout << "GAME LOSTED!\n\n";
	}

	else 
	{
		std::cout << "YOU WIN! There is your statistic: \n\n";

		printTable(swordUnits, "Sword units");
		std::cout << "\n\n";
		printTable(wizards, "Wizard units");
	}

	_getch();
}