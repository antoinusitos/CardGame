// CardGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>

enum typeMana
{
	blue,
	black,
	red,
};

char* PrintEnum(typeMana t)
{
	if (t == typeMana::blue)
	{
		return "blue";
	}
	else if (t == typeMana::black)
	{
		return "black";
	}
	else if (t == typeMana::red)
	{
		return "red";
	}
	else
	{
		return "no type";
	}
}

enum typeCard
{
	mana,
	monster,
	blast,
};

struct Mana
{
	int cost;
	typeCard currentTypeCard = typeCard::mana;

	typeMana currentType;

	Mana(typeMana theType, int theCost)
	{
		cost = theCost;
		currentType = theType;
		if (currentType == typeMana::blue)
		{
			cost = 3;
		}
		else if (currentType == typeMana::black)
		{
			cost = 5;
		}
		else if (currentType == typeMana::red)
		{
			cost = 10;
		}
	}
};

struct Monster
{
	int cost;
	typeCard currentTypeCard = typeCard::monster;

	int attack;
	int defense;

	Monster(int theCost, int theAttack, int theDefense)
	{
		cost = theCost;
		attack = theAttack;
		defense = theDefense;
	}
};

struct Blast
{
	int cost;
	typeCard currentTypeCard = typeCard::blast;

	int attack;

	Blast(int theCost, int theAttack)
	{
		cost = theCost;
		attack = theAttack;
	}
};

struct Card
{
	int cost;
	typeCard currentTypeCard;
};

struct Player
{
	int id;
	int pv = 1;
	int mana = 0;
	Card* deck[100] = {};
	Card* hand[7] = {};
	Card* terrain[5] = {};
	int indexDeck = 0;
	int indexTerrain = 0;
	bool canPoseMana = true;

	void(*FlushHand)(Card*[]);
	void (*PrintCard)(Card* theCard);

	Card* mana1;
	Card* mana2;
	Card* mana3;
	Card* monster1;
	Card* monster2;
	Card* blast1;
	Card* blast2;

	Player(int theID)
	{
		id = theID;
		mana1 = (Card*)new Mana(typeMana::blue, 0);
		mana2 = (Card*)new Mana(typeMana::black, 0);
		mana3 = (Card*)new Mana(typeMana::red, 0);

		monster1 = (Card*)new Monster(1, 10, 10);
		monster2 = (Card*)new Monster(3, 30, 5);

		blast1 = (Card*)new Blast(1, 10);
		blast2 = (Card*)new Blast(3, 30);

		for (int i = 0; i < 100; ++i)
		{
			int random = rand() % 100;
			if (random <= 15)
			{
				deck[i] = mana1;
			}
			else if (random <= 30)
			{
				deck[i] = mana2;
			}
			else if (random <= 45)
			{
				deck[i] = monster1;
			}
			else if (random <= 60)
			{
				deck[i] = monster2;
			}
			else if (random <= 75)
			{
				deck[i] = blast1;
			}
			else if (random <= 90)
			{
				deck[i] = blast2;
			}
			else
			{
				deck[i] = mana3;
			}
		}
	}

	void Destroy()
	{
		delete mana1;
		delete mana2;
		delete mana3;
		delete monster1;
		delete monster2;
		delete blast1;
		delete blast2;

		mana1 = nullptr;
		mana2 = nullptr;
		mana3 = nullptr;
		monster1 = nullptr;
		monster2 = nullptr;
		blast1 = nullptr;
		blast2 = nullptr;
	}

