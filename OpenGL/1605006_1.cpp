

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <vector>
#define pi (2*acos(0.0))
using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle,angle2,angle3,angle4;
//double a;

struct point
{
	double x,y,z;
};

struct point pos;
struct point u;
struct point r;
struct point l;
bool flag1=false;
vector<point> gun;
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 180,0,0);
			glVertex3f(-160,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(0.6,0.6,0.6);
	glBegin(GL_QUADS);{
		/*glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);*/
		glVertex3f( 0, a,a);
		glVertex3f( 0,-a,a);
		glVertex3f(0,-a,-a);
		glVertex3f(0, a,-a);
	}glEnd();
}

void drawRedSquare(double a,double x,double y,double z)
{
    glColor3f(1,0,0);
    if((abs(y)+4)<200 && (abs(z)+4)<200){


	glBegin(GL_QUADS);{
		/*glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);*/
		glVertex3f(x, y+a,z+a);
		glVertex3f(x,y-a,z+a);
		glVertex3f(x,y-a,z-a);
		glVertex3f(x, y+a,z-a);
	}glEnd();
    }
}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        //glColor3f(1,1,1);
		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			   if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
			    //upper hemisphere
			    //glColor3f(1,0,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

                if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
                //glColor3f(0,1,0);
                glVertex3f(-points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(-points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(-points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(-points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}


}
void drawUpperSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        //glColor3f(1,1,1);
		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			   if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
			    //upper hemisphere
			    //glColor3f(1,0,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glRotatef(angle4,0,1,0);
			}glEnd();
		}
	}
}

void drawLowerSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        //glColor3f(1,1,1);
		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			   if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
			    //upper hemisphere
			    //glColor3f(1,0,0);
				glVertex3f(-points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(-points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(-points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(-points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
			}glEnd();
		}
	}
}

void drawSS()
{
    /*glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);*/
   // glRotatef(180,1,0,0);
    drawSphere(30,100,20);


}

