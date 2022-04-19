

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <vector>
#include "1605006_Header.h"
#include "bitmap_image.hpp"
#define pi (2*acos(0.0))
#define FOV 80
#define windowWidth 500
#define windowHeight 500
using namespace std;
string PATH="E:\\Aa study material (4-1)\\Computer Graphics\\Offline_3\\1605006(Ray_Tracing)\\";
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle,angle2,angle3,angle4;
int numberOfObjects;
int numberOfLights;
int imageWidth;
int imageHeight;
//externs
vector<Object*> objects;
vector<Light*> lights;
int recursion_level;


struct point pos;
struct point u;
struct point r;
struct point l;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
        glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
        glColor3f(0.0, 0.0, 1.0);
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

void capture()
{
    string imagePath=PATH+"1605006_out.bmp";
    cout<<"start capturing"<<endl;
    bitmap_image image(imageWidth,imageHeight);
    for(int i=0;i<imageWidth;i++){
        for(int j=0;j<imageHeight;j++){
            image.set_pixel(i,j,0,0,0);
        }
    }
    //image.save_image("E:\\Aa study material (4-1)\\Computer Graphics\\Offline_3\\1605006(Ray_Tracing)\\1605006_out.bmp");;
    image.save_image(imagePath);;
    double planeDistance=(windowHeight/2)/(tan(FOV*(pi/360)));
    //cout<<planeDistance<<endl;
    struct point topleft;
    topleft.x = pos.x + (l.x * planeDistance) - r.x * (windowWidth/2.0) + u.x * (windowHeight/2.0);
    topleft.y = pos.y + (l.y * planeDistance) - r.y * (windowWidth/2.0) + u.y * (windowHeight/2.0);
    topleft.z = pos.z + (l.z * planeDistance) - r.z * (windowWidth/2.0) + u.z * (windowHeight/2.0);
    double du=(windowWidth*1.0)/imageWidth;
    double dv=(windowHeight*1.0)/imageHeight;
    //cout<<du<<"::"<<dv<<endl;

    topleft.x = topleft.x+ (r.x*(0.5*du))-(u.x*0.5*dv);
    topleft.y = topleft.y+ (r.y*(0.5*du))-(u.y*0.5*dv);
    topleft.z = topleft.z+ (r.z*(0.5*du))-(u.z*0.5*dv);
    //cout<<topleft.x<<":"<<topleft.y<<":"<<topleft.z<<endl;
    int nearest=-1;
    double t;

    for(int i=0;i<imageWidth;i++){
        for(int j=0;j<imageHeight;j++){
            struct point curPixel;
            curPixel.x=topleft.x+(r.x*(i*du))-(u.x*j*dv);
            curPixel.y=topleft.y+(r.y*(i*du))-(u.y*j*dv);
            curPixel.z=topleft.z+(r.z*(i*du))-(u.z*j*dv);
            struct point RayDirection(curPixel.x-pos.x,curPixel.y-pos.y,curPixel.z-pos.z);
            RayDirection=normalize(RayDirection);
            Ray newRay(pos,RayDirection);
            double tMin=99999;
           double *color=new double[3];
          // double color[3];
            color[0]=color[1]=color[2]=0.0;
            double *dummyColor=new double[3];
            for(int k=0;k<objects.size();k++){
                t=objects[k]->intersect(&newRay,dummyColor,0);

                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                tMin=objects[nearest]->intersect(&newRay,color,1);
                image.set_pixel(i,j,color[0]*255,color[1]*255,color[2]*255);

            }


            delete[] dummyColor;
            delete[] color;
        }

    }
    //image.save_image("E:\\Aa study material (4-1)\\Computer Graphics\\Offline_3\\1605006(Ray_Tracing)\\1605006_out.bmp");;

    image.save_image(imagePath);;
    cout<<"image captured"<<endl;

    //exit(0);


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
        case '0':
            capture();
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

		case GLUT_KEY_LEFT:
		    cameraAngle -= 0.03;
			pos.x-=(5*r.x);
			pos.y-=(5*r.y);
			pos.z-=(5*r.z);

			break;
		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos.x+=(5*r.x);
			pos.y+=(5*r.y);
			pos.z+=(5*r.z);
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
		    cameraAngle += 0.03;
			pos.x+=(5*r.x);
			pos.y+=(5*r.y);
			pos.z+=(5*r.z);
			break;
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

			}
			else if(state==GLUT_UP){

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
	//drawGrid();
    for(int i=0;i<objects.size();i++){
        objects[i]->draw();
    }
    for(int i=0;i<lights.size();i++){
        lights[i]->draw();
    }


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){


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
    /*pos.x=170;
    pos.y=120;
    pos.z=50;*/
    pos.x=170;
    pos.y=150;
    pos.z=50;


    l.x=-(1/sqrt(2));
    l.y=-(1/sqrt(2));
    l.z=0;
    r.x=-(1/sqrt(2));
    r.y=(1/sqrt(2));
    r.z=0;
    u.x=0;
    u.y=0;
   //old u.z=-1;
    u.z=1;

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
	gluPerspective(FOV,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}
void loadData()
{
    ifstream in;

    string inputPath=PATH+"scene.txt";
    in.open(inputPath);
    //in.open("E:\\Aa study material (4-1)\\Computer Graphics\\Offline_3\\1605006(Ray_Tracing)\\scene.txt");
    if(!in.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    in>>recursion_level;
    in>>imageHeight;
    imageWidth=imageHeight;
    in>>numberOfObjects;
    Object *temp;
    for(int i=0;i<numberOfObjects;i++){
        string command;
        in>>command;
        if(command.compare("sphere")==0||command.compare("Sphere")==0){
            double x,y,z,radius;
            in>>x>>y>>z;
            in>>radius;
            struct point c(x,y,z);

            temp=new Sphere(c,radius);
            double r,g,b;
            in>>r>>g>>b;
            temp->setColor(r,g,b);
            double a,d,s,refl;
            in>>a>>d>>s>>refl;
            temp->setCoEfficients(a,d,s,refl);
            int shine;
            in>>shine;
            temp->setShine(shine);
            objects.push_back(temp);
        }
        else if(command.compare("triangle")==0 || command.compare("Triangle")==0){
            struct point x1;
            struct point y1;
            struct point z1;
            in>>x1.x>>x1.y>>x1.z;
            in>>y1.x>>y1.y>>y1.z;
            in>>z1.x>>z1.y>>z1.z;

            temp=new Triangle(x1,y1,z1);
            double r,g,b;
            in>>r>>g>>b;
            temp->setColor(r,g,b);
            double a,d,s,refl;
            in>>a>>d>>s>>refl;

            temp->setCoEfficients(a,d,s,refl);
            int shine;
            in>>shine;
            temp->setShine(shine);
            objects.push_back(temp);
        }
        else if(command.compare("general")==0 || command.compare("General")==0){
            double A,B,C,D,E,F,G,H,I,J,length,width,height;
            struct point refPoint;
            in>>A>>B>>C>>D>>E>>F>>G>>H>>I>>J;
            in>>refPoint.x>>refPoint.y>>refPoint.z>>length>>width>>height;

            temp=new GeneralQuadratic(A,B,C,D,E,F,G,H,I,J,refPoint,length,width,height);
            double r,g,b;
            in>>r>>g>>b;
            temp->setColor(r,g,b);
            double a,d,s,refl;
            in>>a>>d>>s>>refl;
            temp->setCoEfficients(a,d,s,refl);
            int shine;
            in>>shine;
            temp->setShine(shine);
            objects.push_back(temp);
        }
    }
    in>>numberOfLights;
    Light *tmp;
    for(int j=0;j<numberOfLights;j++){
        struct point LPos;
        in>>LPos.x>>LPos.y>>LPos.z;

        tmp=new Light(LPos);
        double r,g,b;
        in>>r>>g>>b;
        tmp->setColor(r,g,b);
        lights.push_back(tmp);
    }
    temp=new Floor(1000,20);
    temp->setCoEfficients(0.4,0.2,0.1,0.2);
    temp->setShine(5);
    objects.push_back(temp);

}
void freeMemory()
{
    for(int g=0;g<objects.size();g++){
        delete objects[g];
    }
    for(int h=0;h<lights.size();h++){
        delete lights[h];
    }
    objects.clear();
    lights.clear();
}
int main(int argc, char **argv){


    loadData();
    atexit(freeMemory);
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth,windowHeight);
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

	glutMainLoop();
	 	//The main loop of OpenGL

	return 0;
}

