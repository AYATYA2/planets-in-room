#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glext.h>
//PFNGLACTIVETEXTUREPROC glActiveTexture;

#define TEXWIDTH 512
#define TEXHEIGHT 256
#define TRUE 1
#define FALSE 0
//GLfloat green[] = {0.0,0.0,1.0,1.0};
//\u8996\u7dda\u306e\u65b9\u5411\u3092\u8868\u30592\u3064\u306e\u89d2\u5ea6
GLfloat angle[]= {0,//xz\u8ef8
                  0//xy\u8ef8
                };
GLdouble pos[]={//\u30ab\u30e1\u30e9\u306e\u5ea7\u6a19
  0,-25,0
};
GLfloat light0pos[]={0.0,3.0,5.0,1.0};
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* \u76f4\u63a5\u5149\u5f37\u5ea6 */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* \u74b0\u5883\u5149\u5f37\u5ea6 */
GLfloat light1pos[]={5.0,-20.0,0.0,1.0};
GLfloat green[]={0.0,1.0,1.0,1.0};
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
int boxwidth=25;
int boxheight=25;
int objcount=0;
int txturecount=0;
struct Obj
{
  GLdouble position[3];//\u4f4d\u7f6e
  GLdouble radius;//\u5927\u304d\u3055
  GLdouble verosity[3];//\u901f\u5ea6
  int texture;
};
struct Obj  Objects[100];

GLdouble normal[][3] = {
  { 1.0, 1.0,1.0 },
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};

GLdouble vertex[][3] = {
  { -1.0, -1.0, -1.0 },
  { 1.0, -1.0, -1.0 },
  { 1.0, 1.0, -1.0 },
  { -1.0, 1.0, -1.0 },
  { -1.0, -1.0, 1.0 },
  { 1.0, -1.0, 1.0 },
  { 1.0, 1.0, 1.0 },
  { -1.0, 1.0, 1.0 }
};

GLuint textureID[5];
 
GLubyte textures[5][TEXWIDTH][TEXHEIGHT][4];
int face[][4] = {
  { 0, 1, 2, 3 },
  { 1, 5, 6, 2 },
  { 5, 4, 7, 6 },
  { 4, 0, 3, 7 },
  { 4, 5, 1, 0 },
  { 3, 2, 6, 7 }
};

