#ifndef POINT_H
#define POINT_H

class Point
{
	int x, y;
public:
	int getX() const;
	int getY() const;
	Point();
	Point(int x, int y);
	void set(int x, int y);
	bool operator==(const Point &p2)const;
	bool operator<(const Point &p2)const;
	Point operator-();
	void transformRotate();
	void transformMirrorX();
	void transformMirrorY();
	friend const Point operator+(const Point &a, const Point &b);
	friend const Point operator-(const Point &a, const Point  &b);
};

#endif // !POINT_H