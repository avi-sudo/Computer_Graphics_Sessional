
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
#include "bitmap_image.hpp"
#define pi (2*acos(0.0))
#define REFINDEX 1.5
using namespace std;
struct point
{
	double x,y,z;
	point(){}
	point(double a,double b,double c){
        x=a;
        y=b;
        z=c;
	}
};
void clippingColor(double r,double g,double b)
{
    if(r<0){
        r=0;
    }else if(r>1){
        r=1;
    }
    if(g<0){
        g=0;
    }else if(g>1){
        g=1;
    }
    if(b<0){
        b=0;
    }else if(b>1){
        b=1;
    }

}
struct point normalize(struct point a)
{
    double mod=sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
    a.x/=mod;
    a.y/=mod;
    a.z/=mod;
    return a;
}
double dotProduct(point a,point b){
    double p;
    p=(a.x*b.x)+(a.y*b.y)+(a.z*b.z);
    return p;
}
point crossProduct(point a,point b){
    struct point product;
    product.x=(a.y*b.z)-(b.y*a.z);
    product.y=(a.z*b.x)-(a.x*b.z);
    product.z=(a.x*b.y)-(a.y*b.x);
    return product;
}
double getMin(double a,double b){
    return (a<b?a:b);
}
double getMax(double a,double b){
    return (a>b?a:b);
}
struct point getOppositeVector(struct point A){
    A.x=-A.x;
    A.y=-A.y;
    A.z=-A.z;
    return A;
}
class Ray
{
public:
    struct point Ro;
    struct point Rd;
    Ray(){}
    Ray(struct point a,struct point b){
        Ro=a;
        Rd=b;
    }
};
class Light
{
public:
    struct point Light_pos;
    double color[3];
    Light(){}
    Light(struct point Light_pos){
        this->Light_pos=Light_pos;
    }
    void setColor(double r,double g,double b)
    {
        this->color[0]=r;
        this->color[1]=g;
        this->color[2]=b;
    }
    void draw()
    {
        glPointSize(5);
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_POINTS);
        {
            glVertex3f(Light_pos.x,Light_pos.y,Light_pos.z);
        }glEnd();

    }
};
class Object
{
public:
    struct point reference_point;
    double height, width, length;

    double color[3];
    double coEfficients[4];// reflection coefficients
    int shine;// exponent term of specular component
    Object(){}

    void setColor(double r,double g,double b)
    {
        this->color[0]=r;
        this->color[1]=g;
        this->color[2]=b;
    }
    void setShine(int shine)
    {
        this->shine=shine;
    }
    void setCoEfficients(double a,double b,double c,double d)
    {
        this->coEfficients[0]=a;
        this->coEfficients[1]=b;
        this->coEfficients[2]=c;
        this->coEfficients[3]=d;
    }
    virtual void draw(){}
    virtual double getIntersectingT(Ray *r)
    {
        return -1;
	}
	virtual double intersect(Ray *r, double *current_color, int level){
        return -1;
	}
	struct point getReflectedRay(Ray *r,struct point normal)
	{
	    struct point reflection;
        reflection.x = r->Rd.x-normal.x * 2.0 * dotProduct(r->Rd,normal);
        reflection.y = r->Rd.y-normal.y * 2.0 * dotProduct(r->Rd,normal);
        reflection.z = r->Rd.z-normal.z * 2.0 * dotProduct(r->Rd,normal);
        //reflection=getOppositeVector(reflection);
        return normalize(reflection);
	}
	virtual struct point getNormal(struct point a)
	{
        return normalize(a);
	}
	struct point getRefractedRay(Ray *r, struct point normal)
    {
        struct point refraction;
        double product = dotProduct(r->Rd,normal);

        double k = 1 - (pow(REFINDEX,2) * (1 - pow(product,2)));

        if(k < 0){
            refraction.x = 0;
            refraction.y = 0;
            refraction.z = 0;
        }
        else{
            refraction.x = (REFINDEX * r->Rd.x) - (REFINDEX * product + sqrt(k)) * normal.x;
            refraction.y = (REFINDEX * r->Rd.y) - (REFINDEX * product + sqrt(k)) * normal.y;
            refraction.z = (REFINDEX * r->Rd.z) - (REFINDEX * product + sqrt(k)) * normal.z;
        }


