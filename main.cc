#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <time.h>

void end_game(std::string s, sf::Vector2f pos, sf::RenderWindow& window){
	sf::Clock contador;
	sf::Font font;
	sf::Text text;
	if(!font.loadFromFile("Fonts/FreeMono.ttf")) std::cout << "Could not load font" << std::endl;
	text.setFont(font); 
	text.setString(s); 
	text.setCharacterSize(50);
	text.setColor(sf::Color::Black);
	text.setPosition(pos.x/4, pos.y/2);
	window.draw(text);
	window.display();
	while(contador.getElapsedTime().asSeconds() <= 1.5){
		
	}
}

std::string numToString(int num){
	std::ostringstream convert;
	convert << num;
	return convert.str();
}

bool isWhite(sf::Image& image, float px, float py){
	return image.getPixel(px, py) == sf::Color::White;
}

int main(){

	//CONSTANT AND 'GLOBAL' VARIABLES
	int speed = 100, speed2 = 100;
	float time_to_next_sprite = 0.09;
	enum direction { down, left, right, up , none};
	const int mx[5] = {0,	-1,    1,   0,   0};
	const int my[5] = {1,	 0,    0,  -1,   0};

	//'VECTOR2's
	sf::Vector2f spriteSize(28,28);
	sf::Vector2f spriteSize2(28,28);
	sf::Vector2f shotSize(639,23);
	sf::Vector2f melocSize(16,14);
	sf::Vector2f zumoSize(14,14);
	sf::Vector2f backgroundSize(639, 480);
	sf::Vector2i spriteSource(0,down);
	sf::Vector2i spriteSource2(0,down);
	sf::Vector2f originalSpriteSize(0,0);
	sf::Vector2f originalSpriteSize2(0,0);
	sf::Vector2f playerPosition(350,280);
	sf::Vector2f playerPosition2(280,350);
	sf::Vector2f text1pos(60,370);
	sf::Vector2f text2pos(50,395);
	std::vector<std::pair<int, sf::Sprite> > bayas;
	//SFML OBJECTS
	sf::Event event;
	sf::Clock deltaClock, melocClock, zumoClock, zumo2Clock, pokeClock, poke2Clock, spawnClock, shotClock, shotClock2, parClock, shotTimer, shotTimer2;
	sf::Image image;
	sf::Sprite player, player2, background, meloc, zumo, stone, pokeball, zreza, shot, shot2;
	sf::Texture tplayer, tplayer2, tbackground, tmeloc, tzumo, tstone, tpokeball, tzreza, tshot;
	sf::Font FreeMono;
	sf::Text text1, text2;

	//LOAD TEXTURES
	if(!image.loadFromFile("Images/map.png")) std::cout << "collision map not loaded" << std::endl;
 	if(!tzumo.loadFromFile("Images/zumo.png")) std::cout << "zumo texture Not Loaded " << std::endl;
	if(!tmeloc.loadFromFile("Images/meloc.png")) std::cout << "meloc texture Not Loaded " << std::endl;
	if(!tzreza.loadFromFile("Images/zreza.png")) std::cout << "zreza texture Not Loaded " << std::endl;
	if(!tstone.loadFromFile("Images/piedratrueno.png")) std::cout << "stone texture Not Loaded " << std::endl;
	if(!tpokeball.loadFromFile("Images/poke.png")) std::cout << "poke texture Not Loaded " << std::endl;
	if(!tbackground.loadFromFile("Images/ground.png")) std::cout << "background texture Not Loaded " << std::endl;
	if(!tplayer.loadFromFile("Images/pikachu.png")) std::cout << "personatge Not Loaded " << std::endl;
	if(!tplayer2.loadFromFile("Images/pikachu2.png")) std::cout << "personatge2 Not Loaded " << std::endl;
	if(!tshot.loadFromFile("Images/shot.png")) std::cout << "shot Not Loaded " << std::endl;
	if(!FreeMono.loadFromFile("Fonts/FreeMono.ttf")) std::cout << "Could not load font" << std::endl;
	text1.setFont(FreeMono); text2.setFont(FreeMono);
	text1.setCharacterSize(15); text2.setCharacterSize(15);
	text1.setColor(sf::Color(255, 145, 0)); text2.setColor(sf::Color::Yellow);
	text1.setPosition(text1pos);
	text2.setPosition(text2pos);
	int piedras = 0;
	spriteSize.x = originalSpriteSize.x = tplayer.getSize().x/4;
	spriteSize.y = originalSpriteSize.y = tplayer.getSize().y/4;

	spriteSize2.x = originalSpriteSize2.x = tplayer2.getSize().x/4;
	spriteSize2.y = originalSpriteSize2.y = tplayer2.getSize().y/4;

	//VARIABLES
	int melocCount = 0, melocCount2 = 0;
	bool speedup = false, speedup2 = false, raichu = false, raichu2 = false, poke=false, poke2 = false, par = false, par2 = false;
	direction d = none, q = none;
	float deltatime = 0, movx = 0.0, movy = 0.0, movx2 = 0.0, movy2 = 0.0, scont = 0.0, scont2 = 0.0;
	bool keypressed = false;
	float distx = spriteSize.x/4, disty = spriteSize.y/4, distx2 = spriteSize2.x/4, disty2 = spriteSize2.y/4;

	//Set player's properties
	player.setTexture(tplayer);
	player.setPosition(playerPosition);


	player2.setTexture(tplayer2);
	player2.setPosition(playerPosition2);

	shot.setTexture(tshot);
	shot2.setTexture(tshot);

	//Set Background's properties
	background.setTexture(tbackground);
	bool shoot = false, shoot2 = false;

	//CREATE THE WINDOW
	direction lastDir = down, lastDir2 = down;
	sf::RenderWindow window(sf::VideoMode(backgroundSize.x,backgroundSize.y), "Pika!");
	std::cout << "Howdy fellow trainer! Here's your chance to help Pikachu" << std::endl
	<< "collect all the Pechaberries he can get!" << std::endl
	<< "If you get the chance to collect a Mobile Orb, you'll be able to move faster!" << std::endl
	<< "Use them wisly, as they last only 10 seconds." << std::endl
	<< "Be careful with Pokeballs, they'll slow you down, you may want to avoid them." << std::endl
	<< "Although no one has seen it yet, some say a mighty stone could appear" << std::endl
	<< "and turn Pikachu into an even more powerfull creature. Keep your eyes wide open." << std::endl
	<< "Your score will appear down here, good luck!" << std::endl;
 	sf::Music music;
    if (!music.openFromFile("Sounds/pikaGirl.wav")) {
        std::cout << "music failed" << std::endl;
    }
    music.setLoop(true);         // make it loop
    // Play it
    music.play();

	//GAME LOOP
	while(window.isOpen()){

		//Loop for handling events
		while(window.pollEvent(event)){
			switch (event.type){
				//Close event
				case sf::Event::Closed:
					window.close();
					break;
				//KeyPressed event
				case  sf::Event::KeyPressed:
					//Close key
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}/*
					//Increasing Speed
					if (event.key.code == sf::Keyboard::P) {
						if(speed < 280){
							speed += 20;
							speed2 += 20;
							time_to_next_sprite -= 0.01;
						}
					}
					//Decreasing Speed
					if (event.key.code == sf::Keyboard::O) {
						if(speed > 80){
							speed -= 20;
							speed2 -= 20;
							time_to_next_sprite += 0.01;
						}
					}*/
					break;
				//Default
				default:
					//Do nothing
					break;
			}
		}	

		//Deltatime 
		deltatime = deltaClock.restart().asSeconds();
		scont += deltatime;
		scont2 += deltatime;

		//Update Direction 'd'
		d = none;
		bool can_shoot = shotClock.getElapsedTime().asSeconds() >= 2;
		bool can_shoot2 = shotClock2.getElapsedTime().asSeconds() >= 2;
	        //By keyboard
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	{d = up;if (!shoot) 	lastDir = d;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{d = down;if (!shoot) 	lastDir = d;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{d = left;if (!shoot) 	lastDir = d;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	{d = right;if (!shoot) 	lastDir = d;}
		

		q = none;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {q = up;if (!shoot2) 	lastDir2 = q;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))	{q = down;if (!shoot2) 	lastDir2 = q;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))	{q = left;if (!shoot2) 	lastDir2 = q;}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))	{q = right;if (!shoot2) lastDir2 = q;}


		//std::cout<<"TIMER----"<<shotTimer.getElapsedTime().asSeconds()<<std::endl;
		//std::cout<<"-------------------CLOCK----"<<shotClock.getElapsedTime().asSeconds()<<std::endl;
		if (can_shoot){
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
				shoot = true;
				std::cout << "P1 tries to shoot" << std::endl;
				shotClock.restart();
			}
			shotTimer.restart();
		}
		else if(shotTimer.getElapsedTime().asSeconds() >= 0.5){
			shoot = false;
		}
		if (can_shoot2){
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
				shoot2 = true;
				std::cout << "P2 tries to shoot" << std::endl;
				shotClock2.restart();
			}
			shotTimer2.restart();
		}
		else if(shotTimer2.getElapsedTime().asSeconds() >= 0.5){
			shoot2 = false;
		}