void rotateLeft(double A){
    double a;
    a=A*(pi/180.0);
    float tlx,tly,tlz;
    tlx=l.x;
    tly=l.y;
    tlz=l.z;
    l.x=(l.x*cos(a))+(r.x*sin(-a));
    l.y=(l.y*cos(a))+(r.y*sin(-a));
    l.z=(l.z*cos(a))+(r.z*sin(-a));
    r.x=(r.x*cos(a))+(tlx*sin(a));
    r.y=(r.y*cos(a))+(tly*sin(a));
    r.z=(r.z*cos(a))+(tlz*sin(a));
}
void rotateRight(double A){
    double a;
    a=A*(pi/180.0);
    float tlx,tly,tlz;
    tlx=l.x;
    tly=l.y;
    tlz=l.z;
    l.x=(l.x*cos(a))+(r.x*sin(a));
    l.y=(l.y*cos(a))+(r.y*sin(a));
    l.z=(l.z*cos(a))+(r.z*sin(a));
    r.x=(r.x*cos(a))+(tlx*sin(-a));
    r.y=(r.y*cos(a))+(tly*sin(-a));
    r.z=(r.z*cos(a))+(tlz*sin(-a));
}
void lookUp(double A)
{
    double a;
    a=A*(pi/180.0);
    float tlx,tly,tlz;
    tlx=l.x;
    tly=l.y;
    tlz=l.z;
    l.x=(l.x*cos(a))+(u.x*sin(a));
    l.y=(l.y*cos(a))+(u.y*sin(a));
    l.z=(l.z*cos(a))+(u.z*sin(a));
    u.x=(u.x*cos(a))+(tlx*sin(-a));
    u.y=(u.y*cos(a))+(tly*sin(-a));
    u.z=(u.z*cos(a))+(tlz*sin(-a));
}
void lookDown(double A)
{
    double a;
    a=A*(pi/180.0);
    float tlx,tly,tlz;
    tlx=l.x;
    tly=l.y;
    tlz=l.z;
    l.x=(l.x*cos(a))+(u.x*sin(-a));
    l.y=(l.y*cos(a))+(u.y*sin(-a));
    l.z=(l.z*cos(a))+(u.z*sin(-a));
    u.x=(u.x*cos(a))+(tlx*sin(a));
    u.y=(u.y*cos(a))+(tly*sin(a));
    u.z=(u.z*cos(a))+(tlz*sin(a));
}
void tiltAnti(double A)
{
    double a;
    a=A*(pi/180.0);
    float tx,ty,tz;
    tx=u.x;
    ty=u.y;
    tz=u.z;
    /*r.x=(r.x*cos(a))+(u.x*sin(-a));
    r.y=(r.y*cos(a))+(u.y*sin(-a));
    r.z=(r.z*cos(a))+(u.z*sin(-a));*/
    u.x=(u.x*cos(a))+(r.x*sin(a));
    u.y=(u.y*cos(a))+(r.y*sin(a));
    u.z=(u.z*cos(a))+(r.z*sin(a));
    r.x=(r.x*cos(a))+(tx*sin(-a));
    r.y=(r.y*cos(a))+(ty*sin(-a));
    r.z=(r.z*cos(a))+(tz*sin(-a));

}
void tiltClk(double A)
{
    double a;
    a=A*(pi/180.0);
    float tlx,tly,tlz;
    tlx=r.x;
    tly=r.y;
    tlz=r.z;
    r.x=(r.x*cos(a))+(u.x*sin(a));
    r.y=(r.y*cos(a))+(u.y*sin(a));
    r.z=(r.z*cos(a))+(u.z*sin(a));
    u.x=(u.x*cos(a))+(tlx*sin(-a));
    u.y=(u.y*cos(a))+(tly*sin(-a));
    u.z=(u.z*cos(a))+(tlz*sin(-a));
}
void drawHalfSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
                //lower hemisphere
                if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
                //glColor3f(0,1,0);
                glVertex3f(-points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(-points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(-points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(-points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}

}
void drawCylinder(double r,int slices,int stacks)
{

    struct point points[100][100];
	int i,j;
	double h;
	h=stacks;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		//h=radius*sin(((double)i/(double)stacks)*(pi/2));
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=i;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			   if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
			    //upper hemisphere
			    //glColor3f(1,0,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
                //glColor3f(0,1,0);
                glVertex3f(-points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(-points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(-points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(-points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}

}
void drawquarter(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));

		r=(2*radius)-r;
		for(j=0;j<=slices;j++)
		{
			//points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].x=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			   if(j%2==0){
                glColor3f(1,1,1);
            }
            else glColor3f(0,0,0);
			    //upper hemisphere
			    //glColor3f(1,0,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

			}glEnd();
		}
	}

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			rotateLeft(3);
			break;
        case '2':
			//drawgrid=1-drawgrid;
			rotateRight(3);
			break;
        case '3':
			//drawgrid=1-drawgrid;
			lookUp(3);
			break;
        case '4':
			//drawgrid=1-drawgrid;
			lookDown(3);
			break;
        case '5':
			//drawgrid=1-drawgrid;
			tiltClk(3);
			break;
        case '6':
			//drawgrid=1-drawgrid;
			tiltAnti(3);
			break;
        case 'q':
            angle=max(angle-1,double(-30.0));
            //angle-=1;
            break;
        case 'w':
            angle=min(angle+1,double(30.0));
            //angle+=1;
            break;
        case 's':
            angle2=max(angle2-1,double(-30.0));
            //angle2-=1;
            break;
        case 'a':
            angle2=min(angle2+1,double(30.0));
            //angle2+=1;
            break;
        case 'd':
            angle3=min(angle3+1,double(30.0));
            //angle3+=1;
            break;
        case 'f':
            angle3=max(angle3-1,double(-30.0));
            //angle3-=1;
            break;
        case 'e':
            angle4=min(angle4+1,double(30.0));
            //angle4+=1;
            break;
        case 'r':
            angle4=max(angle4-1,double(-30.0));
            //angle4-=1;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos.x=pos.x-(5*l.x);
			pos.y=pos.y-(5*l.y);
			pos.z=pos.z-(5*l.z);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos.x=pos.x+(5*l.x);
			pos.y=pos.y+(5*l.y);
			pos.z=pos.z+(5*l.z);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos.x+=(5*r.x);
			pos.y+=(5*r.y);
			pos.z+=(5*r.z);
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos.x-=(5*r.x);
			pos.y-=(5*r.y);
			pos.z-=(5*r.z);
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=(5*u.x);
			pos.y+=(5*u.y);
			pos.z+=(5*u.z);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x-=(5*u.x);
			pos.y-=(5*u.y);
			pos.z-=(5*u.z);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				flag1=true;
			}
			else if(state==GLUT_UP){
                flag1=false;
			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){
                drawaxes=1-drawaxes;
		    }

			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	1,0,0);
	gluLookAt(pos.x,pos.y,pos.z,    pos.x+l.x,pos.y+l.y,pos.z+l.z,  u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    glPushMatrix();
    {
    glRotatef(angle,0,0,1);
	//drawSphere(30,100,20);
	drawLowerSphere(30,100,20);

	glRotatef(angle4,0,1,0);
    drawUpperSphere(30,100,20);
	glTranslatef(30,0,0);

    glRotatef(angle2,0,1,0);
    glRotatef(angle3,1,0,0);
    glTranslatef(10,0,0);
    drawHalfSphere(10,100,20);

    glTranslatef(40,0,0);
    drawCylinder(10,50,40);
    glTranslatef(40,0,0);
    drawquarter(10,50,40);
	}
	glPopMatrix();


    glPushMatrix();
    {
        //glRotatef(90,1,0,0);
        glTranslatef(200,0,0);
        //glRotatef(90,1,0,0);
        drawSquare(200);


    }
    glPopMatrix();

    //drawRedSquare(20,0,0);
    glPushMatrix();
    {


    if(flag1==true) {
        struct point g;
        double a=((pi/180.0)*angle);
        double a2=((pi/180.0)*angle2);
        double a3=((pi/180.0)*angle4);
        double b=tan(a)*195;
        double c=(tan(a3)*195)+(tan(a2)*165);

        g.x=195;g.y=b;g.z=-c;
        gun.push_back(g);

    }
    for(int k=0;k<gun.size();k++){
        drawRedSquare(2,gun[k].x,gun[k].y,gun[k].z);
    }
    }

    glPopMatrix();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    //maximum_angle=30;
    //double temp=a;
    //a+=0.5;


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
    pos.x=100;
    pos.y=100;
    pos.z=0;
    l.x=-(1/sqrt(2));
    l.y=-(1/sqrt(2));
    l.z=0;
    r.x=(1/sqrt(2));
    r.y=-(1/sqrt(2));
    r.z=0;
    u.x=0;
    u.y=0;
    u.z=-1;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