        return normalize(refraction);//refraction;

    }
};
extern string PATH;
extern int recursion_level;
extern vector<Light*> lights;
extern vector<Object*> objects;

class Sphere:public Object
{
public:
    Sphere(struct point centre,double radius){
        reference_point=centre;
        length=radius;
        height=radius;
    }
    void draw(){
        glColor3f(color[0], color[1], color[2]);

        struct point points[100][100];
        int i,j;
        double h,r;
        int stacks = 30;
        int slices = 30;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=length*sin(((double)i/(double)stacks)*(pi/2.0));
            r=length*cos(((double)i/(double)stacks)*(pi/2.0));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2.0*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2.0*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x + reference_point.x, points[i][j].y + reference_point.y, points[i][j].z + reference_point.z);
                    glVertex3f(points[i][j+1].x + reference_point.x, points[i][j+1].y + reference_point.y, points[i][j+1].z + reference_point.z);
                    glVertex3f(points[i+1][j+1].x + reference_point.x, points[i+1][j+1].y + reference_point.y, points[i+1][j+1].z + reference_point.z);
                    glVertex3f(points[i+1][j].x + reference_point.x, points[i+1][j].y + reference_point.y, points[i+1][j].z + reference_point.z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x + reference_point.x,points[i][j].y + reference_point.y,-points[i][j].z + reference_point.z);
                    glVertex3f(points[i][j+1].x + reference_point.x,points[i][j+1].y + reference_point.y,-points[i][j+1].z + reference_point.z);
                    glVertex3f(points[i+1][j+1].x + reference_point.x,points[i+1][j+1].y + reference_point.y,-points[i+1][j+1].z + reference_point.z);
                    glVertex3f(points[i+1][j].x + reference_point.x,points[i+1][j].y + reference_point.y,-points[i+1][j].z + reference_point.z);
                }glEnd();
            }
        }
    }
    struct point getNormal(struct point a)
    {
        struct point normal;
        normal.x=a.x-reference_point.x;
        normal.y=a.y-reference_point.y;
        normal.z=a.z-reference_point.z;
        return normalize(normal);
    };
    double getIntersectingT(Ray *r)
    {
        struct point p1(r->Ro.x-reference_point.x,r->Ro.y-reference_point.y,r->Ro.z-reference_point.z);
        double a=dotProduct(r->Rd,r->Rd);
        double b=2.0*(dotProduct(r->Rd,p1));
        double c=(dotProduct(p1,p1)-(pow(length,2)));
        double d=pow(b,2)-(4*a*c);
        if(d<0){
            return -1;
        }
        double t1=(-b+sqrt(d))/(2.0*a);
        double t2=(-b-sqrt(d))/(2.0*a);
        if(t1<=0 && t2<=0){
            return -1;
        }else if(t1<=0 && t2>0){
            return t2;
        }else if(t1>0 && t2<=0){
            return t1;
        }else{
            return getMin(t1,t2);
        }

    }
    double intersect(Ray *r, double *current_color, int level){
        double t=getIntersectingT(r);
        if(t<=0){
            return -1;
        }
        if(level==0){
            return t;
        }
        struct point intersection;
        intersection.x=r->Ro.x+(r->Rd.x*t);
        intersection.y=r->Ro.y+(r->Rd.y*t);
        intersection.z=r->Ro.z+(r->Rd.z*t);

        current_color[0]=this->color[0]*this->coEfficients[0];
        current_color[1]=this->color[1]*this->coEfficients[0];
        current_color[2]=this->color[2]*this->coEfficients[0];
        struct point normal;
        normal=this->getNormal(intersection);

       // struct point reflection;
       // reflection=getReflectedRay(r,normal);
        for(int i=0;i<lights.size();i++){
            struct point LDir;
            LDir.x=lights[i]->Light_pos.x-intersection.x;
            LDir.y=lights[i]->Light_pos.y-intersection.y;
            LDir.z=lights[i]->Light_pos.z-intersection.z;
            //LDir=getOppositeVector(LDir);
            double lengthOfLDir=sqrt(pow(LDir.x,2)+pow(LDir.y,2)+pow(LDir.z,2));
            LDir=normalize(LDir);
            struct point LStart;
            LStart.x=intersection.x+(LDir.x*0.0001);
            LStart.y=intersection.y+(LDir.y*0.0001);
            LStart.z=intersection.z+(LDir.z*0.0001);
            Ray L(LStart,LDir);
            bool obscured=false;
            for(int j=0;j<objects.size();j++){
                double tr=objects[j]->getIntersectingT(&L);
                if(tr>0 && abs(tr)<lengthOfLDir){
                    obscured=true;
                    break;
                }
            }
            if(obscured==false){
                double lambertValue=dotProduct(L.Rd,normal);
                //double phongValue=dotProduct(reflection,r->Rd);
                double phongValue=dotProduct(getReflectedRay(&L,normal),r->Rd);
                //double phongValue=dotProduct(getOppositeVector(getReflectedRay(&L,normal)),getOppositeVector(r->Rd));
                lambertValue=getMax(lambertValue,0);
                phongValue=getMax(phongValue,0);
                current_color[0]+=(lights[i]->color[0]*lambertValue*this->color[0]*this->coEfficients[1])+(lights[i]->color[0]*pow(phongValue,this->shine)*this->coEfficients[2]);//*this->color[0]);
                current_color[1]+=(lights[i]->color[1]*lambertValue*this->color[1]*this->coEfficients[1])+(lights[i]->color[1]*pow(phongValue,this->shine)*this->coEfficients[2]);//*this->color[1]);
                current_color[2]+=(lights[i]->color[2]*lambertValue*this->color[2]*this->coEfficients[1])+(lights[i]->color[2]*pow(phongValue,this->shine)*this->coEfficients[2]);//*this->color[2]);
            }
        }



        //reflection
        if(level>=recursion_level) return t;
         //double tMin=99999;
        if(level<recursion_level){
                /*if(dotProduct(normal,r->Rd)<0){
            normal=getOppositeVector(normal);
            }*/
            struct point reflection;
            reflection=getReflectedRay(r,normal);
             //reflection=getOppositeVector(reflection);
            struct point ReflectStart;
            ReflectStart.x=intersection.x+reflection.x*0.0001;
            ReflectStart.y=intersection.y+reflection.y*0.0001;
            ReflectStart.z=intersection.z+reflection.z*0.0001;

            Ray ReflectedRay(ReflectStart,reflection);

            int nearest=-1;
            double tMin=99999;
            double *reflectedColor=new double[3];
           //double reflectedColor[3];
            //reflectedColor[0]=reflectedColor[1]=reflectedColor[2]=0.0;
            double *dummyColor=new double[3];
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&ReflectedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                 tMin=objects[nearest]->intersect(&ReflectedRay,reflectedColor,level+1);
                 if(tMin!=-1){
                    current_color[0]+=reflectedColor[0]*this->coEfficients[3];
                    current_color[1]+=reflectedColor[1]*this->coEfficients[3];
                    current_color[2]+=reflectedColor[2]*this->coEfficients[3];
                }

            }
             //refraction
           /* struct point refraction;
            refraction=getRefractedRay(r,normal);
            struct point refractStart;
            refractStart.x=intersection.x+(refraction.x*0.0001);
            refractStart.y=intersection.y+(refraction.y*0.0001);
            refractStart.z=intersection.z+(refraction.z*0.0001);
            Ray RefractedRay(refractStart,refraction);
            nearest=-1;
            tMin=99999;
            double *refractedColor=new double[3];
            refractedColor[0]=refractedColor[1]=refractedColor[2]=0.0;
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&RefractedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                tMin=objects[nearest]->intersect(&RefractedRay,refractedColor,level+1);
                if(tMin!=-1){
                    current_color[0]+=refractedColor[0]*this->coEfficients[3]*0.5;
                    current_color[1]+=refractedColor[1]*this->coEfficients[3]*0.5;
                    current_color[2]+=refractedColor[2]*this->coEfficients[3]*0.5;
                }

            }*/

            clippingColor(current_color[0],current_color[1],current_color[2]);
            delete[] dummyColor;
            delete[] reflectedColor;
            //delete[] refractedColor;
            return tMin;
        }
        //return t;

    }

};
class Floor:public Object
{
public:
    string texturePath=PATH+"1605006_texture.bmp";
    int textureWidth;
    int textureHeight;
    bitmap_image textureImage;
    //double textureHeight,textureWidth;
    Floor(double floorWidth,double tileWidth){
        reference_point.x=(-1.0)*(floorWidth/2);
        reference_point.y=(-1.0)*(floorWidth/2);
        reference_point.z=0;
        length=tileWidth;
        textureImage=bitmap_image(texturePath);
        textureWidth=textureImage.width();
        textureHeight=textureImage.height();
        //textureImage=bitmap_image("E:\\Aa study material (4-1)\\Computer Graphics\\Offline_3\\1605006(Ray_Tracing)\\1605006_texture.bmp");

    }
    void draw(){
        struct point temporary=reference_point;
        int flag = 0;
        for(temporary.y=reference_point.y; temporary.y < -reference_point.y; temporary.y += length)
        {
            for(temporary.x=reference_point.x; temporary.x < -reference_point.x; temporary.x += length)
            {
                if(flag == 1)
                {
                    glColor3f(0, 0, 0);
                }
                else
                {
                    glColor3f(1, 1, 1);
                }


                glBegin(GL_QUADS);{
                glVertex3f(temporary.x, temporary.y, temporary.z);
                glVertex3f(temporary.x, temporary.y + length, temporary.z);
                glVertex3f(temporary.x + length, temporary.y + length, temporary.z);
                glVertex3f(temporary.x + length, temporary.y, temporary.z);
                }glEnd();
                flag =  flag^1;

            }

            flag = flag^1;
        }
    }
    struct point getNormal(struct point a)
    {
        struct point normal;
        normal.x=0;
        normal.y=0;
        normal.z=1;
        return normal;
    }
    double getIntersectingT(Ray* r)
    {
        struct point normal = getNormal(reference_point);
        if(r->Rd.z==0){
            return -1;
        }
        return (-1.0)*(dotProduct(normal, r->Ro) / (dotProduct(normal, r->Rd)));
    }
    double intersect(Ray *r, double *current_color, int level){
        double t=getIntersectingT(r);
        if(t<=0){
            return -1;
        }

        struct point intersection;
        intersection.x=r->Ro.x+(r->Rd.x*t);
        intersection.y=r->Ro.y+(r->Rd.y*t);
        intersection.z=r->Ro.z+(r->Rd.z*t);
        if(intersection.x < reference_point.x || intersection.x > -reference_point.x || intersection.y < reference_point.y || intersection.y > -reference_point.y){
            return -1;
        }
        if(level==0){
            return t;
        }

        int x = (int)((intersection.x - reference_point.x) / length);
        int y = (int)((intersection.y - reference_point.y) / length);
        //int xT = ((int)(intersection.x) - (int)(reference_point.x)) % (int)length;
        //int yT = ((int)(intersection.y) - (int)(reference_point.y)) % (int)length;
        double mappingX=length/(textureWidth*1.0);
        double mappingY=length/(textureHeight*1.0);
        struct point bottomLeftTiles;
        bottomLeftTiles.x=reference_point.x+(x*length);
        bottomLeftTiles.y=reference_point.y+(y*length);
        int xT=(int)((intersection.x-bottomLeftTiles.x)/mappingX);
        int yT=(int)((intersection.y-bottomLeftTiles.y)/mappingY);
        unsigned char red,green,blue;
        if((x + y) % 2 == 0){
            color[0] = color[1] = color[2] = 1;
        }
        else {
           // color[0] = color[1] = color[2] = 0;
           textureImage.get_pixel(xT,yT,red,green,blue);
           color[0]=red/255.0;
           color[1]=green/255.0;
           color[2]=blue/255.0;
        }


        current_color[0]=this->color[0]*this->coEfficients[0];
        current_color[1]=this->color[1]*this->coEfficients[0];
        current_color[2]=this->color[2]*this->coEfficients[0];
        struct point normal;
        normal=this->getNormal(intersection);

        for(int i=0;i<lights.size();i++){
            struct point LDir;
            LDir.x=lights[i]->Light_pos.x-intersection.x;
            LDir.y=lights[i]->Light_pos.y-intersection.y;
            LDir.z=lights[i]->Light_pos.z-intersection.z;

            double lengthOfLDir=sqrt(pow(LDir.x,2)+pow(LDir.y,2)+pow(LDir.z,2));
            LDir=normalize(LDir);

            struct point LStart;

            LStart.x=intersection.x+(LDir.x*0.0001);
            LStart.y=intersection.y+(LDir.y*0.0001);
            LStart.z=intersection.z+(LDir.z*0.0001);

            Ray L(LStart,LDir);
            bool obscured=false;
           for(int j=0;j<objects.size();j++){
                double tr=objects[j]->getIntersectingT(&L);
                if(tr>0 && abs(tr)<lengthOfLDir){
                    obscured=true;
                    break;
                }
            }
            if(obscured==false){

                double lambertValue=dotProduct(L.Rd,normal);
                //double phongValue=dotProduct(reflection,r->Rd);
                //struct point opposite(-r->Rd.x,-r->Rd.y,-r->Rd.z);
                double phongValue=dotProduct(getReflectedRay(&L,normal),r->Rd);

                lambertValue=getMax(lambertValue,0);
                phongValue=getMax(phongValue,0);
                current_color[0]+=(lights[i]->color[0]*lambertValue*this->color[0]*this->coEfficients[1])+(lights[i]->color[0]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[1]+=(lights[i]->color[1]*lambertValue*this->color[1]*this->coEfficients[1])+(lights[i]->color[1]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[2]+=(lights[i]->color[2]*lambertValue*this->color[2]*this->coEfficients[1])+(lights[i]->color[2]*pow(phongValue,this->shine)*this->coEfficients[2]);
            }
        }

            //reflection
            if(level>=recursion_level) return t;

        if(level<recursion_level){

            struct point reflection;
            reflection=getReflectedRay(r,normal);

            struct point ReflectStart;
            ReflectStart.x=intersection.x+reflection.x*0.0001;
            ReflectStart.y=intersection.y+reflection.y*0.0001;
            ReflectStart.z=intersection.z+reflection.z*0.0001;

            Ray ReflectedRay(ReflectStart,reflection);

            int nearest=-1;
            double tMin=99999;
            double *reflectedColor=new double[3];
            //double reflectedColor[3];
            //reflectedColor[0]=reflectedColor[1]=reflectedColor[2]=0.0;
            double *dummyColor=new double[3];
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&ReflectedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                  tMin=objects[nearest]->intersect(&ReflectedRay,reflectedColor,level+1);
                 if(tMin!=-1){
                    current_color[0]+=reflectedColor[0]*this->coEfficients[3];
                    current_color[1]+=reflectedColor[1]*this->coEfficients[3];
                    current_color[2]+=reflectedColor[2]*this->coEfficients[3];
                }

            }

            clippingColor(current_color[0],current_color[1],current_color[2]);
            delete[] dummyColor;
            delete[] reflectedColor;
            return tMin;
        }


        //return t;
        //return tMin;
    }
};
class Triangle:public Object
{
public:
    struct point a;
    struct point b;
    struct point c;
    Triangle(struct point a,struct point b,struct point c){
        this->a=a;
        this->b=b;
        this->c=c;
    }
    void draw()
    {
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x,a.y,a.z);
			glVertex3f(b.x,b.y,b.z);
			glVertex3f(c.x,c.y,c.z);
        }
        glEnd();
    }
    struct point getNormal(struct point p)
    {
        struct point m;
        struct point n;
        m.x = b.x - a.x;
        m.y = b.y - a.y;
        m.z = b.z - a.z;

        n.x = c.x - a.x;
        n.y = c.y - a.y;
        n.z = c.z - a.z;
        struct point normal = crossProduct(m,n);
        normal = normalize(normal);

        if(dotProduct(p, normal) > 0){
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }

        return normal;
    }
    double getIntersectingT(Ray *r)
    {
        struct point side1;
        struct point side2;
        struct point side3;

        side1.x=a.x-b.x;
        side1.y=a.y-b.y;
        side1.z=a.z-b.z;


        side2.x=a.x-c.x;
        side2.y=a.y-c.y;
        side2.z=a.z-c.z;

        side3.x=a.x-r->Ro.x;
        side3.y=a.y-r->Ro.y;
        side3.z=a.z-r->Ro.z;

        double A=0.0;

        A+=(side1.x*(side2.y*r->Rd.z-r->Rd.y*side2.z));
        A-=(side1.y*(side2.x*r->Rd.z-r->Rd.x*side2.z));
        A+=(side1.z*(side2.x*r->Rd.y-r->Rd.x*side2.y));

        if(A==0)
        {
            return -1;
        }

        double Gamma=0.0;

        Gamma+=(side1.x*(side3.y*r->Rd.z-r->Rd.y*side3.z));
        Gamma-=(side1.y*(side3.x*r->Rd.z-r->Rd.x*side3.z));
        Gamma+=(side1.z*(side3.x*r->Rd.y-r->Rd.x*side3.y));
        Gamma/=A;
        //cout<<Gamma<<endl;
        if(Gamma<0 || Gamma>1.0)
        {
            return -1;
        }

        double Beta=0.0;

        Beta+=(side3.x*(side2.y*r->Rd.z-r->Rd.y*side2.z));

        Beta-=(side3.y*(side2.x*r->Rd.z-r->Rd.x*side2.z));

        Beta+=(side3.z*(side2.x*r->Rd.y-r->Rd.x*side2.y));

        Beta/=A;
        //cout<<Beta<<endl;
        if(Beta<0 || Beta>1-Gamma)
        {
            return -1;
        }

        double t=0.0;

        t+=(side1.x*(side2.y*side3.z-side3.y*side2.z));
        t-=(side1.y*(side2.x*side3.z-side3.x*side2.z));
        t+=(side1.z*(side2.x*side3.y-side3.x*side2.y));
        t/=A;
        //cout<<t<<endl;
        if(t<0){
            return -1;
        }
        return t;

    }
    double intersect(Ray *r, double *current_color, int level){
        double t=getIntersectingT(r);
        if(t<=0){
            return -1;
        }
        if(level==0){
            return t;
        }
        struct point intersection;
        intersection.x=r->Ro.x+(r->Rd.x*t);
        intersection.y=r->Ro.y+(r->Rd.y*t);
        intersection.z=r->Ro.z+(r->Rd.z*t);

        current_color[0]=this->color[0]*this->coEfficients[0];
        current_color[1]=this->color[1]*this->coEfficients[0];
        current_color[2]=this->color[2]*this->coEfficients[0];
        struct point normal;
        normal=this->getNormal(intersection);
        struct point reflection;
        reflection=getReflectedRay(r,normal);
        for(int i=0;i<lights.size();i++){
            struct point LDir;
            LDir.x=lights[i]->Light_pos.x-intersection.x;
            LDir.y=lights[i]->Light_pos.y-intersection.y;
            LDir.z=lights[i]->Light_pos.z-intersection.z;
            double lengthOfLDir=sqrt(pow(LDir.x,2)+pow(LDir.y,2)+pow(LDir.z,2));
            LDir=normalize(LDir);
            struct point LStart;
            LStart.x=intersection.x+(LDir.x*0.0001);
            LStart.y=intersection.y+(LDir.y*0.0001);
            LStart.z=intersection.z+(LDir.z*0.0001);
            Ray L(LStart,LDir);
            bool obscured=false;
            for(int j=0;j<objects.size();j++){
                double tr=objects[j]->getIntersectingT(&L);
                if(tr>0 && abs(tr)<lengthOfLDir){
                    obscured=true;
                    break;
                }
            }
            if(obscured==false){
                double lambertValue=dotProduct(L.Rd,normal);
                //double phongValue=dotProduct(reflection,r->Rd);
                double phongValue=dotProduct(getReflectedRay(&L,normal),r->Rd);
                lambertValue=getMax(lambertValue,0);
                phongValue=getMax(phongValue,0);
                current_color[0]+=(lights[i]->color[0]*lambertValue*this->color[0]*this->coEfficients[1])+(lights[i]->color[0]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[1]+=(lights[i]->color[1]*lambertValue*this->color[1]*this->coEfficients[1])+(lights[i]->color[1]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[2]+=(lights[i]->color[2]*lambertValue*this->color[2]*this->coEfficients[1])+(lights[i]->color[2]*pow(phongValue,this->shine)*this->coEfficients[2]);
            }
        }

       //reflection
       if(level>=recursion_level) return t;

        if(level<recursion_level){
            struct point ReflectStart;
            ReflectStart.x=intersection.x+reflection.x*0.0001;
            ReflectStart.y=intersection.y+reflection.y*0.0001;
            ReflectStart.z=intersection.z+reflection.z*0.0001;
            Ray ReflectedRay(ReflectStart,reflection);

            int nearest=-1;
            double tMin=99999;
            double *reflectedColor=new double[3];
            reflectedColor[0]=reflectedColor[1]=reflectedColor[2]=0.0;
            double *dummyColor=new double[3];
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&ReflectedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                 tMin=objects[nearest]->intersect(&ReflectedRay,reflectedColor,level+1);
                if(tMin!=-1){
                    current_color[0]+=reflectedColor[0]*this->coEfficients[3];
                    current_color[1]+=reflectedColor[1]*this->coEfficients[3];
                    current_color[2]+=reflectedColor[2]*this->coEfficients[3];
                }

            }
             //refraction
           /* struct point refraction;
            refraction=getRefractedRay(r,normal);
            struct point refractStart;
            refractStart.x=intersection.x+(refraction.x*0.0001);
            refractStart.y=intersection.y+(refraction.y*0.0001);
            refractStart.z=intersection.z+(refraction.z*0.0001);
            Ray RefractedRay(refractStart,refraction);
            nearest=-1;
            tMin=99999;
            double *refractedColor=new double[3];
            refractedColor[0]=refractedColor[1]=refractedColor[2]=0.0;
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&RefractedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                tMin=objects[nearest]->intersect(&RefractedRay,refractedColor,level+1);
                if(tMin!=-1){
                    current_color[0]+=refractedColor[0]*this->coEfficients[3]*0.5;
                    current_color[1]+=refractedColor[1]*this->coEfficients[3]*0.5;
                    current_color[2]+=refractedColor[2]*this->coEfficients[3]*0.5;
                }

            }*/


            clippingColor(current_color[0],current_color[1],current_color[2]);
            delete[] dummyColor;
            delete[] reflectedColor;
            return tMin;
        }




    }
};
class GeneralQuadratic:public Object
{
public:
    double A,B,C,D,E,F,G,H,I,J;
    GeneralQuadratic(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J,struct point reference_point,double length,double width,double height){
        this->A=A;
        this->B=B;
        this->C=C;
        this->D=D;
        this->E=E;
        this->F=F;
        this->G=G;
        this->H=H;
        this->I=I;
        this->J=J;
        this->reference_point=reference_point;
        this->length=length;
        this->width=width;
        this->height=height;
    }
    struct point getNormal(struct point p)
    {
        struct point normal;
        normal.x = 2 * A * p.x + D * p.y + E * p.z + G;
        normal.y = 2 * B * p.y + D * p.x + F * p.z + H;
        normal.z = 2 * C * p.z + E * p.x + F * p.y + I;

