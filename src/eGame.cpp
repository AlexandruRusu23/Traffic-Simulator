#include "eGame.h"

eGame::eGame() {}
eGame::~eGame(){}

void eGame::Init()
{
	//setting the view area using glOrtho
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-VISIBLE_X, VISIBLE_X, -VISIBLE_Y, VISIBLE_Y, -1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//initialize objects data and texture
	Data.Load();
	Road.Load(Data.GetID(IMG_ROAD));
	Player.Init(Data.GetID(IMG_PORSCHE));

	newCarButton.InitButton(16.5, -6.5, 3, 3, Data.GetID(IMG_NEW_BUTTON));
	newMercedes.InitButton(16.5, -1.1, 3, 1.8, Data.GetID(IMG_MERCEDES));
	newRetro.InitButton(16.5, -2.9, 3, 1.8, Data.GetID(IMG_RETRO));
	newPolice.InitButton(16.5, -4.7, 3, 1.8, Data.GetID(IMG_POLICE));

	changePlayer.InitButton(12, -6.5, 3, 3, Data.GetID(IMG_CHANGE_PLAYER_BUTTON));
	newPorsche.InitButton(12, -2.9, 3, 1.8, Data.GetID(IMG_PORSCHE));
	newTir.InitButton(11, -4.7, 5, 1.8, Data.GetID(IMG_TIR));

	speedUpPlayer.InitButton(-19.8, -1.2, 3.5, 2, Data.GetID(IMG_PLUS_BUTTON));
	speedDownPlayer.InitButton(-16.3, -1.2, 3.5, 2, Data.GetID(IMG_MINUS_BUTTON));
	cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_OFF));
	aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_ON));
	
	panel.InitButton(-11.3, -1.1, 22, 8.8, Data.GetID(IMG_PANEL));
	aosInfo.InitButton(-8, -1.8, 4, 1, Data.GetID(IMG_AOS_INFO));
	stateInfo.InitButton(-8, -2.8, 3, 1, Data.GetID(IMG_STATE_INFO));
	aosInfoText.InitButton(-3.8, -1.8, 7, 1, Data.GetID(IMG_NOTHING_TEXT));
	stateInfoText.InitButton(-3.8, -2.8, 4, 1, Data.GetID(IMG_SPEEDING_TEXT));
	
	meters.InitButton(-1.5, -7, 5.8, 1, Data.GetID(IMG_METERS));
	plusButton.InitButton(-8, -7, 2, 2, Data.GetID(IMG_PLUS_BUTTON));
	minusButton.InitButton(5.4, -7, 2, 2, Data.GetID(IMG_MINUS_BUTTON));

	playButton.InitButton(-19.8, -7.8, 2, 2, Data.GetID(IMG_PLAY_BUTTON));

	AddCarState = 0;
	laneToChange = -1;
	PlayerOvertake = true;
	PlayerCruising = false;
	gameRunning = true;
	cruiseDistance = 5;
	cruiseSpeed = 0.27;

	cruiseModeReduce = false;
	cruiseModeAdapt = false;
	cruiseModeReturn = true;

	laneSpeed[LANE_1] = 0.32;
	laneSpeed[LANE_1] = 0.32;
	laneSpeed[LANE_3] = 0.20;
	laneSpeed[LANE_4] = 0.20;
}

