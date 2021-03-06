/***************************
 *                         *
 *   Andres Barrera        *
 *   Wladimir Albornoz     *
 *                         *
 ***************************/
//
//Librerias
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>
//Constantes
#define NUM_TEXTURAS 16
#define MAX_TAM_ESFERA 10000
//
//Estructuras
typedef struct config_tag {
    short int SCREEN_SIZE_X;
    short int SCREEN_SIZE_Y;
    char FULLSCREEN;
} configuracion;

typedef	struct {
    GLubyte	*imageData;
    GLuint	bpp;
    GLuint	width;
    GLuint	height;
    GLuint	texID;
} Imagen;

typedef struct Point {
  double x;
  double y;
  double z;
} point;

struct vertice{
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float su;
    float tv;
}vertice;

//Variables
GLfloat anos;
GLfloat limite=3.000;
GLint dia;
GLint hora;
GLint min;
GLfloat z;
GLfloat der;
//GLfloat izq;
GLfloat arr;

int aux2=0;
float mov;
float mov2;
int mov3;
float distancia=0;
int explo;
float cant;
float aux=-1;
int d0=0,d1=0,d2=0,d10=0,d3=0,d4=0,d5=0,d6=0,d7=0,d8=0,d9=0,d11=0,d12=0,d13=0,d14=0,d15=0,d16=0,d17=0,d18=0,d19=0;
float control=0;
int control1=-1;
float a0=0,a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0,a8=0,a9=0,a10=0,a11=0,a12=0,a13=0,a14=0,a15=0,a16=0,a17=0,a18=0,a19=0;

float a[]={-17,-15.5, -13.4,-7.0,-2.0,0,2,7.0,13.0,15.3,17,-7,-2,0.1,2.1,7,13.1,15.4};
float b[]={0, 2.0, 7.0, 13.4, 15.3, 17.0, 15.5, 13.4, 7.1, 2.1,0.1,-13,-15.3,-17,-15.5,-13.4,-7,-3};
int d[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

GLfloat mat_emision_explosion[] = {1, 0.0, 0.0, 1.0};
GLint i;

static int slices=16;
static int stacks= 16;

char *textura_datos[NUM_TEXTURAS];  /* Aquí almacenaremos las texturas */
GLuint textura[NUM_TEXTURAS];       /* Aquí llevaremos el número de texturas*/
configuracion config;               /* Variable para almacenar la configuración */
char resultado_carga_tga[100];
int sw=10;
float x3=-13.4;

static int window;
static int menu_id;
static int solarSytem_submenu_id;
static int planets_submenu_id;
static int value = 0;
//Declaramos los planetas tipo GLfloat
GLfloat mercurio_matrix[16];
GLfloat venus_matrix[16];
GLfloat tierra_matrix[16];
GLfloat marte_matrix[16];
GLfloat jupiter_matrix[16];
GLfloat saturno_matrix[16];
GLfloat urano_matrix[16];
GLfloat neptuno_matrix[16];
GLfloat pluton_matrix[16];
//Tipo Planetas con coordenadas para clasificacion 3D
double mercurioXYZ[3];
double venusXYZ[3];
double tierraXYZ[3];
double marteXYZ[3];
double jupiterXYZ[3];
double saturnoXYZ[3];
double uranoXYZ[3];
double neptunoXYZ[3];
double plutonXYZ[3];
//Ajustamos los valores de las coordenadas XYZ
float sol_[]={0.0,0.0,0.0};
float mercurio_[]={2.0,0.0,0.0};
float venus_[]={3.0,0.0,0.0};
float tierra_[]={3.5,0.0,0.0};
float marte_[]={7.0,0.0,0.0};
float jupiter_[]={10.0,0.0,0.0};
float saturno_[]={15.0,0.0,0.0};
float urano_[]={20.0,0.0,0.0};
float neptuno_[]={25.0,0.0,0.0};
float pluton_[]={30.0,0.0,0.0};
//Velocidad
float speed = 1;
int textOFF = 0;

/***********************
 *                     *
 *  F U N C I O N E S  * 
 *                     *
 ***********************/
void *CargaTGA(char *filename,int *tam) {
    GLubyte TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    GLuint temp,i;
    GLuint type=GL_RGBA;
    Imagen texture;
    GLubyte *aux;
    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        strcpy(resultado_carga_tga, "archivo"); //Copia del string
        return NULL;
    }
    
    /* Esto abre y comprueba que es un TGA */
    fread(TGAcompare,1,sizeof(TGAcompare),file);
    if (memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0){
        strcpy(resultado_carga_tga, "memcmp");
        return NULL;
    }
    /* Leemos la cabecera*/
    fread(header,1,sizeof(header),file);
    /* Determinamos el tamaño */
    texture.width = header[1] * 256 + header[0];
    texture.height = header[3] * 256 + header[2];
    // Vemos las características y comprobamos si son correctas
    if( texture.width	<=0	||
        texture.height	<=0	||
        texture.width >256      ||
        texture.height !=texture.width || ( header[4]!=32)) {
        fclose(file);
        strcpy(resultado_carga_tga, "tamagno"); //Copiamos otra vez el string
        return NULL;
    }
    /* Calculamos la memoria que será necesaria */
    texture.bpp	= header[4];
    bytesPerPixel = texture.bpp/8;
    imageSize = texture.width*texture.height*bytesPerPixel;
    /* Reservamos memoria */
    texture.imageData=(GLubyte *)malloc(imageSize);
    //sprintf(resultado_carga_tga, "%d", imageSize);
    /* Cargamos y hacemos alguna comprobaciones */
    if( texture.imageData==NULL || fread(texture.imageData, 1, imageSize, file)!=imageSize) {
        if(texture.imageData!=NULL)
            free(texture.imageData);
        fclose(file);
        strcpy(resultado_carga_tga, "imagedata");
        return NULL;
    }
    /* El TGA viene en formato BGR, lo pasamos a RGB */
    for(i=0; i<(GLuint)(imageSize); i+=bytesPerPixel) {
        temp=texture.imageData[i];
        texture.imageData[i] = texture.imageData[i + 2];
        texture.imageData[i + 2] = temp;
    }
    fclose (file);
    /* Ahora, cambiamos el orden de las líneas, como si hiciesemos
    un flip vertical. */
    aux=(GLubyte *)malloc(imageSize);
    for(i=0; i<texture.height; i++)
    memcpy(&aux[imageSize-((i+1)*texture.width*4)],&texture.imageData[i*texture.width*4],texture.width*4);
    /* tam devolverá el tamaño */
    *tam=texture.width;
    /* Liberamos memoria */
    free(texture.imageData);
    return aux;
}

