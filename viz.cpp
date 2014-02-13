#include "math.h"
#include "string.h"
#include "stdio.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"
#define TRAIN 0
#define TEST 1
#define file TEST
/*
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15
*/
FILE *fin;
float alpha,beta;
char cifra[784];
int icifra=0;
int number;

void doshit();
class Matrix
{	
	public:	
	float f[16];	
	
	Matrix()
	{	memset(f,0,sizeof(*f)*16);
	}

	Matrix(float diag)
	{	memset(f,0,sizeof(*f)*16);
		int i;
		for(i=0;i<16;i+=5)
		{	f[i]=diag;	
		}
	}

	Matrix(float *f)
	{	int i;
		for(i=0;i<16;i++)
		{	this->f[i]=f[i];	
		}	
	}

	void translate(float x, float y, float z)
	{	f[12]+=x;
		f[13]+=y;
		f[14]+=z;		
	}
	void rotate(float angle,float x,float y,float z)
	{		
					
	}

	static Matrix srotatex(float rad)
	{	Matrix r;
		r.set(0,0,1.0);
		float sinx=sin(rad);				
		float cosx=cos(rad);				
		r.set(1,1,cosx);
		r.set(1,2,sinx);
		r.set(2,1,-sinx);
		r.set(2,2,cosx);
		r.set(3,3,1.0);
		return r;
	}

	static Matrix srotatey(float rad)
	{	Matrix r;
		r.set(1,1,1.0);
		float sinx=sin(rad);				
		float cosx=cos(rad);				
		r.set(0,0,cosx);
		r.set(0,2,sinx);
		r.set(2,0,-sinx);
		r.set(2,2,cosx);
		r.set(3,3,1.0);
		return r;
	}
/*
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15
*/

	void rotatex(float rad)
	{	int i;	
		Matrix rotation=Matrix::srotatex(rad);
		Matrix justRotation=getRotation();
		Matrix mulRotation=rotation.multiply(justRotation);
		Matrix result=mulRotation.add(getTranslation());
		for(i=0;i<16;i++)
		{	f[i]=mulRotation.f[i];		
		}	
	}
	Matrix add(Matrix thing)
	{	Matrix result;
		int i;
		for(i=0;i<16;i++)
		{	result.f[i]=thing.f[i]+f[i];		
		}
		return result;
	}
	Matrix getRotation()
	{	return getRotation(*this);
	}

	//scaling is still here
	Matrix getRotation(Matrix m)
	{	int i;	
		Matrix r;	
		memcpy(r.f,m.f,16*sizeof(float));		
		for(i=12;i<=14;i++)
		{	r.f[i]=0.0;}
		return r;				
	}
	//no scaling yet again
	Matrix getTranslation()
	{	return getTranslation(*this);
	}
	Matrix getTranslation(Matrix m)
	{	Matrix r(1.0);
		int i;
		for(i=12;i<=14;i++)
		{	r.f[i]=m.f[i];}
		return r;
	}	
	void rotatey(float rad)
	{	int i;	
		Matrix r=Matrix::srotatey(rad);
		Matrix k=multiply(r);
		for(i=0;i<16;i++)
		{	f[i]=k.f[i];		
		}	
	}

	float get(int i,int j)
	{	return f[4*j+i];
	}

	void set(int i,int j,float v)
	{	f[4*j+i]=v;
	}

	Matrix multiply(Matrix p)
	{	int i,j,k;
		float t;	
		Matrix r;

		for(i=0;i<4;i++)
		{	for(j=0;j<4;j++)
			{ t=0;	
				for(k=0;k<4;k++)
				{	t+=get(i,k)*p.get(k,j);
				}
				r.set(i,j,t);
			}
		}
		return r;	
	}	

	void print()
	{	int i,j;
		for(i=0;i<4;i++)
		{	for(j=0;j<4;j++)
			{	printf("%f ",f[i+j*4]);
			}
			printf("\n");
		}
		printf("\n");
	}
	void setMatrix()
	{	glLoadMatrixf(f);	
	}
};

Matrix global(1.0);

class Object
{	private:
	Matrix m;	
	public:
					
};

