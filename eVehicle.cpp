#include "eVehicle.h"

eVehicle::eVehicle() {}
eVehicle::~eVehicle(){}

void eVehicle::Init(int Lane, float xValue, int speed_units, int id_texture)
{
	this->Lane = Lane;
	this->xValue = xValue;
	this->id_texture = id_texture;

	switch(this->Lane)
	{
		case LANE_1:
			this->Speed = SPEED_STEP * speed_units;
			this->yValue = Y_L1;
		break;
		case LANE_2:
			this->Speed = SPEED_STEP * speed_units;
			this->yValue = Y_L2;
		break;
		case LANE_3:
			this->Speed = SPEED_STEP * speed_units;
			this->yValue = Y_L3;
		break;
		case LANE_4:
			this->Speed = SPEED_STEP * speed_units;
			this->yValue = Y_L4;
		break;
	}
}
void eVehicle::Draw()
{
	glBindTexture(GL_TEXTURE_2D, id_texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(Lane > LANE_2)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(xValue             , yValue             );
			glTexCoord2f(0.0f, 1.0f); glVertex2f(xValue - CAR_LENGTH, yValue             );
			glTexCoord2f(0.0f, 0.0f); glVertex2f(xValue - CAR_LENGTH, yValue - CAR_WIDTH );
			glTexCoord2f(1.0f, 0.0f); glVertex2f(xValue             , yValue - CAR_WIDTH );
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(xValue             , yValue             );
			glTexCoord2f(0.0f, 1.0f); glVertex2f(xValue + CAR_LENGTH, yValue             );
			glTexCoord2f(0.0f, 0.0f); glVertex2f(xValue + CAR_LENGTH, yValue - CAR_WIDTH );
			glTexCoord2f(1.0f, 0.0f); glVertex2f(xValue             , yValue - CAR_WIDTH );
		glEnd();	
	}
	glBindTexture(GL_TEXTURE_2D,0);

	glDisable(GL_BLEND);
}

void eVehicle::Move()
{
	if(Lane > LANE_2)
	{
		if(xValue > -(VISIBLE_X + 4*CAR_LENGTH) || xValue - CAR_LENGTH < VISIBLE_X + 4*CAR_LENGTH )
		{
			xValue += Speed;
		}
		if(xValue < -(VISIBLE_X + 4*CAR_LENGTH))
			xValue = (VISIBLE_X + CAR_LENGTH) + 4*CAR_LENGTH;
		if(xValue - CAR_LENGTH > VISIBLE_X + 4*CAR_LENGTH)
			xValue = -(VISIBLE_X + 4*CAR_LENGTH);
	}
	else
	{
		if(xValue > -(VISIBLE_X + 4*CAR_LENGTH) )
		{
			xValue -= Speed;
		}
		else
		{
			xValue = VISIBLE_X + 4*CAR_LENGTH;
		}
	}
}

void eVehicle::CalcRealSpeed(float road_speed)
{
	int road_kmh = int(road_speed/SPEED_STEP * 3); // player_speed

	int veh_kmh = int ( (road_speed - abs(Speed)) / SPEED_STEP * 3 );

	veh_kmh = road_kmh - veh_kmh; // diferenta de viteza dintre player si masina

	int result;

	if(Speed == 0)
		result = road_kmh;
	else
	{
		if(Speed > 0)
		{
			result = road_kmh + veh_kmh;
		}
		else
		{
			result = road_kmh - veh_kmh;
		}
	}

	//printf("Player speed: %d Car Speed: %d\n", road_kmh, result ); 

}

void eVehicle::increaseSpeed()
{
	this->Speed += SPEED_STEP;
}

void eVehicle::decreaseSpeed()
{
	this->Speed -= SPEED_STEP;
}