double* add3dv(double v[],double v1[],double v2[]){
   v[0]=v1[0]+v2[0];
   v[1]=v1[1]+v2[1];
   v[2]=v1[2]+v2[2];
  return v;
}
double* scale3dv(double v[],double v1[],double n){
  
   v[0]=(n*v1[0]);
   v[1]=(n*v1[1]);
   v[2]=(n*v1[2]);
  return v;
}
void settexture(int n){
  if(n>=txturecount){
    return;
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    /* \u30c6\u30af\u30b9\u30c1\u30e3\u3092\u62e1\u5927\u30fb\u7e2e\u5c0f\u3059\u308b\u65b9\u6cd5\u306e\u6307\u5b9a */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  /* \u30c6\u30af\u30b9\u30c1\u30e3\u306e\u5272\u308a\u5f53\u3066 */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, textures[n]);
    

  glBindTexture(GL_TEXTURE_2D,textureID[n]);
}
void cube(double vec[],double scale)
{
  int i;
  int j;
  double v[]={0,0,0};
  glBegin(GL_QUADS);
  for (j = 0; j < 6; ++j) {
    glNormal3dv(normal[j]);
    for (i = 0; i < 4; ++i) {
      glVertex3dv(add3dv(v,scale3dv(v,vertex[face[j][i]],scale),vec)
);
    }
  }
  glEnd();
}
void boll(double vec[],double scale,int n){
  
   int i,j;
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,white);
   glEnable(GL_TEXTURE_2D);
   settexture(n);
   
   for (j=1;j<=4;j++){
       glBegin(GL_QUADS);
        //glNormal3dv(normal[n]); 
        for (i=0;i<8;i++){
        glTexCoord2d(0.125*(double)i,0.5-0.125*(double)j);
        glVertex3d(vec[0]+scale*cos((j*M_PI/8.0))*cos(M_PI_4*i),vec[1]+scale*sin(j*M_PI/8.0),vec[2]+scale*cos(j*M_PI/8.0)*sin(M_PI_4*i));
        glTexCoord2d(0.125*(double)i,0.5-0.125*(double)(j-1));
        glVertex3d(vec[0]+scale*cos((j-1)*M_PI/8.0)*cos(M_PI_4*i),vec[1]+scale*sin((j-1)*M_PI/8.0),vec[2]+scale*cos((j-1)*M_PI/8.0)*sin(M_PI_4*i));
        glTexCoord2d(0.125*(double)(i+1),0.5-0.125*(double)(j-1));
        glVertex3d(vec[0]+scale*cos((j-1)*M_PI/8.0)*cos(M_PI_4*(i+1)),vec[1]+scale*sin((j-1)*M_PI/8.0),vec[2]+scale*cos((j-1)*M_PI/8.0)*sin(M_PI_4*(i+1)));
        glTexCoord2d(0.125*(double)(i+1),0.5-0.125*(double)j);
        glVertex3d(vec[0]+scale*cos((j*M_PI/8.0))*cos(M_PI_4*(i+1)),vec[1]+scale*sin(j*M_PI/8.0),vec[2]+scale*cos(j*M_PI/8.0)*sin(M_PI_4*(i+1)));
        }
      glEnd();
      glBegin(GL_QUADS);
        //glNormal3dv(normal[n]); 
        for (i=0;i<8;i++){
        glTexCoord2d(0.125*(double)i,0.125*(double)j+0.5);
        glVertex3d(vec[0]+scale*cos((j*M_PI/8.0))*cos(M_PI_4*i),vec[1]+scale*sin(-j*M_PI/8.0),vec[2]+scale*cos(j*M_PI/8.0)*sin(M_PI_4*i));
        glTexCoord2d(0.125*(double)i,0.125*(double)(j-1)+0.5);
        glVertex3d(vec[0]+scale*cos((j-1)*M_PI/8.0)*cos(M_PI_4*i),vec[1]+scale*sin(-(j-1)*M_PI/8.0),vec[2]+scale*cos((j-1)*M_PI/8.0)*sin(M_PI_4*i));
        glTexCoord2d(0.125*(double)(i+1),0.125*(double)(j-1)+0.5);
        glVertex3d(vec[0]+scale*cos((j-1)*M_PI/8.0)*cos(M_PI_4*(i+1)),vec[1]+scale*sin(-(j-1)*M_PI/8.0),vec[2]+scale*cos((j-1)*M_PI/8.0)*sin(M_PI_4*(i+1)));
        glTexCoord2d(0.125*(double)(i+1),0.125*(double)j+0.5);
        glVertex3d(vec[0]+scale*cos((j*M_PI/8.0))*cos(M_PI_4*(i+1)),vec[1]+scale*sin(-j*M_PI/8.0),vec[2]+scale*cos(j*M_PI/8.0)*sin(M_PI_4*(i+1)));
        }
      glEnd();
   }
  glDisable(GL_TEXTURE_2D);
}
void room(){
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
  
  glBegin(GL_QUADS);
  glNormal3dv(normal[0]);
  glVertex3d(25,-25,25);
  glVertex3d(-25,-25,25);
  glVertex3d(-25,-25,-25);
  glVertex3d(25,-25,-25);
  
  
  glVertex3d(-25,25,25);
  glVertex3d(25,25,25);
  glVertex3d(25,-25,25);
  glVertex3d(-25,-25,25);
  
  glVertex3d(-25,25,-25);
  glVertex3d(-25,25,25);
  glVertex3d(-25,-25,25);
  glVertex3d(-25,-25,-25);
  
  glVertex3d(25,25,-25);
  glVertex3d(-25,25,-25);
  glVertex3d(-25,-25,-25);
  glVertex3d(25,-25,-25);

  glVertex3d(25,-25,-25);
  glVertex3d(25,25,-25);
  glVertex3d(25,25,25);
  glVertex3d(25,-25,25);
  
  glVertex3d(-25,25,-25);
  glVertex3d(25,25,-25);  
  glVertex3d(25,25,25);
  glVertex3d(-25,25,25);

  glEnd();

}
void display(void){
 /* \u8996\u70b9\u4f4d\u7f6e\u3068\u8996\u7dda\u65b9\u5411 */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glutSetCursor(GLUT_CURSOR_NONE);
  gluLookAt(pos[0],pos[1]+ 1.0, pos[2], cosf(angle[0])*cosf(angle[1])+pos[0],sinf(angle[1])+1+pos[1],sinf(angle[0])*cosf(angle[1])+pos[2], 0,1,0);
  glLightfv(GL_LIGHT0,GL_POSITION,light0pos);
  glLightfv(GL_LIGHT1,GL_POSITION,light1pos);
  room();
  glPushMatrix();
  double a[]={5*cosf(angle[0])*cosf(angle[1])+pos[0],5*sinf(angle[1])+1,5*(sinf(angle[0])*cosf(angle[1]))+pos[2]};
  cube(a,0.05);
  
  int i,j;
  srand((unsigned)time(NULL));
  for (i=0;i<objcount;i++){
    add3dv(Objects[i].position,Objects[i].position,Objects[i].verosity);
    for (j=0;j<3;j++){
     if(Objects[i].position[j]+Objects[i].radius>25){
      Objects[i].position[j]=50-Objects[i].position[j]-Objects[i].radius;
      Objects[i].verosity[j]*=(-1);
      } 
     if(Objects[i].position[j]-Objects[i].radius<-25){
      Objects[i].position[j]=-50-Objects[i].position[j]+Objects[i].radius;
      Objects[i].verosity[j]*=(-1);
     } 
    }
 
    //settexture(Objects[i].texture);
    boll(Objects[i].position,Objects[i].radius,Objects[i].texture);
  }
  glPopMatrix();
  glutSwapBuffers();
}

