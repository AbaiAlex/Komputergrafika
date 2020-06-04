#include <algorithm>
#include <math.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <bevgrafmath2017.h>

using namespace std;
vec3 up = { 0.0,1.0,0.0 };
float r = 20, centrum = 8;
GLsizei winWidth = 600, winHeight = 600;
float s = 5;
mat4 camera;
vec3 C = { 0, 0, s };
GLint keyStates[256];
vec2 dragged = { -1,-1 };
mat4 w2v, projection, rotation;
float step = 0.05;
float Rx = 0.0;
float Ry = 0.0;
vec4 Tp[4][4];
mat4 M;
struct Sides
{
	int I;
	vec3 points[3]; 
	vec3 n; 
	vec3 v; 
	vec3 center;
};
vector<Sides> side;
vec3 Q[4][4] =
{
	{{ -5,-3,-5 },{ -5,3,-2 },{ -5,3,2 },{ -5,-3,5 }},
	{{ -2,3,-5 },{ -2,3,-2 },{ -2,3,2 },{ -2,3,5 } },
	{{ 2,3,-5 },{ 2,3,-2 },{ 2,3,2 },{ 2,3,5 }},
	{{ 5,-3,-5 },{ 5,3,-2 },{ 5,3,2 },{ 5,-3,5 }},
};
vec2 t_Q[4][4];

float b(int i, float t)
{
	switch (i)
	{
	case 0: 
		return -1 * t * t * t + 3 * t * t - 3 * t + 1; 
		break;
	case 1: 
		return 3 * t * t * t - 6 * t * t + 3 * t; 
		break;
	case 2: 
		return -3 * t * t * t + 3 * t * t; 
		break;
	case 3: 
		return t * t * t; 
		break;
	default:
		cout << "b is invalid" << endl;
	}
}

vec4 S(float s, float t)
{
	vec4 result_S = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			result_S += Tp[i][j] * b(i, s) * b(j, t);
		}
	}
	return result_S;
}

void CameraPosition()
{
	vec3 target = { 0.0,0.0,0.0 };
	vec3 start = { float(r * cos(Ry)),Rx, float(r * sin(Ry)) };
	vec3 z = normalize(start - target);
	vec3 x = normalize(cross(up, z));
	vec3 y = normalize(cross(z, x));
	camera = coordinateTransform(start, x, y, z);
}

bool sorting(Sides& a, Sides& b) {
	bool distance = a.n.z < b.n.z;
	return distance;
}

void beziersurface()
{
	side.clear();
	Sides sides;
	int k = 0;
	for (float s = 0; s < 1.0; s += step)
	{
		for (float t = 0; t < 1.0; t += step)
		{
			sides.points[0] = hToIh(S(s, t));
			sides.points[1] = hToIh(S(s, t + step));
			sides.points[2] = hToIh(S(s + step, t + step));
			sides.I = k;
			sides.n = cross(sides.points[1] - sides.points[0], sides.points[2] - sides.points[0]);
			sides.center = { (sides.points[0] + sides.points[1] + sides.points[2]) / 3 };
			sides.v = dist(sides.center, C);
			k++;
			side.push_back(sides);
			
			sides.points[0] = hToIh(S(s, t));
			sides.points[1] = hToIh(S(s + step, t + step));
			sides.points[2] = hToIh(S(s+step , t));
			sides.I = k;
			sides.n = cross(sides.points[1] - sides.points[0], sides.points[2] - sides.points[0]);
			sides.center = { (sides.points[0] + sides.points[1] + sides.points[2]) / 3 };
			sides.v = dist(sides.center, C);
			k++;
			side.push_back(sides);
		}
	}
	sort(side.begin(), side.end(), sorting);
}

void initMatrices()
{
	projection = perspective(s);
	vec2 windowSize = { 2, 2 };
	vec2 windowPosition = { -1, -1 };
	vec2 viewportSize = { 400, 400 };
	vec2 viewportPosition = { winWidth / 2 - viewportSize.x / 2, winHeight / 2 - viewportSize.y / 2 };
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2.0);
	glPointSize(5.0);
	initMatrices();
}

