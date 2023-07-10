#include "Game.h"
#include <string> 

/**
 * Initialize global variables
 *
 * Includes variables for the height and width of the window
 *
 */
void Game::initVariables(){
    this->gameWidth = 800;
    this->gameHeight = 600;
    this->paddleSize = sf::Vector2f(100, 100);
    this->paddleSpeed = 600.f;
    this->isPlaying = false;
    this->ballAngle = 0.f; // TODO
    this->ballRadius = 40.f;
    this->pi = 3.14159f;
    this->ballSpeed = 400.f;
    this->menuOption = 0;
    this->ready = true;
}

/**
 * Creates a new window instance with the global size variables
 */
void Game::initWindow(){
	this->window = new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(gameWidth), static_cast<unsigned int>(gameHeight), 32), "Template",
                            sf::Style::Titlebar | sf::Style::Close);
    this->window->setVerticalSyncEnabled(true);
    if(!this->backgroundTexture.loadFromFile("resources/basicMenu.png"))
        return exit(0);
    this->background = sf::Sprite(backgroundTexture);
}

/**
 * Font file can be found in the resource file
 * 
 * return True if file is found
 */
void Game::initFonts(){
    if (!this->font.loadFromFile("resources/cheeseburger.ttf"))
        return exit(0);
}

/**
 * Creates a global text variable that is meant to display
 * a simple message on the window.
 */
void Game::initMessages(){
    this->defaultMessage.setFont(font);
    this->defaultMessage.setCharacterSize(40);
    this->defaultMessage.setPosition(350.f, 300.f);
    this->defaultMessage.setFillColor(sf::Color::Black);
    this->defaultMessage.setString("Start");
}

/**
 * Creates paddle that moves along the x axis
 */
void Game::initPaddle(){
      // Create the left paddle
    this->paddle.setSize(paddleSize - sf::Vector2f(3, 3));
    this->paddle.setOutlineThickness(3);
    this->paddle.setOutlineColor(sf::Color::Black);
    this->paddle.setFillColor(sf::Color(100, 100, 200));
    this->paddle.setOrigin(paddleSize / 2.f);
    if(!this->paddleTexture.loadFromFile("resources/leftpaddle.png"))
        return exit(0);
    this->paddle.setTexture(&paddleTexture);

    this->dannyTexture.loadFromFile("resources/dannysprite.png");
    this->dannySprite = sf::IntRect(32, 0, 32, 48); //128 x 192
    this->sprite = sf::Sprite(dannyTexture,dannySprite);
}

void Game::initBall(){
    // Create the rum ball
    this->ball.setRadius(ballRadius - 3);
    //this->ball.setOutlineThickness(2);
    this->ball.setOutlineColor(sf::Color::Black);
    this->ball.setFillColor(sf::Color::White);
    this->ball.setOrigin(ballRadius / 1, ballRadius / 1);
    if (!this->balltexture.loadFromFile("resources/rumball.png"))
        return exit(0);
    this->ball.setTexture(&balltexture);
}

/**
 * Polls for left and right arrows to move paddle
 */
