//
// Created by Lumaio on 7/18/2015.
//

#ifndef SLIDE_GAME_H
#define SLIDE_GAME_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <Thor/Input.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include "Graphics.h"
#include "Enemie.h"

class Game {
private:
	sf::RenderWindow *window;
	thor::ActionMap<std::string> emap;

	Graphics *graphics;
	sf::View camera;

	std::vector<sf::Vector2f> lanes;
	std::vector<Enemie> enemies;

	std::vector<std::string> sayings = {
					"OH YEAH",
					"YOU GOT IT",
					"CCCCCOMBOOO",
					"UNSTOPPABLE",
					"WOW",
					"HOLY SHIT",
					"RIP MY BRAIN"
	};

	std::vector<std::string> dead_sayings = {
					"YOU SUCK",
					"GET OUT",
					"LOSER",
					"GET GOOD",
					"FUCK YOU",
					"KEEP CRYING",
					"GO HOME",
					"KILL YOURSELF"
	};

	float saying_timer = 0;
	int current_saying = 0;
	int current_saying_dead = 0;

	sf::Vector2f player;

	float sine_timer = 0;

	float diff = 1;
	float spawn_timer = 0;

	int to_spawn = 1;

	int score = 0;
	int combo = 0;

	bool dead = false;

	float getCenterOfLanes() {
		int middle = lanes.size()/2-1;
		if (middle % 2 == 0)
			return lanes[middle].y+10;

		return lanes[lanes.size()/2].y;
		//return 0;
	}

public:
	Game();
	~Game()=default;

	void Update(float dt);
	void Render(float dt);

	void Start();

};


#endif //SLIDE_GAME_H