void  Transform()
{
	mat4 M = w2v * projection * camera;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			vec4 PH = ihToH(Q[i][j]);
			Tp[i][j] = M * PH;
			if (Tp[i][j].w != 0)
			{
				t_Q[i][j] = { hToIh(Tp[i][j]).x, hToIh(Tp[i][j]).y };
			}
		}
	}
	glutPostRedisplay();
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < side.size(); i++)
	{
		if (dot( C - side[i].points[0],side[i].n) > 0)
		{
			if (side[i].I%2==0)
			{
				glColor3f(0.0, 0.5, 0.5);
			}
			else
			{
				glColor3f(0.0, 0.8, 0.8);
			}
		}
		else
		{
			if (side[i].I % 2 == 0)
			{
				glColor3f(0.0, 0.4, 1.0);
			}
			else
			{
				glColor3f(0.0, 0.3, 0.6);
			}
		}
		glBegin(GL_POLYGON);
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(side[i].points[j].x, side[i].points[j].y);
		}
		glEnd();
	}
	for (int i = 0; i < 4; i++)
	{
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 4; j++)
		{
			glVertex2d(t_Q[i][j].x, t_Q[i][j].y);
		}
		glEnd();
	}
	for (int i = 0; i < 3; i++)
	{
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		for (int j = 0; j < 4; j++)
		{
			glVertex2d(t_Q[i][j].x, t_Q[i][j].y);
			glVertex2d(t_Q[i + 1][j].x, t_Q[i + 1][j].y);
		}
		glEnd();
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glColor3f(1.0, 0.0, 0.0);
			if (i == dragged.x && j == dragged.y)
				glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_POINTS);
			glVertex2d(t_Q[i][j].x, t_Q[i][j].y);
			glEnd();
		}
	}
}

void display()
{
	CameraPosition();
	Transform();
	beziersurface();
	Draw();
	glutSwapBuffers();
}

vec2 getActivePoint1_v2(GLint sens, GLint x, GLint y)
{
	GLint i, j, s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dist2(vec2(t_Q[i][j].x, t_Q[i][j].y), P) < s)
			{
				return vec2(i, j);
			}
		}
	}
	return vec2(-1, -1);
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	vec2 i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		
		if ((i = getActivePoint1_v2(10, xMouse, winHeight - yMouse)) != vec2(-1, -1))
		{
			dragged = i;
			cout << "selected" << endl;
		}
		else {
			cout << "missed" << endl;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
	{
		if (dragged != vec2(-1, -1)) {
			cout << "dropped" << endl;
		}
		dragged = { -1,-1 };
	}
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = 0;
}

void keyOperations(int value)
{
	int tmp_x = dragged.x;
	int tmp_y = dragged.y;
	if (keyStates['w']) { Rx += 0.05; }
	if (keyStates['s']) { Rx -= 0.05; }
	if (keyStates['e']) { Ry += 0.05; }
	if (keyStates['q']) { Ry -= 0.05; }
	if (keyStates['a']) { r += 0.5; }
	if (keyStates['d']) { r -= 0.5; }
	if (dragged != vec2(-1, -1)) {
		if (keyStates['X'])
		{
			Q[tmp_x][tmp_y].x += 0.05;
		}
		if (keyStates['x'])
		{
			Q[tmp_x][tmp_y].x -= 0.05;
		}
		if (keyStates['Y'])
		{
			Q[tmp_x][tmp_y].y += 0.05;
		}
		if (keyStates['y'])
		{
			Q[tmp_x][tmp_y].y -= 0.05;
		}
		if (keyStates['Z'])
		{
			Q[tmp_x][tmp_y].z += 0.05;
		}
		if (keyStates['z'])
		{
			Q[tmp_x][tmp_y].z -= 0.05;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(5, keyOperations, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bezier felület");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(5, keyOperations, 0);
	glutMainLoop();
	return 0;
}