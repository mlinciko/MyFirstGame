#include <SFML/Graphics.hpp>
#include <iostream>
#include "map1.h"
#include "view.h"
#include <sstream>

using namespace std;
using namespace sf;

class Player
{
private: float x, y = 0;
public: 
	float w, h, dx, dy, speed;
	int dir;
	int playerScore;
	bool win = false;
	bool life = true;
	int health = 50;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H)
	{
		dir = 0; speed = 0; playerScore = 0;
		dx = 0; dy = 0; speed = 0; dir = 0;
		File = F;
		w = W; h = H;
		image.loadFromFile(File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;//идем вправо
		case 1: dx = -speed; dy = 0; break;//идем влево
		case 2: dx = 0; dy = speed; break;//идем вниз
		case 3: dx = 0; dy = -speed; break;//идем наверх
		}

		x += dx * time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();

		if (health <= 0)
		{
			life = false;
			speed = 0;
		}
	}
	float getPlayerCoordinateX() {
		return x;
	}
	float getPlayerCoordinateY() {
		return y;
	}
	void interactionWithMap()
	{
		for(int i = y/32; i <(y + h)/32; i++)
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '1' || TileMap[i][j] == '2'|| TileMap[i][j] == '3' || TileMap[i][j] == '4' || TileMap[i][j] == '5' || TileMap[i][j] == '6' || TileMap[i][j] == '7' || TileMap[i][j] == 'a' || TileMap[i][j] == 'b' || TileMap[i][j] == 'c' || TileMap[i][j] == 'd' || TileMap[i][j] == 'e' || TileMap[i][j] == 'f' || TileMap[i][j] == 'k' || TileMap[i][j] == 'l') {
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх 
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}
				if (TileMap[i][j] == '0') { //если символ равен '0' (морковка)
					TileMap[i][j] = ' ';//убираем морковку, типа взяли бонус. 
					playerScore += 10;
				}
				if (TileMap[i][j] == '8') {//телепортация на(x,y)
					x = 92; y = 1088;
				}
				if (TileMap[i][j] == 'm') {
					x = 352; y = 32;
				}
				if (TileMap[i][j] == 'q') {//если персонаж заходит на выход переменная = true
					win = true;
				}
				if (TileMap[i][j] == 'r') {//если персонаж собирает гриб, здоровье уменьшается на 10
					TileMap[i][j] = ' ';
					health -= 10;
				}


			}
	}
 
};

RenderWindow window;

float speed = 0.1;
float CurrentFrame = 0;