void Game::movePaddle(){
    float deltaTime = clock.restart().asSeconds();
    // Move user paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        (sprite.getPosition().x - paddleSize.y / 2 > 5.f)){
        sprite.move(-paddleSpeed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        (paddle.getPosition().x + paddleSize.y / 2 < gameWidth - 5.f)){
        sprite.move(paddleSpeed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){

        ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        (paddle.getPosition().x - paddleSize.y / 2 > 5.f)){
        sprite.move(0.f, -paddleSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        (paddle.getPosition().x + paddleSize.y / 2 < gameWidth - 5.f)){
        sprite.move(0.f, paddleSpeed * deltaTime);
    }
    float factor = ballSpeed * deltaTime;
    ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
}

/**
 * return True if window is still opened
 */
const bool Game::running() const{
	return this->window->isOpen();
}

/**
 * Checks if any key has been pressed and runs an event accordingly
 */
void Game::pollEvents(){
    while (this->window->pollEvent(this->event)){
            // Window closed or escape key pressed: exit
            if ((this->event.type == sf::Event::Closed) ||
               ((this->event.type == sf::Event::KeyPressed) && (this->event.key.code == sf::Keyboard::Escape))){
                this->window->close();
                break;
            }
            if (((this->event.type == sf::Event::KeyPressed) && (this->event.key.code == sf::Keyboard::Space)) ||
                (this->event.type == sf::Event::TouchBegan)){
                if (!this->isPlaying && ready){
                    // (re)start the game
                    this->isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    paddle.setPosition(10.f + paddleSize.x / 2.f, gameHeight - 10.f - paddleSize.y);
                    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

                    // Reset the ball angle
                    do{
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = static_cast<float>(std::rand() % 360) * 2.f * pi / 360.f;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
                else{ // Quit option
                    this->window->close();
                    break;
                }
                    
            }
            // TODO menu screen button
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                if(menuOption == 0)
                    this->menuOption = 1;
                else
                    this->menuOption = (menuOption - 1) % 2; // Loops options
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                this->menuOption = (menuOption + 1) % 2;   
            }
                 
        }
}
/**
 * Checks if the center on the ball goes past any
 * point on the x and y wall axes.
 * Ends game if ball hits bottom line.
 */
void Game::checkWallCollisions(){
    // Check collisions between the ball and the screen
    const std::string inputString = "Press space to restart or\nescape to exit.";
    if (ball.getPosition().x <= 0.f){
        this->ballAngle = pi + ballAngle - static_cast<float>(std::rand() % 20) * pi / 180;
        this->ball.setPosition(0.1f, ball.getPosition().y);
        this->ballSpeed += 40;
    }
    if (ball.getPosition().x > gameWidth){
        this->ballAngle = pi - ballAngle - static_cast<float>(std::rand() % 20) * pi / 180;
        this->ball.setPosition(gameWidth, ball.getPosition().y);
        this->ballSpeed += 40;
    }
    if (ball.getPosition().y - ballRadius < 0.f){
        this->ballAngle = -ballAngle;
        this->ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
        this->ballSpeed += 40;
    }
    if (ball.getPosition().y + ballRadius > gameHeight){
        this->ballAngle = -ballAngle;
        ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
    }
}

/**
 * Checks if ball collides with given shape
 * 
 * param shape - either the paddle or a block
 * param isBlock - shape collided is a block
 */
void Game::checkCollisions(){
    // Check the collisions between the ball and the paddles
    if (ball.getPosition().x - ballRadius < sprite.getPosition().x + paddleSize.x / 2 &&
        ball.getPosition().x - ballRadius > sprite.getPosition().x - paddleSize.x / 2 &&
        ball.getPosition().y + ballRadius - 20.0f > sprite.getPosition().y - paddleSize.y / 2 - 0.1f &&
        ball.getPosition().y - ballRadius + 20.0f <= sprite.getPosition().y + paddleSize.y / 2 + 0.1f){
            this->isPlaying = false;
    }
}

/**
 * Players can only start when it displays "Start"
 */
void Game::checkMenu(){
    if(menuOption == 1){
        this->ready = false;
        this->defaultMessage.setString("Quit");
    }
    else{
        this->ready = true;
        this->defaultMessage.setString("Start");
    }
        
}

void Game::danny(){
    if (this->dannyClock.getElapsedTime().asSeconds() > 0.5f){
      if (dannySprite.left > 64) 
        dannySprite.left = 0;
      else
        dannySprite.left += 32;

      sprite.setTextureRect(dannySprite);
      this->dannyClock.restart();
    }
}

/**
 * Main run file that polls and displays
 */
void Game::rungame(){
    // Handle events
    this->pollEvents();
    this->danny();
    // Clear the window
    this->window->clear(sf::Color(0, 0, 0));
    if (this->isPlaying){
        // Move user paddle
        this->movePaddle();
        this->window->draw(ball);
        this->window->draw(sprite);
        this->checkWallCollisions();
        this->checkCollisions();
        }
    else{
        this->checkMenu();
        this->window->draw(background);
        this->window->draw(defaultMessage);
    }

    // Display things on screen
    this->window->display();
}

/**
 * Default global variables for new game class instance
 */
Game::Game(){
	this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initMessages();
    this->initPaddle();
    this->initBall();
}