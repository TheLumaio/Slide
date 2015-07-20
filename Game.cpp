//
// Created by Lumaio on 7/18/2015.
//

#include "Game.h"

Game::Game()
{
	sf::ContextSettings settings;
	settings.depthBits = 0;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 2;
	settings.minorVersion = 3;

	window = new sf::RenderWindow(sf::VideoMode(320, 240), "Slide", sf::Style::Close, settings);
	graphics = new Graphics(*window);

	camera = sf::View(sf::FloatRect(0, 0, 320, 240));
	window->setView(camera);

	p_texture.loadFromFile("particle.png");
	emitter.setEmissionRate(100);
	emitter.setParticleLifetime(sf::seconds(5));
	emitter.setParticleColor(sf::Color(150, 150, 150, 100));

	system.setTexture(p_texture);
	system.addEmitter(thor::refEmitter(emitter));
	system.addAffector( thor::TorqueAffector(100.f) );
	system.addAffector( thor::ForceAffector(sf::Vector2f(0.f, 100.f)) );

	velocity = thor::PolarVector2f(0, -90);

	music.openFromFile("track.ogg");
	music.setVolume(25);
	music.setLoop(true);
	music.play();

	emap["close"]  = thor::Action(sf::Event::Closed);
	emap["add"] 	 = thor::Action(sf::Keyboard::A,     thor::Action::PressOnce);
	emap["remove"] = thor::Action(sf::Keyboard::D,     thor::Action::PressOnce);
	emap["up"] 		 = thor::Action(sf::Keyboard::Up,    thor::Action::PressOnce);
	emap["down"]   = thor::Action(sf::Keyboard::Down,  thor::Action::PressOnce);
	emap["left"]   = thor::Action(sf::Keyboard::Left,  thor::Action::Hold);
	emap["right"]  = thor::Action(sf::Keyboard::Right, thor::Action::Hold);
	emap["reset"]  = thor::Action(sf::Keyboard::R,     thor::Action::PressOnce);

	lanes.emplace_back(sf::Vector2f(0, 30));
	lanes.emplace_back(sf::Vector2f(0, 50));
	lanes.emplace_back(sf::Vector2f(0, 70));

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

	// Retry
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

	// Update score
	if (score > high)
		high = score;

	// Movement
	if (emap.isActive("up") && player.y > 0)
		player.y--;
	if (emap.isActive("down") && player.y < lanes.size()-1)
		player.y++;
	if (emap.isActive("left"))
		player.x-=100*dt;
	if (emap.isActive("right"))
		player.x+=100*dt;

	// Add enemies ---------- This is bad, I might fix it later
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
		if (e.y == player.y)
		{
			if (e.x+10 > player.x && e.x < player.x+10) {
				current_saying_dead = rand()%dead_sayings.size();
				saveScore();
				saying_timer = 0;
				current_saying = 0;
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

		// Speed up after a while
		if (diff > 0.25)
			diff -= 0.000005;

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

	// Particles
	window->draw(system);

	graphics->Rectangle(0,15, 320, 75, sf::Color(0, 0, 0, 200));

	// Display score
	graphics->Print(50, 0, "SCORE: " + std::to_string(score));
	graphics->Print(150, 0, "BEST: " + std::to_string(high));

	// Lanes
	for (sf::Vector2f &lane : lanes)
		graphics->Rectangle(lane.x, lane.y, 320, 2);

	// Enemies
	for (Enemie &e : enemies)
		graphics->Rectangle(e.x, lanes[e.y].y-5, 10, 10, sf::Color::Red);

	// Player
	graphics->Rectangle(player.x, lanes[player.y].y-5, 10, 10, sf::Color::Green);

	// Score messages
	if (score % 20 == 0 && score > 0) {
		saying_timer = 3;
		current_saying = rand()%sayings.size();
	}
	if (saying_timer > 0) {
		saying_timer -= dt;

		graphics->Print(50+rand2(5), 100+rand2(5), sayings[current_saying], 24, sf::Color(rand()%255, rand()%255, rand()%255));
		graphics->Print(50+rand2(5), 100+rand2(5), sayings[current_saying], 24, sf::Color(rand()%255, rand()%255, rand()%255));
	}


	// Dead
	if (dead) {
		graphics->Rectangle(0, 0, 320, 240, sf::Color(0, 0, 0, 200));
		graphics->Print(20, -20, "DEAD", 100);
		graphics->Print(50, -50, "R to restart", 30);

		graphics->Print(50+rand2(5), 100+rand2(5), dead_sayings[current_saying_dead], 24, sf::Color(rand()%255, rand()%255, rand()%255));
		graphics->Print(50+rand2(5), 100+rand2(5), dead_sayings[current_saying_dead], 24, sf::Color(rand()%255, rand()%255, rand()%255));
	}

}

void Game::Start()
{
	sf::Clock clock;

	while (window->isOpen())
	{

		sf::Time timer = clock.restart();
		float dt = timer.asSeconds();

		system.update(timer);

		velocity.r = rand()%360;
		velocity.phi+=0.1;

		emitter.setParticlePosition(sf::Vector2f(player.x+5, lanes[player.y].y));
		emitter.setParticleVelocity(thor::Distributions::deflect(velocity, rand()%255));

		float color = rand()%255;
		emitter.setParticleColor(sf::Color(color, color, color));

		emap.update(*window);
		if (emap.isActive("close"))
			window->close();

		Update(dt);
		Render(dt);

		window->display();

	}
}

bool Game::fileExists(const std::string& filename)
{
	struct stat buf;
	return stat(filename.c_str(), &buf) != -1;
}

void Game::loadScore()
{
	if (!fileExists("score")) return;
	std::string hsc;
	std::ifstream file;
	file.open("score");
	file >> hsc;
	file.close();
	high = std::stoi(hsc);
}

void Game::saveScore()
{
	std::ofstream file;
	file.open("score");
	file << high;
	file.close();
}

float Game::rand2(float offset)
{
	return -offset+(offset*2)*((float)rand())/RAND_MAX;
}