void eGame::Keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
		break;
		case '1':
			newVehicle.Init(LANE_1, VISIBLE_X + 4, 0.32, Data.GetID(IMG_MERCEDES));
			if(checkFreeSpawnSpace(LANE_1, VISIBLE_X + 4))
				Vehicle.push_back(newVehicle);
		break;
		case '2':
			newVehicle.Init(LANE_2, VISIBLE_X + 4, 0.32, Data.GetID(IMG_RETRO));
			if(checkFreeSpawnSpace(LANE_2, VISIBLE_X + 4))
				Vehicle.push_back(newVehicle);
		break;
		case '3':
			newVehicle.Init(LANE_3, 15, -0.07, Data.GetID(IMG_POLICE));
			if(checkFreeSpawnSpace(LANE_3, 15))
				Vehicle.push_back(newVehicle);
		break;
		case '4':
			newVehicle.Init(LANE_4, 15, -0.07, Data.GetID(IMG_MERCEDES));
			if(checkFreeSpawnSpace(LANE_4, 15))
				Vehicle.push_back(newVehicle);
		break;

		case 'o':
			Road.increaseSpeed();
		break;
		case 'k':
			Road.decreaseSpeed();
		break;

		case '9':
			if(PlayerOvertake == true)
			{
				PlayerOvertake = false;
				PlayerCruising = true;
			}
			else
			{
				PlayerCruising = false;
				PlayerOvertake = true;
			}
		break;

		case 'q':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_1)
					(*it).increaseSpeed();
			}
		break;
		case 'a':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_1)
					(*it).decreaseSpeed();
			}		
		break;
		
		case 'w':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_2)
					(*it).increaseSpeed();
			}
		break;
		case 's':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_2)
					(*it).decreaseSpeed();
			}		
		break;
		case 'e':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_3)
				{
					(*it).increaseSpeed();
					laneSpeed[LANE_3] += 3;
				}
			}
		break;
		case 'd':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_3)
				{
					(*it).decreaseSpeed();
					laneSpeed[LANE_3] -= 3;
				}
			}		
		break;
		case 'r':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_4)
				{
					(*it).increaseSpeed();
					laneSpeed[LANE_4] += 3;
				}
			}
		break;
		case 'f':
			for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
			{
				if((*it).getLane() == LANE_4)
				{
					(*it).decreaseSpeed();
					laneSpeed[LANE_4] -= 3;
				}
			}		
		break;

		case 'z':
			if(Player.getLane() == LANE_1)
				Player.MoveToLane(Player.getLane(), GO_TO_LEFT);
			else	
				Player.MoveToLane(Player.getLane()-1, GO_TO_LEFT);
		break;
		case 'c':
			if(Player.getLane() == LANE_4)
				Player.MoveToLane(Player.getLane(), GO_TO_RIGHT);
			else
				Player.MoveToLane(Player.getLane()+1, GO_TO_RIGHT);
		break;
	}
}

