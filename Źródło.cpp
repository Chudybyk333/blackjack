#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include "Nag³ówek.h"
#include "Nag³ówek1.h"
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
using namespace sf;
using namespace std;

vector<deck> karta1(52);//g??wny vector uzupe?niany deckiem kart
int balance = 5000;
int bet1 = 0;//zmienne do pomocy w betach przy splitcie
int bet2 = 0;
//split
bool splitmode = false, secondhanduse = false, splitexecuted = false, splitallowed = false;
//splithit
bool bust = false;
//zmienne do obs?ugi faz gry
bool betPlaced = false, betBarEnable = true, first = true, gameFinish = false, gameFinish2 = true, ryj = false;
//pomoc double down
bool hitused = false;
int value_gracz, value_gracz2, value_dealer;//do przechowywania warto?ci kart
int position = 260;//pozycja do rysowania kart
int idealer = 0;//licznik kart dealera
int igracz = 0;//licznik kart gracza
int igracz2 = 0;//potrzebne do splita
vector<deck> karty_dealer(10);//do przechowywania kart przyznanych dealerowi
vector<deck> karty_gracz(10);//do przechowywania kart gracza
vector<deck> karty_gracz2(10);//potrzebne do splita
textBox bet;
void blackjack()
{
	balance += 6 * bet.betValue;
	ryj = true;
	gameFinish = true;
}
void kopiakarty(deck& kopia, deck& z)
{
	kopia.wartosc = z.wartosc;
	kopia.kolor = z.kolor;
	kopia.path = z.path;
	kopia.texture.loadFromFile(z.path);
	kopia.sprite.setTexture(kopia.texture);
}
int wartosc_d(vector <deck> dealer)
{
	int wartosc = 0;
	int ace = 0;
	for (int i = 0; i < dealer.size(); i++)
	{
		if (dealer[i].wartosc == 11)
		{
			ace++;
		}
		wartosc += dealer[i].wartosc;
	}
	while (wartosc > 21 && ace > 0)
	{
		wartosc -= 10;
		ace--;
	}
	return wartosc;
}
int wartosc_g(vector <deck> gracz)
{
	int wartosc = 0;
	int ace = 0;
	for (int i = 0; i < gracz.size(); i++)
	{
		if (gracz[i].wartosc == 11)
		{
			ace++;
		}
		wartosc += gracz[i].wartosc;
	}
	while (wartosc > 21 && ace > 0)
	{
		wartosc -= 10;
		ace--;
	}
	return wartosc;
}
void Hit(vector <deck>& karty_gracz, vector <deck>& karta1, int& igracz)
{
	hitused = true;
	igracz++;
	int a = rand() % karta1.size();
	kopiakarty(karty_gracz[igracz], karta1[a]);
	karta1.erase(karta1.begin() + a);
	value_gracz = wartosc_g(karty_gracz);
	if (value_gracz > 21)
	{
		if (splitexecuted == false)
		{
			ryj = true;
			gameFinish = true;
		}
		else
		{
			bet1 = bet.betValue;
			gameFinish = true;
			secondhanduse = true;
			hitused = false;
		}
	}
}
void Stand(vector <deck>& karty_gracz, vector <deck>& karty_dealer, vector <deck>& karta1, int& idealer)
{
	if (splitexecuted == false)
	{
		ryj = true;
		value_dealer = wartosc_d(karty_dealer);
		while (value_dealer < 17)
		{
			idealer++;
			int a = rand() % karta1.size();
			kopiakarty(karty_dealer[idealer], karta1[a]);
			karta1.erase(karta1.begin() + a);
			value_dealer = wartosc_d(karty_dealer);
		}
		value_gracz = wartosc_g(karty_gracz);
		if (value_gracz > 21 || value_dealer > value_gracz && value_dealer <= 21)
		{
			gameFinish = true;
		}
		else if (value_gracz > value_dealer && value_gracz <= 21 || value_dealer > 21)
		{
			balance += 2 * bet.betValue;
			gameFinish = true;
		}
		else if (value_gracz == value_dealer)
		{
			balance += bet.betValue;
			gameFinish = true;
		}
	}
	if (splitexecuted == true)
	{
		bet1 = bet.betValue;
		gameFinish = true;
	}
}
void Doubledown(vector <deck>& karty_gracz, vector <deck>& karty_dealer, vector <deck>& karta1, int& igracz, int& idealer)
{
	if (splitexecuted == false)
	{
		ryj = true;
		balance -= bet.betValue;
		bet.betValue *= 2;
		igracz++;
		int dd = rand() % karta1.size();
		kopiakarty(karty_gracz[igracz], karta1[dd]);
		karta1.erase(karta1.begin() + dd);
		value_gracz = wartosc_g(karty_gracz);
		value_dealer = wartosc_d(karty_dealer);
		while (value_dealer < 17)
		{
			idealer++;
			int d = rand() % karta1.size();
			kopiakarty(karty_dealer[idealer], karta1[d]);
			karta1.erase(karta1.begin() + d);
			value_dealer = wartosc_d(karty_dealer);
		}
		if (value_gracz > 21 || value_dealer > value_gracz && value_dealer <= 21)
		{
			gameFinish = true;
		}
		else if (value_gracz > value_dealer && value_gracz <= 21 || value_dealer > 21)
		{
			balance += 2 * bet.betValue;
			gameFinish = true;
		}
		else if (value_gracz == value_dealer)
		{
			balance += bet.betValue;
			gameFinish = true;
		}
	}
	else
	{
		balance -= bet.betValue;
		bet.betValue *= 2;
		bet1 = bet.betValue;
		bet.betValue /= 2;
		igracz++;
		int dd = rand() % karta1.size();
		kopiakarty(karty_gracz[igracz], karta1[dd]);
		karta1.erase(karta1.begin() + dd);
		gameFinish = true;
		secondhanduse = true;
	}
}
void kartydodawane()
{
	for (int i = 0; i < 13; i++)//hearts
	{
		if (i + 1 >= 2 && i + 1 <= 10)
		{
			karta1[i].wartosc = i + 1;
			karta1[i].kolor = "hearts";
			karta1[i].path = "images/" + to_string(karta1[i].wartosc) + "_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i + 1 == 1)
		{
			karta1[i].wartosc = 11;
			karta1[i].kolor = "hearts";
			karta1[i].path = "images/ace_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i + 1 == 11)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "hearts";
			karta1[i].path = "images/jack_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i + 1 == 12)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "hearts";
			karta1[i].path = "images/queen_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i + 1 == 13)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "hearts";
			karta1[i].path = "images/king_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
	}
	for (int i = 13; i < 26; i++)//diamonds
	{
		if (i - 13 + 1 >= 2 && i - 13 + 1 <= 10)
		{
			karta1[i].wartosc = i - 13 + 1;
			karta1[i].kolor = "diamonds";
			karta1[i].path = "images/" + to_string(karta1[i].wartosc) + "_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 13 + 1 == 1)
		{
			karta1[i].wartosc = 11;
			karta1[i].kolor = "diamonds";
			karta1[i].path = "images/ace_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 13 + 1 == 11)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "diamonds";
			karta1[i].path = "images/jack_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 13 + 1 == 12)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "diamonds";
			karta1[i].path = "images/queen_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 13 + 1 == 13)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "diamonds";
			karta1[i].path = "images/king_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}

	}
	for (int i = 26; i < 39; i++)//spades
	{
		if (i - 26 + 1 >= 2 && i - 26 + 1 <= 10)
		{
			karta1[i].wartosc = i - 26 + 1;
			karta1[i].kolor = "spades";
			karta1[i].path = "images/" + to_string(karta1[i].wartosc) + "_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 26 + 1 == 1)
		{
			karta1[i].wartosc = 11;
			karta1[i].kolor = "spades";
			karta1[i].path = "images/ace_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 26 + 1 == 11)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "spades";
			karta1[i].path = "images/jack_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 26 + 1 == 12)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "spades";
			karta1[i].path = "images/queen_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 26 + 1 == 13)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "spades";
			karta1[i].path = "images/king_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
	}
	for (int i = 39; i < 52; i++)//clubs
	{
		if (i - 39 + 1 >= 2 && i - 39 + 1 <= 10)
		{
			karta1[i].wartosc = i - 39 + 1;
			karta1[i].kolor = "clubs";
			karta1[i].path = "images/" + to_string(karta1[i].wartosc) + "_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 39 + 1 == 1)
		{
			karta1[i].wartosc = 11;
			karta1[i].kolor = "clubs";
			karta1[i].path = "images/ace_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 39 + 1 == 11)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "clubs";
			karta1[i].path = "images/jack_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 39 + 1 == 12)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "clubs";
			karta1[i].path = "images/queen_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
		else if (i - 39 + 1 == 13)
		{
			karta1[i].wartosc = 10;
			karta1[i].kolor = "clubs";
			karta1[i].path = "images/king_of_" + karta1[i].kolor + ".png";
			karta1[i].texture.loadFromFile(karta1[i].path);
			karta1[i].sprite.setTexture(karta1[i].texture);
		}
	}
}
void Split(vector <deck>& karty_gracz, vector <deck>& karta1, vector <deck>& karty_gracz2)
{
	splitmode = true;
	gameFinish2 = false;
	kopiakarty(karty_gracz2[igracz2], karty_gracz[igracz]);
	karty_gracz.erase(karty_gracz.begin() + 1);

	int a = rand() % karta1.size();
	kopiakarty(karty_gracz[igracz], karta1[a]);
	karta1.erase(karta1.begin() + a);

	igracz2++;
	int b = rand() % karta1.size();
	kopiakarty(karty_gracz2[igracz2], karta1[b]);
	karta1.erase(karta1.begin() + b);
	balance -= bet.betValue;
}
void splithit(vector <deck>& karty_gracz2, vector <deck>& karta1, int& igracz2)
{
	bet2 = bet.betValue;
	hitused = true;
	igracz2++;
	int a = rand() % karta1.size();
	kopiakarty(karty_gracz2[igracz2], karta1[a]);
	karta1.erase(karta1.begin() + a);
	value_gracz2 = wartosc_g(karty_gracz2);
	if (value_gracz2 > 21)
	{
		ryj = true;
		bust = true;
	}
}
void splitstand(vector <deck>& karty_gracz2, vector <deck>& karty_dealer, vector <deck>& karta1, int& idealer)
{
	ryj = true;
	bet2 = bet.betValue;
}
void splitdoubledown(vector <deck>& karty_gracz2, vector <deck>& karty_dealer, vector <deck>& karta1, int& igracz2, int& idealer)
{
	ryj = true;
	balance -= bet.betValue;
	bet.betValue *= 2;
	bet2 = bet.betValue;
	igracz2++;
	int dd = rand() % karta1.size();
	kopiakarty(karty_gracz2[igracz2], karta1[dd]);
	karta1.erase(karta1.begin() + dd);
	gameFinish2 = true;
}
void splitwynik(vector <deck>& karty_gracz, vector <deck>& karty_gracz2, vector <deck>& karty_dealer, vector <deck>& karta1, int& igracz, int& idealer, int& igracz2)
{
	value_gracz = wartosc_g(karty_gracz);
	value_gracz2 = wartosc_g(karty_gracz2);
	value_dealer = wartosc_d(karty_dealer);
	while (value_dealer < 17)
	{
		idealer++;
		int d = rand() % karta1.size();
		kopiakarty(karty_dealer[idealer], karta1[d]);
		karta1.erase(karta1.begin() + d);
		value_dealer = wartosc_d(karty_dealer);
	}
	if (value_gracz > 21 || value_dealer > value_gracz && value_dealer <= 21)
	{
		gameFinish2 = true;
	}
	else if (value_gracz > value_dealer && value_gracz <= 21 || value_dealer > 21)
	{
		balance += 2 * bet1;
		gameFinish2 = true;
	}
	else if (value_gracz == value_dealer)
	{
		balance += bet1;
		gameFinish2 = true;
	}
	if (value_gracz2 > 21 || value_dealer > value_gracz2 && value_dealer <= 21)
	{
		gameFinish2 = true;
	}
	else if (value_gracz2 > value_dealer && value_gracz2 <= 21 || value_dealer > 21)
	{
		balance += 2 * bet2;
		gameFinish2 = true;
	}
	else if (value_gracz2 == value_dealer)
	{
		balance += bet2;
		gameFinish2 = true;
	}
}
int main()
{
	kartydodawane();
	srand(time(NULL));
	//dodawanie rzeton?w
	chips rzeton1;
	rzeton1.wartoœæ = 1;
	rzeton1.texture.loadFromFile("images/1.png");
	rzeton1.sprite.setTexture(rzeton1.texture);
	chips rzeton10;
	rzeton10.wartoœæ = 10;
	rzeton10.texture.loadFromFile("images/10.png");
	rzeton10.sprite.setTexture(rzeton10.texture);
	chips rzeton100;
	rzeton100.wartoœæ = 100;
	rzeton100.texture.loadFromFile("images/100.png");
	rzeton100.sprite.setTexture(rzeton100.texture);
	chips rzeton1000;
	rzeton1000.wartoœæ = 1000;
	rzeton1000.texture.loadFromFile("images/1000.png");
	rzeton1000.sprite.setTexture(rzeton1000.texture);
	chips rzetonmax;
	rzetonmax.wartoœæ = balance;
	rzetonmax.texture.loadFromFile("images/max.png");
	rzetonmax.sprite.setTexture(rzetonmax.texture);
	//t?o
	Texture backgrund;
	backgrund.loadFromFile("images/background.png");
	Sprite background(backgrund);
	background.setPosition(0, 0);
	//buttons
	Texture b1, b2, b3, b4, b5;
	b1.loadFromFile("images/PLACE1.png");
	b2.loadFromFile("images/HIT.png");
	b3.loadFromFile("images/STAND.png");
	b4.loadFromFile("images/DOUBLE DOWN.png");
	b5.loadFromFile("images/SPLIT.png");
	Sprite place(b1);
	Sprite hit(b2);
	Sprite stand(b3);
	Sprite double_down(b4);
	Sprite split(b5);
	//deck grafika
	Texture decki_2;
	decki_2.loadFromFile("images/decki.png");
	Sprite decki(decki_2);
	Texture plecy;
	plecy.loadFromFile("images/cardBack.png");
	Sprite back(plecy);


	RenderWindow window(VideoMode(1920, 1080), "Blackjack", Style::Default);
	srand(time(NULL));
	while (window.isOpen())
	{
		if (karta1.size() < 15)
		{
			karta1.clear();
			karta1.resize(52);
			kartydodawane();
		}
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::TextEntered)
			{
				bet.input(e, betBarEnable);
			}
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{
					float x = Mouse::getPosition(window).x;
					float y = Mouse::getPosition(window).y;
					// place button
					if (x >= 560 && y >= 525 && x <= 740 && y <= 585 && bet.betValue > 0 && bet.betValue <= balance && betPlaced == false)
					{
						betBarEnable = false;
						betPlaced = true;
						balance -= bet.betValue;
					}
					else if (splitexecuted == true)
					{
						if (secondhanduse == false)
						{
							if (x >= 725 && y >= 525 && x <= 825 && y <= 685 && betBarEnable == false)
							{
								if (betPlaced == true && value_gracz < 21)
								{
									Hit(karty_gracz, karta1, igracz);
								}
							}
							// stand button
							else if (x >= 845 && y >= 525 && x <= 1025 && y <= 585 && betBarEnable == false)
							{
								Stand(karty_gracz, karty_dealer, karta1, idealer);
								secondhanduse = true;
							}
							//double down
							else if (x >= 355 && y >= 525 && y <= 585 && x <= 785 && betPlaced == true && hitused == false)
							{
								Doubledown(karty_gracz, karty_dealer, karta1, igracz, idealer);
								secondhanduse = true;
							}
						}
						else
						{
							if (x >= 725 && y >= 525 && x <= 825 && y <= 685 && betBarEnable == false && bust == false)
							{
								splithit(karty_gracz2, karta1, igracz2);
								if (bust == true)
								{
									splitwynik(karty_gracz, karty_gracz2, karty_dealer, karta1, igracz, idealer, igracz2);
								}
							}
							// stand button
							else if (x >= 845 && y >= 525 && x <= 1025 && y <= 585 && betBarEnable == false)
							{
								splitstand(karty_gracz2, karty_dealer, karta1, idealer);
								splitwynik(karty_gracz, karty_gracz2, karty_dealer, karta1, igracz, idealer, igracz2);
							}
							//double down
							else if (x >= 355 && y >= 525 && y <= 585 && x <= 785 && betPlaced == true && hitused == false)
							{
								splitdoubledown(karty_gracz2, karty_dealer, karta1, igracz2, idealer);
								splitwynik(karty_gracz, karty_gracz2, karty_dealer, karta1, igracz, idealer, igracz2);
							}
						}

					}
					else if (splitexecuted == false)
					{
						// hit button
						if (x >= 725 && y >= 525 && x <= 825 && y <= 685 && betBarEnable == false)
						{
							if (betPlaced == true && value_gracz < 21)
							{
								Hit(karty_gracz, karta1, igracz);
							}
						}
						// stand button
						else if (x >= 845 && y >= 525 && x <= 1025 && y <= 585 && betBarEnable == false)
						{
							Stand(karty_gracz, karty_dealer, karta1, idealer);
						}
						//double down
						else if (x >= 355 && y >= 525 && y <= 585 && x <= 785 && betPlaced == true && hitused == false)
						{
							Doubledown(karty_gracz, karty_dealer, karta1, igracz, idealer);
						}
						//split
						else if (karty_gracz[0].wartosc == karty_gracz[1].wartosc)
						{
							splitallowed = true;
							if (x >= 1045 && y >= 525 && y <= 625 && x <= 1190 && betBarEnable == false && splitallowed == true)
							{
								splitexecuted = true;
								splitallowed = false;
								Split(karty_gracz, karta1, karty_gracz2);
							}
						}
					}
					// bet 1
					if (x >= 90 && y >= 600 && x <= 140 && y <= 650 && betBarEnable == true)
					{
						bet.addButton(1);
					}
					// bet 10
					if (x >= 90 && y >= 660 && x <= 140 && y <= 710 && betBarEnable == true)
					{
						bet.addButton(10);
					}
					// bet 100
					if (x >= 90 && y >= 720 && x <= 140 && y <= 770 && betBarEnable == true)
					{
						bet.addButton(100);
					}
					// bet 1000
					if (x >= 90 && y >= 780 && x <= 140 && y <= 830 && betBarEnable == true)
					{
						bet.addButton(1000);
					}
					// bet max
					if (x >= 90 && y >= 840 && x <= 140 && y <= 890 && betBarEnable == true)
					{
						bet.betValue = 0;
						bet.addButton(balance);
					}
				}
			}
		}
		if (betPlaced == false)
		{
			window.clear();
			window.draw(background);
			place.setPosition(560, 525);
			window.draw(place);
		}
		else
		{
			window.draw(background);
			double_down.setPosition(355, 525);
			window.draw(double_down);
			hit.setPosition(725, 525);
			window.draw(hit);
			stand.setPosition(845, 525);
			window.draw(stand);
			split.setPosition(1045, 525);
			window.draw(split);

			if (first == true && betPlaced == true)//rozdanie pierwszych dw?ch kart dla gracza i delera
			{
				int s1 = rand() % karta1.size();
				kopiakarty(karty_gracz[igracz], karta1[s1]);
				karta1.erase(karta1.begin() + s1);

				int d1 = rand() % karta1.size();
				kopiakarty(karty_dealer[idealer], karta1[d1]);
				karta1.erase(karta1.begin() + d1);

				igracz++;
				int s2 = rand() % karta1.size();
				kopiakarty(karty_gracz[igracz], karta1[s2]);
				karta1.erase(karta1.begin() + s2);

				idealer++;
				int d2 = rand() % karta1.size();
				kopiakarty(karty_dealer[idealer], karta1[d2]);
				karta1.erase(karta1.begin() + d2);
				if (karty_gracz[0].wartosc + karty_gracz[1].wartosc == 21)
				{
					blackjack();
				}
				first = false;
			}
		}
		if (igracz != 0 && idealer != 0)
		{
			for (int i = 0; i <= igracz; i++)
			{
				karty_gracz[i].sprite.setPosition(position + i * 60, 630);
				window.draw(karty_gracz[i].sprite);
			}
			for (int i = 0; i <= idealer; i++)
			{
				if (ryj == false && i == 0)
				{
					back.setPosition(position + i * 260, 30);
					window.draw(back);
				}
				else
				{
					karty_dealer[i].sprite.setPosition(position + i * 260, 30);
					window.draw(karty_dealer[i].sprite);
				}
			}
			if (splitmode == true)
			{
				for (int i = 0; i <= igracz2; i++)
				{
					karty_gracz2[i].sprite.setPosition(800 + i * 60, 630);
					window.draw(karty_gracz2[i].sprite);
				}

			}
		}

		rzeton1.sprite.setPosition(90, 600);
		window.draw(rzeton1.sprite);
		rzeton10.sprite.setPosition(90, 660);
		window.draw(rzeton10.sprite);
		rzeton100.sprite.setPosition(90, 720);
		window.draw(rzeton100.sprite);
		rzeton1000.sprite.setPosition(90, 780);
		window.draw(rzeton1000.sprite);
		rzetonmax.sprite.setPosition(90, 840);
		window.draw(rzetonmax.sprite);




		decki.setPosition(30, 30);
		window.draw(decki);

		string b;
		b = "Balance: " + to_string(balance);
		Text balanceText;
		Font font;
		font.loadFromFile("fonts/Arial.ttf");
		balanceText.setFont(font);
		balanceText.setOutlineColor(Color::Black);
		balanceText.setOutlineThickness(2);
		balanceText.setString(b);
		balanceText.setPosition(30, 300);
		balanceText.setCharacterSize(45);
		window.draw(balanceText);
		bet.drawBetBar(window);
		window.display();
		if (gameFinish == true && gameFinish2 == true)
		{
			sleep(seconds(3));
			if (balance == 0)
			{
				window.close();
			}
			betBarEnable = true;
			betPlaced = false;
			karty_gracz.erase(karty_gracz.begin(), karty_gracz.begin() + igracz);
			karty_dealer.erase(karty_dealer.begin(), karty_dealer.begin() + idealer);
			karty_gracz2.erase(karty_gracz2.begin(), karty_gracz2.begin() + igracz2);
			karty_gracz.resize(10);
			karty_gracz2.resize(10);
			karty_dealer.resize(10);
			igracz = 0;
			idealer = 0;
			igracz2 = 0;
			value_gracz = 0;
			value_dealer = 0;
			value_gracz2 = 0;
			first = true;
			gameFinish = false;
			position = 260;
			ryj = false;
			hitused = false;
			gameFinish2 = true;
			splitallowed = false;
			splitmode = false;
			splitexecuted = false;
			bust = false;
			secondhanduse = false;
			bet1 = 0;
			bet2 = 0;
		}
	}
	return 0;
}