	bool Play(int nb, Player* theEnemy)
	{
		if (hand[nb - 1] != nullptr)
		{
			std::cout << "You choose : " << std::endl;
			Card* c = hand[nb - 1];
			PrintCard(c);
			if (c->currentTypeCard == typeCard::mana && canPoseMana)
			{
				mana += c->cost;
				std::cout << "You gain : " << c->cost << " mana" << std::endl;
				canPoseMana = false;
				hand[nb - 1] = nullptr;
				return true;
			}
			else if (c->currentTypeCard == typeCard::mana && canPoseMana == false)
			{
				std::cout << "You already played a mana card" << std::endl;
			}
			else
			{
				if (c->currentTypeCard == typeCard::monster)
				{
					Monster* m = (Monster*)c;
					if (m->cost <= mana && indexTerrain < 5)
					{
						mana -= m->cost;
						std::cout << mana << " mana left" << std::endl;
						std::cout << "You send a monster : attack : " << m->attack << " defense : " << m->defense << std::endl;
						PlaceOnTerrain(hand[nb - 1]);
						hand[nb - 1] = nullptr;
						return true;
					}
					else if (m->cost)
					{
						std::cout << "Not enough mana to send this monster" << std::endl;
						return false;
					}
					else
					{
						std::cout << "Not enough place on terrain" << std::endl;
						return false;
					}
				}
				else if (c->currentTypeCard == typeCard::blast)
				{
					Blast* b = (Blast*)c;
					if (b->cost <= mana)
					{
						mana -= b->cost;
						std::cout << mana << " mana left" << std::endl;

						Player* theEnnemy;

						theEnnemy = theEnemy;

						if (theEnnemy->indexTerrain == 0)
						{
							theEnnemy->pv -= b->attack;
							hand[nb - 1] = nullptr;
							std::cout << "enemy was hit, pv : " << theEnnemy->pv << std::endl;
							return true;
						}
						else
						{

							std::cout << "\nPick an ennemy card between 1 and 5 \n" << std::endl;
							std::cout << "1-";
							PrintCard(theEnnemy->terrain[0]);
							std::cout << "2-";
							PrintCard(theEnnemy->terrain[1]);
							std::cout << "3-";
							PrintCard(theEnnemy->terrain[2]);
							std::cout << "4-";
							PrintCard(theEnnemy->terrain[3]);
							std::cout << "5-";
							PrintCard(theEnnemy->terrain[4]);

							std::cout << "0-Cancel" << std::endl;

							int ennemy = -1;
							while (ennemy == -1)
							{
								std::cin >> ennemy;
								if (ennemy == 0)
								{
									break;
								}
								else if (theEnnemy->terrain[ennemy - 1] == nullptr)
								{
									ennemy = -1;
								}
								else
								{
									Monster* m2 = (Monster*)theEnnemy->terrain[ennemy - 1];
									if (mana >= b->cost)
									{
										m2->defense -= b->attack;
										mana -= b->cost;
										if (m2->defense <= 0)
										{
											std::cout << "Monster was kill ! " << std::endl;
											theEnnemy->terrain[ennemy - 1] = nullptr;
											theEnnemy->indexTerrain--;
										}
										else
											std::cout << "Monster was hit ! defense : " << m2->defense << std::endl;
									}
									else
									{
										std::cout << "Not enough mana to attack" << std::endl;
									}
									system("pause");
								}
							}
							hand[nb - 1] = nullptr;
							return true;
						}
					}
					else
					{
						std::cout << "Not enough mana to use this blast" << std::endl;
						return false;
					}
				}
			}
		}
	}

	void PlaceOnTerrain(Card* theCard)
	{
		for (int i = 0; i < 5; ++i)
		{
			if (terrain[i] == nullptr)
			{
				terrain[i] = theCard;
				indexTerrain++;
				return;
			}
		}
	}
};

Player* p1 = new Player(1);
Player* p2 = new Player(2);

void Flush(Card* theHand[20])
{
	int test = 0;
	int testMax = 100;
	while (test < testMax)
	{
		int r = rand() % 100;
		int r2 = r;
		while (r2 == r)
		{
			r2 = rand() % 100;
		}
		Card* temp = theHand[r];
		theHand[r] = theHand[r2];
		theHand[r2] = temp;
		test++;
	}
}