/*
		shotTimer.restart(); shoot = false; std::cout << "P1 HAS TO REST" << std::endl;}
		else if(shotTimer.getElapsedTime().asSeconds() >= 2)){ shoot = true; shotClock.restart(); 
			std::cout << "P1 tries to shoot" << std::endl;*/

		if (poke or par) shoot = false;
		if (poke2 or par2) shoot2 = false;

		//If there is a direction
		if(not shoot and d != none){
			//Set value to movement variables and update spritesource
			if(spriteSource.y == d){
				movx += mx[d]*speed*deltatime;
				movy += my[d]*speed*deltatime;
			}
			else spriteSource.y = d; //so you can turn without walking
			if (scont >= time_to_next_sprite){
				scont = 0;
				++spriteSource.x;
			}
		}

		if (not shoot2 and q != none){
			//Set value to movement variables and update spritesource
			if(spriteSource2.y == q){
				movx2 += mx[q]*speed2*deltatime;
				movy2 += my[q]*speed2*deltatime;
			}
			else spriteSource2.y = q; //so you can turn without walking
			if (scont2 >= time_to_next_sprite){
				scont2 = 0;
				++spriteSource2.x;
			}
		}
		

		int destix = playerPosition.x+movx;
		int destiy = playerPosition.y+movy;

		
		int destix2 = playerPosition2.x+movx2;
		int destiy2 = playerPosition2.y+movy2;

		sf::Vector2f shotPosition = playerPosition; //cambiar
		sf::Vector2f shotPosition2 = playerPosition2; //cambiar


		float rotation, rotation2/*, shotdistx, shotdisty*/;
		/*shotdistx = playerPosition.x;
		shotdisty = playerPosition.y;
		while(isWhite(image,shotdistx,shotdisty)) {
			
			switch(lastDir){
				case up:
					--shotdisty;
					break;
				case down:
					++shotdisty;
					break;
				case left:
					--shotdistx;
					break;
				case right:
					++shotdistx;
					break;
				default: break;
			}
		}*/
		if (lastDir == up){
			rotation = 90;
			//shot.setScale(1, playerPosition.y-shotdisty/shotSize.y);
			shotPosition.y = playerPosition.y - spriteSize.y;
		}
		else if (lastDir == down){
			rotation = -90;
			//shot.setScale(1, playerPosition.y-shotdisty/shotSize.y);
		}
		else if (lastDir == right){
			rotation = 180;
			shotPosition.x = playerPosition.x + spriteSize.x/2;
			shotPosition.y -= spriteSize.y/2;
			//shot.setScale(1, playerPosition.x-shotdistx/shotSize.y);
		}
		else{
			rotation = 0;
			shotPosition.x = playerPosition.x - spriteSize.x/2;
			shotPosition.y -= spriteSize.y/2;
			//shot.setScale(1, playerPosition.x-shotdistx/shotSize.y);
		}


		if (lastDir2 == up){
			rotation2 = 90;
			shotPosition2.y = playerPosition2.y - spriteSize2.y;
		}
		else if (lastDir2 == down)rotation2 = -90;
		else if (lastDir2 == right){
			rotation2 = 180;
			shotPosition2.x = playerPosition2.x + spriteSize2.x/2;
			shotPosition2.y -= spriteSize2.y/2;
		}
		else{
			rotation2 = 0;
			shotPosition2.x = playerPosition2.x - spriteSize2.x/2;
			shotPosition2.y -= spriteSize2.y/2;
		}/*
		int shotdistance = 0, shotdistance2 = 0;
		sf::Vector2f punt(0.0,0.0);
		sf::Vector2f scale;
		punt.x = image.getSize().x*playerPosition.x/tbackground.getSize().x/background.getScale().x;
		punt.y = image.getSize().y*playerPosition.y/tbackground.getSize().y/background.getScale().y;
		bool vertical = (lastDir == up or lastDir == down);
		while (isWhite(image, punt.x, punt.y)){
			shotdistance += 1;
			if (vertical)
				punt.x = image.getSize().x*(playerPosition.x+shotdistance)/tbackground.getSize().x/background.getScale().x;
			else punt.y = image.getSize().y*(playerPosition.y+shotdistance)/tbackground.getSize().y/background.getScale().y;
		}
		if(vertical){
			scale.x = 1;
			scale.y = (shotdistance/shotSize.x);
		}
		else {
			scale.y = 1;
			scale.x = (shotdistance/shotSize.x);
		}*/
		shot.setOrigin(shotSize.x, shotSize.y/2);
		//shot.scale(scale.x, scale.y);
		shot.setPosition(shotPosition.x, shotPosition.y);
		shot.setRotation(rotation);
		/*
		vertical = (lastDir2 == up or lastDir2 == down);
		punt.x = image.getSize().x*playerPosition2.x/tbackground.getSize().x/background.getScale().x;
		punt.y = image.getSize().y*playerPosition2.y/tbackground.getSize().y/background.getScale().y;
		while (isWhite(image, punt.x, punt.y)){
			shotdistance2 += 1.0;
			if (vertical)
				punt.x = image.getSize().x*(playerPosition2.x+shotdistance2)/tbackground.getSize().x/background.getScale().x;
			else punt.y = image.getSize().y*(playerPosition2.y+shotdistance2)/tbackground.getSize().y/background.getScale().y;
		}
		if(vertical){
			scale.x = 1;
			scale.y = (shotdistance2/shotSize.x);
		}
		else {
			scale.y = 1;
			scale.x = (shotdistance2/shotSize.x);
		}*/
		shot2.setOrigin(shotSize.x, shotSize.y/2);
		//shot.scale(scale.x, scale.y);
		shot2.setPosition(shotPosition2.x, shotPosition2.y);
		shot2.setRotation(rotation2);
		
		//Calcule the point in the image corresponding to the point the player is in the background
		float px, py;
		px = image.getSize().x*destix/tbackground.getSize().x/background.getScale().x;
		py = image.getSize().y*destiy/tbackground.getSize().y/background.getScale().y;

		//Check if the color of the corresponding point is White (if it is let the player move)
		if( isWhite(image, px, py) && isWhite(image, px, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, py) &&
			isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, py) 
			){
				playerPosition.x += movx;
				playerPosition.y += movy;
		}

		movx = movy = 0;

		px = image.getSize().x*destix2/tbackground.getSize().x/background.getScale().x;
		py = image.getSize().y*destiy2/tbackground.getSize().y/background.getScale().y;

		//Check if the color of the corresponding point is White (if it is let the player move)
		if( isWhite(image, px, py) && isWhite(image, px, image.getSize().y*(destiy2-disty2)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix2+distx2)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy2-disty2)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix2-distx2)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy2-disty2)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix2+distx2)/tbackground.getSize().x/background.getScale().x, py) &&
			isWhite(image, image.getSize().x*(destix2-distx2)/tbackground.getSize().x/background.getScale().x, py) 
			){
				playerPosition2.x += movx2;
				playerPosition2.y += movy2;
		}

		movx2 = movy2 = 0;

		//Checking the sprite sources and position to be ok
		if(spriteSource.x >= 4) spriteSource.x = 0;
		float aux = background.getTexture()->getSize().y * background.getScale().y;
		if(playerPosition.x < 0) playerPosition.x = 0;
		else if (playerPosition.y < 0) playerPosition.y = 0;
		else if (playerPosition.x > window.getSize().x) playerPosition.x = window.getSize().x-1;
		else if (playerPosition.y > aux) playerPosition.y = aux -1;

		if(spriteSource2.x >= 4) spriteSource2.x = 0;
		aux = background.getTexture()->getSize().y * background.getScale().y;
		if(playerPosition2.x < 0) playerPosition2.x = 0;
		else if (playerPosition2.y < 0) playerPosition2.y = 0;
		else if (playerPosition2.x > window.getSize().x) playerPosition2.x = window.getSize().x-10;
		else if (playerPosition2.y > aux) playerPosition2.y = aux -10;
		//Clear the window
		window.clear();

		//Set player properties (some are unnecessary but is useful in case there are changes i.e. the screen changes)
		player.setOrigin(spriteSize.x/2, spriteSize.y);
		player.setPosition(playerPosition.x, playerPosition.y);
		player.setTextureRect(sf::IntRect(spriteSource.x*spriteSize.x, spriteSource.y*spriteSize.y, spriteSize.x, spriteSize.y));

		player2.setOrigin(spriteSize2.x/2, spriteSize2.y);
		player2.setPosition(playerPosition2.x, playerPosition2.y);
		player2.setTextureRect(sf::IntRect(spriteSource2.x*spriteSize2.x, spriteSource2.y*spriteSize2.y, spriteSize2.x, spriteSize2.y));

		srand(time (NULL));

		float temps = (rand()%1 + 4);
		int spawn = (rand()%259 + 1); 
		
		//Set window view, draw and display
		window.draw(background);
		if (melocClock.getElapsedTime().asSeconds() >= temps){
			float melocX = rand()%639, melocY = rand()%480;
			while (!isWhite(image, melocX, melocY)){
				melocX = rand()%639;
				melocY = rand()%480;
			}
			meloc.setTexture(tmeloc);
			meloc.setOrigin(melocSize.x/2, melocSize.y);
			meloc.setPosition(melocX, melocY);
			bayas.push_back(std::make_pair(1,meloc));
			melocClock.restart();
		}

		if (spawnClock.getElapsedTime().asSeconds() >= 1){
			if (spawn%15 == 0){
				float zumoX = rand()%639, zumoY = rand()%480;
				while (!isWhite(image, zumoX, zumoY)){
					zumoX = rand()%639;
					zumoY = rand()%480;
				}
				zumo.setTexture(tzumo);
				zumo.setOrigin(zumoSize.x/2, zumoSize.y);
				zumo.setPosition(zumoX, zumoY);
				bayas.push_back(std::make_pair(0,zumo));
			}


			if (rand() < 0.1 * ((double)RAND_MAX + 1.0)){
				float zrezaX = rand()%639, zrezaY = rand()%480;
				while (!isWhite(image, zrezaX, zrezaY)){
					zrezaX = rand()%639;
					zrezaY = rand()%480;
				}
				zreza.setTexture(tzreza);
				zreza.setOrigin(6, 14);
				zreza.setPosition(zrezaX, zrezaY);
				bayas.push_back(std::make_pair(2,zreza));
			}

            if (rand() < 0.02 * ((double)RAND_MAX + 1.0)){
				float pokeballX = rand()%639, pokeballY = rand()%480;
				while (!isWhite(image, pokeballX, pokeballY)){
					pokeballX = rand()%639;
					pokeballY = rand()%480;
				}
				pokeball.setTexture(tpokeball);
				pokeball.setOrigin(7, 7);
				pokeball.setPosition(pokeballX, pokeballY);
				bayas.push_back(std::make_pair(3,pokeball));
			}

			if ((rand() < 0.0001 * ((double)RAND_MAX + 1.0)) and raichu == false and piedras < 2){
				++piedras;
				float stoneX = rand()%639, stoneY = rand()%480;
				while (!isWhite(image, stoneX, stoneY)){
					stoneX = rand()%639;
					stoneY = rand()%480;
				}
				stone.setTexture(tstone);
				stone.setOrigin(zumoSize.x/2, zumoSize.y);
				stone.setPosition(stoneX, stoneY);
				bayas.push_back(std::make_pair(4,stone));
			}
			spawnClock.restart();
		}
		if(shoot2 and player.getGlobalBounds().intersects(shot2.getGlobalBounds())){ par = true; parClock.restart();}
		else if (shoot and player2.getGlobalBounds().intersects(shot.getGlobalBounds())){par2 = true; parClock.restart();}
		int intersection;
		for (int i = 0; i < bayas.size(); ++i){
			if(player.getGlobalBounds().intersects(bayas[i].second.getGlobalBounds())) intersection = 1;
			else if (player2.getGlobalBounds().intersects(bayas[i].second.getGlobalBounds())) intersection = 2;
			else intersection = 0;
			bool borra = true;
			if (intersection > 0){
				if (bayas[i].first == 0){ //es fugaesfera
					if (intersection == 1){
						speedup = true;
						zumoClock.restart();
					}
					else {
						speedup2 = true;
						zumo2Clock.restart();
					}
				}
				else if (bayas[i].first == 1){ //es baya meloc
					if(intersection == 1) ++melocCount;
					else ++melocCount2;
					std::cout<< melocCount << "-P1-------------P2-" << melocCount2 << std::endl;
				}
				else if (bayas[i].first == 2){ //es baya zreza
					if(intersection == 1) melocCount += 5;
					else melocCount2 += 5;
					std::cout<< melocCount << "-P1-------------P2-" << melocCount2 << std::endl;
				}
				else if (bayas[i].first == 3){ //es pokeball
					if (intersection == 1){
						poke = true;
						raichu = false;
						if(!tplayer.loadFromFile("Images/pokeball.png")) std::cout << "pokeball Not Loaded " << std::endl;
						spriteSize.x = 14; 
						spriteSize.y = 14;
						distx = spriteSize.x/4, disty = spriteSize.y/4;
						pokeClock.restart();
					}
					else {
						poke2 = true;
                        if(!tplayer2.loadFromFile("Images/pokeball.png")) std::cout << "pokeball Not Loaded " << std::endl;
						spriteSize2.x = 14; 
						spriteSize2.y = 14;
						distx2 = spriteSize2.x/4, disty2 = spriteSize2.y/4;
						poke2Clock.restart();
					}
				}
				else if (bayas[i].first == 4){ //es piedra trueno
					if (intersection == 1){
						raichu = true;
						if(!tplayer.loadFromFile("Images/raichu.png")) std::cout << "raichu Not Loaded " << std::endl;
						spriteSize.x = tplayer.getSize().x/4;
						spriteSize.y = tplayer.getSize().y/4;
						distx = spriteSize.x/4, disty = spriteSize.y/4;
					}
					else if (intersection == 2){
						raichu = true;
						if(!tplayer2.loadFromFile("Images/raichu2.png")) std::cout << "raichu Not Loaded " << std::endl;
						spriteSize2.x = tplayer2.getSize().x/4;
						spriteSize2.y = tplayer2.getSize().y/4;
						distx2 = spriteSize2.x/4, disty2 = spriteSize2.y/4;
					}
					else borra = false;
				}
				if (borra) bayas.erase(bayas.begin()+i);
			}
			else window.draw(bayas[i].second);
		}
		if (shoot) window.draw(shot);
		if (shoot2) window.draw(shot2);
		std::string puntuacion, puntuacion2;
		puntuacion = numToString(melocCount);
		puntuacion2 = numToString(melocCount2);
		text1.setString("Player 1: " + puntuacion);
		text2.setString("Player 2: " + puntuacion2);
		window.draw(player);
		window.draw(player2);
		window.draw(text1);
		window.draw(text2);
		window.display();
		bool final = false;
        if (melocCount >= 10){
			std::cout << "PLAYER 1 WON!" << std::endl;
			end_game("Player 1 won!", backgroundSize, window);
			final = true;
		}
		else if (melocCount2 >= 10){
			std::cout << "PLAYER 2 WON!" << std::endl;
			end_game("Player 2 won!", backgroundSize, window);
			final = true;
        }

        if (final or (poke and pokeClock.getElapsedTime().asSeconds() >= 5)){
            poke = false;
            std::cout << "vaig a carregar el personatge 1.... " << std::endl;
            std::cout << "poke val" << poke << std::endl;
            if(raichu) if(!tplayer.loadFromFile("Images/raichu.png")) std::cout << "personatge Not Loaded " << std::endl;
            else{ if(!tplayer.loadFromFile("Images/pikachu.png")) std::cout << "personatge Not Loaded " << std::endl;}
            spriteSize.x = originalSpriteSize.x = tplayer.getSize().x/4;
            spriteSize.y = originalSpriteSize.y = tplayer.getSize().y/4;
        }
        if (final or (poke2 and poke2Clock.getElapsedTime().asSeconds() >= 5)){
            poke2 = false;
            std::cout << "vaig a carregar el personatge 2.... " << std::endl;
            std::cout << "poke2 val" << poke2 << std::endl;
            if(!tplayer2.loadFromFile("Images/pikachu2.png")) std::cout << "personatge Not Loaded " << std::endl;
            spriteSize2.x = originalSpriteSize.x = tplayer2.getSize().x/4;
            spriteSize2.y = originalSpriteSize.y = tplayer2.getSize().y/4;
        }

		if (final){
			raichu = raichu2 = speedup = speedup2 = false;
			poke = poke2 = false;
			shoot = shoot2 = false;
			melocCount = melocCount2 = 0;
			bayas.erase(bayas.begin(), bayas.end());
			final = false;
            player2.setPosition(playerPosition2);
            player.setPosition(playerPosition);
            std::cout << "posicions posades" << std::endl;
		}

        if (par and parClock.getElapsedTime().asSeconds() >= 3) par = false;
        if (par2 and parClock.getElapsedTime().asSeconds() >= 3) par2 = false;
        if (zumoClock.getElapsedTime().asSeconds() >= 10) speedup = false;
        if (zumo2Clock.getElapsedTime().asSeconds() >= 10) speedup2 = false;

        if(poke or par) speed = 20;
        else if (speedup) speed = 220;
        else if(raichu) speed = 150;
        else speed = 100;

        if(poke2 or par2) speed2 = 20;
        else if (speedup2) speed2 = 220;
        else speed2 = 100;

	}
}