void eGame::Mouse(int button,int state,float x,float y)
{
	if (state == 1) return;
	y = windowHeight - y;
	x = (float)(x / ((float)windowWidth/(VISIBLE_X * 2))) - 20;
	y = (float)(y / ((float)windowHeight/(VISIBLE_Y * 2))) - 10;

	if (newCarButton.insideButton(x,y)) //AddCar button pressed
	{
		AddCarState = 1;
		return;
	}

	if (playButton.insideButton(x, y))
	{
		if(gameRunning == true)
		{
			gameRunning = false;
			playButton.InitButton(-19.8, -7.8, 2, 2, Data.GetID(IMG_PAUSE_BUTTON));
		}
		else
		{
			gameRunning = true;
			playButton.InitButton(-19.8, -7.8, 2, 2, Data.GetID(IMG_PLAY_BUTTON));
		}
	}

	int tir_restriction = 0;

	if (Player.getLength() == TIR_LENGTH)
		tir_restriction = 0.07;


	if (speedUpPlayer.insideButton(x, y))
	{
		if(cruiseSpeed < 0.29)
		{
			cruiseSpeed += 0.01;
			if(Road.getSpeed() < KM_H_110 - tir_restriction)
			{
				Road.increaseSpeed();
				for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
				{
					if((*it).getLane() > LANE_2)
					{
						(*it).decreaseSpeed();
					}
					else
					{
						(*it).increaseSpeed();
					}
				}
			}
		}
	}

	if( Player.insideArea(x, y))
	{
		showPlayerInfo = 1; 
	}

	if( playerInfo.insideButton(x, y))
	{
		showPlayerInfo = 0;
	}

	if (speedDownPlayer.insideButton(x, y))
	{
		if(cruiseSpeed > 0.07)
		{
			cruiseSpeed -= 0.01;
			if(Road.getSpeed() > KM_H_18)
			{
				Road.decreaseSpeed();
				for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
				{
					if((*it).getLane() > LANE_2)
					{
						(*it).increaseSpeed();	
					}
					else
					{
						if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.04)
							(*it).decreaseSpeed();
					}
				}
			}
		}
	}

	if(cruiseButton.insideButton(x,y))
	{
		if(PlayerCruising == true)
		{
			cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_OFF));
			aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_ON));
			PlayerCruising = false;
			PlayerOvertake = true;
		}
		else
		{
			if(laneToChange == -1)
			{	
				cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_ON));
				aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_OFF));
				PlayerCruising = true;
				PlayerOvertake = false;
			}
		}
	}

	if (aosButton.insideButton(x,y))
	{
		if(PlayerOvertake == true)
		{
			aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_OFF));
			cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_ON));
			PlayerOvertake = false;
			PlayerCruising = true;
		}
		else
		{
			aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_ON));
			cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_OFF));
			PlayerOvertake = true;
			PlayerCruising = false;
		}
	}

	if (plusButton.insideButton(x, y))
	{
		if(cruiseDistance < 20)
		{
			cruiseDistance ++;
		}
	}

	if (minusButton.insideButton(x, y))
	{
		if(cruiseDistance > 5)
		{
			cruiseDistance --;
		}
	}

	if (AddCarState == 1)
	{
		if (newRetro.insideButton(x,y))
		{
			carChoice = IMG_RETRO;
			AddCarState = 2;
			return;
		}
		if (newPolice.insideButton(x,y))
		{
			carChoice = IMG_POLICE;
			AddCarState = 2;
			return;
		}
		if (newMercedes.insideButton(x,y))
		{
			carChoice = IMG_MERCEDES;
			AddCarState = 2;
			return;
		}
		return;
	} 	
	
	float spd;

	if (AddCarState == 2)
	{
		AddCarState = 0;

		if(y <= (Y_L1 + 1) && y >= (Y_L1 - 1))
		{	
			spd = Road.getSpeed() + 0.05;
			newVehicle.Init(LANE_1, x, spd, Data.GetID(carChoice));
			if(checkFreeSpawnSpace(LANE_1, x ))
				Vehicle.push_back(newVehicle);
		}
		if(y <= (Y_L2 + 1) && y >= (Y_L2 - 1))
		{	
			spd = Road.getSpeed() + 0.05;
			newVehicle.Init(LANE_2, x, spd, Data.GetID(carChoice));
			if(checkFreeSpawnSpace(LANE_2, x ))
				Vehicle.push_back(newVehicle);
		}
		if(y <= (Y_L3 + 1) && y >= (Y_L3 - 1))
		{
			spd = Road.getSpeed() - laneSpeed[LANE_3];
			spd = -spd;
			newVehicle.Init(LANE_3, x, spd, Data.GetID(carChoice));
			if(checkFreeSpawnSpace(LANE_3, x ))
				Vehicle.push_back(newVehicle);
		}
		if(y <= (Y_L4 + 1) && y >= (Y_L4 - 1))
		{	
			spd = Road.getSpeed() - laneSpeed[LANE_4];
			spd = -spd;
			newVehicle.Init(LANE_4, x, spd, Data.GetID(carChoice));
			if(checkFreeSpawnSpace(LANE_4, x ))
				Vehicle.push_back(newVehicle);
		}
		return;
	}

	if (changePlayer.insideButton(x,y))
	{
		changePlayerSkin = 1;
	}

	if (changePlayerSkin == 1)
	{
		if(newPorsche.insideButton(x,y))
		{
			Player.setID(Data.GetID(IMG_PORSCHE));
			Player.setLength(CAR_LENGTH);
		}
		if(newTir.insideButton(x,y))
		{
			if(Road.getSpeed() <= KM_H_110 - 0.07)
			{
				Player.setID(Data.GetID(IMG_TIR));
				Player.setLength(TIR_LENGTH);
			}
		}
	}

}

void eGame::MouseMotion(float x,float y)
{
	y = windowHeight - y;
	x = (float)(x / ((float)windowWidth/(VISIBLE_X * 2))) - 20;
	y = (float)(y / ((float)windowHeight/(VISIBLE_Y * 2))) - 10;

	if (newCarButton.insideButton(x,y)) //AddCar button pressed
	{
		if (AddCarState == 0) 
			AddCarState = 1;
		return;
	}

	if (changePlayer.insideButton(x,y))
	{
		if(changePlayerSkin == 0)
			changePlayerSkin = 1;
		return;
	}

	if ( (x < 16 || y > -1) && AddCarState == 1)
		AddCarState = 0;

	if( (x<11.5 || y > -1 || x > 16) && changePlayerSkin == 1)
		changePlayerSkin = 0;
}