void PrintCard(Card* theCard)
{
	if (theCard != nullptr)
	{
		if (theCard->currentTypeCard == typeCard::blast)
		{
			Blast* c = (Blast*)theCard;
			std::cout << "Blast : attack : " << c->attack << " cost : " << c->cost << std::endl;
		}
		else if (theCard->currentTypeCard == typeCard::monster)
		{
			Monster* c = (Monster*)theCard;
			std::cout << "Monster : attack : " << c->attack << " defense : " << c->defense << " cost : " << c->cost << std::endl;
		}
		else if (theCard->currentTypeCard == typeCard::mana)
		{
			Mana* c = (Mana*)theCard;
			std::cout << "Mana : give : " << c->cost << " type : " << PrintEnum(c->currentType) << std::endl;
		}
	}
	else
	{
		std::cout << std::endl;
	}
}

void Playerturn(Player* thePlayer, Player* theEnnemy)
{
	/** PLAYER TURN **/

	bool havePick = false;

	for (int i = 0; i < 7; ++i)
	{
		if (thePlayer->hand[i] == nullptr && thePlayer->indexDeck < 100 && havePick == false)
		{
			thePlayer->hand[i] = thePlayer->deck[thePlayer->indexDeck];
			thePlayer->indexDeck++;
			havePick = true;
		}
	}

	bool played = false;

	while (played == false)
	{
		system("cls");
		
		std::cout << "\nPLAYER " << thePlayer->id << " \n" << std::endl;

		std::cout << "PV : " << thePlayer->pv << std::endl;
		std::cout << "Mana : " << thePlayer->mana << std::endl;

		std::cout << "Your hand : \n" << std::endl;

		std::cout << "1-";
		PrintCard(thePlayer->hand[0]);
		std::cout << "2-";
		PrintCard(thePlayer->hand[1]);
		std::cout << "3-";
		PrintCard(thePlayer->hand[2]);
		std::cout << "4-";
		PrintCard(thePlayer->hand[3]);
		std::cout << "5-";
		PrintCard(thePlayer->hand[4]);
		std::cout << "6-";
		PrintCard(thePlayer->hand[5]);
		std::cout << "7-";
		PrintCard(thePlayer->hand[6]);

		std::cout << "\n0-Attack" << std::endl;
		std::cout << "8-Pass" << std::endl;
		std::cout << "9-Discard" << std::endl;

		std::cout << "\nPick a card between 1 and 7 \n" << std::endl;

		int choix = -1;
		while (choix == -1)
		{
			std::cin >> choix;
			if (choix < 0 || choix > 9)
			{
				choix = -1;
			}
		}

		if (choix == 8)
			played = true;
		else if (choix == 9)
		{
			int discarded = -1;
			while (discarded == -1)
			{
				std::cout << "\nPick a card between 1 and 7 to discard or 0 to cancel \n" << std::endl;
				std::cin >> discarded;
				if (discarded == 0)
				{
					break;
				}
				else if (discarded < 1 || discarded > 7)
				{
					discarded = -1;
				}
				else
				{
					thePlayer->hand[discarded - 1] = nullptr;
					played = true;
				}
			}
		}
		else if (choix == 0)
		{
			system("cls");
			std::cout << "\nPick a card between 1 and 5 to attack \n" << std::endl;
			std::cout << "1-";
			PrintCard(thePlayer->terrain[0]);
			std::cout << "2-";
			PrintCard(thePlayer->terrain[1]);
			std::cout << "3-";
			PrintCard(thePlayer->terrain[2]);
			std::cout << "4-";
			PrintCard(thePlayer->terrain[3]);
			std::cout << "5-";
			PrintCard(thePlayer->terrain[4]);

			std::cout << "0-Cancel" << std::endl;

			int take = -1;
			while (take == -1)
			{
				std::cin >> take;
				if (take == 0)
				{
					played = false;
					break;
				}
				else if (take < 1 || take > 5)
				{
					take = -1;
				}
				else if (thePlayer->terrain[take - 1] == nullptr)
				{
					take = -1;
				}
				else if (thePlayer->terrain[take - 1] != nullptr)
				{
					if (theEnnemy->indexTerrain == 0)
					{
						Monster* m = (Monster*)p1->terrain[take - 1];
						if (thePlayer->mana >= m->cost)
						{
							theEnnemy->pv -= m->attack;
							thePlayer->mana -= m->cost;
							std::cout << "Enemy was hit ! pv : " << theEnnemy->pv << std::endl;
						}
						else
						{
							std::cout << "Not enough mana to attack" << std::endl;
						}
						system("pause");
					}
					else
					{
						std::cout << "\nPick an ennemy card between 1 and 5 \n" << std::endl;
						std::cout << "1-";
						PrintCard(theEnnemy->terrain[0]);
						std::cout << "2-";
						PrintCard(theEnnemy->terrain[1]);
						std::cout << "3-";
						PrintCard(theEnnemy->terrain[2]);
						std::cout << "4-";
						PrintCard(theEnnemy->terrain[3]);
						std::cout << "5-";
						PrintCard(theEnnemy->terrain[4]);

						std::cout << "0-Cancel" << std::endl;

						int ennemy = -1;
						while (ennemy == -1)
						{
							std::cin >> ennemy;
							if (ennemy == 0)
							{
								break;
							}
							else if (theEnnemy->terrain[ennemy - 1] == nullptr)
							{
								ennemy = -1;
							}
							else
							{
								Monster* m2 = (Monster*)theEnnemy->terrain[ennemy - 1];
								Monster* m1 = (Monster*)thePlayer->terrain[take - 1];
								if (thePlayer->mana >= m1->cost)
								{
									m2->defense -= m1->attack;
									thePlayer->mana -= m1->cost;
									if (m2->defense <= 0)
									{
										std::cout << "Monster was kill ! " << std::endl;
										theEnnemy->terrain[ennemy - 1] = nullptr;
										theEnnemy->indexTerrain--;
									}
									else
										std::cout << "Monster was hit ! defense : " << m2->defense << std::endl;
								}
								else
								{
									std::cout << "Not enough mana to attack" << std::endl;
								}
								system("pause");
							}
						}
					}
				}
			}
			played = false;
		}
		else
		{
			thePlayer->Play(choix, theEnnemy);
			system("pause");
		}
	}

	/** PLAYER ENDTURN **/
}