int main()
{
	//редактируем окно
	window.create(VideoMode(640, 480), "Game");
	window.setFramerateLimit(60);//кол-во кадров в секунду
	view.reset(sf::FloatRect(0, 0, 640, 480));

	Font font;//шрифт
	font.loadFromFile("BalooBhaijaan-Regular.ttf");
	Text text1("Score: ", font, 20);
	text1.setFillColor(sf ::Color::Yellow);

	Text text2("Game over!", font, 60);//текс выводящий GameOver
	text2.setFillColor(sf::Color::Red);

	Text text3("Score: ", font, 30);//текст выводящий итоговый счет
	text3.setFillColor(sf::Color::Green);

	Text text4("Health: ", font, 20);//текс выводящий здоровье
	text4.setFillColor(sf::Color::Magenta);
	
	Player p("image.png", 32, 32, 40, 80);
	
	Clock clock;

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заливаем в текстуру картинку
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом
	//пока окно открыто
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		cout << time << endl;
		Event event;
		while (window.pollEvent(event))//проведем события
		{
			if (event.type == Event::Closed)//если нажимаем на крестик окно закрывается
				window.close();
		}
		
		if ( p.win == false) {
			if (p.life) {
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					p.dir = 1; p.speed = 0.15;
					CurrentFrame += 0.007*time;
					if (CurrentFrame > 4) CurrentFrame -= 4;
					p.sprite.setTextureRect(IntRect(int(CurrentFrame) * 40, 80, 40, 80));
				}
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					p.dir = 0; p.speed = 0.15;
					CurrentFrame += 0.007*time;
					if (CurrentFrame > 4) CurrentFrame -= 4;
					p.sprite.setTextureRect(IntRect(int(CurrentFrame) * 40, 160, 40, 80));
				}
				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					p.dir = 3; p.speed = 0.15;
					CurrentFrame += 0.007*time;
					if (CurrentFrame > 4) CurrentFrame -= 4;
					p.sprite.setTextureRect(IntRect(int(CurrentFrame) * 40, 240, 40, 80));
				}
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					p.dir = 2; p.speed = 0.15;
					CurrentFrame += 0.007*time;
					if (CurrentFrame > 4) CurrentFrame -= 4;
					p.sprite.setTextureRect(IntRect(int(CurrentFrame) * 40, 0, 40, 80));

				}
			}
		}
		getPlayerCoordinateforview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
		p.update(time);
		window.setView(view);
		window.clear();
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == '0')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ 0, то рисуем 1й квадратик
				if (TileMap[i][j] == '1')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ 1, то рисуем 2й квадратик
				if ((TileMap[i][j] == '2')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 2, то рисуем 3й квадратик
				if ((TileMap[i][j] == '3')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//и тд
				if ((TileMap[i][j] == '4')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				if ((TileMap[i][j] == '5')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
				if ((TileMap[i][j] == '6')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
				if ((TileMap[i][j] == '7')) s_map.setTextureRect(IntRect(224, 0, 32, 32));
				if ((TileMap[i][j] == ' ')) s_map.setTextureRect(IntRect(256, 0, 32, 32));
				if ((TileMap[i][j] == '8')) s_map.setTextureRect(IntRect(288, 0, 32, 32));
				if ((TileMap[i][j] == '9')) s_map.setTextureRect(IntRect(320, 0, 32, 32));
				if ((TileMap[i][j] == 'a')) s_map.setTextureRect(IntRect(352, 0, 32, 32));
				if ((TileMap[i][j] == 'b')) s_map.setTextureRect(IntRect(384, 0, 32, 32));
				if ((TileMap[i][j] == 'c')) s_map.setTextureRect(IntRect(416, 0, 32, 32));
				if ((TileMap[i][j] == 'd')) s_map.setTextureRect(IntRect(448, 0, 32, 32));
				if ((TileMap[i][j] == 'e')) s_map.setTextureRect(IntRect(480, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(512, 0, 32, 32));
				if ((TileMap[i][j] == 'k')) s_map.setTextureRect(IntRect(544, 0, 32, 32));
				if ((TileMap[i][j] == 'l')) s_map.setTextureRect(IntRect(576, 0, 32, 32));
				if ((TileMap[i][j] == 'n')) s_map.setTextureRect(IntRect(608, 0, 32, 32));
				if ((TileMap[i][j] == 'm')) s_map.setTextureRect(IntRect(640, 0, 32, 32));
				if ((TileMap[i][j] == 'r')) s_map.setTextureRect(IntRect(672, 0, 32, 32));
				if ((TileMap[i][j] == 'q')) s_map.setTextureRect(IntRect(704, 0, 32, 32));

				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		
		std::ostringstream playerScoreString;    // объявили переменную
		playerScoreString << p.playerScore;		//занесли в нее число очков, то есть формируем строку
		text1.setString("Score: " + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		text1.setPosition(view.getCenter().x - 165, view.getCenter().y - 200);//задаем позицию текста, отступая от центра камеры
		window.draw(text1);//рисую этот текст

		text2.setPosition(view.getCenter().x-125, view.getCenter().y-30);

		std::ostringstream playerHealthString;    // объявили переменную
		playerHealthString << p.health;		//занесли в нее число очков, то есть формируем строку
		text4.setString("Health: " + playerHealthString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		text4.setPosition(view.getCenter().x - 165, view.getCenter().y - 180);//задаем позицию текста, отступая от центра камеры
		window.draw(text4);//рисую этот текст

		text3.setString("Score:" + playerScoreString.str());
		text3.setPosition(view.getCenter().x - 45, view.getCenter().y+ 20);//задаем позицию
		
		window.draw(p.sprite);//рисует игрока

		if (p.life == false) {//если игрок съел 5 грибов на экран выводится Game Over		
			window.draw(text2);// игра окончена
			window.draw(text3);//итоговый счет
		}
		if (p.win == true)
		{
			window.draw(text2);// игра окончена
			window.draw(text3);//итоговый счет

		}
		window.display();

	}
	return 0;
}