#include "eVehicle.h"

eVehicle::eVehicle() {}
eVehicle::~eVehicle(){}

void eVehicle::Init(int Lane, float xValue,float speed_units, int id_texture)
{
	this->Lane = Lane;
	this->xValue = xValue;
	this->id_texture = id_texture;

	switch(this->Lane)
	{
		case LANE_1:
			this->Speed = speed_units;
			this->yValue = Y_L1;
		break;
		case LANE_2:
			this->Speed = speed_units;
			this->yValue = Y_L2;
		break;
		case LANE_3:
			this->Speed = speed_units;
			this->yValue = Y_L3;
		break;
		case LANE_4:
			this->Speed = speed_units;
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
		if(xValue >= -3*VISIBLE_X || xValue - CAR_LENGTH <= 3*VISIBLE_X)
		{
			xValue += Speed;
		}
		if(xValue < -3*VISIBLE_X)
			xValue = 3*VISIBLE_X;
		if(xValue - CAR_LENGTH > 3*VISIBLE_X)
			xValue = -3*VISIBLE_X;
	}
	else
	{
		if(xValue > -4*VISIBLE_X)
		{
			xValue -= Speed;
		}
		else
		{
			xValue = 4*VISIBLE_X;
		}
	}
}

void eVehicle::increaseSpeed()
{
	this->Speed += SPEED_STEP;
}

void eVehicle::decreaseSpeed()
{
	this->Speed -= SPEED_STEP;
}