void eGame::drawUIButtons()
{
	playButton.drawButton();
	//Render AddCar button
	newCarButton.drawButton();

	changePlayer.drawButton();

	speedUpPlayer.drawButton();
	speedDownPlayer.drawButton();
	cruiseButton.drawButton();
	aosButton.drawButton();

	panel.drawButton();
	aosInfo.drawButton();
	stateInfo.drawButton();
	aosInfoText.drawButton();
	stateInfoText.drawButton();
	meters.drawButton();
	plusButton.drawButton();
	minusButton.drawButton();
	cifra1.drawButton();
	cifra2.drawButton();

	if(blinkRatio < 50)
	{
		blinker.drawButton();
		blinkRatio++;
	}
	else
	{
		if(blinkRatio < 100)
			blinkRatio ++;
		else
			blinkRatio = 0;
	}

	if(laneToChange != -1)
	{
		stateInfoText.InitButton(-3.8, -2.8, 4, 1, Data.GetID(IMG_OVERTAKING_TEXT));
	}
	else
	{
		if(PlayerCruising == true)
		{
			stateInfoText.InitButton(-3.8, -2.8, 4, 1, Data.GetID(IMG_CRUISING_TEXT));
		}
		else
		{
			stateInfoText.InitButton(-3.8, -2.8, 4, 1, Data.GetID(IMG_SPEEDING_TEXT));
		}
	}

	if(checkFreeArea(Player.getLane() - 1, LOOK_FRONT, 4*FRONT_LIMIT) && checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 4*FRONT_LIMIT)
			&& Player.getLane() < LANE_4)
	{
		aosInfoText.InitButton(-3.8, -1.8, 10, 1, Data.GetID(IMG_BOTH_LANES_TEXT));
	}
	else
	{
		if(checkFreeArea(Player.getLane() - 1, LOOK_FRONT, 4*FRONT_LIMIT))
			aosInfoText.InitButton(-3.8, -1.8, 10, 1, Data.GetID(IMG_LEFT_LANE_TEXT));
		else
			if(checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 4*FRONT_LIMIT) && Player.getLane() < LANE_4)
				aosInfoText.InitButton(-3.8, -1.8, 10, 1, Data.GetID(IMG_RIGHT_LANE_TEXT));
			else
				aosInfoText.InitButton(-3.8, -1.8, 7, 1, Data.GetID(IMG_NOTHING_TEXT));
	}

	if(showPlayerInfo == 1)
	{
		playerInfo.drawButton();
		numar1.drawButton();
		numar2.drawButton();
		numar3.drawButton();
		kilometru.drawButton();
	}

	//Show car type options
	if (AddCarState == 1)
	{
		newMercedes.drawButton();
		newPolice.drawButton();
		newRetro.drawButton();
	}

	if(changePlayerSkin == 1)
	{
		newPorsche.drawButton();
		newTir.drawButton();
	}
}

void eGame::Render()
{

	if (clock() - fpsclock < 1000000 / FPS) { //60 fps
		return;
	}
	fpsclock = clock();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Road.Draw();

	Player.Draw();

	for(vector <eVehicle>::iterator it=Vehicle.begin(); it!= Vehicle.end(); it++)
	{
		(*it).Draw();
	}
	
	drawUIButtons();

	if(gameRunning == true)
		Update();
	
	glutSwapBuffers();
}