int carga_texturas() {
    int tam[NUM_TEXTURAS];
    int i;
    char nombre_archivo[30];
    for(i = 0; i < NUM_TEXTURAS; i++) {
        sprintf(nombre_archivo,"img/textura%d.tga", i);
        textura_datos[i]=(char *)CargaTGA(nombre_archivo,&tam[i]);
        if (textura_datos[i] == NULL){
            //strcpy(resultado_carga_tga, "-1");
            return -1;
        }
        /* Genera una textura, referenciada por el entero textura */
        glGenTextures (NUM_TEXTURAS, &textura[i]);
        /* Esta función indica que será una textura en 2D. Las siguientes
        funciones hará referncia a esta textura */
        glBindTexture (GL_TEXTURE_2D, textura[i]);
        /* Aquí ajustamos algunos parámetros de la textura, concretamente los filtros */
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        /* Con esta función de GLU, creamos un mipmap. Nótese que especificamos
        el tamaño con tam, que devolvia la función CargaTGA*/
        gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tam[i], tam[i], GL_RGBA, GL_UNSIGNED_BYTE, textura_datos[i]);
        /* Liberamos la memoria que ya no utilizaremos para obtener un mejor rendimiento */
        free(textura_datos[i]);
    }
    return 0;
}

int RenderVertices(int renderPrimitive, /*BYTE*/char  *pVertex, unsigned int vertexSize, unsigned int vStart, unsigned int numVertex) { //,unsigned int vertexFormat ) 
    GLenum primitive;
    unsigned int offset;
    if ( !pVertex )
        return -1;
    switch ( renderPrimitive ) {
        case 0:
            primitive= GL_POINTS;
            break;
        case 1:
            primitive= GL_LINE_STRIP;
            break;
        case 2:
            primitive= GL_LINES;
            break;
        case 3:
            primitive= GL_TRIANGLE_FAN;
            break;
        case 4:
            primitive= GL_TRIANGLE_STRIP;
            break;
        default: //eTriangleList
            primitive= GL_TRIANGLES;
            break;
    }
    
    offset= vStart*vertexSize;
    glEnableClientState(GL_VERTEX_ARRAY);
    //(3,El vértice se compone de 3 nº reales., Tamaño de la estructura que define el vértice., Dónde empieza el primer vrtice a renderizar.)
    glVertexPointer(3, GL_FLOAT, vertexSize, pVertex);	
    offset += 3*sizeof(float);
    
    // Especificamos las normales
    glEnableClientState	(GL_NORMAL_ARRAY);
    //( Tipo de coordenada de la normal., Tamaño de la estructura que define el vértice.,Dónde empieza la primera normal.)
    glNormalPointer(GL_FLOAT, vertexSize, pVertex+offset);
    offset += 3*sizeof(float);

    // Especificamos las coordenadas de textura
    glEnableClientState	( GL_TEXTURE_COORD_ARRAY );
    //(Nº de coordenadas de textura.,Tipo de coordenadas de textura.,Tamaño de la estructura que define el vértice.,Dónde empieza la primera coordenada.)
    glTexCoordPointer	( 2, GL_FLOAT, vertexSize, pVertex+offset );
    offset += 2*sizeof(float);
   
    // Renderizamos
    glDrawArrays( primitive, 0, numVertex );
    return 0;
}
//Funcion para renderizar la esfera
void render_esfera(float m_Radius, int sl, int st , int id_textura) {
    struct vertice m_Vertex[sl*st*2];
    float x0, y0, z0;
    float x1, y1, z1;
    float sinxy;
    int i, j, v= 0;
    unsigned int numVertex  = 2*sl*st;
    float        xzAngle    = 2.0f*3.1415/(float)sl;
    float        xyAngle    = 2.0f*3.1415/(float)st;
    glEnable(GL_TEXTURE_2D);
    // Con esta función indicamos que vamos a utilizar la textura
    glBindTexture(GL_TEXTURE_2D, textura[id_textura]);
    glColor3d(1,1,1); //El color que le asignamos en 3D
    if ( numVertex <= MAX_TAM_ESFERA ) {
    	for (i=0; i < sl; i++){// Meridianos
            for (j=0; j < st; j++){// Paralelos
                sinxy= sin(j*xyAngle);  // Para no calcularlo 4 veces
                x0= sinxy * cos(i*xzAngle);
                y0= cos(j*xyAngle);
                z0= sinxy * sin(i*xzAngle);
                // Vertice
                m_Vertex[v].x = m_Radius*x0;
                m_Vertex[v].y = m_Radius*y0;
                m_Vertex[v].z = m_Radius*z0;
               // Normal
                m_Vertex[v].nx= x0;
                m_Vertex[v].ny= y0;
                m_Vertex[v].nz= z0;
               //m_Vertex[v].su= 1.0f-i/(float)slices;               // SU= 1-i*xzAngle/2PI
                m_Vertex[v].su= i/(float)sl;                    // SU= i*xzAngle/2PI
                m_Vertex[v].tv= ((float)st-2.0f*j)/(float)st;   // TV= 1-j*xyAngle/PI
                v++;
                x1= sinxy * cos((i+1)*xzAngle);
                y1= y0;
                z1= sinxy * sin((i+1)*xzAngle);
                m_Vertex[v].x = m_Radius*x1;
                m_Vertex[v].y = m_Radius*y1;
                m_Vertex[v].z = m_Radius*z1;
                m_Vertex[v].nx= x1;
                m_Vertex[v].ny= y1;
                m_Vertex[v].nz= z1;
                //   m_Vertex[v].su= 1.0f-(i+1)/(float)slices;
                m_Vertex[v].su= (i+1)/(float)sl;
                m_Vertex[v].tv= m_Vertex[v-1].tv;   // Igual que el anterior.
                v++;
            }
        }
        RenderVertices( 4, (char *)m_Vertex, sizeof(vertice), 0, numVertex);//, eFVF_XYZ|eFVF_NORMAL|eFVF_TEX1 );
    }
    glDisable(GL_TEXTURE_2D);
}

