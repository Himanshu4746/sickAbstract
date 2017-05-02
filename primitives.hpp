
int windowWidth = 800,
    windowHeight = 600;
// coodinates of the center for the primitives objects.
int yPrimCoo,temp,xPrimCoo[6];

int flag=0,winStatus=0;

int windowStatus = 0;

int currentLevel = 1;

int primitiveColor[6][3] = {{239,83,80},{56,142,60},{33,150,243},{255,152,0},{69,39,160},{255,0,255}};
// recalculate the centers for the objects.
void calCenters()
{
    yPrimCoo = windowHeight-(0.15*windowHeight);
    temp = windowWidth/7;
    xPrimCoo[0] = temp;
    xPrimCoo[1] = 2*temp;
    xPrimCoo[2] = 3*temp;
    xPrimCoo[3] = 4*temp;
    xPrimCoo[4] = 5*temp;
    xPrimCoo[5] = 6*temp;
}

void *currentfont;
void setFont(void *font) //function to change the font of the text
{
	currentfont = font;
}
void drawstring(float x, float y, float z, char *str)//To render the text on the screen
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = str;*c != '\0';c++)
		glutBitmapCharacter(currentfont, *c);
}

void drawAxes()
{
    glColor4ub(255,87,51,255);

    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex3f(windowWidth/2,windowHeight*0.70,0.0);
        glVertex3f(windowWidth/2,0.0,0.0);
    glEnd();
}
void drawGrid()
{
    //int num = windowWidth/(5.0*4);
    glColor4ub(255,87,51,255);

    glPointSize(3.0);
    glBegin(GL_POINTS);
        for(int i=-200;i<=200;i+=20)
        {
             glVertex3f((windowWidth*0.25)+i,(windowHeight*0.35),0.0);
             glVertex3f((windowWidth*0.25),(windowHeight*0.35)+i,0.0);
        }
    glEnd();
}

void chooseColor(int r,int g,int b,int stat)
{
    if(stat==1)
        glColor4ub(r,g,b,255);
    else
        glColor4ub(61,61,61,255);

}

// movement global variables.
class movement
{
    public:
        int xMov,yMov;
        movement()
        {
            xMov=yMov=0;
        }
        void setDisp(int x,int y)
        {
            xMov=x;
            yMov=y;
        }
}mov[6];
class level
{
public:
    char lv[10],centerShape[27];
    int lvNo,shapeCount,shapeIndex[6],centerShapeIndex,completed;
    movement disp[6];
    level()
    {
        lvNo=2;
        strcpy(lv,"");
        shapeCount=3;

        shapeIndex[0]=0;
        shapeIndex[1]=0;
        shapeIndex[2]=1;
        shapeIndex[3]=1;
        shapeIndex[4]=1;
        shapeIndex[5]=0;

        strcpy(centerShape,"");

        disp[2].setDisp(0,0);
        disp[3].setDisp(0,5);
        disp[4].setDisp(0,-5);
    }

    void break_and_set_level(char *line,int n)
    {
        int temp=0;
        char *x=strtok(line,":");   // level number -atoi

        // checking if this is the level which is requested.
        if(atoi(x)!=n)
            return;

        char *y=strtok(NULL,":");   // level string -str
        char *z=strtok(NULL,":");   // shape count -atoi
        char *a=strtok(NULL,":");   // primitive status -atoi
        char *b=strtok(NULL,":");   // hint for the level -str
        char *c=strtok(NULL,":");   // primitive object coodinates -str
        char *innele=strtok(a,",");
        temp = 0;
        while( innele != NULL )
       {
            shapeIndex[temp++]=atoi(innele);
            innele = strtok(NULL, ",");
       }
        char *coo=strtok(c,",");
        temp = 0;
        while( coo != NULL )
        {
            int x = atoi(coo);
            coo = strtok(NULL, ",");

            int y = atoi(coo);
            coo = strtok(NULL, ",");
            disp[temp++].setDisp(x,y);
        }
        // set level number.
        lvNo = atoi(x);

        // set level string.
        strcpy(lv,y);

        // set shape count for the given level.
        shapeCount = atoi(z);

        // set center chape hint.
        strcpy(centerShape,b);
    }

    void DataExtract(int n)
    {
        char data[100];
        FILE *fp;
        fp = fopen("config","r");
        while(fgets(data, 255, (FILE*) fp)) {
            char *line=strtok(data,";");
            break_and_set_level(line,n);
        }
        fclose(fp);
    }

    void selectLevel(int n)
    {
        DataExtract(n);
    }
}play;

class PrimiviteShapes
{
public:
    void drawSquare(int x,int y)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="1.";
        drawstring(x,y+45,0.0,name);