void eGame::Update()
{
	Road.Move();

	for(vector<eVehicle>::iterator it=Vehicle.begin(); it!=Vehicle.end(); it++)
	{
		(*it).Move();
	}

	if(PlayerOvertake == true)
	{
		cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_OFF));
		aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_ON));
		AOS();
	}
	if(PlayerCruising == true)
	{
		aosButton.InitButton(-19.6, -5.5, 6.7, 2, Data.GetID(IMG_AOS_OFF));
		cruiseButton.InitButton(-19.6, -3.3, 6.7, 2, Data.GetID(IMG_CRUISE_BUTTON_ON));
		CRUISE();
	}

	int km = (int) (Road.getSpeed()/SPEED_STEP * 3);
	int nr3 = km%10; km/=10; int nr2 = km%10; km/=10; int nr1 = km%10; km = 0;

	playerInfo.InitButton(Player.getxValue(), Player.getyValue() + 3, 4, 4, Data.GetID(IMG_PLAYER_INFO));
	numar1.InitButton(Player.getxValue() + 0.5, Player.getyValue() + 2.3, 1, 1, Data.GetID(nr1 + 30));
	numar2.InitButton(Player.getxValue() + 1.5, Player.getyValue() + 2.3, 1, 1, Data.GetID(nr2 + 30));
	numar3.InitButton(Player.getxValue() + 2.5, Player.getyValue() + 2.3, 1, 1, Data.GetID(nr3 + 30));
	kilometru.InitButton(Player.getxValue() + 1, Player.getyValue() + 1.3, 2, 1, Data.GetID(IMG_KMH));

	int crs = cruiseDistance;
	int n2 = crs%10; crs/=10; int n1 = crs%10; 

	cifra1.InitButton(-5, -7, 1.6, 2, Data.GetID(n1 + 30));
	cifra2.InitButton(-3.4, -7, 1.6, 2, Data.GetID(n2 + 30));
}

void eGame::Reshape(int w,int h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0,0,w,h);
}

bool eGame::checkFreeSpawnSpace(int lane, float xPos)
{
	if(Player.getLane() == lane)
	{
		if(abs (Player.getxValue() - xPos) < cruiseDistance + 12 )
			return false;
	}
	for(vector<eVehicle>::iterator it=Vehicle.begin(); it!=Vehicle.end(); it++)
	{
		if((*it).getLane() == lane)
		{
			if(abs( (*it).getxValue() - xPos) < cruiseDistance + 12 )
			{
				return false;
			}
		}
	}
	return true;
}

bool eGame::checkFreeArea(int lane, int direction, int limit)
{
	for(vector<eVehicle>::iterator it=Vehicle.begin(); it!=Vehicle.end(); it++)
	{
		if((*it).getLane() == lane)
		{
			if(direction == LOOK_FRONT)
			{
				if(Player.getxValue() < (*it).getxValue())
					if(abs(Player.getxValue() - ((*it).getxValue() - CAR_LENGTH)) < limit )
						return false;
			}
			if(direction == LOOK_BEHIND)
			{
				if(Player.getxValue() > (*it).getxValue())
					if(abs((Player.getxValue() - Player.getLength()) - (*it).getxValue()) < limit )
						return false;	
			}
			if(direction == LOOK_MIDDLE)
			{
				if( Player.getxValue() + limit > ((*it).getxValue() - CAR_LENGTH))
				{
					if((Player.getxValue() - Player.getLength()) - limit < (*it).getxValue())
						return false;
				}
			}
		}
	}
	return true;
}

/*
--------------------------------------
behind  |       middle       | front
--------------------------------------
  behind    | **PLAYER** |   front 
-------------------------------------- 
behind  |     middle         | front

*/

