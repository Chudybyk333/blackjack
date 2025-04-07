#pragma once
#include <iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
using namespace std;
using namespace sf;
class deck
{
public:
	int wartosc = 0;
	string kolor;
	string path;
	Texture texture;
	Sprite sprite;
};