void key(int key, int x, int y)
{	printf("%d",key);
	switch(key)
	{	case GLUT_KEY_RIGHT:
			printf("right\n");
			global.translate(0.1,0,0);
			break;
		case GLUT_KEY_LEFT:
			printf("left\n");
			global.translate(-0.1,0,0);
			break;
		case GLUT_KEY_UP:
			printf("top\n");
			global.translate(0,0.1,0);
			break;
		case GLUT_KEY_DOWN:
			printf("bottom\n");
			global.translate(0,-0.1,0);
			break;
	}
	glutPostRedisplay();
	fflush(stdout);	
}
void readfile(int);
void otherkeys(unsigned char key,int x,int y)
{	printf("%c\n",key);
	fflush(stdout);
	if('0'<=key&&key<='9')
	{	number=number*10+key-'0';
	}
	switch(key)
	{	case '+':
			icifra++;
			printf("%d\n",icifra);
			readfile(icifra);	
			break;
		case '-':
			icifra--;
			printf("%d\n",icifra);
			readfile(icifra);	
			break;
		case 'w':
				alpha+=0.1;
//			global.rotatex(0.1);
			break;
		case 's':
				alpha-=0.1;
//			global.rotatex(-0.1);
			break;
	 	case 'd':
				beta+=0.1;
//			global.rotatey(0.1);
			break;
		case 'a':
				beta-=0.1;
				break;
//			global.rotatey(-0.1);
		case 13:
			if(file==TEST)
			{	if(0<number&&number<=28000)
				{
					readfile(number);
					printf("%d\n",number);
				}
			}
			else if(file==TRAIN)
			{	if(0<number&&number<=42000)
				{
					readfile(number);
					printf("%d\n",number);
				}
			}
			number=0;
			break;
	}
	doshit();
	glutPostRedisplay();
}
void doshit()
{	global=Matrix(1.0);
	Matrix rx=Matrix::srotatex(alpha);
	Matrix ry=Matrix::srotatey(beta);
	global=rx.multiply(ry);
	global.translate(-8.0,-6,-20);
}
void drawCube()
{	int i,j,d;
	glBegin(GL_LINES);
		for(i=0;i<8;i++)
		{	for(j=i+1;j<8;j++)
			{	d=j^i;
				if(d==1||d==2||d==4)
				{	glVertex3i(i%2,i/2%2,i/4%2 );	
					glVertex3i(j%2,j/2%2,j/4%2 );	
				}					
			}
		}
	glEnd();
}
void drawSquare(int x,int y,int c)
{	
	glColor3ub(c,c,c);	
	glBegin(GL_POLYGON);
	int z=-10;	
	glVertex3i(x,y,z);
	glVertex3i(x,y+1,z);
	glVertex3i(x+1,y+1,z);
	glVertex3i(x+1,y,z);
	glEnd();
}
void drawCifra()
{	int i,j;
	for(i=0;i<28;i++)
	{	for(j=0;j<28;j++)
		{	drawSquare(i,j,cifra[28*i+j]);
		}
	}
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINE_LOOP);
	int z=-10;
	glVertex3i(0,0,z);			
	glVertex3i(28,0,z);			
	glVertex3i(28,28,z);			
	glVertex3i(0,28,z);			
	glEnd();
}
void renderFunction()
{	int i;
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);	
	glColor3f(1.0,1.0,1.0);	
	glMatrixMode(GL_PROJECTION); 
        glLoadIdentity();
//				glRotatef(angle2,0.0,0.0,1.0);
        gluPerspective(60, 1.333, 0.1, 1000);
        
	glMatrixMode(GL_MODELVIEW);
	global.setMatrix();
	glTranslatef(0,0,-10.0);
	drawCifra();	

	glColor3f(1.0,0.0,0.0);	
	for(i=1;i<8;i++)
	{	global.setMatrix();
		glTranslatef(i*2.0,0,0);
		drawCube();
	}

	glColor3f(0.0,1.0,0.0);	
	for(i=1;i<6;i++)
	{
		global.setMatrix();
		glTranslatef(0,i*2.0,0);
		drawCube();
	}
	
	glColor3f(0.0,0.0,1.0);	
	for(i=0;i<5;i++)
	{
		global.setMatrix();
		glTranslatef(0,0,i*2.0);
		drawCube();
	}

	glFlush();
}

/*mouse*/
int prevx,prevy,dx,dy;
void rotate(int dx,int dy)
{	printf("%f %f\n",dy/10.0,dy/10.0);
	alpha-=dy/50.0;
	beta+=dx/50.0;
	doshit();
}

void passive(int x,int y)
{	prevx=x;prevy=y;}

void active(int x,int y)
{	dx=x-prevx;
	prevx=x;
	dy=y-prevy;
	prevy=y;
	rotate(dx,dy);
}

void someshit(int x,int y,int a,int b)
{	printf("asd\n");
}
void globalInit()
{	global.translate(-8.0,-6.0,-20.0);
//	global.rotatex(1);
}
void cifraInit()
{
	int i,j;
	for(i=0;i<28;i++)
		for(j=0;j<28;j++)
			cifra[i*28+j]=((i+j)%2)*255;
}
void readfile(int offset)
{	offset--;
	if(file==TEST)
	{ offset=(28*28)*offset;
	}
	else if (file==TRAIN)
	{	offset=(28*28+1)*offset;
	}
	fseek(fin, offset, SEEK_SET);
	int i,j;
	char c;
	if(file==TRAIN) 
		fscanf(fin,"%c",&c);
	for(i=27;i>=0;i--)
	{	for(j=0;j<28;j++)
		{	fscanf(fin,"%c",&cifra[28*j+i]);
		}
	}
}
int fileinit()
{
	if(file==TRAIN)
	{
    	fin=fopen("trainbin","rb");
	}
	else if(file==TEST)
	{
    	fin=fopen("testbin","rb");
	}

	if(fin==NULL)
	{   return 0;
	}
    return 1;
}

int main (int argc, char* argv[])
{	
    if(!fileinit())
    {
        printf("input file missing\n");
        return 0;
    }
	globalInit();	
	readfile(icifra);
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(400,300);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Open GL bla bla");
	glutDisplayFunc(renderFunction);
	glutIdleFunc(renderFunction);
	glutSpecialFunc(key);
	glutKeyboardFunc(otherkeys);
	glutMotionFunc(active);
	glutPassiveMotionFunc(passive);
	glutMainLoop();	
	return 0;
}
