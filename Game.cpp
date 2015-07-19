//
// Created by Lumaio on 7/18/2015.
//

#include "Game.h"

Game::Game()
{
	sf::ContextSettings settings;
	settings.depthBits = 0;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 2;
	settings.minorVersion = 3;

	window = new sf::RenderWindow(sf::VideoMode(320, 240), "Slide", sf::Style::Close, settings);

	graphics = new Graphics(*window);

	camera = sf::View(sf::FloatRect(0, 0, 320, 240));
	//camera.setCenter(320/2, getCenterOfLanes());

	window->setView(camera);

	emap["close"] = thor::Action(sf::Event::Closed);
	emap["add"] = thor::Action(sf::Keyboard::A, thor::Action::PressOnce);
	emap["remove"] = thor::Action(sf::Keyboard::D, thor::Action::PressOnce);

	emap["up"] = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	emap["down"] = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
	emap["left"] = thor::Action(sf::Keyboard::Left, thor::Action::Hold);
	emap["right"] = thor::Action(sf::Keyboard::Right, thor::Action::Hold);

	emap["reset"] = thor::Action(sf::Keyboard::R, thor::Action::PressOnce);

	lanes.emplace_back(sf::Vector2f(0, 30));
	lanes.emplace_back(sf::Vector2f(0, 50));
	lanes.emplace_back(sf::Vector2f(0, 70));

	enemies.emplace_back(Enemie(0, 1, 1));

	player.x = 320/2-5;
	player.y = 1;
}

void Game::Update(float dt)
{
	// Update camera
	camera.setCenter(320/2, getCenterOfLanes());
	sine_timer += dt;
	camera.setRotation((float)sin(sine_timer)*10);
	window->setView(camera);

	if (dead) {
		if (emap.isActive("reset"))
		{
			player.y = 0;
			player.x = 320/2-5;
			while (lanes.size() > 3)
				lanes.pop_back();
			enemies.clear();
			score = 0;
			spawn_timer = 0;
			diff = 1;
			dead = false;
		}
		return;
	}

	/* Just for debugging
	if (emap.isActive("add"))
		lanes.emplace_back(sf::Vector2f(0, lanes[lanes.size()-1].y+20));
	if (emap.isActive("remove"))
		lanes.pop_back();
	*/

	if (emap.isActive("up") && player.y > 0)
		player.y--;
	if (emap.isActive("down") && player.y < lanes.size()-1)
		player.y++;
	if (emap.isActive("left"))
		player.x-=100*dt;
	if (emap.isActive("right"))
		player.x+=100*dt;

	// Add enemies
	spawn_timer += dt;
	if (spawn_timer > diff)
	{
		int flag = rand()%2;
		int flag2 = 0;
		if (flag==1)
			flag2 = 0;
		else
			flag2 = 1;

		int oldflag = 0;

		for (int i = 0; i < 2; i++)
		{
			oldflag = flag2;
			enemies.emplace_back(Enemie(310*flag, rand()%(lanes.size()), flag2));
			while (oldflag == flag2)
				flag2 = rand()%2;
		}

		spawn_timer=0;
	}

	// Update enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemie &e = enemies[i];

		// Player death
		std::cout << player.y << " " << e.y << std::endl;
		if (e.y == player.y)
		{
			if (e.x+10 > player.x && e.x < player.x+10) {
				current_saying_dead = rand()%dead_sayings.size();
				dead = true;
			}
		}

		// Remove if off screen
		if (e.x < 0 || e.x > 320)
		{
			e.dispose(diff);
			enemies.erase(enemies.begin()+i);
			score++;
		}

		diff -= 0.000005;
		if (diff < 0.25)
		{
			//lanes.emplace_back(sf::Vector2f(0, lanes[lanes.size()-1].y+20));
			diff = 1;
			to_spawn++;
		}

		// Update position
		if (e.flag==1)
			e.x+=200*dt;
		else
			e.x-=200*dt;
	}
}

void Game::Render(float dt)
{
	graphics->ClearColor(sf::Color::Black);

	graphics->Print(0, 0, "SCORE: " + std::to_string(score));
	graphics->Print(100, 0, "DIFF: " + std::to_string(diff));

	// Lanes
	for (sf::Vector2f &lane : lanes)
		graphics->Rectangle(lane.x, lane.y, 320, 2);

	// Enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemie &e = enemies[i];
		graphics->Rectangle(e.x, lanes[e.y].y-5, 10, 10, sf::Color::Red);
	}

	// Player
	graphics->Rectangle(player.x, lanes[player.y].y-5, 10, 10, sf::Color::Green);

	if (score % 20 == 0 && score > 0) {
		saying_timer = 3;
		current_saying = rand()%sayings.size();
	}

	if (saying_timer > 0) {
		saying_timer -= dt;
		graphics->Print(50, 100, sayings[current_saying], 24, sf::Color(rand()%255, rand()%255, rand()%255));
	}

	if (dead) {
		graphics->Rectangle(0, 0, 320, 240, sf::Color(0, 0, 0, 200));
		graphics->Print(20, -20, "DEAD", 100);
		graphics->Print(50, -50, "R to restart", 30);
		graphics->Print(50, 100, dead_sayings[current_saying_dead], 24, sf::Color(rand()%255, rand()%255, rand()%255));
	}

}

void Game::Start()
{
	sf::Clock clock;

	while (window->isOpen())
	{

		sf::Time timer = clock.restart();
		float dt = timer.asSeconds();

		emap.update(*window);
		if (emap.isActive("close"))
			window->close();

		Update(dt);
		Render(dt);

		window->display();

	}
}