//Función creadora de estrellas (NO USADA)
void muerte (float x, float y , float z){
    //float i = 1.5;
    glPushMatrix ();
    GLfloat light_direction[] = { 1.0, 1.0, 0.0 };
    GLfloat light_position[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_emision[] = {1.0, 0.9, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.9, 0.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 0.0, 1.0f};
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = anos/365;
    printf("%f", anos);
    if(anos < 3.000){
        glScalef( anos, anos,anos );
        render_esfera(2, 100, 32 , 4);
        glTranslatef (x, y, z);
        
    }
    else{ 
        if(limite > 0.2){
        glScalef( limite, limite, limite );
        render_esfera(2, 100, 32 , 4);
        glTranslatef (x, y, z);
        limite-=0.001;
        }
        
    }
    glPopMatrix ();
    
}
//Funciones creadoras de los planetas
void sol(float x, float y, float z){
    float i = 1.5;
    glPushMatrix ();
    GLfloat light_direction[] = { 1.0, 1.0, 0.0 };
    GLfloat light_position[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_emision[] = {1.0, 0.9, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.9, 0.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 0.0, 1.0f};
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    if(limite < 3  && limite > -0.19){
        glScalef( limite, limite, limite );
        glTranslatef (x, y, z);
        render_esfera(2, 100, 32 , 4);
        glPopMatrix();
    }
    else{
        glTranslatef (x, y, z);
        render_esfera(2, 100, 32 , 0);
        glPopMatrix ();
    }
    //else{
     //   glScalef( limite, limite, limite );
      //  glTranslatef (x, y, z);
       // render_esfera(1, 100, 32 , 0);    
    //}
}

void mercurio(float x,float y,float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    if(limite<=2.98){
        glRotatef( anos/0.34, 0,0,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.0, 0, 0 , 0);
    }
    else{    
        glRotatef( anos/0.34, 0,1,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.03, 100, 32 , 3);
    }
    //glutSolidSphere (0.03, 100, 100);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+0.3, (GLfloat)y+2.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "MERCURIO";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void venus(float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_venus);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_venus);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    if(limite<=2.80){
        glRotatef( anos/0.6, 0,0,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.0, 0, 0 , 0);
    }
    else{
        glRotatef( anos/0.6, 0,1,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.08, 100, 32 , 2);
    }
    //glutSolidSphere (0.08, 100, 100);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1, (GLfloat)y-2.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "VENUS";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void tierra(float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0, 0.2, 1.0f}; 
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 686)*360;
    // glColor3f(0.0f, 1.0f, 0.0f);
    if(limite<=2.80){
        glRotatef( anos, 0,1,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.0, 0, 0 , 0);
    }
    else{
        glRotatef( anos, 0,1,0 );
        glTranslatef (x, y, z_); 
        render_esfera(0.08, 100, 32 , 1);
    }
    //glutSolidSphere (0.08, 100, 100);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1, (GLfloat)y+0.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "TIERRA";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void marte (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.5, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_marte);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_marte);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/1.88, 0,1,0 );
    glTranslatef (x,y,z_); 
    render_esfera(0.046, 100, 32 , 4);    
    //glutSolidSphere (0.046, 100, 100);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1, (GLfloat)y-1, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "MARTE";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void jupiter (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    glRotatef( anos/11, 0,1,0 );
    glTranslatef (x, y, z_); 
    render_esfera(0.959, 100, 32 , 5);
    glGetFloatv(GL_MODELVIEW_MATRIX,jupiter_matrix);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1.5, (GLfloat)y-1.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "JUPITER";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void saturno (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    anos = ((float) dia / 365)*360;
    //Rotación 90°: Los ejes pasan de estar x-y-z => x-z-y.
    //Se aplica la rotación a ambos objetos, cambiando el eje en torno al cual 
    //gira el planeta (Primero en torno al eje Y, ahora en torno al eje Z).
    glRotatef(90, 1, 0, 0);
    glRotatef(-anos/25, 0, 0, 1);   
    glTranslatef (x, y, z_);
    render_esfera(0.805, 100, 32 ,6);
    glScalef(1, 1, 0.015);
    glutSolidTorus(0.3, 1.5, 32 , 32);
    glGetFloatv(GL_MODELVIEW_MATRIX, saturno_matrix);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1.5, (GLfloat)y-1.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "SATURNO";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void urano (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0.0, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.0, 0.5, 0.0, 1.0f};
    GLfloat mat_specular[] = {0.0, 0.5, 0.0, 1.0f};
    GLfloat mat_ambient[] = {0.0, 0.5, 0.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    glRotatef(anos/84, 0,1,0 );
    glTranslatef (x, y, z_); 
    render_esfera(0.362, 100, 32 , 7);
    glGetFloatv(GL_MODELVIEW_MATRIX, urano_matrix);
    glPopMatrix ();
    
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1.5, (GLfloat)y-1.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "URANO";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void neptuno (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0.0, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.3, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    glRotatef( anos/164, 0,1,0 );
    glTranslatef (x, y, z_); 
    render_esfera(0.328, 100, 32 , 8);
    glPopMatrix ();
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+1.5, (GLfloat)y-3.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "NEPTUNO";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}

void pluton (float x, float y, float z_){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.2, 0.5, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    glRotatef( anos/185, 0,1,0 );
    glTranslatef (x, y, z_); 
    render_esfera(0.170, 100, 32 , 9);
    glPopMatrix ();
    //Agregamos el nombre del planeta sólo al principio
    if ( textOFF == 0 && der == -5 && arr == 0 && z == -35) {
        glPushMatrix ();
        glDisable(GL_LIGHTING);
        glRasterPos3f((GLfloat)x+4.5, (GLfloat)y-2.5, (GLfloat)z_);
        glColor3f(1,1,1);
        const char *message = "PLUTON";
        while (*message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix ();
    }
}
//Fin funciones planetas

int walkFromTO ( float fromX, float fromZ, float toX, float toZ ) {
    float newDistanceX =  (toX - fromX)/50;
    float newDistanceZ =  (toZ - fromZ)/50;
    der = fromX + newDistanceX;
    z = fromZ + newDistanceZ;

    if ( fabs(toX-der) > 0.1 || fabs(toZ-z)> 0.1 )
        return 1;
    else
        return 0;
    
}

void getOrbitStartPoint (double angulo, double *coordenadas, float x, float y, float z) {
    coordenadas[0] = cos(angulo)*x + sin(angulo)*z;
    coordenadas[1] = y;
    coordenadas[2] = -sin(angulo)*x + cos(angulo)*z;
}

void menuChoise ( ){
    float flotante = 0;
    double angulo;
    switch (value){
        case 0:
            //set camera position
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 1:
            printf("Catastrofe\n");
            textOFF = 1;
            hora += 50.0;
            aux2 = 1;
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            muerte(der, arr, z);
            break;
        case 2:
            printf("Iniciar Movimiento \n" );
            textOFF = 1;
            hora += 50.0;
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 3:
            printf("Detener Movimiento\n");
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            hora  = 0.0;
            break;
        case 4:
            printf("Sol\n");
            walkFromTO (der, z, 0.0, -7.6);
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 5:
            printf("Salir\n");
            exit(1);
            break;
    }
    printf ( "Speed = %F\n", speed);
    printf ( "( x, y,  z ) = ( %F, %F , %F)\n", der, arr, z);
    printf ( "Angulo = %F\n", angulo);
}

//Aplicamos la técnica SkyBox para aplicar una textura de fondo (Situar la escena dentro de un cubo con textura)
//El orden siempre es FRENTE>IZQUIERDA>ATRÁS>DERECHA>ARRIBA>ABAJO
void loadSkyBox ( ) {
    glPushMatrix();
    
     //Habilitamos y deshabilitamos algunas características.
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    
    //FRENTE
    glBindTexture(GL_TEXTURE_2D, textura[10]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  200.5f, -200.5f, -200.5f );
        glTexCoord2f(1, 0); glVertex3f( -200.5f, -200.5f, -200.5f );
        glTexCoord2f(1, 1); glVertex3f( -200.5f,  200.5f, -200.5f );
        glTexCoord2f(0, 1); glVertex3f(  200.5f,  200.5f, -200.5f );
    glEnd();
    
    //IZQUIERDA
    glBindTexture(GL_TEXTURE_2D, textura[11]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  200.5f, -200.5f,  200.5f );
        glTexCoord2f(1, 0); glVertex3f(  200.5f, -200.5f, -200.5f );
        glTexCoord2f(1, 1); glVertex3f(  200.5f,  200.5f, -200.5f );
        glTexCoord2f(0, 1); glVertex3f(  200.5f,  200.5f,  200.5f );
    glEnd();
    
    //ATRÁS
    glBindTexture(GL_TEXTURE_2D, textura[12]);
    glBegin(GL_QUADS);
       glTexCoord2f(0, 0); glVertex3f( -200.5f, -200.5f,  200.5f );
       glTexCoord2f(1, 0); glVertex3f(  200.5f, -200.5f,  200.5f );
       glTexCoord2f(1, 1); glVertex3f(  200.5f,  200.5f,  200.5f );
       glTexCoord2f(0, 1); glVertex3f( -200.5f,  200.5f,  200.5f );
    glEnd();
    
    //DERECHA
    glBindTexture(GL_TEXTURE_2D, textura[13]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -200.5f, -200.5f, -200.5f );
        glTexCoord2f(1, 0); glVertex3f( -200.5f, -200.5f,  200.5f );
        glTexCoord2f(1, 1); glVertex3f( -200.5f,  200.5f,  200.5f );
        glTexCoord2f(0, 1); glVertex3f( -200.5f,  200.5f, -200.5f );
    glEnd();
    
    //ARRIBA
    glBindTexture(GL_TEXTURE_2D, textura[14]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -200.5f,  200.5f, -200.5f );
        glTexCoord2f(0, 0); glVertex3f( -200.5f,  200.5f,  200.5f );
        glTexCoord2f(1, 0); glVertex3f(  200.5f,  200.5f,  200.5f );
        glTexCoord2f(1, 1); glVertex3f(  200.5f,  200.5f, -200.5f );
    glEnd();
    
    //ABAJO
    glBindTexture(GL_TEXTURE_2D, textura[15]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -200.5f, -200.5f, -200.5f );
        glTexCoord2f(0, 1); glVertex3f( -200.5f, -200.5f,  200.5f );
        glTexCoord2f(1, 1); glVertex3f(  200.5f, -200.5f,  200.5f );
        glTexCoord2f(1, 0); glVertex3f(  200.5f, -200.5f, -200.5f );
    glEnd();

    glPopAttrib();
    glPopMatrix();
    
}

void displayevent(void) {
    // limpia la escena (ventana)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    loadSkyBox();
     // inicializa la Matriz de transformación de coordenadas (Matriz del Modelo)
    glLoadIdentity();
    // verfica superficies visibles
    glEnable( GL_DEPTH_TEST );
    
    menuChoise( );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    //Dibujamos los planetas
    sol ( sol_[0],sol_[1],sol_[2] );
    mercurio ( mercurio_[0], mercurio_[1], mercurio_[2] );
    venus ( venus_[0], venus_[1], venus_[2] );
    tierra ( tierra_[0], tierra_[1], tierra_[2] );
    marte ( marte_[0], marte_[1], marte_[2] );
    jupiter ( jupiter_[0], jupiter_[1], jupiter_[2] );
    saturno ( saturno_[0], saturno_[1], saturno_[2] );
    urano ( urano_[0], urano_[1], urano_[2] );
    neptuno ( neptuno_[0], neptuno_[1], neptuno_[2] );
    pluton ( pluton_[0], pluton_[1], pluton_[2] );
    glutSwapBuffers();
}

void animacion() {
    // cambia el angulo de rotación constantemente (animación)
    if(hora>23){
        dia++;
        if(dia>364)
            anos++;
    }   
    // movimiento de la estrella nemesis
    if(hora>23)
        mov += mov2;
    if(mov>=59.99)
        mov2=(-1)*0.05;
    if(mov<0.01)
        mov2=0.05;
    if(control==1){
        if(control1==0||d0==1) { a0=a0+0.1; b[0]=b[0]+0.1;d0=1; } 
        if(control1==1||d1==1) { a1=a1+0.1; b[1]=b[1]+0.1;d1=1; }  
        if(control1==2||d2==1) { a2=a2+0.1;b[2]=b[2]+0.1; d2=1; }  
        if(control1==3||d3==1) { a3=a3-0.1; b[3]=b[3]+0.1;d3=1; }  
        if(control1==4||d4==1) { a4=a4-0.1; b[4]=b[4]+0.1;d4=1; }  
        if(control1==5||d5==1) { a5=a5-0.1; b[5]=b[5]+0.1;d5=1; }  
        if(control1==6||d6==1) { a6=a6-0.1; b[6]=b[6]+0.1;d6=1; } 
        if(control1==7||d7==1) { a7=a7-0.1; b[7]=b[7]+0.1;d7=1; }  
        if(control1==8||d8==1) { a8=a8-0.1; b[8]=b[8]+0.1;d8=1; }  
        if(control1==9||d9==1) { a9=a9-0.1; b[9]=b[9]+0.1;d9=1; }  
        if(control1==10||d10==1) { a10=a10-0.1; b[10]=b[10]+0.1;d10=1; }
        if(control1==11||d11==1) { a11=a11-0.1;b[11]=b[11]+0.1; d11=1; }
        if(control1==12||d12==1) { a12=a12-0.1; b[12]=b[12]+0.1;d12=1; }  
        if(control1==13||d13==1) { a13=a13-0.1; b[13]=b[13]+0.1;d13=1; }  
        if(control1==14||d14==1) { a14=a14-0.1; b[14]=b[14]+0.1;d14=1; } 
        if(control1==15||d15==1) { a15=a15-0.1; b[15]=b[15]+0.1;d15=1; }  
        if(control1==16||d16==1) { a16=a16-0.1; b[16]=b[16]+0.1;d16=1; }  
        if(control1==17||d17==1) { a17=a17-0.1; b[17]=b[17]+0.1;d17=1; }		
    }
    // redibuja la escena
    glutPostRedisplay();
}

void menu(int num){
    if(num == 0){
        glutDestroyWindow(window);
        exit(0);
    } else {
        value = num;
    }
    glutPostRedisplay();
}

void createMenu(void){
    //submenú del sistema solar
    solarSytem_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Catastrofe", 1);
    glutAddMenuEntry("Iniciar Movimiento", 2);
    glutAddMenuEntry("Detener Movimiento", 3);
    planets_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Sol", 4);
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Sistema Solar", solarSytem_submenu_id);
    glutAddSubMenu("Ir a", planets_submenu_id);
    glutAddMenuEntry("Salir", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//Función para trabajar con teclas especiales (F1,F2,...)
void specialkeyevent( int key, int Xx, int Yy ) {
    // manejo de teclas especiales
    switch ( key ) {
        // cambia la z para la traslación de la esfera
        case GLUT_KEY_UP:    z += 1.1; break;
        case GLUT_KEY_DOWN:  z -= 1.1; break;
        case GLUT_KEY_LEFT:  der += 1.05; break;
        case GLUT_KEY_RIGHT: der -= 1.05; break;
        // cambia el delta del angulo de rotación
        case GLUT_KEY_F1:    hora  = 0.0; break;
        case GLUT_KEY_F2:    hora += 50.0; textOFF=1; break;
        case GLUT_KEY_F3:    arr -= 0.05; break;
        case GLUT_KEY_F4:    arr += 0.05; break;
        case GLUT_KEY_F5:    arr = 0.00; break;
        case GLUT_KEY_F6:    der = 0.00; break;
        case GLUT_KEY_F8:    explo = 1; break;
        case GLUT_KEY_F7:    aux2= 1; break;
   }
   // redibuja la escena
   glutPostRedisplay();
}

void reshapeevent(GLsizei width, GLsizei height) {
    // establecer el área de visualizacion en la ventana
    glViewport(0,0,width,height);
    // seleccionar la matriz de Proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Vista en Perspectiva
    gluPerspective(60,(GLfloat)width/(GLfloat)height,  0.01, 500);
    // Restaurar a la matriz del Modelo (escena)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    
    int aux;	
    // inicialización del GLUT
    glutInit( &argc, argv );
    // inicialiación de la ventana
    glutInitWindowSize( 1020, 850 );  //Tamaño de la Ventana Creada
    glutInitWindowPosition( 100, 100 );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow( "Catastrofe Global - Fin del Sol" );
    glEnable(GL_SMOOTH);
    // inicialización de los datos del programa
    anos = 0;
    dia  = 0;
    hora = 0;
    min  = 0;
    explo= 0;
    mov  = 0;
    mov2 = 0.01;
    mov3 = 0;
    //derr = x; arr = y; z = z
    der  = -5;
    arr  = 5;
    z    = -35;
    //Variable para controlar el menú.
    value = 0;
    //Creamos el menú.
    createMenu();
    // registro de los eventos
    glutReshapeFunc (reshapeevent);
    glutDisplayFunc( displayevent );
    //Cambio de teclas por menú.
    glutSpecialFunc( specialkeyevent );
    //Controlamos no sólo las teclas especiales, las normales también 
    //(Para aumentar y reducir velocidad de rotación)
    //glutKeyboardFunc (Keyboard);
    glutIdleFunc( animacion );
    aux = carga_texturas();
    // lazo de eventos
    glutMainLoop();
    return 0;
}