        return normalize(normal);
    }
    double getIntersectingT(Ray* r)
    {
        double xo=r->Ro.x,xd=r->Rd.x,yo=r->Ro.y,yd=r->Rd.y,zo=r->Ro.z,zd=r->Rd.z;
        double a = A * pow(xd,2) + B * pow(yd,2) + C * pow(zd,2) + D * xd * yd + E * xd * zd + F * yd * zd;
        double b = 2 * A * xo * xd + 2 * B * yo * yd + 2 * C * zo * zd + D * (xo * yd + yo * xd) + E * (xo * zd + zo * xd) + F * (yo * zd + yd * zo) + G * xd + H * yd + I * zd;
        double c = A * pow(xo,2) + B * pow(yo,2) + C * pow(zo,2) + D * xo * yo + E * xo * zo + F * yo * zo + G * xo + H * yo + I * zo + J;

        if(a == 0){
            return -c/b;
        }
        double d = pow(b,2) - 4 * a * c;

        if(d < 0) return -1;


        double t0 = (-b - sqrt(d))/(2 * a);
        double t1 = (-b + sqrt(d))/(2 * a);

        struct point intersection1, intersection2;
        intersection1.x = r->Ro.x + r->Rd.x * t0;
        intersection1.y = r->Ro.y + r->Rd.y * t0;
        intersection1.z = r->Ro.z + r->Rd.z * t0;

        intersection2.x = r->Ro.x + r->Rd.x * t1;
        intersection2.y = r->Ro.y + r->Rd.y * t1;
        intersection2.z = r->Ro.z + r->Rd.z * t1;
        bool flag1=true,flag2=true;
        if(length!=0)
        {
            if(intersection1.x<reference_point.x || intersection1.x > reference_point.x+length){
                flag1=false;
            }
            if(intersection2.x<reference_point.x || intersection2.x > reference_point.x+length){
                flag2=false;
            }
        }
        if(width!=0)
        {
            if(intersection1.y<reference_point.y || intersection1.y > reference_point.y+width){
                flag1=false;
            }
            if(intersection2.y<reference_point.y || intersection2.y > reference_point.y+width){
                flag2=false;
            }
        }
        if(height!=0)
        {
            if(intersection1.z<reference_point.z || intersection1.z > reference_point.z+height){
                flag1=false;
            }
            if(intersection2.z<reference_point.z || intersection2.z > reference_point.z+height){
                flag2=false;
            }
        }
        if(flag1==true&&flag2==true){
            return getMin(t0,t1);
        }else if(flag1==true){
            return t0;
        }else if(flag2==true){
            return t1;
        }else return -1;
    }
    double intersect(Ray *r, double *current_color, int level){
        double t=getIntersectingT(r);
        if(t<=0){
            return -1;
        }
        if(level==0){
            return t;
        }
        struct point intersection;
        intersection.x=r->Ro.x+(r->Rd.x*t);
        intersection.y=r->Ro.y+(r->Rd.y*t);
        intersection.z=r->Ro.z+(r->Rd.z*t);

        current_color[0]=this->color[0]*this->coEfficients[0];
        current_color[1]=this->color[1]*this->coEfficients[0];
        current_color[2]=this->color[2]*this->coEfficients[0];
        struct point normal;
        normal=this->getNormal(intersection);
       struct point reflection;
        reflection=getReflectedRay(r,normal);
        for(int i=0;i<lights.size();i++){
            struct point LDir;
            LDir.x=lights[i]->Light_pos.x-intersection.x;
            LDir.y=lights[i]->Light_pos.y-intersection.y;
            LDir.z=lights[i]->Light_pos.z-intersection.z;
            double lengthOfLDir=sqrt(pow(LDir.x,2)+pow(LDir.y,2)+pow(LDir.z,2));
            LDir=normalize(LDir);
            struct point LStart;
            LStart.x=intersection.x+(LDir.x*0.0001);
            LStart.y=intersection.y+(LDir.y*0.0001);
            LStart.z=intersection.z+(LDir.z*0.0001);
            Ray L(LStart,LDir);
            bool obscured=false;
            for(int j=0;j<objects.size();j++){
                double tr=objects[j]->getIntersectingT(&L);
                if(tr>0 && abs(tr)<lengthOfLDir){
                    obscured=true;
                    break;
                }
            }
            if(obscured==false){
                double lambertValue=dotProduct(L.Rd,normal);
                //double phongValue=dotProduct(reflection,r->Rd);
                double phongValue=dotProduct(getReflectedRay(&L,normal),r->Rd);
                lambertValue=getMax(lambertValue,0);
                phongValue=getMax(phongValue,0);
                current_color[0]+=(lights[i]->color[0]*lambertValue*this->color[0]*this->coEfficients[1])+(lights[i]->color[0]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[1]+=(lights[i]->color[1]*lambertValue*this->color[1]*this->coEfficients[1])+(lights[i]->color[1]*pow(phongValue,this->shine)*this->coEfficients[2]);
                current_color[2]+=(lights[i]->color[2]*lambertValue*this->color[2]*this->coEfficients[1])+(lights[i]->color[2]*pow(phongValue,this->shine)*this->coEfficients[2]);
            }
        }

        //reflection
        if(level>=recursion_level) return t;


        if(level<recursion_level){
            struct point ReflectStart;
            ReflectStart.x=intersection.x+reflection.x*0.0001;
            ReflectStart.y=intersection.y+reflection.y*0.0001;
            ReflectStart.z=intersection.z+reflection.z*0.0001;
            Ray ReflectedRay(ReflectStart,reflection);

            int nearest=-1;
            double tMin=99999;
            double *reflectedColor=new double[3];
            reflectedColor[0]=reflectedColor[1]=reflectedColor[2]=0.0;
            double *dummyColor=new double[3];
            for(int k=0;k<objects.size();k++){
                double t=objects[k]->intersect(&ReflectedRay,dummyColor,0);
                if(t<=0){
                    continue;
                }
                else if(t<tMin){
                    tMin=t;
                    nearest=k;


                }
            }
            if(nearest!=-1){
                tMin=objects[nearest]->intersect(&ReflectedRay,reflectedColor,level+1);
                if(tMin!=-1){
                    current_color[0]+=reflectedColor[0]*this->coEfficients[3];
                    current_color[1]+=reflectedColor[1]*this->coEfficients[3];
                    current_color[2]+=reflectedColor[2]*this->coEfficients[3];
                }

            }

            clippingColor(current_color[0],current_color[1],current_color[2]);
            delete[] dummyColor;
            delete[] reflectedColor;
            return tMin;
        }

    }

};