//AOS AI 
void eGame::AOS()
{
	if(Road.getSpeed() < cruiseSpeed)
	{
		if(decreaseSpeedRatio == 5)
		{
			Road.increaseSpeed();
			for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
			{
				if((*it).getLane() > LANE_2)
				{		
					(*it).decreaseSpeed();
				}
				else
				{
					(*it).increaseSpeed();
				}
			}
		}
		decreaseSpeedRatio ++;
		if(decreaseSpeedRatio > 5) decreaseSpeedRatio = 0;
	}

	if(!checkFreeArea(Player.getLane(), LOOK_FRONT, 8*FRONT_LIMIT)) // AOS detect a car in front of player
	{
			switch (Player.getLane())
			{
				case LANE_4:

					if(!checkFreeArea(Player.getLane(), LOOK_FRONT, 4*FRONT_LIMIT))
					{
						Player.setBlinker(-1);
						blinker.InitButton(-10.5, -4.5, 2, 2, Data.GetID(IMG_ARROW_LEFT));
						if (!checkFreeArea(Player.getLane(), LOOK_FRONT, 2*FRONT_LIMIT))
						{
							if(checkFreeArea(Player.getLane()-1, LOOK_FRONT, 4*FRONT_LIMIT) && checkFreeArea(Player.getLane()-1, LOOK_MIDDLE, MIDDLE_LIMIT))
							{
								if(laneToChange == -1)
									laneToChange = Player.getLane()-1;
								if(laneToChange != -1)
								{
									if(Player.getLane() > laneToChange)
										Player.MoveToLane(laneToChange, GO_TO_LEFT);
								}
							}
							else
							{
								if(Road.getSpeed() < laneSpeed[Player.getLane()] + 0.06)
								{
									if(checkFreeArea(Player.getLane()-1, LOOK_MIDDLE, MIDDLE_LIMIT+1))
									{
										if(laneToChange == -1)
											laneToChange = Player.getLane()-1;
										if(laneToChange != -1)
										{
											if(Player.getLane() > laneToChange)
												Player.MoveToLane(laneToChange, GO_TO_LEFT);
										}		
									}
									else
									{
										PlayerCruising = true;
										PlayerOvertake = false;
									}
								}
								else
								{
									PlayerCruising = true;
									PlayerOvertake = false;	
								}
							}
						}
					}

				break;

				case LANE_3:
						
						if(checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 7*FRONT_LIMIT))
						{
							Player.setBlinker(1);
							blinker.InitButton(8, -4.5, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
						}
						else
						{
							if(checkFreeArea(Player.getLane() - 1, LOOK_FRONT, 7*FRONT_LIMIT))
							{
								if(Player.getLength() != TIR_LENGTH)
								{
									Player.setBlinker(-1);	
									blinker.InitButton(-10.5, -4.5, 2, 2, Data.GetID(IMG_ARROW_LEFT));
								}	
							}
						}

						if(!checkFreeArea(Player.getLane(), LOOK_FRONT, 5*FRONT_LIMIT))
						{
							if (!checkFreeArea(Player.getLane(), LOOK_FRONT, 2*FRONT_LIMIT))
							{
								if(checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 4*FRONT_LIMIT) && checkFreeArea(Player.getLane()+1, LOOK_MIDDLE, MIDDLE_LIMIT))
								{
									Player.setBlinker(1);
									blinker.InitButton(8, -4.5, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
									if(laneToChange == -1)
										laneToChange = Player.getLane() + 1;
									if(laneToChange != -1)
									{
										if(Player.getLane() < laneToChange)
											Player.MoveToLane(laneToChange, GO_TO_RIGHT);
									}
								}
								else
								{
									if(Player.getLength() != TIR_LENGTH)
									{
										if(checkFreeArea(Player.getLane()-1, LOOK_FRONT, 8*FRONT_LIMIT) && checkFreeArea(Player.getLane()-1, LOOK_MIDDLE, MIDDLE_LIMIT))
										{
											if(Road.getSpeed() < KM_H_110)
											{
												if(increaseSpeedRatio == 3)
												{
													Road.increaseSpeed();
													for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
													{
														if((*it).getLane() > LANE_2)
														{
															(*it).decreaseSpeed();
														}
														else
														{
															(*it).increaseSpeed();
														}
													}
												}
												increaseSpeedRatio ++;
												if(increaseSpeedRatio > 3) increaseSpeedRatio =0;
											}

											Player.setBlinker(-1);
											blinker.InitButton(-10.5, -4.5, 2, 2, Data.GetID(IMG_ARROW_LEFT));
											if(laneToChange == -1)
												laneToChange = Player.getLane()-1;
											if(laneToChange != -1)
											{
												if(Player.getLane() > laneToChange)
													Player.MoveToLane(laneToChange, GO_TO_LEFT);
											}
										}
										else
										{
											PlayerOvertake = false;
											PlayerCruising = true;
										}
									}
									else
									{
										PlayerOvertake = false;
										PlayerCruising = true;
									}
								}
							}
						}
				break;

				case LANE_2:
					Player.setBlinker(1);
					blinker.InitButton(8, -4.5, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
					if(checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 3/2*FRONT_LIMIT) && checkFreeArea(Player.getLane() + 1, LOOK_MIDDLE, MIDDLE_LIMIT))
					{
						if(laneToChange == -1)
							laneToChange = Player.getLane() + 1;
						if(laneToChange != -1)
						{
							if(Player.getLane() < laneToChange)
								Player.MoveToLane(laneToChange, GO_TO_RIGHT);
						}
					}
					else
					{
						PlayerCruising = true;
						PlayerOvertake = false;
					}

				break;	
			}
	}
	else
	{
		if(Player.getLane() == LANE_2)
		{
			Player.setBlinker(1);
			blinker.InitButton(8, -4.5, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
			if(checkFreeArea(Player.getLane() + 1, LOOK_FRONT, 2*FRONT_LIMIT) && checkFreeArea(Player.getLane() + 1, LOOK_MIDDLE, MIDDLE_LIMIT))
			{
				if(laneToChange == -1)
					laneToChange = Player.getLane() + 1;
				if(laneToChange != -1)
				{
					if(Player.getLane() < laneToChange)
						Player.MoveToLane(laneToChange, GO_TO_RIGHT);
				}
			}
		}
	}

	if(Player.getLane() == LANE_3 && laneToChange == -1)
	{
		if(Road.getSpeed() > cruiseSpeed)
		{
			if(decreaseSpeedRatio == 5)
			{
				Road.decreaseSpeed();
				for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
				{
					if((*it).getLane() > LANE_2)
					{		
						(*it).increaseSpeed();
					}
					else
					{
						if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.04)
							(*it).decreaseSpeed();
					}
				}
			}
			decreaseSpeedRatio ++;
			if(decreaseSpeedRatio > 5) decreaseSpeedRatio = 0;
		}
	}

	if(Player.getLane() == laneToChange)
	{
		laneToChange = -1;
		Player.setBlinker(0);
		blinker.InitButton(30, 30, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
	}
	if(cruiseSpeed == 0.1 && Player.getLane() != LANE_2)
	{
		cruiseSpeed = 0.27;
	}
	if(Road.getSpeed() < laneSpeed[Player.getLane()])
	{
		PlayerCruising = true;
		PlayerOvertake = false;
	}
}

void eGame::CRUISE()
{
	// if AOS didn't complete a maneuvre
	int drct = -1;
	if(laneToChange != -1)
	{
		if(Player.getLane() < laneToChange)
			drct = GO_TO_RIGHT;
		else
			drct = GO_TO_LEFT;
	}
	if(drct != -1)
		Player.MoveToLane(laneToChange, drct);

	if(Player.getLane() == laneToChange)
		laneToChange = -1;

	if(Player.getLane() == LANE_2)
	{
		if(!checkFreeArea(Player.getLane(), LOOK_FRONT, 8*FRONT_LIMIT))
		{
			if(checkFreeArea(Player.getLane() + 1, LOOK_MIDDLE, MIDDLE_LIMIT + 1))
			{
				laneToChange = LANE_3;
			}
			else
			{
				if(Road.getSpeed() > 0.07)
				{
					if(decreaseSpeedRatio == 1)
					{
						Road.decreaseSpeed();
						for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
						{
							if((*it).getLane() > LANE_2)
							{		
								(*it).increaseSpeed();
								(*it).setxValue((*it).getxValue() + 0.05);
							}
							else
							{
								if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.05)
									(*it).decreaseSpeed();
								(*it).setxValue((*it).getxValue() - 0.1);
							}
						}
					}
					decreaseSpeedRatio ++;
					if(decreaseSpeedRatio > 1) decreaseSpeedRatio = 0;
				}
			}
		}
		else
		{
			if(checkFreeArea(Player.getLane() + 1, LOOK_MIDDLE, MIDDLE_LIMIT) && checkFreeArea(Player.getLane() + 1, LOOK_FRONT, FRONT_LIMIT))
			{
				laneToChange = LANE_3;
			}
		}
	}

	//blinker settings
	if(Player.getLane() == LANE_2)
	{
		Player.setBlinker(1);
		blinker.InitButton(8, -4.5, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
	}
	else
	{
		Player.setBlinker(0);
		blinker.InitButton(30, 30, 2, 2, Data.GetID(IMG_ARROW_RIGHT));
	}

	if (checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance) && Road.getSpeed() == laneSpeed[Player.getLane()])
	{
		if(Player.getLength() != TIR_LENGTH)
		{	
			PlayerCruising = false;
			PlayerOvertake = true;
		}
	}

	if (!checkFreeArea(Player.getLane(), LOOK_BEHIND, 2*BEHIND_LIMIT) && Road.getSpeed() < laneSpeed[Player.getLane()])
	{
		cruiseSpeed = 0.27;
	}

	cruiseModeReturn = false;
	cruiseModeReduce = false;
	cruiseModeAdapt = false;

	if(!checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance))
	{
		if(Road.getSpeed() <= laneSpeed[Player.getLane()] && Road.getSpeed() > laneSpeed[Player.getLane()] - 0.03)
		{
			cruiseModeReduce = false;
			cruiseModeAdapt = true;
		}
		else
		{
			cruiseModeReduce = true;
		}
	}
	else
	{
		if(checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance) && Road.getSpeed() < laneSpeed[Player.getLane()])
		{
			cruiseModeReturn = true;
		}
		else
		{
			if(checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance + cruiseDistance/5))
			{
				cruiseModeReturn = true;
			}
		}
	}

	if(Road.getSpeed() == laneSpeed[Player.getLane()] - 0.03)
		cruiseModeAdapt = false;

	if(cruiseModeReduce == true)
	{
		if(!checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance))
		{
			if(Road.getSpeed() > laneSpeed[Player.getLane()] + 0.01)
			{
				if(decreaseSpeedRatio == 5)
				{
					Road.decreaseSpeed();
					for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
					{
						if((*it).getLane() > LANE_2)
						{		
							(*it).increaseSpeed();
						}
						else
						{
							if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.04)
								(*it).decreaseSpeed();
						}
					}
				}
				decreaseSpeedRatio ++;
				if(decreaseSpeedRatio > 5) decreaseSpeedRatio = 0;
			}
		}
	}
	
	if(cruiseModeAdapt == true)
	{
		if(!checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance))
		{
			if(Road.getSpeed() > laneSpeed[Player.getLane()] - 0.03)
			{
				if(decreaseSpeedRatio == 10)
				{
					Road.decreaseSpeed();
					for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
					{
						if((*it).getLane() > LANE_2)
						{		
							(*it).increaseSpeed();
						}
						else
						{
							if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.04)
								(*it).decreaseSpeed();
						}
					}
				}
				decreaseSpeedRatio ++;
				if(decreaseSpeedRatio > 10) decreaseSpeedRatio = 0;
			}
		}
	}

	if(cruiseModeReturn == true)
	{
		if(checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance + cruiseDistance/5))
		{
			if(Road.getSpeed() < cruiseSpeed)
			{
				if(increaseSpeedRatio == 1)
				{
					Road.increaseSpeed();
					for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
					{
						if((*it).getLane() > LANE_2)
						{
							(*it).decreaseSpeed();
						}
						else
						{
							(*it).increaseSpeed();
						}
					}
				}
				increaseSpeedRatio ++;
				if(increaseSpeedRatio > 1) increaseSpeedRatio =0;
			}
			if(Road.getSpeed() > cruiseSpeed)
			{
				if(decreaseSpeedRatio == 1)
				{
					Road.decreaseSpeed();
					for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
					{
						if((*it).getLane() > LANE_2)
						{
							(*it).increaseSpeed();
						}
						else
						{
							if(abs(Road.getSpeed() - (*it).getSpeed()) > 0.04)
								(*it).decreaseSpeed();
						}
					}
				}
				decreaseSpeedRatio ++;
				if(decreaseSpeedRatio > 1) decreaseSpeedRatio =0;
			}
		}
		else
		{
			if(checkFreeArea(Player.getLane(), LOOK_FRONT, cruiseDistance))
			{
				if(Road.getSpeed() < laneSpeed[Player.getLane()])
				{
					if(increaseSpeedRatio == 2)
					{
						Road.increaseSpeed();
						for(vector<eVehicle>::iterator it= Vehicle.begin(); it!=Vehicle.end(); it++)
						{
							if((*it).getLane() > LANE_2)
							{
								(*it).decreaseSpeed();
							}
							else
							{
								(*it).increaseSpeed();
							}
						}
					}
					increaseSpeedRatio ++;
					if(increaseSpeedRatio > 2) increaseSpeedRatio =0;
				}
			}
		}
	}
}