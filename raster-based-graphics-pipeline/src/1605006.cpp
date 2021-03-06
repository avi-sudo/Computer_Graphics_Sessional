#include<bits/stdc++.h>
#include<iostream>
#include<string>
#include <time.h>
#include<list>
#include<stack>
#include<iomanip>
#include "bitmap_image.hpp"
#define pi (2*acos(0.0))
using namespace std;

struct point{
    double x;
    double y;
    double z;
};
struct Triangle{
    double pointMatrix[3][4][4];
};
struct ZTriangle{
    struct point points[3];
    int color[3];
};
struct MATRIX{
    double array2D[4][4];
};
struct point eye;
struct point look;
struct point up;
struct point i;
struct point j;
struct point k;


double fovY,aspectRatio,near,far;
double Screen_width,Screen_height,leftLimitx,bottomLimity,rightLimitx,topLimity,frontLimitz,rearLimitz;
double dx,dy,Top_y,Left_x,Bottom_y,Right_x;
vector<Triangle> triangleVector;
vector<ZTriangle> ZtriangleVector;
vector<double (*)[4]>pointVector;

double globalMatrix[4][4];



stack<double (*)[4]> pointStack;


void print(double arr[4][4],int a){
    for(int i=0;i<4;i++){
        for(int j=0;j<a;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void arrayCopy(double mat1[4][4],double mat[4][4]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            mat1[i][j]=mat[i][j];
        }
    }
}
void printTriangle(int type){
    ofstream outputFile;
    if(type==1){
        outputFile.open("stage1.txt");
    }else if(type==2){
        outputFile.open("stage2.txt");
    }else if(type==3){
        outputFile.open("stage3.txt");
    }


    for(int i=0;i<triangleVector.size();i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                double d=triangleVector[i].pointMatrix[j][k][0];

                //std::cout<<std::fixed<<std::setprecision(3)<<d;
                    outputFile<<std::fixed<<std::setprecision(7)<<d<<"  ";
            }//cout<<endl;
            outputFile<<endl;
        }
        //cout<<endl;

        outputFile<<endl;

    }
    outputFile.close();
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
point rodrigues(point axis,point A,double angle){
    double angle2=(angle*(pi/180.0));
    struct point c;
    double temp,temp1;
    if(angle==90){
        temp=0;
    }else{
        temp=cos(angle2);
    }
    if(angle==180){
        temp1=0;
    }
    else{
        temp1=sin(angle2);
    }
    c.x=(temp*axis.x)+((1-cos(angle2))*(dotProduct(A,axis)*A.x))+(temp1*((crossProduct(A,axis)).x));
    c.y=(temp*axis.y)+((1-cos(angle2))*(dotProduct(A,axis)*A.y))+(temp1*((crossProduct(A,axis)).y));
    c.z=(temp*axis.z)+((1-cos(angle2))*(dotProduct(A,axis)*A.z))+(temp1*((crossProduct(A,axis)).z));
    return c;
}
void matrixMultiplication(double mat[4][4],double mat2[4][4],int a){
    double result[4][4];
    //print(mat2,a);
    double sum=0;
    for(int i=0;i<4;i++){
        for(int j=0;j<a;j++){
                result[i][j]=0;
           for(int k=0;k<4;k++){
                result[i][j]+=mat[i][k]*mat2[k][j];
           }
        }
    }


    if(a==4){
        arrayCopy(globalMatrix,result);

    }else{

        pointVector.clear();

        pointVector.push_back(result);
    }


}
void printZBuffer(double** ZBuffer,double limit){
    ofstream outputFile;
    outputFile.open("z_buffer.txt");
    for(int i=0;i<Screen_height;i++){
        for(int j=0;j<Screen_width;j++){
                if(ZBuffer[i][j]<limit){
                    //cout<<ZBuffer[i][j]<<" ";
                    outputFile<<std::fixed<<std::setprecision(6)<<ZBuffer[i][j]<<"\t";
                }
        }
        //cout<<endl;
        outputFile<<endl;
    }
    outputFile.close();
}
void freeMemory(double** ZBuffer){
    for(int i=0;i<Screen_height;i++){
        free(ZBuffer[i]);
    }
    free(ZBuffer);
}
double getIntersection(double x1,double x2,double y1,double y2,double ys){

    double Xa=x1-((x1-x2)*((y1-ys)/(y1-y2)));
    return Xa;
}
double computeZvalue(double zb,double za,double xb,double xa,double xp){
    double Zp=zb-((zb-za)*((xb-xp)/(xb-xa)));
    return Zp;
}
double getMax(double a,double b){
    return (a>b?a:b);
}
double getMin(double a,double b){
    return (a<b?a:b);
}
double getMaximum(double a,double b,double c){
    return (a>b?(a>c?a:c):(b>c?b:c));
}
double getMinimum(double a,double b,double c){
    return (a<b?(a<c?a:c):(b<c?b:c));
}
bool isBetweenTriangle(double x,double min_x,double max_x){
    if(x<min_x || x>max_x){
        return false;
    }
    else return true;
}
int main(){

    srand (time(0));
    int pushCount=-1;;
    i.x=j.y=k.z=1;
    i.y=i.z=j.x=j.z=k.y=k.x=0;
//cout<<crossProduct(i,k).z;
    double matrix1[4][4],mat4[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
                if(i==j){
                   matrix1[i][j]=1;
                }
            else
            matrix1[i][j]=0;
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<1;j++){

            mat4[i][j]=2;
        }
    }

    arrayCopy(globalMatrix,matrix1);

    double pushMatrix[100][4][4];
    stack<MATRIX> pushmat;
    int cnt=0;
    int cnt2=0;
    string line,intermediate,Line;
    ifstream myfile;
    ifstream myfile2;
    ifstream myfile3;