void resize(int w, int h){
   glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
   /* \u30e2\u30c7\u30eb\u30d3\u30e5\u30fc\u5909\u63db\u884c\u5217\u306e\u8a2d\u5b9a */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluPerspective(90.0, (double)w / (double)h, 0, 1000.0);
  gluLookAt( pos[0],pos[1]+ 1.0, pos[2], cosf(angle[0])*cosf(angle[1]),sinf(angle[1])+1,sinf(angle[0])*cosf(angle[1]), 0,1,0);
  
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':  /* '\033' \u306f ESC \u306e ASCII \u30b3\u30fc\u30c9 */
    exit(0);
  case 'a':
  case 'A':
     pos[0]+=cos(angle[0]-M_PI_2)*cosf(angle[1])*0.1;
     pos[2]+=sin(angle[0]-M_PI_2)*cosf(angle[1])*0.1;  
      if(pos[0]>50){
        pos[0]=50;
      }
      if(pos[0]<-50){
        pos[0]=-50;
      }
      if(pos[2]>50){
        pos[2]=50;
      }
      if(pos[2]<-50){
        pos[2]=-50;
      }
    break;
  case 's':
  case 'S':
     pos[0]-=cos(angle[0])*cosf(angle[1])*0.1;
     pos[2]-=sin(angle[0])*cosf(angle[1])*0.1;
      if(pos[0]>50){
        pos[0]=50;
      }
      if(pos[0]<-50){
        pos[0]=-50;
      }
      if(pos[2]>50){
        pos[2]=50;
      }
      if(pos[2]<-50){
        pos[2]=-50;
      }
    break;
  case 'd':
  case 'D':
     pos[0]+=cos(angle[0]+M_PI_2)*0.1;
     pos[2]+=sin(angle[0]+M_PI_2)*0.1;  
      if(pos[0]>50){
        pos[0]=50;
      }
      if(pos[0]<-50){
        pos[0]=-50;
      }
      if(pos[2]>50){
        pos[2]=50;
      }
      if(pos[2]<-50){
        pos[2]=-50;
      }
    break;
  case 'w':
  case 'W':
     pos[0]+=cos(angle[0])*0.1;
     pos[2]+=sin(angle[0])*0.1;
      if(pos[0]>50){
        pos[0]=50;
      }
      if(pos[0]<-50){
        pos[0]=-50;
      }
      if(pos[2]>50){
        pos[2]=50;
      }
      if(pos[2]<-50){
        pos[2]=-50;
      }
    break;
  default:
   // printf("%f %f %f\n",angle[0],pos[0],pos[2]);
    break;
  }
}

