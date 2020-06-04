#include <math.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <bevgrafmath2017.h>
using namespace std;
constexpr int winWidth = 800, winHeight = 600;
GLint dragged = -1;
bool curve1 = true;
bool curve2 = true;
bool curve3 = true;
bool curve4 = true;
vector<vec2> points[5];
GLint keyStates[256];
float b = 0;
int k;
float beta(int k)
{
	if (k == 0) return sqrt(2 + b);
	return sqrt(2 + beta(k - 1));
}
float gamma(int k)
{
	return -1 / (3 * (1 - pow(beta(k), 2)) * (1 + beta(k)));
}
float alpha(int i, int k)
{
	switch (i)
	{
	case 0:
		return (-90 * gamma(k) - 1) / 60;
		break;
	case 1:
		return (90 * gamma(k) + 43) / 60;
		break;
	case 2:
		return (90 * gamma(k) + 17) / 60;
		break;
	case 3:
		return (-90 * gamma(k) + 1) / 60;
		break;
	}
}

void keyOperations() {
	if (keyStates['7'])
	{
		keyStates['7'] = 0;
		if (b <= -2)
		{
			b = -2;
		}
		else {
			b -= 0.01;
		}
		cout << b << endl;
	}
	if (keyStates['9'])
	{
		keyStates['9'] = 0;
		b += 0.01;
		cout << b << endl;
	}
	if (keyStates['8'])
	{
		keyStates['8'] = 0;
		cout << "Enter tension: ";
		cin >> b;
		if (b < -2)
		{
			b = -2;
			cout << "Incorrect tension!" << endl;
		}
		else
		{
			cout << b << endl;
		}

	}
	if (keyStates['5'])
	{
		keyStates['5'] = 0;
		points[0].clear();
	}
	if (keyStates['1'])
	{
		keyStates['1'] = 0;
		if (curve1)
		{
			curve1 = false;
		}
		else
		{
			curve1 = true;
		}
	}
	if (keyStates['2'])
	{
		keyStates['2'] = 0;
		if (curve2)
		{
			curve2 = false;
		}
		else
		{
			curve2 = true;
		}
	}
	if (keyStates['3'])
	{
		keyStates['3'] = 0;
		if (curve3)
		{
			curve3 = false;
		}
		else
		{
			curve3 = true;
		}
	}
	if (keyStates['4'])
	{
		keyStates['4'] = 0;
		if (curve4)
		{
			curve4 = false;
		}
		else
		{
			curve4 = true;
		}
	}
	glutPostRedisplay();
}
void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glPointSize(5);
	glLineWidth(1);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	keyOperations();
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_LINES);
	for (int i = 0; i < winHeight; i += 20)
	{
		glVertex2f(0, i);
		glVertex2f(winWidth, i);
	}
	for (int i = 0; i < winWidth; i += 20)
	{
		glVertex2f(i, 0);
		glVertex2f(i, winHeight);
	}
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points[0].size(); i++)
		glVertex2f(points[0][i].x, points[0][i].y);
	glEnd();
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < points[0].size(); i++)
		glVertex2f(points[0][i].x, points[0][i].y);
	glEnd();

	if (points[0].size() > 3 && b != -1)
	{


		for (int k = 0; k < 4; k++)
		{
			for (int j = 0; j < points[k].size(); j++)
			{
				int j1;
				if (j != 0)
				{
					j1 = j - 1;
				}
				else
				{
					j1 = points[k].size() - 1;
				}
				int j2 = (j + 1) % points[k].size();
				int j3 = (j + 2) % points[k].size();
				points[k + 1].push_back(points[k][j]);
				points[k + 1].push_back(alpha(0, k) * points[k][j1] + alpha(1, k) * points[k][j] + alpha(2, k) * points[k][j2] + alpha(3, k) * points[k][j3]);
				points[k + 1].push_back(alpha(3, k) * points[k][j1] + alpha(2, k) * points[k][j] + alpha(1, k) * points[k][j2] + alpha(0, k) * points[k][j3]);
			}
		}

		glColor3f(1.0, 0.0, 0.0);
		if (curve1)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points[1].size(); i++)
				glVertex2f(points[1][i].x, points[1][i].y);
			glEnd();
		}

		glColor3f(0.0, 1.0, 0.0);
		if (curve2)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points[2].size(); i++)
				glVertex2f(points[2][i].x, points[2][i].y);
			glEnd();
		}

		glColor3f(0.0, 0.0, 1.0);
		if (curve3)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points[3].size(); i++)
				glVertex2f(points[3][i].x, points[3][i].y);
			glEnd();
		}

		glColor3f(1.0, 0.0, 1.0);
		if (curve4)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points[4].size(); i++)
				glVertex2f(points[4][i].x, points[4][i].y);
			glEnd();
		}
		for (int i = 1; i < 5; i++) {
			points[i].clear();
		}
	}
	glutSwapBuffers();
}

GLint getActivePoint1(vector<vec2> p[5], const GLint size, const GLint sens, const GLint x, const GLint y)
{
	GLint s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (int i = 0; i < size; i++)
	{
		if (dist2(p[0][i], P) < s)
		{
			return i;
		}
	}
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		if ((i = getActivePoint1(points, points[0].size(), 10, xMouse, winHeight - yMouse)) != -1)
		{
			dragged = i;
		}
		else
		{
			points[0].push_back(vec2(xMouse, winHeight - yMouse));
		}
	}

	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
	{
		dragged = -1;
	}
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0)
	{
		points[0][dragged].x = xMouse;
		points[0][dragged].y = winHeight - yMouse;
		glutPostRedisplay();
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


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("4.hazi - Abai Alex");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	return 0;
}