//FILE myfile;
    myfile.open("scene.txt");

    if(!myfile.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    //model transformation
    while(getline(myfile, line)) {
    // do things here

        cnt++;
        if(cnt==1){
                vector<string>tokens;
            stringstream check1(line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            eye.x=atof(tokens[0].c_str());
            eye.y=atof(tokens[1].c_str());
            eye.z=atof(tokens[2].c_str());


        }
        else if(cnt==2){
                vector<string>tokens;
            stringstream check1(line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            look.x=atof(tokens[0].c_str());
            look.y=atof(tokens[1].c_str());
            look.z=atof(tokens[2].c_str());


        }
        else if(cnt==3){
            vector<string>tokens;
            stringstream check1(line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            up.x=atof(tokens[0].c_str());
            up.y=atof(tokens[1].c_str());
            up.z=atof(tokens[2].c_str());


        }
        else if(cnt==4){
            vector<string>tokens;
            stringstream check1(line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            fovY=atof(tokens[0].c_str());
            aspectRatio=atof(tokens[1].c_str());
            near=atof(tokens[2].c_str());
            far=atof(tokens[3].c_str());
        }
        else{
            string command=line;
            if(command=="triangle"){
                    string line2;
                    struct Triangle tr;
                for(int k=0;k<3;k++){
                    if(getline(myfile,line2)){
                        vector<string>tokens;
            stringstream check1(line2);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            tr.pointMatrix[k][0][0]=atof(tokens[0].c_str());
            tr.pointMatrix[k][1][0]=atof(tokens[1].c_str());
            tr.pointMatrix[k][2][0]=atof(tokens[2].c_str());
            tr.pointMatrix[k][3][0]=1;
                matrixMultiplication(globalMatrix,tr.pointMatrix[k],1);
                    for(int m=0;m<4;m++){
                        tr.pointMatrix[k][m][0]=pointVector[0][m][0];
                    }
                }


                }
                triangleVector.push_back(tr);
            }
            else if(command=="translate"){
                string line3;
                double tx,ty,tz;

                if(getline(myfile,line3)){
                    vector<string>tokens;
                    stringstream check1(line3);
                    while(getline(check1, intermediate, ' '))
                    {
                        tokens.push_back(intermediate);
                    }
                    tx=atof(tokens[0].c_str());
                    ty=atof(tokens[1].c_str());
                    tz=atof(tokens[2].c_str());
                }
                double trMatrix[4][4]={{1,0,0,tx}, {0,1,0,ty}, {0,0,1,tz},{0,0,0,1}};
                //print(globalMatrix,4);
                matrixMultiplication(globalMatrix,trMatrix,4);
               // print(globalMatrix,4);
            }
            else if(command=="scale"){
                string line4;
                double sx,sy,sz;

                if(getline(myfile,line4)){
                    vector<string>tokens;
                    stringstream check1(line4);
                    while(getline(check1, intermediate, ' '))
                    {
                        tokens.push_back(intermediate);
                    }
                    sx=atof(tokens[0].c_str());
                    sy=atof(tokens[1].c_str());
                    sz=atof(tokens[2].c_str());
                }
                double scMatrix[4][4]={{sx,0,0,0}, {0,sy,0,0}, {0,0,sz,0},{0,0,0,1}};

                matrixMultiplication(globalMatrix,scMatrix,4);

            }
            else if(command=="rotate"){
                string line5;
                double angle,ax,ay,az;
                struct point A;
                struct point c1;
                struct point c2;
                struct point c3;
                if(getline(myfile,line5)){
                    vector<string>tokens;
                    stringstream check1(line5);
                    while(getline(check1, intermediate, ' '))
                    {
                        tokens.push_back(intermediate);
                    }
                    angle=atof(tokens[0].c_str());
                    ax=atof(tokens[1].c_str());
                    ay=atof(tokens[2].c_str());
                    az=atof(tokens[3].c_str());
                }
                double magnitude=sqrt((ax*ax)+(ay*ay)+(az*az));
                A.x=ax/magnitude;
                A.y=ay/magnitude;
                A.z=az/magnitude;

                c1=rodrigues(i,A,angle);
                c2=rodrigues(j,A,angle);
                c3=rodrigues(k,A,angle);
                double rtMatrix[4][4]={{c1.x,c2.x,c3.x,0}, {c1.y,c2.y,c3.y,0}, {c1.z,c2.z,c3.z,0},{0,0,0,1}};
                matrixMultiplication(globalMatrix,rtMatrix,4);

            }
            else if(command=="push"){
                    pushCount++;

                double t[4][4];
                arrayCopy(t,globalMatrix);

                //arrayCopy(pushMatrix[pushCount],t);
                //print(pushMatrix[pushCount],4);
                struct MATRIX m;
                arrayCopy(m.array2D,t);
                pushmat.push(m);
                //print(m.array2D,4);
            }
            else if(command=="pop"){

                //arrayCopy(globalMatrix,pushMatrix[pushCount]);
                pushCount--;
                struct MATRIX m=pushmat.top();
                pushmat.pop();
                arrayCopy(globalMatrix,m.array2D);
                //print(m.array2D,4);
            }
            else if(command=="end"){
                break;
            }

        }

    }
    printTriangle(1);
    //view transformation
    struct point l;
    struct point u;
    struct point r;
    l.x=look.x-eye.x;
    l.y=look.y-eye.y;
    l.z=look.z-eye.z;
    double mag1=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x=(l.x/mag1);
    l.y=(l.y/mag1);
    l.z=(l.z/mag1);
    r=crossProduct(l,up);
    double mag2=sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
    r.x=(r.x/mag2);
    r.y=(r.y/mag2);
    r.z=(r.z/mag2);
    u=crossProduct(r,l);
    double TRmatrix[4][4]={{1,0,0,-eye.x}, {0,1,0,-eye.y}, {0,0,1,-eye.z},{0,0,0,1}};
    double RTmatrix[4][4]={{r.x,r.y,r.z,0}, {u.x,u.y,u.z,0}, {-l.x,-l.y,-l.z,0},{0,0,0,1}};
    matrixMultiplication(RTmatrix,TRmatrix,4);

    for(int i=0;i<triangleVector.size();i++){
        for(int k=0;k<3;k++){
            matrixMultiplication(globalMatrix,triangleVector[i].pointMatrix[k],1);
                    for(int m=0;m<4;m++){
                        triangleVector[i].pointMatrix[k][m][0]=pointVector[0][m][0];
                    }
        }
    }
    printTriangle(2);

    //projection transformation
    double fovX,t,r2;
    fovX = fovY * aspectRatio;
    t = near * tan((fovY/2)*(pi/180.0));
    r2 = near * tan((fovX/2)*(pi/180.0));

    double PJmatrix[4][4]={{near/r2,0,0,0}, {0,near/t,0,0}, {0,0,-(far+near)/(far-near),-(2*far*near)/(far-near)},{0,0,-1,0}};

    for(int i=0;i<triangleVector.size();i++){
        for(int k=0;k<3;k++){
            matrixMultiplication(PJmatrix,triangleVector[i].pointMatrix[k],1);
                    for(int m=0;m<4;m++){

                        triangleVector[i].pointMatrix[k][m][0]=pointVector[0][m][0];
                    }
                    for(int g=0;g<4;g++){
                        triangleVector[i].pointMatrix[k][g][0]=(triangleVector[i].pointMatrix[k][g][0]/triangleVector[i].pointMatrix[k][3][0]);
                    }
        }
    }
    printTriangle(3);


    //Z-Buffer algo
    ifstream input;
    input.open("stage3.txt");
    for(int i=0;i<triangleVector.size();i++){
        for(int k=0;k<3;k++){
            double x,y,z;
            input>>x>>y>>z;
            triangleVector[i].pointMatrix[k][0][0]=x;
            triangleVector[i].pointMatrix[k][1][0]=y;
            triangleVector[i].pointMatrix[k][2][0]=z;
        }
    }



    for(int i=0;i<triangleVector.size();i++){
        struct ZTriangle triangle;
        for(int k=0;k<3;k++){
            triangle.points[k].x=triangleVector[i].pointMatrix[k][0][0];

            triangle.points[k].y=triangleVector[i].pointMatrix[k][1][0];
            triangle.points[k].z=triangleVector[i].pointMatrix[k][2][0];
            triangle.color[k]=rand()%256;
            //cout<<triangle.color[k]<<endl;

        }
        ZtriangleVector.push_back(triangle);
    }
    myfile2.open("config.txt");

    if(!myfile2.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    //model transformation
    while(getline(myfile2, Line)) {
    // do things here

        cnt2++;
        if(cnt2==1){
                vector<string>tokens;
            stringstream check1(Line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            Screen_width=atof(tokens[0].c_str());
            Screen_height=atof(tokens[1].c_str());
        }
        else if(cnt2==2){
                vector<string>tokens;
            stringstream check1(Line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            leftLimitx=atof(tokens[0].c_str());
            rightLimitx=leftLimitx*(-1);
        }
        else if(cnt2==3){
                vector<string>tokens;
            stringstream check1(Line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            bottomLimity=atof(tokens[0].c_str());
            topLimity=bottomLimity*(-1);
        }
        else{
            vector<string>tokens;
            stringstream check1(Line);
           while(getline(check1, intermediate, ' '))
            {
            tokens.push_back(intermediate);
            }
            frontLimitz=atof(tokens[0].c_str());
            rearLimitz=atof(tokens[1].c_str());
        }
    }
    //cout<<Screen_height<<Screen_width<<rightLimitx<<rightLimity<<rearLimitz<<frontLimitz<<endl;
    dx=(rightLimitx-leftLimitx)/(Screen_width);
    dy=(topLimity-bottomLimity)/(Screen_height);
    Top_y=(topLimity-(dy/2.0));
    Left_x=(leftLimitx+(dx/2.0));
    Bottom_y=Top_y*(-1);
    Right_x=Left_x*(-1);

    int sw=(int)Screen_width;
    int sh=(int)Screen_height;
    double** ZBuffer=new double*[sh];
    for(int i=0;i<Screen_height;i++){
        ZBuffer[i]=new double[sw];
    }
    //double ZBuffer[(int)Screen_width][(int)Screen_height];
    bitmap_image image((int)Screen_width,(int)Screen_height);
    for(int i=0;i<sw;i++){
        for(int j=0;j<sh;j++){
            image.set_pixel(i,j,0,0,0);

        }
    }
    image.save_image("out.bmp");;

    for(int i=0;i<Screen_height;i++){
        for(int j=0;j<Screen_width;j++){
            ZBuffer[i][j]=rearLimitz;
        }
    }
    for(int i=0;i<ZtriangleVector.size();i++){
        double max_y=getMaximum(ZtriangleVector[i].points[0].y,ZtriangleVector[i].points[1].y,ZtriangleVector[i].points[2].y);
        if(max_y>Top_y){
            max_y=Top_y;
        }
        double min_y=getMinimum(ZtriangleVector[i].points[0].y,ZtriangleVector[i].points[1].y,ZtriangleVector[i].points[2].y);
        if(min_y<Bottom_y){
            min_y=Bottom_y;
        }
        int topRow=ceil((Top_y-max_y)/dy);
        int bottomRow=floor((Top_y-min_y)/dy);
        //int topRow=round((Top_y-max_y)/dy);
        //int bottomRow=round((Top_y-min_y)/dy);
        for(int j=topRow;j<=bottomRow;j++){
            double max_x=getMaximum(ZtriangleVector[i].points[0].x,ZtriangleVector[i].points[1].x,ZtriangleVector[i].points[2].x);
            if(max_x>Right_x){
                max_x=Right_x;
            }
            double min_x=getMinimum(ZtriangleVector[i].points[0].x,ZtriangleVector[i].points[1].x,ZtriangleVector[i].points[2].x);
            if(min_x<Left_x){
                min_x=Left_x;
            }
            double ys=Top_y-(j*dy);
            struct point A,B,C;
            double xa,xb,za,zb,zp;
            A.x=getIntersection(ZtriangleVector[i].points[0].x,ZtriangleVector[i].points[1].x,ZtriangleVector[i].points[0].y,ZtriangleVector[i].points[1].y,ys);
            B.x=getIntersection(ZtriangleVector[i].points[2].x,ZtriangleVector[i].points[0].x,ZtriangleVector[i].points[2].y,ZtriangleVector[i].points[0].y,ys);
            C.x=getIntersection(ZtriangleVector[i].points[1].x,ZtriangleVector[i].points[2].x,ZtriangleVector[i].points[1].y,ZtriangleVector[i].points[2].y,ys);

            A.z=getIntersection(ZtriangleVector[i].points[0].z,ZtriangleVector[i].points[1].z,ZtriangleVector[i].points[0].y,ZtriangleVector[i].points[1].y,ys);
            B.z=getIntersection(ZtriangleVector[i].points[2].z,ZtriangleVector[i].points[0].z,ZtriangleVector[i].points[2].y,ZtriangleVector[i].points[0].y,ys);
            C.z=getIntersection(ZtriangleVector[i].points[1].z,ZtriangleVector[i].points[2].z,ZtriangleVector[i].points[1].y,ZtriangleVector[i].points[2].y,ys);
            if(isBetweenTriangle(A.x,min_x,max_x)==false){
                xa=getMin(B.x,C.x);
                xb=getMax(B.x,C.x);
                if(xa==B.x){
                    za=B.z;
                    zb=C.z;
                }
                else{
                    za=C.z;
                    zb=B.z;
                }

            }
            else if(isBetweenTriangle(B.x,min_x,max_x)==false){
                 xa=getMin(A.x,C.x);
                 xb=getMax(A.x,C.x);
                 if(xa==A.x){
                    za=A.z;
                    zb=C.z;
                }
                else{
                    za=C.z;
                    zb=A.z;
                }

            }
            else if(isBetweenTriangle(C.x,min_x,max_x)==false){
                 xa=getMin(B.x,A.x);
                 xb=getMax(B.x,A.x);
                 if(xa==B.x){
                    za=B.z;
                    zb=A.z;
                }
                else{
                    za=A.z;
                    zb=B.z;
                }

            }
            if(xa<Left_x){
                xa=Left_x;
            }
            if(xb>Right_x){
                xb=Right_x;
            }
            //int leftColumn=floor((xa-Left_x)/dx);
            //int rightColumn=floor((xb-Left_x)/dx);
            int leftColumn=round((xa-Left_x)/dx);
            int rightColumn=round((xb-Left_x)/dx);
            //cout<<leftColumn<<":"<<rightColumn<<endl;
            for(int k=leftColumn;k<=rightColumn;k++){
                double xp=Left_x+(k*dx);
                zp=computeZvalue(zb,za,xb,xa,xp);
                if(zp>frontLimitz && zp<ZBuffer[j][k]){
                    ZBuffer[j][k]=zp;

                    image.set_pixel(k,j,ZtriangleVector[i].color[0],ZtriangleVector[i].color[1],ZtriangleVector[i].color[2]);

                }
            }
        }
        image.save_image("out.bmp");;
    }


    printZBuffer(ZBuffer,rearLimitz);


    //output bmp image
    //image.save_image("output.bmp");;

    freeMemory(ZBuffer);
    return 0;

}