void InitPlayers()
{
	p1->FlushHand = Flush;
	p1->FlushHand(p1->deck);
	p1->PrintCard = PrintCard;

	p2->FlushHand = Flush;
	p2->FlushHand(p2->deck);
	p2->PrintCard = PrintCard;

	for (int i = 0; i < 7; ++i)
	{
		if (p1->hand[i] == nullptr && p1->indexDeck < 20)
		{
			p1->hand[i] = p1->deck[p1->indexDeck];
			p1->indexDeck++;
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		p1->terrain[i] = nullptr;
	}

	for (int i = 0; i < 7; ++i)
	{
		if (p2->hand[i] == nullptr && p2->indexDeck < 20)
		{
			p2->hand[i] = p2->deck[p2->indexDeck];
			p2->indexDeck++;
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		p2->terrain[i] = nullptr;
	}
}

void Destroyer()
{
	p1->Destroy();
	p2->Destroy();

	delete p1;
	delete p2;
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	bool inGame = true;

	InitPlayers();

	int winner = 0;

	while (inGame)
	{
		system("cls");

		p1->canPoseMana = true;
		p2->canPoseMana = true;
		
		Playerturn(p1, p2);

		if (p2->pv <= 0)
		{
			inGame = false;
			winner = 1;
			break;
		}

		std::cout << "\n " << std::endl;

		Playerturn(p2, p1);

		if (p1->pv <= 0)
		{
			inGame = false;
			winner = 2;
			break;
		}
	}

	std::cout << "Player " << winner << " has win the game" << std::endl;

	Destroyer();

	system("pause");

	return 0;
}