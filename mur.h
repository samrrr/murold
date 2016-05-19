#include "mur.cpp"

typedef struct{
	int food;
	float z[5];
	int t;//1-grass 2-food point 3-food store 4-stone
}poiint;

class POLE
{
private:
	int x, y;
	int murx, mury;
	poiint **i;
	int is_gen;

public:
	POLE()
	{
		i = NULL;
		x = 0;
		y = 0;
		is_gen = 0;
	}
	poiint get(int _x, int _y)
	{
		if (_x < 0 || _x >= x || _y < 0 || _y >= y)
		{
			poiint p;
			p.food = 0;
			p.t = 4;
			p.z[0] = 0;
			p.z[1] = 0;
			p.z[2] = 0;
			p.z[3] = 0;
			p.z[4] = 0;
			return p;
		}
		return i[_x][_y];
	}
	void init(int _x,int _y)
	{
		int r, o, j;

		if (_x < 20 || _x > 1000 || _y < 20 || _y > 1000)
		{
			out.put("POLE: init error");
			return;
		}
		x = _x;
		y = _y;

		is_gen = 1;
		
		i = (poiint**)malloc(sizeof(poiint*)*x);
		for (r = 0; r < x; r++)
			i[r] = (poiint*)malloc(sizeof(poiint)*y);

		for (r = 0; r < x; r++)
			for (o = 0; o < y; o++)
				i[r][o].t = 1;
	}

	void gen()
	{
		int r, o, j;

		if (x < 20 || y < 20)
		{
			out.put("Gen bad");
			return;
		}
		murx = rand() % (x - 19) + 10;
		mury = rand() % (y - 19) + 10;

		for (r = 0; r < x; r++)
			for (o = 0; o < y; o++)
				i[r][o].t = 1;

		for (r = 0; r < (x + 100) * (y + 100) / 1000; r++)
		{
			int xx, yy,rad;

			xx = rand() % (x + 100) - 50;
			yy = rand() % (y + 100) - 50;
			rad = 4 + rand() % 6;

			for (int i1 = -rad; i1 <= rad; i1++)
				for (int i2 = -rad; i2 <= rad; i2++)
				{
					if (xx + i1 >= 0 && yy + i2 >= 0 && xx + i1 < x && yy + i2 < y)
					{
						i[xx + i1][yy + i2].t = 4;
						i[xx + i1][yy + i2].food = 0;
						i[xx + i1][yy + i2].z[0] = 0;
						i[xx + i1][yy + i2].z[1] = 0;
						i[xx + i1][yy + i2].z[2] = 0;
						i[xx + i1][yy + i2].z[3] = 0;
						i[xx + i1][yy + i2].z[4] = 0;
					}
				}
		}
	}

	void clear()
	{
		int r, o, j;
		
		for (r = 0; r < x; r++)
			free(i[r]);
		free(i);

		i = NULL;
		x = 0;
		y = 0;

		is_gen = 0;
	}
};

class MUR{
private:
	int x, y;
	int t;
	char role[100];
	int food;
	char buf[1024];
	float mark_level;
public:
	MUR()
	{
		t = 0;
	}
	MUR(int _x,int _y,int _t)
	{
		t = 1;
		x = _x;
		y = _y;
		mark_level = 1;
		strcpy(role, "base");
	}

	void upd()
	{
		if (t != 1)
			return;

		if (strcmp(role, "base") == 0)
		{
			x = x + rand() % 3 - 1;
			y = y + rand() % 3 - 1;
		}
		

		if (x < 10)
			t = 0;
		if (x < 10)
			t = 0;
		if (x > 290)
			t = 0;
		if (x > 290)
			t = 0;
	}
};