void shoot(void){
  //printf("sho");
}
void mouse(int button, int state, int x, int y)
{
 
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      /* \u30a2\u30cb\u30e1\u30fc\u30b7\u30e7\u30f3\u958b\u59cb */
      glutIdleFunc(shoot);
    }
    else {
      /* \u30a2\u30cb\u30e1\u30fc\u30b7\u30e7\u30f3\u505c\u6b62 */
      glutIdleFunc(0);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
    //\u8996\u91ce\u89d2\u3092\u72ed\u3081\u308b
    }
    else {
      //\u3082\u3068\u306b\u623b\u3059\u3002
    }
    break;
  default:
    break;
  }
}
  
void motion(int x, int y) {
  static GLboolean wrap = FALSE;

  if(!wrap) {
    int ww = glutGet(GLUT_WINDOW_WIDTH);
    int wh = glutGet(GLUT_WINDOW_HEIGHT);

    float dx = x - ww / 2;
    float dy = y - wh / 2;
    //printf("%d %d",dx,dy);
    // Do something with dx and dy here
    
    // move mouse pointer back to the center of the window
    wrap = TRUE;
    glutWarpPointer(ww / 2, wh / 2);
    angle[0]+=dx*0.001;
    angle[1]+=dy*0.001;
    if(angle[0]<-M_PI){
      angle[0]+=M_PI*2;
    }else if(angle[0]>M_PI){
      angle[0]-=M_PI*2;
    }
    if(angle[1]<-M_PI){
      angle[1]+=2*M_PI;
    }
    if(angle[1]>M_PI){
      angle[1]-=2*M_PI;
    }
    //printf("%f %f \n", angle[0], angle[1]);
 //   glutPostRedisplay();
  } else {
    wrap = FALSE;
  }  
 
}

void timer(int value){
   /* \u753b\u9762\u3092\u518d\u63cf\u5199 */
   glutPostRedisplay();
   /* 100\u30df\u30ea\u79d2\u5f8c\u306b\u518d\u5b9f\u884c */
   glutTimerFunc(100, timer, 0);
}

void ballinit(int n,struct Obj *o){
  objcount=n;
  int i;
  for (i=0;i<n;i++){
    o[i].position[0]=rand()%25-12;
    o[i].position[1]=rand()%25-12;
    o[i].position[2]=rand()%25-12;
    o[i].radius=rand()%5+1;
    o[i].verosity[0]=rand()%3-2;
    o[i].verosity[1]=rand()%3-2;
    o[i].verosity[2]=rand()%3-2;
    o[i].texture=textureID[rand()%5];
   // o[i].texture=textureID[4];
  }
}

void loadtexture(char *c){
    /* \u30c6\u30af\u30b9\u30c1\u30e3\u306e\u8aad\u307f\u8fbc\u307f\u306b\u4f7f\u3046\u914d\u5217 */
  
  FILE *fp;
  GLubyte texture[TEXWIDTH][TEXHEIGHT][4];
  /* \u30c6\u30af\u30b9\u30c1\u30e3\u753b\u50cf\u306e\u8aad\u307f\u8fbc\u307f */
  if ((fp = fopen(c, "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    int i,j,k;
    for(i=0;i<TEXWIDTH;i++){
      for(j=0;j<TEXHEIGHT;j++){
        for(k=0;k<4;k++){
          textures[txturecount][i][j][k] = texture[i][j][k];
        }
      }
    }
    glBindTexture(GL_TEXTURE_2D,textureID[txturecount]);
    fclose(fp);
  
    txturecount++;
   
  }else {
    perror(c);
  }
  
  

}

void init(void){
 glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
 //glCullFace(GL_FRONT);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,green);
  glLightfv(GL_LIGHT1,GL_SPECULAR,green);
  glGenTextures(5,textureID);
  loadtexture("./moon.raw");
  loadtexture("./plute.raw");
  loadtexture("./jup.raw");
  loadtexture("./ven.raw");
  loadtexture("./ear0xuu2.raw");
  
  ballinit(40,Objects); 
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
}

int main(int argc, char *argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(motion);
  glutMouseFunc(mouse);
  glutTimerFunc(100,timer,0);
  init();
  glutMainLoop();
  return 0;
}