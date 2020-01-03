#pragma once

class Shape
{
public:


	float xPos;
	float yPos;
	float zPos;

	float * mesh;

	Shape()
	{
		xPos = 0.0f;
		yPos = 0.0f;
		zPos = -1.0f;
	}

	virtual void getVerterxPtr() = 0;

};

class triangle : public Shape
{
public:
	float triangleV[9] = {
		0.05f, -0.05f, 0.0f,
		0.0f, 0.05f, 0.0f,
		-0.05f, -0.05f, 0.0f
	};

	virtual void getVerterxPtr() {
		mesh = &(triangleV[0]);
	}
};

class square : public Shape
{
public:
	float squareV[18] = {
		0.05f, 0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		-0.05f, -0.05f, 0.0f,

		-0.05f, -0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		0.05f, 0.05f, 0.0f
	};
	square() {

	}

	virtual void getVerterxPtr() {
		mesh = &(squareV[0]);
	}
};

class diamond : public Shape
{
public:
	float diamondV[18] = {
		0.0f, 0.05f, 0.0f,
		-0.05f, 0.0f, 0.0f,
		0.0f, -0.05f, 0.0f,

		0.0f, 0.05f, 0.0f,
		0.0f, -0.05f, 0.0f,
		0.05f, 0.0f, 0.0f
	};

	diamond() {

	}
	virtual void getVerterxPtr() {
		mesh = &(diamondV[0]);
	}
};

float matrix1[16];
float matrix2[16];
float matrix3[16];

void loadIdentity(float * m)
{

	for (int i = 0; i < 16; i++)
	{
		if (i % 5 == 0) //Checks to see if the numbers are multiple of 5's. 0,5,10,15,20 etc
		{
			m[i] = 1;//If so, make them numbers 1
		}
		else
		{
			m[i] = 0;//If not, leave them as 0
		}
	}
}

void translation(float * m, float x, float y, float z)
{
	for (int i = 0; i < 16; i++)
	{
		if (i % 5 == 0) //Checks to see if the numbers are multiple of 5's. 0,5,10,15,20 etc
		{
			m[i] = 1;//If so, make them numbers 1
		}
		else
		{
			m[i] = 0;//If not, leave them as 0
		}
	}
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

void multiplication(float * result, float * m1, float * m2)
{
	result[0] = (m1[0] * m2[0]) + (m1[4] * m2[1]) + (m1[8] * m2[2]) + (m1[12] * m2[3]);
	result[1] = (m1[1] * m2[0]) + (m1[5] * m2[1]) + (m1[9] * m2[2]) + (m1[13] * m2[3]);
	result[2] = (m1[2] * m2[0]) + (m1[6] * m2[1]) + (m1[10] * m2[2]) + (m1[14] * m2[3]);
	result[3] = (m1[3] * m2[0]) + (m1[7] * m2[1]) + (m1[11] * m2[2]) + (m1[15] * m2[3]);

	result[4] = (m1[0] * m2[4]) + (m1[4] * m2[5]) + (m1[8] * m2[6]) + (m1[12] * m2[7]);
	result[5] = (m1[1] * m2[4]) + (m1[5] * m2[5]) + (m1[9] * m2[6]) + (m1[13] * m2[7]);
	result[6] = (m1[2] * m2[4]) + (m1[6] * m2[5]) + (m1[10] * m2[6]) + (m1[14] * m2[7]);
	result[7] = (m1[3] * m2[4]) + (m1[7] * m2[5]) + (m1[11] * m2[6]) + (m1[15] * m2[7]);

	result[8] = (m1[0] * m2[8]) + (m1[4] * m2[9]) + (m1[8] * m2[10]) + (m1[12] * m2[11]);
	result[9] = (m1[1] * m2[8]) + (m1[5] * m2[9]) + (m1[9] * m2[10]) + (m1[13] * m2[11]);
	result[10] = (m1[2] * m2[8]) + (m1[6] * m2[9]) + (m1[10] * m2[10]) + (m1[14] * m2[11]);
	result[11] = (m1[3] * m2[8]) + (m1[7] * m2[9]) + (m1[11] * m2[10]) + (m1[15] * m2[11]);

	result[12] = (m1[0] * m2[12]) + (m1[4] * m2[13]) + (m1[8] * m2[14]) + (m1[12] * m2[15]);
	result[13] = (m1[1] * m2[12]) + (m1[5] * m2[13]) + (m1[9] * m2[14]) + (m1[13] * m2[15]);
	result[14] = (m1[2] * m2[12]) + (m1[6] * m2[13]) + (m1[10] * m2[14]) + (m1[14] * m2[15]);
	result[15] = (m1[3] * m2[12]) + (m1[7] * m2[13]) + (m1[11] * m2[14]) + (m1[15] * m2[15]);
}

class slotMachine {
public:
	std::vector<Shape*> slots;
	//dynamic_cast<slot*>(triangle, square, diamond);

	bool isSpinning;
	slotMachine()
	{
		slots.push_back(new triangle());
		slots.push_back(new square());
		slots.push_back(new diamond());

		isSpinning = false;
		moveReels();

	}

	void draw()
	{

		for (int i = 0; i < 3; i++)
		{
			slots[i]->getVerterxPtr();


			glVertexPointer(3, GL_FLOAT, 0, slots[i]->mesh);

			loadIdentity(matrix1);
			glLoadIdentity();
			glTranslatef(slots[i]->xPos, slots[i]->yPos, slots[i]->zPos);
			translation(matrix2, slots[i]->xPos, slots[i]->yPos, slots[i]->zPos);
			glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	}

	void moveReels()
	{
		slots[0]->xPos = -0.2f;
		slots[0]->yPos = 0.0f;
		slots[0]->zPos = -1.0f;

		slots[1]->xPos = 0.0f;
		slots[1]->yPos = 0.0f;
		slots[1]->zPos = -1.0f;

		slots[2]->xPos = 0.2f;
		slots[2]->yPos = 0.0f;
		slots[2]->zPos = -1.0f;
	}
};
