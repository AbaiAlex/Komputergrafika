#include <math.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <bevgrafmath2017.h>

using namespace std;
float value;
constexpr int winWidth = 800, winHeight = 600;
int index;
int p=2;
int n;
GLint keyStates[256];
GLint dragged = -1;
vec2 curve;
vec2 joint;
vector<vec2> points;
vector<float> t_i;
vector<float> w;

void printnode()
{
	for (int i = 0; i < t_i.size(); i++)
	{
		cout << t_i[i] << " | ";
	}
	cout << endl;
}

void keyOperations()
{
	if (keyStates['r'])
	{
		keyStates['r'] = 0;
		printnode();
	}
	if (keyStates['f'])
	{
		keyStates['f'] = 0;
		for (int i = 0; i < points.size(); i++)
		{
			cout << "The weight of the " << i <<" point is " << w[i] << endl;
		}
	}
	if (keyStates['-'] && p > 1)
	{
		keyStates['-'] = 0;
		p--;
		t_i.pop_back();
		cout << "p =" << p << endl;
	}
	if (keyStates['+'] && p < points.size()-1)
	{
		keyStates['+'] = 0;
		p++;
		t_i.push_back(t_i.size());
		cout << "p ="<< p << endl;
	}
	if (keyStates['e'])
	{
		keyStates['e'] = 0;
		
		cout << "Enter the node index: ";
		cin >> index;
		if (index<0 || index>t_i.size()-1)
		{
			cout << "Incorrect index!" << endl;
		}
		else{
			cout << "Enter the node value: ";
			cin >> value;
			if (index != 0)
			{
				if (index != t_i.size()-1)
				{
					if (value<t_i[index - 1] || value>t_i[index + 1])
					{
						cout << "Incorrect value!" << endl;
					}
					else
					{
						t_i[index] = value;
						printnode();
					}
				}
				else
				{
					if (value < t_i[index - 1])
					{
						cout << "Incorrect value!" << endl;
					}
					else
					{
						t_i[index] = value;
						printnode();
					}
				}
			}
			else
			{
				if (value < 0 || value > t_i[index + 1])
				{
					cout << "Incorrect value!" << endl;
				}
				else
				{
					t_i[index] = value;
					printnode();
				}
			}
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
	glEnable(GL_POINT_SMOOTH);
	t_i.push_back(t_i.size());
	t_i.push_back(t_i.size());
	printnode();
}

float N(int i, int p, float t)
{
	float result_N,denominator_1,denominator_2;
	if (p == 0 )
	{
		if (t_i[i] <= t && t < t_i[i + 1]) 
		{
			return 1;
		}
		else
		{
		return 0;
		}
	}
	denominator_1 = t_i[i + p] - t_i[i];
	denominator_2 = t_i[i + p + 1] - t_i[i + 1];
	if (denominator_1 !=0 && denominator_2!=0)
	{
		result_N = ((t - t_i[i]) / denominator_1) * N(i, p - 1, t) + ((t_i[i + p + 1] - t )/ denominator_2) * N(i + 1, p - 1, t);
	}
	else if(denominator_1 != 0 && denominator_2 == 0)
	{
		result_N = ((t - t_i[i] )/ denominator_1) * N(i, p - 1, t);
	}
	else if (denominator_2 != 0 && denominator_1 == 0)
	{
		result_N = ((t_i[i + p + 1] - t )/ denominator_2) * N(i + 1, p - 1, t);
	}
	else
	{
		result_N = 0;
	}
	return result_N;
}

float R(int i, int p, float t)
{
	float result_R,denominator=0;
	for (int j = 0; j < n; j++) 
	{	
		denominator += (N(j,p,t)*w[j]) ;
	}
	if (denominator == 0)
	{
		return 0;
	}
	else 
	{
		result_R = N(i, p, t) * w[i] / denominator;
		return result_R;
	}
}

vec2 Q(float t)
{
	vec2 result_Q = { 0,0 };
	for (int i = 0; i < n; i++)
	{
		result_Q += R(i, p, t) * points[i];
	}
	return result_Q;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	n = points.size();
	keyOperations();

	//kontrollpoligon
	glLineWidth(1.0);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	//kontrollpontok
	glPointSize(5.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	if (n > 2) {
		glLineWidth(2.0);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		for (float t=t_i[p];t<=t_i[n];t+=0.01)
		{
			curve = Q(t);
			glVertex2f(curve.x, curve.y);
		}
		glEnd();

		glPointSize(3.0);
		glColor3f(1.0, 0.8, 0.0);
		glBegin(GL_POINTS);
		for (int t = t_i[p + 1]; t < t_i[w.size()]; t++)
		{
			joint = Q(t);
			glVertex2d(joint.x, joint.y);
		}
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

GLint getActivePoint1(const vector<vec2> p, const GLint size, const GLint sens, const GLint x, const GLint y)
{
	GLint s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (int i = 0; i < size; i++) 
	{
		if (dist2(p[i], P) < s) 
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
		if ((i = getActivePoint1(points, points.size(), 10, xMouse, winHeight - yMouse)) != -1) 
		{
			dragged = i;
		} 
		else 
		{
			points.push_back(vec2(xMouse, winHeight - yMouse));
			w.push_back(1);
			t_i.push_back(t_i.size());
			if (points.size() == 3)
			{
				t_i.push_back(t_i.size());
			}
			if (t_i[t_i.size() - 2] > t_i[t_i.size() - 1])
			{
				t_i[t_i.size() - 1] = t_i[t_i.size() - 2] + 1;
			}
			printnode();
		}
	}
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) 
	{
		dragged = -1;
	}
	if (button == 3 && action == GLUT_UP) {
		if ((i = getActivePoint1(points, points.size(), 8, xMouse, winHeight - yMouse)) != -1)
		{
			w[i] += 0.1;
			cout << "point=" << i << " and weight=" << w[i] << endl;
		}
	}
	if (button == 4 && action == GLUT_UP) {
		if ((i = getActivePoint1(points, points.size(), 8, xMouse, winHeight - yMouse)) != -1)
		{
			w[i] -= 0.1;
			if (w[i] <= 0)
			{
				w[i] = 0.0001;
			}
			cout << "point=" << i <<" and weight=" << w[i] << endl;
		}
	}
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) 
	{
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
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
	glutCreateWindow("NURBS - Abai Alex");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	return 0;
}