        int num=30;
        glBegin(GL_POLYGON);
            glVertex3f(x-num,y+num,0.0);
            glVertex3f(x+num,y+num,0.0);
            glVertex3f(x+num,y-num,0.0);
            glVertex3f(x-num,y-num,0.0);
        glEnd();
    }
    void drawRectangle(int x,int y)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="2.";
        drawstring(x,y+45,0.0,name);

        int bre=30, len=50;

        glBegin(GL_POLYGON);
            glVertex3f(x-len,y+bre,0.0);
            glVertex3f(x+len,y+bre,0.0);
            glVertex3f(x+len,y-bre,0.0);
            glVertex3f(x-len,y-bre,0.0);
        glEnd();
    }
    void drawCircle(float cx, float cy, float r, int num_segments)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="3.";
        drawstring(cx,cy+45,0.0,name);

        glBegin(GL_POLYGON);
        for (int ii = 0; ii < num_segments; ii++)   {
            float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            float x = r * cosf(theta);//calculate the x component
            float y = r * sinf(theta);//calculate the y component
            glVertex3f(x + cx, y + cy,0.0);//output vertex
        }
        glEnd();
    }
    void drawTriangle(int cx,int cy)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="4.";
        drawstring(cx,cy+45,0.0,name);

        glBegin(GL_TRIANGLES);
            glVertex3f(cx,cy+30,0.0);
            glVertex3f(cx-(30*0.8660), cy-(50.0*0.5), 0.0);
            glVertex3f(cx+(30*0.8660), cy-(50.0*0.5), 0.0);
        glEnd();
    }
    void drawHexagon(int x, int y)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="5.";
        drawstring(x,y+45,0.0,name);

        glBegin(GL_POLYGON);
            glVertex3f(x-30,y,0.0);
            glVertex3f(x-(30*0.5),y+(30*0.8660),0.0);
            glVertex3f(x+(30*0.5),y+(30*0.8660),0.0);
            glVertex3f(x+30,y,0.0);
            glVertex3f(x+(30*0.5),y-(30*0.8660),0.0);
            glVertex3f(x-(30*0.5),y-(30*0.8660),0.0);
        glEnd();
    }
    void drawKite(int x,int y)
    {
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="6.";
        drawstring(x,y+45,0.0,name);

        glBegin(GL_POLYGON);
            glVertex3f(x,y+30,0.0);
            glVertex3f(x-30,y,0.0);
            glVertex3f(x,y-50,0.0);
            glVertex3f(x+30,y,0.0);
        glEnd();
    }
    void draw(int stat)
    {
        switch(stat)
        {
            case 0:drawSquare(xPrimCoo[0],yPrimCoo);break;
            case 1:drawRectangle(xPrimCoo[1],yPrimCoo);break;
            case 2:drawCircle(xPrimCoo[2],yPrimCoo,30,100);break;
            case 3:drawTriangle(xPrimCoo[3], yPrimCoo);break;
            case 4:drawHexagon(xPrimCoo[4],yPrimCoo);break;
            case 5:drawKite(xPrimCoo[5],yPrimCoo);break;
        }
    }
}topBar;
class PrimiviteMovShapes
{
public:
    void drawMovSquare(char a)
    {
        int x,y;
        if(a=='s'){
            x = (windowWidth*0.25)+(10.0*mov[0].xMov);
                y = (windowHeight*0.35)+(10.0*mov[0].yMov);
        }
        else if(a=='r'){
            x = (windowWidth*0.75)+(10.0*play.disp[0].xMov);
                y = (windowHeight*0.35)+(10.0*play.disp[0].yMov);
        }
        chooseColor(primitiveColor[0][0],primitiveColor[0][1],primitiveColor[0][2],play.shapeIndex[0]);
        int num=30;
        glBegin(GL_POLYGON);
            glVertex3f(x-num,y+num,0.0);
            glVertex3f(x+num,y+num,0.0);
            glVertex3f(x+num,y-num,0.0);
            glVertex3f(x-num,y-num,0.0);
        glEnd();
    }
    void drawMovRectangle(char a)
    {
        float x,y;
        int bre=30, len=50;
        if(a=='s'){
            x = (windowWidth*0.25)+(10.0*mov[1].xMov);
                    y = (windowHeight*0.35)+(10.0*mov[1].yMov);
        }
        else if(a=='r'){
            x = (windowWidth*0.75)+(10.0*play.disp[1].xMov);
                    y = (windowHeight*0.35)+(10.0*play.disp[1].yMov);
        }
        chooseColor(primitiveColor[1][0],primitiveColor[1][1],primitiveColor[1][2],play.shapeIndex[1]);
        glBegin(GL_POLYGON);
            glVertex3f(x-len,y+bre,0.0);
            glVertex3f(x+len,y+bre,0.0);
            glVertex3f(x+len,y-bre,0.0);
            glVertex3f(x-len,y-bre,0.0);
        glEnd();
    }
    void drawMovCircle(char a) {
        float num_segments=300,r=30,cx,cy;

        if(a=='s'){
            cx = (windowWidth*0.25)+(10.0*mov[2].xMov);
            cy = (windowHeight*0.35)+(10.0*mov[2].yMov);
        }
        else if(a=='r'){
            cx = (windowWidth*0.75)+(10.0*play.disp[2].xMov);
            cy = (+windowHeight*0.35)+(10.0*play.disp[2].yMov);
        }
        chooseColor(primitiveColor[2][0],primitiveColor[2][1],primitiveColor[2][2],play.shapeIndex[2]);
        glBegin(GL_POLYGON);
        for (int ii = 0; ii < num_segments; ii++)   {
            float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            float x = r * cosf(theta);//calculate the x component
            float y = r * sinf(theta);//calculate the y component
            glVertex3f(x + cx, y + cy,0.0);//output vertex
        }
        glEnd();
    }
    void drawMovTriangle(char a)
    {
        int cx,cy;

        if(a=='s'){
            cx = (windowWidth*0.25)+(10.0*mov[3].xMov);
            cy = (windowHeight*0.35)+(10.0*mov[3].yMov);
        }
        else if(a=='r'){
            cx = (windowWidth*0.75)+(10.0*play.disp[3].xMov);
            cy = (windowHeight*0.35)+(10.0*play.disp[3].yMov);
        }
        chooseColor(primitiveColor[3][0],primitiveColor[3][1],primitiveColor[3][2],play.shapeIndex[3]);
        glBegin(GL_TRIANGLES);
            glVertex3f(cx,cy+30,0.0);
            glVertex3f(cx-(30*0.8660), cy-(50.0*0.5), 0.0);
            glVertex3f(cx+(30*0.8660), cy-(50.0*0.5), 0.0);
        glEnd();
    }
    void drawMovHexagon(char a)
    {
        int x,y;

        if(a=='s'){
            x = (windowWidth*0.25)+(10.0*mov[4].xMov);
            y = (windowHeight*0.35)+(10.0*mov[4].yMov);
        }
        else if(a=='r'){
            x = (windowWidth*0.75)+(10.0*play.disp[4].xMov);
            y = (windowHeight*0.35)+(10.0*play.disp[4].yMov);
        }
        chooseColor(primitiveColor[4][0],primitiveColor[4][1],primitiveColor[4][2],play.shapeIndex[4]);
        glBegin(GL_POLYGON);
            glVertex3f(x-30,y,0.0);
            glVertex3f(x-(30*0.5),y+(30*0.8660),0.0);
            glVertex3f(x+(30*0.5),y+(30*0.8660),0.0);
            glVertex3f(x+30,y,0.0);
            glVertex3f(x+(30*0.5),y-(30*0.8660),0.0);
            glVertex3f(x-(30*0.5),y-(30*0.8660),0.0);
        glEnd();
    }
    void drawMovKite(char a)
    {
        int x,y;

        if(a=='s'){
            x = (windowWidth*0.25)+(10.0*mov[5].xMov);
            y = (windowHeight*0.35)+(10.0*mov[5].yMov);
        }
        else if(a=='r'){
            x = (windowWidth*0.75)+(10.0*play.disp[5].xMov);
            y = (windowHeight*0.35)+(10.0*play.disp[5].yMov);
        }
        chooseColor(primitiveColor[5][0],primitiveColor[5][1],primitiveColor[5][2],play.shapeIndex[5]);
        glBegin(GL_POLYGON);
            glVertex3f(x,y+30,0.0);
            glVertex3f(x-30,y,0.0);
            glVertex3f(x,y-50,0.0);
            glVertex3f(x+30,y,0.0);
        glEnd();
    }
    void draw(int stat, char a)
    {
        // a='r' means result to be displayed in the right side of the screen
        // a='s' means solution to be displayed in the left side of the screen.
        switch(stat)
        {
            case 0:if(play.shapeIndex[0]==1)drawMovSquare(a);break;
            case 1:if(play.shapeIndex[1]==1)drawMovRectangle(a);break;
            case 2:if(play.shapeIndex[2]==1)drawMovCircle(a);break;
            case 3:if(play.shapeIndex[3]==1)drawMovTriangle(a);break;
            case 4:if(play.shapeIndex[4]==1)drawMovHexagon(a);break;
            case 5:if(play.shapeIndex[5]==1)drawMovKite(a);break;
        }
    }

}item;

