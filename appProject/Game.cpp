#include "Game.h"
#include <assert.h>
#include "Square.h"
#include <iostream>
#include "Line.h"
#include "LRight.h"
#include "LLeft.h"
#include "Shape.h"
#include<stdlib.h>

#include "ollo.h"
#include "TLeft.h"
#include "TRight.h"

#include "TimerClock.hpp"
#include <thread>


#include <chrono>
#include <thread>

using namespace std;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;
TimerClock tc;
double now = 0;


using namespace std;
void Game::setShape()
{	int number= rand() % 7  + 1;
	if (atBottom) {
		switch (number)
		{
		case 1:shape = new Square(); break;
		case 2:shape = new Line(); break;
		case 3:shape = new LRight(); break;
		case 4:shape = new LLeft(); break;
		case 5:shape = new ollo(); break;
		case 6:shape = new TLeft(); break;
		default:shape = new TRight(); break;
			break;
		}
		
		
		atBottom = false;
	}
	//Check the previous shape if it has stopped moving, and randomly generate a new shape,set atbottom to false	
}
void Game::checkBottom()
{   //supprimer plus tard
	for (int i = 0; i < 4; i++)
		if (bottom[shape->getCells()[i].getX()] <= shape->getCells()[i].getY()){
			atBottom = true;
			for (int j = 0; j < 4; j++) {
				if (bottom[shape->getCells()[j].getX()] >= shape->getCells()[j].getY())
					bottom[shape->getCells()[j].getX()] = (shape->getCells()[j].getY()-1);
			}
			for (int k = 0; k < 10; k++)
				std::cout << bottom[k] << ",";
			std::cout << endl;
			break;
		}

}
void Game::Controll()
{
	sleep_for(100ms);
	if (IsKeyDown(KEY_RIGHT)){
		if(shape->right()<9)
			shape->getInput(3);
	}
	if (IsKeyDown(KEY_LEFT)){
		if (shape->left() > 0)
		shape->getInput(2);
	}
	if (IsKeyDown(KEY_UP)) shape->getInput(4);
	if (IsKeyDown(KEY_DOWN)) shape->getInput(1);
	
}
void Game::Controll2()
{
	int ct = tc.getTimerSecond();

	if (ct - now > 0)
	{
		if (ct <= 5)//level 1 in 5 sec
		{
			now++;//down once per 1 sec 
		}
		else if (ct <= 10)// level up after 5 sec
		{
			now += 0.5;//down once per 0.5sec
		}
		else// level up after 10 sec
		{
			now += 0.1;//down once per 0.1sec
		}

		shape->getInput(1);
	}
}
Shape* Game::getRandomShape()
{
	return nullptr;
}
/*
The main purpose of this function is to animate when our chessboard actually changes.
But whether our chessboard can change needs to be judged by the CheckCells function.
So, you will judge all the changes we can make in CheckCell, because CheckCell belongs to chessboard class, 
which contains all the information of our chessboard.
When you successfully implement this function, the CheckBottom function in this class can be deleted,
because it is only used to judge a situation of the chessboard.
*/
void Game::animation(std::vector<Vec2<int>> lastpos, Shape* shape, Board* board)
{	//V�rifiez l'�tat de la carte avant de vous d�placer.
	//S'il est possible de se d�placer, d�placez-vous, sinon revenez � la position d'origine
	if (board->CheckCells(shape->getCells())==1) {
		for (int i = 0; i < 4; i++)
			if (shape->getCells()[i].getY() >= 0)
				(*board).SetCell(shape->getCells()[i], WHITE);
		for (int i = 0; i < 4; i++) {
			bool mark = false;
			for (int j = 0; j < 4; j++) {
				if (lastpos[i] == shape->getCells()[j])
				{

					mark = true;
				}
			}
			if (mark == false)
				if (lastpos[i].getX() >= 0 && lastpos[i].getY() >= 0)
					(*board).SetCell(lastpos[i], RED);
		}
	}
	else {
		if (board->CheckCells(shape->getCells()) == 2)
			//sinon revenez � la position d'origine
			shape->setCells(lastpos);
		else
		{	//Si le bas touche une autre forme, placez-la sur le plateau
			for (int j = 0; j < 4; j++)
				if (shape->getCells()[j].getY() >= 0)
					(*board).SetCell(shape->getCells()[j], WHITE);
				else
					std::cout << "game over" << endl;

		}
	}
}
Game::Game(int width, int height, std::string title)
	:board({ 200,120 }, { 10, 20 }, 20, 2)
{	
	assert(!GetWindowHandle());
	InitWindow(width, height, title.c_str());
	SetTargetFPS(60);
	for (int iY = 0; iY < 20; iY++)
		for (int iX = 0; iX < 10; iX++)
		{
			board.SetCell({ iX, iY }, RED);
		}

	shape = new Square();
	score = 0;
	time = 0;
	bottom.resize(10);
	bottom.assign(10, 19);

}

Game::~Game() noexcept
{	
	
	assert(GetWindowHandle());
	CloseWindow();
}

void Game::Tick()
{	
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

bool Game::GameShouldClose() const
{

	return WindowShouldClose();
}

void Game::Draw()
{
	

	ClearBackground(BLACK);
	//tache 2 Info-bulle score next shape level
	//
	board.Draw();


	
}

void Game::Update()
{
	setShape();
	if (!atBottom) {
		std::vector<Vec2<int>> lastpos = shape->getCells();
		//Timer
		Controll2();
		Controll();
		// changer plus tard
		checkBottom();
		if (!atBottom)
			animation(lastpos, shape, &board);
		else {
			animation(lastpos, shape, &board);
			for (int j = 0; j < 4; j++)
				if (shape->getCells()[j].getY() >= 0)
					board.SetCell(shape->getCells()[j], WHITE);
				else
					std::cout << "game over" << endl;
		}
		// changer plus tard
	}

		
	
	
	
}
