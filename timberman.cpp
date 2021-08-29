#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

//function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//left or right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	//create video mode
	VideoMode vm(1366, 768);
	
	//create and open window for game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	//background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);
	
	//create tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(567, 0);
	
	//create bee spite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(1366, 480);
	//bee move?
	bool beeActive = false;
	//bee speed
	float beeSpeed = 0.0f;
	
	//create clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeed[NUM_CLOUDS];
	bool cloudActive[NUM_CLOUDS];
	for(int i = 0; i < NUM_CLOUDS; i++){
		clouds[i].setTexture(textureCloud);
		cloudSpeed[i] = 0;
		cloudActive[i] = false;
	}

	//variable time
	Clock clock;
	
	RectangleShape timeBar;
	float timeBarStartWidth = 280;
	float timeBarHeight = 60;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(1366/2.0f - timeBarStartWidth/2.0f, 686);
	
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
	
	//track whether game is running
	bool paused = true;

	//text
	int score = 0;
	
	Text massageText;
	Text scoreText;
	Text fpsText;
	//fonts
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	//set font to the massage, score and fps
	massageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);
	//assign the massage
	massageText.setString("Press Enter to Start!");
	scoreText.setString("Score: 0");
	//choose the size
	massageText.setCharacterSize(52);
	scoreText.setCharacterSize(60);
	fpsText.setCharacterSize(60);
	//choose a color
	massageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	fpsText.setFillColor(Color::White);
	//position the text
	FloatRect textRect = massageText.getLocalBounds();
	massageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height /2.0f);
	massageText.setPosition(1366 / 2.0f, 768 / 2.0f);
	scoreText.setPosition(20, 20);
	fpsText.setPosition(1050, 20);
	//background for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(350, 100));
	rect1.setPosition(10, 10);
	//background for fps
	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(320, 100));
	rect2.setPosition(1035, 10);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set the texture for each branch
	for(int i = 0; i < NUM_BRANCHES; i++){
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the sprite origin to dead centre
		branches[i].setOrigin(156, 23.5);
	} 

	//prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(406, 504);
	side playerSide = side::LEFT;
	
	//prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(420, 603);
	
	//prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(490, 581);
	const float AXE_POSITION_LEFT = 490;
	const float AXE_POSITION_RIGHT = 752;
	
	//prepare the log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(567, 554);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1000;
	
	//control the player input
	bool acceptInput = false;
	
	//prepare the sound
	//chop sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop; 
	chop.setBuffer(chopBuffer); 
	//death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);
	//out of time sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	
	//control the time score drawn
	int lastDrawn = 0;
	
	while(window.isOpen()){

		if(Keyboard::isKeyPressed(Keyboard::Escape)){
			window.close();
		}
		//start the game
		if(Keyboard::isKeyPressed(Keyboard::Return)){
			paused = false;
			//reset time and score
			score = 0;
			timeRemaining = 6;
			
			//make the branc dissappear - starting in the second position
			for(int i = 1; i < NUM_BRANCHES; i++){
				branchPositions[i] = side::NONE;
			}
			//hidden the gravestone
			spriteRIP.setPosition(675, 1400);
			//set player in position
			spritePlayer.setPosition(406, 504);
			
			acceptInput = true;
		}
		//action
		if(acceptInput){
			if(Keyboard::isKeyPressed(Keyboard::Right)){
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(850, 504);
				//update the branches
				updateBranches(score);
				//log
				spriteLog.setPosition(567, 554);
				logSpeedX = -5000;
				logActive = true;
				
				acceptInput = false;
				//play the chop sound
				chop.play();
			}
			if(Keyboard::isKeyPressed(Keyboard::Left)){
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(406, 504);
				//update the branches
				updateBranches(score);
				//log
				spriteLog.setPosition(567, 554);
				logSpeedX = 5000;
				logActive = true;
				
				acceptInput = false;
				//play the chop sound
				chop.play();
			}
		}
		
		Event event;
		while(window.pollEvent(event)){
			if(event.type == Event::KeyReleased && !paused){
				acceptInput = true;
				//hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		
		//update
		window.clear();
		if(!paused){
			//measure time
			Time dt = clock.restart();
			//substract the amount of time remaining
			timeRemaining -= dt.asSeconds();
			//size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if(timeRemaining <= 0.0f){
				//pause the game
				paused = true;
				//change the text massage
				massageText.setString("Out of time!!!");
				//reposition the text
				FloatRect textRect = massageText.getLocalBounds();
				massageText.setOrigin(textRect.left +  textRect.width/2.0f, textRect.top + textRect.height/2.0f);
				massageText.setPosition(1366 / 2.0f, 768 / 2.0f);
				//playing out of time sound
				outOfTime.play();
			}
			//setup the bee
			if(!beeActive){
				//how fast the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;
				//how height the bee
				srand((int)time(0) * 10);
				float height = (rand() % 250) + 250;
				spriteBee.setPosition(1400, height);
				beeActive = true;
			}else{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if(spriteBee.getPosition().x < -100){
					beeActive = false;
				}
			}
			//setup the cloud
			for(int i = 0; i < NUM_CLOUDS; i++){
				if(!cloudActive[i]){
					srand((int)time(0) * i);
					cloudSpeed[i] = (rand() % 200);
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudActive[i] = true;
				}else{
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()), clouds[i].getPosition().y);
					if(clouds[i].getPosition().x > 1366){
						cloudActive[i] = false;
					}
				}
			}
			
			//update the score each 100 frame
			lastDrawn++;
			if(lastDrawn == 100){
				std::stringstream ss;
				ss << "Score: " << score;
				scoreText.setString(ss.str());	
				//draw the fps
				std::stringstream ss2;
				ss2 << "Fps: " << static_cast<int>(1/dt.asSeconds());
				fpsText.setString(ss2.str());
				//reset the last drawn
				lastDrawn = 0;
			}
			
			//update the branch sprites 
			for(int i = 0; i < NUM_BRANCHES; i++){
				float height = i * 105;
				if(branchPositions[i] == side::LEFT){
					//move sprites to the left side
					branches[i].setPosition(411, height);
					//flip the sprites
					branches[i].setRotation(180);
				}else if(branchPositions[i] == side::RIGHT){
					//move sprites to the right side
					branches[i].setPosition(936, height);
					//don't flip the sprites
					branches[i].setRotation(0);
				}else{
					//hide the branch
					branches[i].setPosition(3000, height);
				}
			} 
			//handle the flying log
			if(logActive){
				spriteLog.setPosition(spriteLog.getPosition().x + logSpeedX * dt.asSeconds(), spriteLog.getPosition().y + logSpeedY * dt.asSeconds());
				if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 1400){
					logActive = false;
					spriteLog.setPosition(567, 554);
				}
			}
			//squished???
			if(branchPositions[5] == playerSide){
				paused = true;
				acceptInput = false;
				//draw the gravestone
				spriteRIP.setPosition(420, 603);
				//hide the player
				spritePlayer.setPosition(1400, 504);
				//change the massage to squished
				massageText.setString("SQUISHED!!!");
				FloatRect textRect = massageText.getLocalBounds();
				massageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
				massageText.setPosition(1366/2.0f, 768/2.0f);
				//play death sound
				death.play();
			}
		}
		
		//draw
		window.draw(spriteBackground);

		for(int i = 0; i < NUM_CLOUDS; i++){
			window.draw(clouds[i]);
		}
	
		for(int i = 0; i < NUM_BRANCHES; i++){
			window.draw(branches[i]);
		} 
		
		window.draw(spriteTree); //tree
		
		window.draw(spritePlayer); //player
		
		window.draw(spriteAxe); //axe
		
		window.draw(spriteLog); //log
		
		window.draw(spriteRIP); //grave
	
		window.draw(spriteBee); //bee
		
		window.draw(rect1); //background for the text
		window.draw(rect2);
		
		window.draw(scoreText); //score
		
		window.draw(fpsText); //fps
		
		window.draw(timeBar); //time bar
		
		if(paused){
			window.draw(massageText);
		}
	
		//display
		window.display();
	}
	
	return 0;
}

//function definition
void updateBranches(int seed){
	for(int j = NUM_BRANCHES - 1; j > 0; j--){
		branchPositions[j] = branchPositions[j - 1];
	}
	//spawn new branch at position 0
	//left, right or none
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r){
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
	