void window3()
{
    // draw the top bar shapes with the specific color with which they are needed.
    for(int i=0;i<6;i++)
    {
        chooseColor(primitiveColor[i][0],primitiveColor[i][1],primitiveColor[i][2],play.shapeIndex[i]);
        topBar.draw(i);
    }

    drawAxes();
    drawGrid();

    // display the level of the stage.
    setFont(GLUT_BITMAP_HELVETICA_18);
    drawstring((3*windowWidth/4-50),(windowHeight*0.70),0.0,play.lv);

    // display the warning for each stage.
    setFont(GLUT_BITMAP_HELVETICA_18);
    drawstring((windowWidth/4-100),(windowHeight*0.03),0.0,play.centerShape);

    // display the win and loss status for the game.
    if(winStatus==0){
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="                    ";
        drawstring((3*windowWidth/4-100),(windowHeight*0.03),0.0,name);
    }
    else if(winStatus==1){
        glColor4f(0.0,1.0,0.0,1.0);
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="Great! Level Cleared";
        drawstring((3*windowWidth/4-80),(windowHeight*0.08),0.0,name);

        glColor4f(1.0,1.0,1.0,1.0);
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name1[]="Press n to jump to next level.";
        drawstring((3*windowWidth/4-100),(windowHeight*0.03),0.0,name1);
    }
    if(winStatus==-1){
        glColor4f(1.0,0.0,0.0,1.0);
        setFont(GLUT_BITMAP_HELVETICA_18);
        char name[]="Sorry! Try again    ";
        drawstring((3*windowWidth/4-80),(windowHeight*0.08),0.0,name);
    }

    // used for drawing the moving figures/shapes.
    item.draw(0,'s');
    item.draw(1,'s');
    item.draw(2,'s');
    item.draw(3,'s');
    item.draw(4,'s');
    item.draw(5,'s');

    // for drawing the solution
    item.draw(0,'r');
    item.draw(1,'r');
    item.draw(2,'r');
    item.draw(3,'r');
    item.draw(4,'r');
    item.draw(5,'r');

}
void keyboardWindow3(unsigned char key, int x, int y)
{
    int count=0;
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'W':
        case 'w':
        // play.shapeIndex[flag] is used to ensure only the
        // figure available is moved nothing else.
            if(mov[flag].yMov<=20&&play.shapeIndex[flag]==1)
                mov[flag].yMov++;
            break;
        case 'S':
        case 's':
        // play.shapeIndex[flag] is used to ensure only the
        // figure available is moved nothing else.
            if(mov[flag].yMov>=-20&&play.shapeIndex[flag]==1)
                mov[flag].yMov--;
            break;
        case 'A':
        case 'a':
        // play.shapeIndex[flag] is used to ensure only the
        // figure available is moved nothing else.
            if(mov[flag].xMov>=-20&&play.shapeIndex[flag]==1)
                mov[flag].xMov--;
            break;
        case 'D':
        case 'd':
            // play.shapeIndex[flag] is used to ensure only the
            // figure available is moved nothing else.
            if(mov[flag].xMov<=20&&play.shapeIndex[flag]==1)
                mov[flag].xMov++;
            break;
        case 'n':
        case 'N':
            // if the player clear this level then in 'n'/'N'
            // we have to jump into the next level.
            currentLevel++;
            // reset the winStatus to initial before going to new level.
            winStatus=0;
            // load the data of the new level.
            play.selectLevel(currentLevel);
            // Resetting the displacement of each of the primitives to
            // their actual value.
            for(int i=0;i<6;i++)
            {
                mov[i].setDisp(0,0);
            }
            break;
        case 13: // when enter is pressed.
            for(int i=0;i<6;i++)
            {
                //printf("%d-->(%d,%d)\n",i,mov[i].xMov,mov[i].yMov);
                if(play.shapeIndex[i]==1)
                    if(mov[i].xMov==play.disp[i].xMov && mov[i].yMov==play.disp[i].yMov)
                        count++;
            }
            if(count==play.shapeCount){
                //printf("win\n");
                winStatus=1;
                count=0;
            }
            else    {
                count=0;
                //printf("loose\n");
                winStatus=-1;
            }
            break;
        case '1':flag=0;break;
        case '2':flag=1;break;
        case '3':flag=2;break;
        case '4':flag=3;break;
        case '5':flag=4;break;
        case '6':flag=5;break;
    }
}
void specialKeyboardWindow3(int key, int x, int y)
{
    switch (key) {
		case GLUT_KEY_LEFT :
            if(mov[flag].xMov>=-20&&play.shapeIndex[flag]==1)
                mov[flag].xMov--;
			break;
		case GLUT_KEY_RIGHT :
            if(mov[flag].xMov<=20&&play.shapeIndex[flag]==1)
                mov[flag].xMov++;
			break;
		case GLUT_KEY_UP :
            if(mov[flag].yMov<=20&&play.shapeIndex[flag]==1)
                mov[flag].yMov++;
            break;
		case GLUT_KEY_DOWN :
            if(mov[flag].yMov>=-20&&play.shapeIndex[flag]==1)
                mov[flag].yMov--;
            break;
	}
}