//  Train Project
// TrainView class implementation
// see the header for details
// look for TODO: to see things you want to add/change
// 



#include "TrainView.H"
#include "TrainWindow.H"

#include "Utilities/3DUtils.H"
#include "Utilities/bitmap.h"//copy from assignment 2
#include <Fl/fl.h>

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"

#define fno 1000//number of water drops

typedef struct {
    float fwx[fno];
    float fwy[fno];
    float fwz[fno];
    float vx[fno];
    float vy[fno];
    float vz[fno];
    float acc;
    int life[fno];
} fountain;
fountain founta;
fountain snow;
float bolly = 0, bollv = 0.1f;
float treex[10], treey[10];
GLubyte *facetex, *wheelimage, *carimage, *bodyimage, * skyimage, *housenimage, *housesimage, *hotairballoonimage, *doorimage;
int sky_w, sky_h, facetex_w, facetex_h, wheel_w, wheel_h, car_w, car_h, body_w, body_h, houses_w, houses_h, housen_w, housen_h, hotairballoon_w, hotairballoon_h, door_w, door_h;
float waterLevel = -3;
bool setup = 0, redrawing = 1;
int eyecount = 0;
float eyerot = 0;

TrainView::TrainView(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l) {
    mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);
    resetArcball();
}

void TrainView::resetArcball() {

    // set up the camera to look at the world
    // these parameters might seem magical, and they kindof are
    // a little trial and error goes a long way
    arcball.setup(this, 40, 250, .2f, .4f, 0);
}

// FlTk Event handler for the window
// TODO: if you want to make the train respond to other events 
// (like key presses), you might want to hack this.

int TrainView::handle(int event) {
    // see if the ArcBall will handle the event - if it does, then we're done
    // note: the arcball only gets the event if we're in world view
    if (tw->worldCam->value())
        if (arcball.handle(event)) return 1;
    if (tw->trainCam->value()) return 1;
    // remember what button was used
    static int last_push;

    switch (event) {
        case FL_PUSH:
            last_push = Fl::event_button();
            if (last_push == 1) {
                doPick();
                damage(1);
                return 1;
            };
            break;
        case FL_RELEASE:
            damage(1);
            last_push = 0;
            return 1;
        case FL_DRAG:
            if ((last_push == 1) && (selectedCube >= 0)) {
                ControlPoint &cp = world->points[selectedCube];

                double r1x, r1y, r1z, r2x, r2y, r2z;
                getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

                double rx, ry, rz;
                mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
                        static_cast<double>(cp.pos.x),
                        static_cast<double>(cp.pos.y),
                        static_cast<double>(cp.pos.z),
                        rx, ry, rz,
                        (Fl::event_state() & FL_CTRL) != 0);
                cp.pos.x = (float) rx;
                cp.pos.y = (float) ry;
                cp.pos.z = (float) rz;
                damage(1);
            }
            break;
            // in order to get keyboard events, we need to accept focus
        case FL_FOCUS:
            return 1;
        case FL_ENTER: // every time the mouse enters this window, aggressively take focus
            focus(this);
            break;
        case FL_KEYBOARD:
            int k = Fl::event_key();
            int ks = Fl::event_state();
            if (k == 'p') {
                if (selectedCube >= 0)
                    printf("Selected(%d) (%g %g %g) (%g %g %g)\n", selectedCube,
                        world->points[selectedCube].pos.x, world->points[selectedCube].pos.y, world->points[selectedCube].pos.z,
                        world->points[selectedCube].orient.x, world->points[selectedCube].orient.y, world->points[selectedCube].orient.z);
                else
                    printf("Nothing Selected\n");
                return 1;
            };
            break;
    }

    return Fl_Gl_Window::handle(event);
}

//Load the BMP file, this function copy form assignment2 Skeleton

GLubyte* TextureLoadBitmap(char *filename, int *w, int *h) /* I - Bitmap file to load */ {
    BITMAPINFO *info; /* Bitmap information */
    void *bits; /* Bitmap pixel bits */
    GLubyte *rgb; /* Bitmap RGB pixels */
    GLubyte err = '0';
    /*
     * Try loading the bitmap and converting it to RGB...
     */
    bits = LoadDIBitmap(filename, &info);
    if (bits == NULL)
        return (NULL);
    rgb = ConvertRGB(info, bits);
    if (rgb == NULL) {
        free(info);
        free(bits);
    };
    printf("%s: %d %d\n", filename, info->bmiHeader.biWidth, info->bmiHeader.biHeight);
    printf("read %s successfully\n", filename);
    *w = info->bmiHeader.biWidth;
    *h = info->bmiHeader.biHeight;
    /* Texture mapping parameters for filter and repeatance */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /*
     * Free the bitmap and RGB images, then return 0 (no errors).
     */
    free(info);
    free(bits);
    return (rgb);
}


// this is the code that actually draws the window
// it puts a lot of the work into other routines to simplify things

void TrainView::draw() {
    glViewport(0, 0, w(), h());
    // clear the window, be sure to clear the Z-Buffer too
    glClearColor(0, 0, .3f, 0); // background should be blue
    // we need to clear out the stencil buffer since we'll use
    // it for shadows
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH);

    // Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // prepare for projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setProjection(); // put the code to set up matrices here

    // TODO: you might want to set the lighting up differently
    // if you do, 
    // we need to set up the lights AFTER setting up the projection

    // enable the lighting
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // top view only needs one light
    if (tw->topCam->value()) {
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    } else {
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
    }
    // set the light parameters
    GLfloat lightPosition1[] = {0, 1, 1, 0}; // {50, 200.0, 50, 1.0};
    GLfloat lightPosition2[] = {1, 0, 0, 0};
    GLfloat lightPosition3[] = {0, -1, 0, 0};
    GLfloat yellowLight[] = {0.5f, 0.5f, .1f, 1.0};
    GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0};
    GLfloat blueLight[] = {.1f, .1f, .3f, 1.0};
    GLfloat grayLight[] = {.3f, .3f, .3f, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);


    //copy from assignment 2 start
    // Texture mapping setting for Microsoft's OpenGL implementation
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    // Texture mapping parameters for filter and repeatance
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //end of copy form assignment 2
    if (!setup) {
        facetex = TextureLoadBitmap("face.bmp", &facetex_w, &facetex_h);
        wheelimage = TextureLoadBitmap("wheel.bmp", &wheel_w, &wheel_h);
        carimage = TextureLoadBitmap("train.bmp", &car_w, &car_h);
        bodyimage = TextureLoadBitmap("car.bmp", &body_w, &body_h);
        skyimage = TextureLoadBitmap("sky.bmp", &sky_w, &sky_h);
        housenimage = TextureLoadBitmap("housen.bmp", &housen_w, &housen_h);
        housesimage = TextureLoadBitmap("houses.bmp", &houses_w, &houses_h);
        hotairballoonimage = TextureLoadBitmap("balloon.bmp", &hotairballoon_w, &hotairballoon_h);
        doorimage = TextureLoadBitmap("door.bmp", &door_w, &door_h);
        for (int i = 0; i < 10; i++) {
            treex[i] = rand() % 2000 / 10.0f - 100;
            treey[i] = rand() % 2000 / 10.0f - 100;
        }
        for (int i = 0; i < fno; i++) {
            snow.life[i] = ((rand() % 100));
            snow.fwx[i] = rand() % 2500 / 10.0f - 100;
            snow.fwy[i] = rand() % 2500 / 10.0f + 20;
            snow.fwz[i] = rand() % 2500 / 10.0f - 100;
            snow.vx[i] = 0;
            snow.vz[i] = 0;
            snow.vy[i] = 0;
        }
        setup = 1;
    }
    if (tw->carriagesNum->value() > 0 && tw->tension->value() < 0.5) tw->tension->value(0.5);
    if (tw->splineBrowser->value() == 1) tw->carriagesNum->value(0);
    if (tw->floorFlatness->value() == 0) {
        tw->nowea->value(1);
        tw->fog->value(0);
        tw->snow->value(0);
        tw->rain->value(0);
    }
    // now draw the ground plane
    setupFloor();
    if (!tw->floorFlatness->value()) {
        glDisable(GL_LIGHTING);
        drawFloor(200, 10);
        glEnable(GL_LIGHTING);
    } else {

        //ground

        float size = 220.0;
        int num = (int) (tw->floorFlatness->value()*10);
        float step = size / (num - 1);
        vector< vector<float> > vecGround(num);

        for (int i = 0; i < num; i++)
            vecGround[i].resize(num);
        if (setup && ground.size() == num * num) {
            for (int z = 0; z < num; z++) {
                for (int x = 0; x < num; x++) {
                    vecGround[z][x] = ground[z * num + x];
                }
            }
        } else {
            ground.clear();
            for (int z = 0; z < num; z++) {
                for (int x = 0; x < num; x++) {
                    float y = ((rand() % 100)) / 10.0f; //(rand() / 3000.f);         // Random height
                    vecGround[z][x] = y;
                    ground.push_back(vecGround[z][x]);
                }
            }
        }
        glDisable(GL_LIGHTING);
        if (tw->rain->value())
            weather(0, 0);
        if (tw->snow->value()) {
            weather(1, 0);
        }
        if (tw->fog->value()) {
            weather(2, 0);
        }
        if (!tw->snow->value()) {//if snow,freezed so the water level do not change
            if (tw->rain->value() && waterLevel <= 0.f)
                waterLevel += 0.1f;
            else if (waterLevel>-5.f)
                waterLevel -= 0.001f;
        }
        if (tw->snow->value())
            glColor3f(0.8f, 0.9f, 1.f);
        else
            glColor3f(0.4f, 0.5f, 1.f);
        glBegin(GL_QUADS);
        glVertex3f(-100, waterLevel, 100);
        glVertex3f(-100, waterLevel, -100);
        glVertex3f(100, waterLevel, -100);
        glVertex3f(100, waterLevel, 100);
        glEnd();
        glEnable(GL_LIGHTING);
        if (!tw->topCam->value()) {
            setupShadows(0);
            drawStuff(false);
            unsetupShadows();
        }
        glPushMatrix();
        glEnable(GL_LIGHTING);
        glTranslatef(-size / 2, -5, -size / 2); //do not draw the chessboard in this case, otherwise overlaping

        // Drawing ground 
        for (int z = 0; z < num - 1; z++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int x = 0; x < num - 1; x++) {
                // if rainning and water surface less than 5, water level++
                if (tw->snow->value())
                    glColor3d(1, 1, 1); //if snow...colour become white
                else
                    glColor3d((10 + vecGround[z][x]) / 256.0, (70.0 + vecGround[z][x + 1]) / 256.0, (10.0 + vecGround[z + 1][x + 1]) / 256.0);
                glNormal3f(vecGround[z + 1][x] - vecGround[z][x], -2, vecGround[z][x] - vecGround[z][x + 1]);
                glVertex3f(x * step, vecGround[z][x], z * step);
                glNormal3f(vecGround[z + 1][x + 1] - vecGround[z + 1][x], -2, vecGround[z + 1][x] - vecGround[z][x + 1]);
                glVertex3f((x + 1) * step, vecGround[z][x + 1], z * step);
                glNormal3f(vecGround[z][x + 1] - vecGround[z + 1][x], -2, vecGround[z + 1][x + 1] - vecGround[z][x]);
                glVertex3f(x * step, vecGround[z + 1][x], (z + 1) * step);
                glNormal3f(vecGround[z][x] - vecGround[z][x + 1], -2, vecGround[z][x + 1] - vecGround[z + 1][x]);
                glVertex3f((x + 1) * step, vecGround[z + 1][x + 1], (z + 1) * step);
            }
            glEnd();
        }

        glPopMatrix();

    }
    setupObjects();
    // we draw everything twice - once for real, and then once for
    // shadows
    drawStuff();
    // this time drawing is for shadows (except for top view)
    if (!tw->topCam->value()) {
        setupShadows(1);
        drawStuff(true);
        unsetupShadows();
    }
    if (tw->fog->value()) {
        glColor3d(0.3, 0.5, 0.8);
        GLUquadric* skyball = gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, sky_w, sky_h, 0, GL_RGB, GL_UNSIGNED_BYTE, skyimage);
        gluQuadricTexture(skyball, GL_TRUE);
        glRotatef(270, 1, 0, 0);
        gluSphere(skyball, 400, 20, 20);

        gluDeleteQuadric(skyball);
        glDisable(GL_TEXTURE_2D);
    }
}

Pnt3f interpoint(const float t, const Pnt3f& p0, const Pnt3f& p1) {
    return (p0 + t * (p1 - p0));
}
// note: this sets up both the Projection and the ModelView matrices
// HOWEVER: it doesn't clear the projection first (the caller handles
// that) - its important for picking

void TrainView::setProjection() {
    // compute the aspect ratio (we'll need it)
    float aspect = static_cast<float>(w()) / static_cast<float>(h());

    if (tw->worldCam->value())
        arcball.setProjection(false);
    else if (tw->topCam->value()) {
        float wi, he;
        if (aspect >= 1) {
            wi = 110;
            he = wi / aspect;
        } else {
            he = 110;
            wi = he*aspect;
        }
        glMatrixMode(GL_PROJECTION);
        glOrtho(-wi, wi, -he, he, 200, -200);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(-90, 1, 0, 0);
    } else {
        // TODO: put code for train view projection here!

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); // No picking in train view
        gluPerspective(65.0, aspect, 0.1, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        // Calculate the orientation matrix values
        int n = (int) ceil(world->trainU);
        Pnt3f p, d;
        if (tw->splineBrowser->value() - 1) {
            const float t = world->trainU - n + 1;
            const float tt = t * t;
            const float ttt = t * tt;
            Pnt3f p0(world->points[(n) % world->points.size()].pos);
            Pnt3f p1(world->points[(n + 1) % world->points.size()].pos);
            Pnt3f m0 = (world->points[(n + world->points.size() - 1) % world->points.size()].pos);
            Pnt3f m1 = (world->points[(n + 2) % world->points.size()].pos);

            switch (tw->splineBrowser->value()) {
                case 2:
                {
                    const float s = (float) tw->tension->value();
                    p = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                    d = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * t) * m1);
                    break;
                }
                case 3:
                {
                    p = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                    d = (0.5f * ((-3.f * tt + 4.f * t - 1.f) * m0 + (9.f * tt - 10.f * t) * p0 + (-9.f * tt + 8.f * t + 1.f) * p1 + (2.f * tt - t) * m1));
                    break;
                }
                case 4:
                {
                    p = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                    d = ((1.f / 6.f) * ((-3.f * tt + 6.f * t - 3.f) * m0 + (9.f * tt - 12.f * t) * p0 + (-9.f * tt + 6.f * t + 3.f) * p1 + (3.f * tt) * m1));
                    break;
                }
            }
        } else {
            p = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].pos, world->points[(n + 1) % world->points.size()].pos);
            d = (world->points[(n + 1) % world->points.size()].pos - world->points[(n) % world->points.size()].pos); //direction of train
        }
        p = -1.f * p;
        Pnt3f& tangent = -1.f * d;
        tangent.normalize();
        Pnt3f normal;

        normal = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].orient, world->points[(n + 1) % world->points.size()].orient);

        Pnt3f normal2 = normal* tangent;
        normal2.normalize();
        normal = tangent*normal2;
        normal.normalize();

        //  matrix for orientation
        Pnt3f & z(tangent), y(normal), x(normal2);
        GLfloat m[] = {
            x.x, y.x, z.x, 0.f,
            x.y, y.y, z.y, 0.f,
            x.z, y.z, z.z, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
        glMultMatrixf(m);

        // Move to the correct position
        glTranslatef(p.x, p.y - 3.f * y.y, p.z);

    }
}

void TrainView::weather(int mode, bool doingShadows) {

    if (mode == 0) {//rain	
        glLineWidth(1.5f);
        for (int i = 0; i < 100; i++) {
            float x, y, z;
            x = rand() % 2500 / 10.0f - 110;
            y = rand() % 2500 / 10.0f;
            z = rand() % 2500 / 10.0f - 110;
            if (!doingShadows) glColor3f(0.6f, 0.7f, 1.f);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x, y + 3.5f, z);
            glEnd();
        }
        glLineWidth(1.f);
    }
    if (mode == 1) {//snow, need ro remember the positions so that they can go to the ground slowly
        if (!doingShadows) glColor3f(1.f, 1.f, 1.f);
        snow.acc = -0.05f;
        for (int i = 0; i < fno; i++) {
            if (snow.life[i]) {
                glPushMatrix();
                glTranslatef(snow.fwx[i] - 50, snow.fwy[i], snow.fwz[i] - 50);
                GLUquadric* snowobj = gluNewQuadric();
                gluSphere(snowobj, 0.5, 6, 6);
                glPopMatrix();
                snow.fwy[i] += (snow.vy[i]);
                if (snow.vy[i]>-1)
                    snow.vy[i] += snow.acc;
                if (snow.fwy[i] > 0)
                    snow.life[i] = (snow.life[i] - 1) % 100;
                else
                    snow.life[i] = 0;
            } else {
                snow.life[i] = ((rand() % 100));
                snow.fwx[i] = rand() % 2500 / 10.0f - 100;
                snow.fwy[i] = rand() % 1000 / 10.0f + 50;
                snow.fwz[i] = rand() % 2500 / 10.0f - 100;
                snow.vx[i] = 0;
                snow.vz[i] = 0;
                snow.vy[i] = 0;
            }
        }

    }
    if (mode == 2) {
        glPushMatrix();
        static float fog_color[] = {0.8f, 0.9f, 1.f, 1.f};
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FOG);
        glFogfv(GL_FOG_COLOR, fog_color);
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.003f);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, -600.0);
        glFogf(GL_FOG_END, 600.0);
        glClearColor(0.8f, 0.9f, 1.f, 1.f);
        glFlush();
        glPopMatrix();
    }
}

float TrainView::arcLen(int n, float t, float dis)//track drawing version
{
    //	int n = (int)ceil(world->trainU);
    Pnt3f po1, po2;
    Pnt3f p0(world->points[(n) % world->points.size()].pos);
    Pnt3f p1(world->points[(n + 1) % world->points.size()].pos);
    Pnt3f m0 = (world->points[(n + world->points.size() - 1) % world->points.size()].pos);
    Pnt3f m1 = (world->points[(n + 2) % world->points.size()].pos);
    //float t = world->trainU-n+1;
    float tt = t * t;
    float ttt = t * tt;
    if (tw->splineBrowser->value() - 1) {
        switch (tw->splineBrowser->value()) {
            case 2:
            {
                const float s = (float) tw->tension->value();
                po1 = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                break;
            }
            case 3:
            {
                po1 = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                break;
            }
            case 4:
            {
                po1 = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                break;
            }
        }
        float pointdist = 0;
        float dt = 0;
        float tempt = t;
        while (pointdist < dis) {//approximation for track drawing
            dt += 0.01f;
            t = tempt + dt;
            if (t >= world->points.size())
                t -= world->points.size();
            tt = t * t;
            ttt = t * tt;
            switch (tw->splineBrowser->value()) {
                case 2:
                {
                    const float s = (float) tw->tension->value();
                    po2 = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                    break;
                }
                case 3:
                {
                    po2 = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                    break;
                }
                case 4:
                {
                    po2 = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                    break;
                }
            }
            Pnt3f l = po1 - po2;
            pointdist = sqrt(l.x * l.x + l.y * l.y + l.z * l.z);

        }
        return dt;
    } else {
        Pnt3f l = world->points[(n) % world->points.size()].pos - world->points[(n + 1) % world->points.size()].pos;
        return dis / (sqrt(l.x * l.x + l.y * l.y + l.z * l.z));
    }
}
// TODO: function that draws the track

void TrainView::drawTrack(bool doingShadows) {//function to draw thr tracks
    float pi = (float) 3.14159;
    glLineWidth(3.f);
    if (!doingShadows) glColor3d(0.4, 0.5, 0.7);
    int numLines = 10;
    float step = 1.f / numLines;
    float radius = 2.9f;
    int n = world->points.size();
    while (redrawing) {
        redrawing = 0;
        switch (tw->splineBrowser->value()) {
            case 1:
            {//linear
                for (size_t j = 0; j < world->points.size(); ++j) {
                    Pnt3f d = (world->points[j].pos - world->points[(j + 1) % n].pos);
                    numLines = (int) sqrt(d.x * d.x + d.y * d.y + d.z * d.z) / 2; //if want to use fixed line segment for each line/cure between 2 pts, delete some where here to use the fixed numLines for each segment between 2pts
                    step = 1.f / numLines;
                    float t = 0.f;
                    glBegin(GL_LINE_STRIP);
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        Pnt3f pos = interpoint(t, world->points[j].pos, world->points[(j + 1) % n].pos);
                        for (int k = 0; k < 10; k++) {
                            if (((treex[k] - pos.x)*(treex[k] - pos.x)+(treey[k] - pos.z)*(treey[k] - pos.z)) < 25) {
                                treex[k] = rand() % 2000 / 10.0f - 100;
                                treey[k] = rand() % 2000 / 10.0f - 100;
                                redrawing = 1;
                            }
                        }
                        Pnt3f dir = (world->points[j].pos - world->points[(j + 1) % n].pos);
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = (dir * up);
                        side.normalize();
                        Pnt3f v = pos + radius * side;
                        glVertex3fv(v.v());
                    }
                    glEnd();
                    t = 0.f;
                    glBegin(GL_LINE_STRIP);
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        Pnt3f pos(interpoint(t, world->points[j].pos, world->points[(j + 1) % n].pos));
                        Pnt3f dir = (world->points[j].pos - world->points[(j + 1) % n].pos);
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = (dir * up);
                        side.normalize();
                        Pnt3f v(pos - radius * side);
                        glVertex3fv(v.v());
                    }
                    glEnd();
                    if (tw->trackBrowser->value() % 2 == 1) {//bars
                        if (tw->trackBrowser->value() == 3) {
                            glBegin(GL_QUAD_STRIP);
                        } else {
                            glBegin(GL_LINES);
                        }
                        for (float t = 0.f, arc_t = 0.f; arc_t <= 1.f; t += step) {
                            Pnt3f pos(interpoint(arc_t, world->points[j].pos, world->points[(j + 1) % n].pos));
                            Pnt3f dir = (world->points[j].pos - world->points[(j + 1) % n].pos);
                            dir.normalize();
                            Pnt3f up = interpoint(arc_t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v1(pos + radius * side);
                            Pnt3f v2(pos - radius * side);
                            glVertex3fv(v1.v());
                            glVertex3fv(v2.v());
                            arc_t += arcLen(j, t, 5);
                        }
                        glEnd();
                    }
                }
                break;
            }
            case 2:
            {//ccubic
                numLines = 25;
                step = 1.f / numLines;
                const float s = (float) tw->tension->value(); //tension, need to add gui	
                for (size_t j = 0; j < world->points.size(); ++j) {
                    Pnt3f d = (world->points[j].pos - world->points[(j + 1) % n].pos);
                    Pnt3f p0(world->points[j].pos);
                    Pnt3f p1(world->points[(j + 1) % n].pos);
                    Pnt3f m0(world->points[(n + j - 1) % n].pos);
                    Pnt3f m1(world->points[(j + 2) % n].pos);
                    glBegin(GL_LINE_STRIP);
                    float t = 0.f;
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        Pnt3f pos;
                        const float tt = t * t;
                        const float ttt = t * tt;
                        pos = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                        for (int k = 0; k < 10; k++) {
                            if (((treex[k] - pos.x)*(treex[k] - pos.x)+(treey[k] - pos.z)*(treey[k] - pos.z)) < 25) {
                                treex[k] = rand() % 2000 / 10.0f - 100;
                                treey[k] = rand() % 2000 / 10.0f - 100;
                                redrawing = 1;
                            }
                        }
                        Pnt3f dir = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * t) * m1);
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = dir*up;
                        side.normalize();
                        Pnt3f v(pos + radius * side);
                        glVertex3fv(v.v());
                    }
                    glEnd();

                    t = 0.f;
                    glBegin(GL_LINE_STRIP);
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        const float tt = t * t;
                        const float ttt = t * tt;
                        Pnt3f pos;
                        pos = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                        Pnt3f dir = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * t) * m1);
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = dir*up;
                        side.normalize();
                        Pnt3f v(pos - radius * side);
                        glVertex3fv(v.v());
                    }
                    glEnd();
                    glLineWidth(5.f);
                    float tstep = arcLen(j, 0.5, 4);
                    float limit = 1.f;
                    // Draw bars
                    if (tw->trackBrowser->value() != 2) {
                        if (tw->trackBrowser->value() == 3) {
                            glBegin(GL_QUAD_STRIP);
                            limit = 1.1f;
                            tstep = step;
                        } else {
                            glBegin(GL_LINES);
                            limit = 1.f;
                        }
                        for (float t = 0.f, arc_t = 0.f; arc_t <= limit; t += tstep) {
                            Pnt3f pos;
                            const float tt = arc_t * arc_t;
                            const float ttt = arc_t * tt;
                            pos = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * arc_t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * arc_t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                            Pnt3f dir = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * arc_t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * arc_t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * arc_t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * arc_t) * m1);
                            dir.normalize();
                            Pnt3f up = interpoint(arc_t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v1(pos + radius * side);
                            Pnt3f v2(pos - radius * side);
                            glVertex3fv(v1.v());
                            glVertex3fv(v2.v());
                            if (!tw->arcLength->value() || tw->trackBrowser->value() == 3)
                                arc_t += step;
                            else
                                arc_t += tstep;
                        }
                        glEnd();
                        glLineWidth(3.f);
                    }
                }
                break;
            }
            case 3:
            {//CR
                numLines = 25;
                step = 1.f / numLines;
                glLineWidth(3.f);
                for (size_t j = 0; j < world->points.size(); ++j) {
                    Pnt3f d = (world->points[j].pos - world->points[(j + 1) % n].pos);
                    Pnt3f p0(world->points[j].pos);
                    Pnt3f p1(world->points[(j + 1) % n].pos);
                    Pnt3f m0(world->points[(n + j - 1) % n].pos);
                    Pnt3f m1(world->points[(j + 2) % n].pos);
                    glBegin(GL_LINE_STRIP);
                    float t = 0.f;
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        Pnt3f pos;
                        const float tt = t * t;
                        const float ttt = t * tt;
                        pos = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                        for (int k = 0; k < 10; k++) {
                            if (((treex[k] - pos.x)*(treex[k] - pos.x)+(treey[k] - pos.z)*(treey[k] - pos.z)) < 25) {
                                treex[k] = rand() % 2000 / 10.0f - 100;
                                treey[k] = rand() % 2000 / 10.0f - 100;
                                redrawing = 1;
                            }
                        }
                        Pnt3f dir = (0.5f * ((-3.f * tt + 4.f * t - 1.f) * m0 + (9.f * tt - 10.f * t) * p0 + (-9.f * tt + 8.f * t + 1.f) * p1 + (2.f * tt - t) * m1));
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = dir*up;
                        side.normalize();
                        Pnt3f v(pos + radius * side);
                        glVertex3fv(v.v());
                    }
                    glEnd();

                    t = 0.f;
                    glBegin(GL_LINE_STRIP);
                    for (int i = 0; i <= numLines; ++i, t += step) {
                        const float tt = t * t;
                        const float ttt = t * tt;
                        Pnt3f pos;
                        pos = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                        Pnt3f dir = (0.5f * ((-3.f * tt + 4.f * t - 1.f) * m0 + (9.f * tt - 10.f * t) * p0 + (-9.f * tt + 8.f * t + 1.f) * p1 + (2.f * tt - t) * m1));
                        dir.normalize();
                        Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                        up.normalize();
                        Pnt3f side = dir*up;
                        side.normalize();
                        Pnt3f v(pos - radius * side);
                        glVertex3fv(v.v());
                    }
                    glEnd();
                    glLineWidth(5.f);
                    float tstep = arcLen(j, 0.5, 4);
                    float limit = 1.f;
                    // Draw bars
                    if (tw->trackBrowser->value() != 2) {
                        if (tw->trackBrowser->value() == 3) {
                            glBegin(GL_QUAD_STRIP);
                            limit = 1.1f;
                            tstep = step;
                        } else {
                            glBegin(GL_LINES);
                            limit = 1.f;
                        }
                        for (float t = 0.f, arc_t = 0.f; arc_t <= limit; t += tstep) {
                            Pnt3f pos;
                            const float tt = arc_t * arc_t;
                            const float ttt = arc_t * tt;
                            pos = (0.5f * ((-1.f * ttt + 2.f * tt - arc_t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + arc_t) * p1 + (ttt - tt) * m1));
                            Pnt3f dir = (0.5f * ((-3.f * tt + 4.f * arc_t - 1.f) * m0 + (9.f * tt - 10.f * arc_t) * p0 + (-9.f * tt + 8.f * arc_t + 1.f) * p1 + (2.f * tt - arc_t) * m1));
                            dir.normalize();
                            Pnt3f up = interpoint(arc_t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v1(pos + radius * side);
                            Pnt3f v2(pos - radius * side);
                            glVertex3fv(v1.v());
                            glVertex3fv(v2.v());
                            if (!tw->arcLength->value() || tw->trackBrowser->value() == 3)
                                arc_t += step;
                            else
                                arc_t += tstep;
                        }
                        glEnd();
                        glLineWidth(3.f);
                    }
                }
                break;
            }
            case 4:
            {//SS
                glLineWidth(3.f);
                for (size_t j = 0; j < world->points.size(); ++j) {
                    Pnt3f d = (world->points[j].pos - world->points[(j + 1) % n].pos);
                    Pnt3f p0(world->points[j].pos);
                    Pnt3f p1(world->points[(j + 1) % n].pos);
                    Pnt3f m0 = (world->points[(n + j - 1) % n].pos);
                    Pnt3f m1 = (world->points[(j + 2) % n].pos);
                    n = world->points.size();
                    float t = 0.f;
                    if (tw->trackBrowser->value() != 3) {
                        glBegin(GL_LINE_STRIP);
                        for (int i = 0; i <= numLines; ++i, t += step) {
                            Pnt3f pos;
                            const float tt = t * t;
                            const float ttt = t * tt;
                            pos = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                            for (int k = 0; k < 10; k++) {
                                if (((treex[k] - pos.x)*(treex[k] - pos.x)+(treey[k] - pos.z)*(treey[k] - pos.z)) < 25) {
                                    treex[k] = rand() % 2000 / 10.0f - 100;
                                    treey[k] = rand() % 2000 / 10.0f - 100;
                                    redrawing = 1;
                                }
                            }
                            Pnt3f dir = ((1.f / 6.f) * ((-3.f * tt + 6.f * t - 3.f) * m0 + (9.f * tt - 12.f * t) * p0 + (-9.f * tt + 6.f * t + 3.f) * p1 + (3.f * tt) * m1));
                            dir.normalize();
                            Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v(pos + radius * side);
                            glVertex3fv(v.v());
                        }
                        glEnd();
                        t = 0.f;
                        glBegin(GL_LINE_STRIP);
                        for (int i = 0; i <= numLines; ++i, t += step) {
                            const float tt = t * t;
                            const float ttt = t * tt;
                            Pnt3f pos;
                            pos = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                            Pnt3f dir = ((1.f / 6.f) * ((-3.f * tt + 6.f * t - 3.f) * m0 + (9.f * tt - 12.f * t) * p0 + (-9.f * tt + 6.f * t + 3.f) * p1 + (3.f * tt) * m1));
                            dir.normalize();
                            Pnt3f up = interpoint(t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v = (pos - radius * side);
                            glVertex3fv(v.v());
                        }
                        glEnd();
                    }
                    glLineWidth(5.f);
                    float tstep = arcLen(j, 0.5, 4);
                    float limit = 1.f;
                    // Draw bars
                    if (tw->trackBrowser->value() != 2) {
                        if (tw->trackBrowser->value() == 3) {
                            glBegin(GL_QUAD_STRIP);
                            limit = 1.1f;
                            tstep = step;
                        } else {
                            glBegin(GL_LINES);
                            limit = 1.f;
                        }
                        for (float t = 0.f, arc_t = 0.f; arc_t <= limit; t += tstep) {
                            Pnt3f pos;
                            const float tt = arc_t * arc_t;
                            const float ttt = arc_t * tt;
                            pos = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * arc_t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * arc_t + 1.f) * p1 + (1.f * ttt) * m1));
                            Pnt3f dir = ((1.f / 6.f) * ((-3.f * tt + 6.f * arc_t - 3.f) * m0 + (9.f * tt - 12.f * arc_t) * p0 + (-9.f * tt + 6.f * arc_t + 3.f) * p1 + (3.f * tt) * m1));
                            dir.normalize();
                            Pnt3f up = interpoint(arc_t, world->points[j].orient, world->points[(j + 1) % n].orient);
                            up.normalize();
                            Pnt3f side = dir*up;
                            side.normalize();
                            Pnt3f v1(pos + radius * side);
                            Pnt3f v2(pos - radius * side);
                            glVertex3fv(v1.v());
                            glVertex3fv(v2.v());
                            if (!tw->arcLength->value() || tw->trackBrowser->value() == 3)
                                arc_t += step;
                            else
                                arc_t += tstep;
                        }
                        glLineWidth(3.f);
                        glEnd();
                    }
                }
                break;
            }

        }
    }
    glLineWidth(1.f);
    redrawing = 1;
}

void TrainView::drawTrain(bool doingShadows) {
    //TODO:
    //move the train to the correct positon and get the correct normal

    int n = (int) ceil(world->trainU);
    Pnt3f p, d;
    if (tw->splineBrowser->value() - 1) {
        const float t = world->trainU - n + 1;
        const float tt = t * t;
        const float ttt = t * tt;
        Pnt3f p0(world->points[(n) % world->points.size()].pos);
        Pnt3f p1(world->points[(n + 1) % world->points.size()].pos);
        Pnt3f m0 = (world->points[(n + world->points.size() - 1) % world->points.size()].pos);
        Pnt3f m1 = (world->points[(n + 2) % world->points.size()].pos);

        switch (tw->splineBrowser->value()) {
            case 2:
            {
                const float s = (float) tw->tension->value();
                p = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                d = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * t) * m1);
                break;
            }
            case 3:
            {
                p = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                d = (0.5f * ((-3.f * tt + 4.f * t - 1.f) * m0 + (9.f * tt - 10.f * t) * p0 + (-9.f * tt + 8.f * t + 1.f) * p1 + (2.f * tt - t) * m1));
                break;
            }
            case 4:
            {
                p = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                d = ((1.f / 6.f) * ((-3.f * tt + 6.f * t - 3.f) * m0 + (9.f * tt - 12.f * t) * p0 + (-9.f * tt + 6.f * t + 3.f) * p1 + (3.f * tt) * m1));
                break;
            }
        }
    } else {
        p = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].pos, world->points[(n + 1) % world->points.size()].pos);
        d = (world->points[(n + 1) % world->points.size()].pos - world->points[(n) % world->points.size()].pos); //direction of train
    }
    Pnt3f& tangent = d;
    tangent.normalize();

    Pnt3f normal;
    if (tw->splineBrowser->value() - 1)//curves
        normal = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].orient, world->points[(n + 1) % world->points.size()].orient); // orientation of train
    else //line
        normal = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].orient, world->points[(n + 1) % world->points.size()].orient); // orientation of train

    Pnt3f normal2(normal * tangent);
    normal2.normalize();
    normal = tangent*normal2;
    normal.normalize();

    glPushMatrix();
    //orient + pos matrix
    Pnt3f & z(tangent), y(normal), x(normal2);
    GLfloat m[] = {
        x.x, x.y, x.z, 0.f,
        y.x, y.y, y.z, 0.f,
        z.x, z.y, z.z, 0.f,
        p.x, p.y, p.z, 1.f
    };
    glMultMatrixf(m);
    //need rearragement to let the train reflection on the water surface be reasonable

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTranslatef(0.f, 0.f, 8.f);

    glPushMatrix();
    glTranslatef(0, 5, -3);
    glPushMatrix();
    if (!doingShadows) glColor3f(1.0, 1.0, 1.0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.98f, 0.5f, 0.98f);
    GLUquadricObj *face;
    face = gluNewQuadric();
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, facetex_w, facetex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, facetex);
        gluQuadricTexture(face, GL_TRUE);
    }
    gluSphere(face, 2, 10, 10);
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glRotatef(180.f, 0.f, 1.f, 0.f);
    glPushMatrix();
    if (!doingShadows) glColor3d(0.0, 0.0, 0.0);
    GLUquadricObj *cylinder;
    cylinder = gluNewQuadric();
    gluCylinder(cylinder, 2, 2, 3, 10, 10);
    if (!doingShadows) glColor3d(0.0625, 0.535, 0.992);
    glTranslatef(0, 0, 3);
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, car_w, car_h, 0, GL_RGB, GL_UNSIGNED_BYTE, carimage);
    }
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 2, 2, 5, 10, 10);
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glTranslatef(0, 4, 1);
    glRotatef(90, 1, 0, 0);

    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, body_w, body_h, 0, GL_RGB, GL_UNSIGNED_BYTE, bodyimage);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(2.5, -1.f, 8.f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(2.5, -1.f, 6.f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-2.5, -1.f, 6.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5, -1.f, 8.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(-2.45f, 7.1f, 6.f);
    glTexCoord2f(1.0f, 1.f);
    glVertex3f(-2.45f, 7.1f, 2.5f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(-2.45f, 3.5f, 2.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-2.45f, 3.5f, 6.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(2.45f, 7.1f, 6.f);
    glTexCoord2f(1.0f, 1.f);
    glVertex3f(2.45f, 7.1f, 2.5f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(2.45f, 3.5f, 2.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(2.45f, 3.5f, 6.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(0.75f, 0.25f);
    glVertex3f(-2.45f, 3.6f, 4.3f);
    glTexCoord2f(0.75f, 0.5f);
    glVertex3f(-2.45f, 3.6f, 2.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-2.45f, 2.f, 2.5f);
    glTexCoord2f(0.5f, 0.25f);
    glVertex3f(-2.45f, 2.f, 4.3f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(0.75f, 0.25f);
    glVertex3f(2.45f, 3.6f, 4.3f);
    glTexCoord2f(0.75f, 0.5f);
    glVertex3f(2.45f, 3.6f, 2.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(2.45f, 2.f, 2.5f);
    glTexCoord2f(0.5f, 0.25f);
    glVertex3f(2.45f, 2.f, 4.3f);
    glEnd();
    glTranslatef(0, 0.05f, 0.5f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(2.5f, 10.f, 8.f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(2.5f, 10.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 10.f, 0.f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-2.5f, 10.f, 8.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.55f);
    glVertex3f(2.45f, 7.f, 7.f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(2.45f, 7.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.45f, 7.f, 0.f);
    glTexCoord2f(0.0f, 0.55f);
    glVertex3f(-2.45f, 7.f, 7.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.55f);
    glVertex3f(-2.45f, 10.f, 7.f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(-2.45f, 10.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.45f, 7.f, 0.f);
    glTexCoord2f(0.0f, 0.55f);
    glVertex3f(-2.45f, 7.f, 7.f);
    glEnd();
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.55f);
    glVertex3f(2.45f, 10.f, 7.f);
    glTexCoord2f(0.5f, 1.f);
    glVertex3f(2.45f, 10.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(2.45f, 7.f, 0.f);
    glTexCoord2f(0.0f, 0.55f);
    glVertex3f(2.45f, 7.f, 7.f);
    glEnd();
    if (!doingShadows)glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPushMatrix();
    if (!doingShadows) glColor3f(1.0, 1.0, 1.0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(5.5f, 1.5f, 2.55f);

    GLUquadricObj *disk;
    disk = gluNewQuadric();
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, wheel_w, wheel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelimage);
        gluQuadricTexture(disk, GL_TRUE);
    }
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(3.0f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(3.0f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPushMatrix();
    if (!doingShadows) glColor3f(1.0, 1.0, 1.0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(5.5f, 1.5f, -2.55f);
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, wheel_w, wheel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelimage);
        gluQuadricTexture(disk, GL_TRUE);
    }
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(3.0f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(3.0f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.5, 3, -6);
    glRotatef(90, 0, 1, 0);
    if (!doingShadows) glColor3f(0.059f, 0.531f, 0.958f);
    gluCylinder(cylinder, 1.5, 1.5, 5, 10, 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.5, 3, -6);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    if (!doingShadows) glColor3f(0.059f, 0.531f, 0.958f);
    gluPartialDisk(disk, 0, 1.5, 10, 2, 0, 180);
    glTranslatef(0, 0, 0.1f);
    if (!doingShadows) glColor3f(0.5f, 0.0f, 0.0f);
    gluPartialDisk(disk, 1.2, 1.3, 10, 2, 0, 180);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.5, 3, -6);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    if (!doingShadows) glColor3d(0.059f, 0.531f, 0.958f);
    gluPartialDisk(disk, 0, 1.5, 10, 2, 0, 180);
    glTranslatef(0, 0, -0.1f);
    if (!doingShadows) glColor3d(0.5f, 0.0f, 0.0f);
    gluPartialDisk(disk, 1.2, 1.3, 10, 2, 0, 180);
    glPopMatrix();
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 4, -9);
    if (!doingShadows) glColor3d(0.0, 0.0, 0.0);
    gluCylinder(cylinder, 0.4, 0.4, 2, 10, 10);
    glBegin(GL_QUADS);
    glVertex3f(-2.5, 10.f, 2.f);
    glVertex3f(-2.5, 7.f, 2.f);
    glVertex3f(2.5, 7.f, 2.f);
    glVertex3f(2.5, 10.f, 2.f);
    if (!doingShadows) glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(-2.5, 7.f, 2.5f);
    glVertex3f(-2.5, 2.f, 2.5f);
    glVertex3f(2.5, 2.f, 2.5f);
    glVertex3f(2.5, 7.f, 2.5f);
    glVertex3f(-2.5, 3.5f, 4.3f);
    glVertex3f(-2.5, 2.f, 4.3f);
    glVertex3f(2.5, 2.f, 4.3f);
    glVertex3f(2.5, 3.5f, 4.3f);
    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(2.5, 2.f, 4.3f);
    glVertex3f(2.5, 2.f, 2.5f);
    glVertex3f(-2.5, 2.f, 2.5f);
    glVertex3f(-2.5, 2.f, 4.3f);
    glVertex3f(2.5, 3.5f, 4.f);
    glVertex3f(2.5, 3.5f, 6.f);
    glVertex3f(-2.5, 3.5f, 6.f);
    glVertex3f(-2.5, 3.5f, 4.f);
    if (!doingShadows) glColor3d(0.0, 0.0, 0.0);
    glVertex3f(2.5, 10.f, 8.f);
    glVertex3f(2.5, 10.f, 6.f);
    glVertex3f(-2.5, 10.f, 6.f);
    glVertex3f(-2.5, 10.f, 8.f);
    glVertex3f(2.5, 10.f, 8.f);
    glVertex3f(2.5, -1.f, 8.f);
    glVertex3f(2.5, -1.f, 6.f);
    glVertex3f(2.5, 10.f, 6.f);
    glVertex3f(-2.5, 10.f, 8.f);
    glVertex3f(-2.5, -1.f, 8.f);
    glVertex3f(-2.5, -1.f, 6.f);
    glVertex3f(-2.5, 10.f, 6.f);
    glVertex3f(-2.5, 10.f, 8.f);
    glVertex3f(-2.5, -1.f, 8.f);
    glVertex3f(2.5, -1.f, 8.f);
    glVertex3f(2.5, 10.f, 8.f);
    glVertex3f(-2.5, 10.f, 6.f);
    glVertex3f(-2.5, -1.f, 6.f);
    glVertex3f(2.5, -1.f, 6.f);
    glVertex3f(2.5, 10.f, 6.f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void TrainView::drawCar(int i, bool doingShadows) {
    float temadt = world->carU[i];
    int n = (int) ceil(temadt);  
    Pnt3f p, d;
    if (tw->splineBrowser->value() - 1) {
        const float t = world->carU[i] - n + 1;
        const float tt = t * t;
        const float ttt = t * tt;
        Pnt3f p0(world->points[(n) % world->points.size()].pos);
        Pnt3f p1(world->points[(n + 1) % world->points.size()].pos);
        Pnt3f m0 = (world->points[(n + world->points.size() - 1) % world->points.size()].pos);
        Pnt3f m1 = (world->points[(n + 2) % world->points.size()].pos);

        switch (tw->splineBrowser->value()) {
            case 2:
            {
                const float s = (float) tw->tension->value();
                p = (((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
                d = ((3.f * (-1.f * s) * tt + 2.f * (2.f * s) * t + (-1.f * s)) * m0 + (3.f * (2.f - s) * tt + 2.f * (s - 3.f) * t) * p0 + (3.f * (s - 2.f) * tt + 2.f * (3.f - 2.f * s) * t + (1.f * s)) * p1 + (3.f * (1.f * s) * tt + 2.f * (-1.f * s) * t) * m1);
                break;
            }
            case 3:
            {
                p = (0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
                d = (0.5f * ((-3.f * tt + 4.f * t - 1.f) * m0 + (9.f * tt - 10.f * t) * p0 + (-9.f * tt + 8.f * t + 1.f) * p1 + (2.f * tt - t) * m1));
                break;
            }
            case 4:
            {
                p = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
                d = ((1.f / 6.f) * ((-3.f * tt + 6.f * t - 3.f) * m0 + (9.f * tt - 12.f * t) * p0 + (-9.f * tt + 6.f * t + 3.f) * p1 + (3.f * tt) * m1));
                break;
            }
        }
    } else {
        p = interpoint(world->trainU - n + 1, world->points[(n) % world->points.size()].pos, world->points[(n + 1) % world->points.size()].pos);
        d = (world->points[(n + 1) % world->points.size()].pos - world->points[(n) % world->points.size()].pos); //direction of train
    }
    Pnt3f& tangent = d;
    tangent.normalize();

    Pnt3f normal;
    if (tw->splineBrowser->value() - 1)//curves
        normal = interpoint(world->carU[i] - n + 1, world->points[(n) % world->points.size()].orient, world->points[(n + 1) % world->points.size()].orient); // orientation of train
    else //line
        normal = interpoint(world->carU[i] - n + 1, world->points[(n) % world->points.size()].orient, world->points[(n + 1) % world->points.size()].orient); // orientation of train

    Pnt3f normal2(normal * tangent);
    normal2.normalize();
    normal = tangent*normal2;
    normal.normalize();

    glPushMatrix();
    //orient + pos matrix
    Pnt3f & z(tangent), y(normal), x(normal2);
    GLfloat m[] = {
        x.x, x.y, x.z, 0.f,
        y.x, y.y, y.z, 0.f,
        z.x, z.y, z.z, 0.f,
        p.x, p.y, p.z, 1.f
    };
    glMultMatrixf(m);
    //need rearragement to let the train reflection on the water surface be reasonable
    if (!doingShadows) glColor3f(0.f, 0.f, 0.f);
    glTranslatef(0, 0, -4);
    glBegin(GL_QUADS);
    glVertex3f(-2.5, 5.f, 0.f);
    glVertex3f(-2.5, 2.f, 0.f);
    glVertex3f(2.5, 2.f, 0.f);
    glVertex3f(2.5, 5.f, 0.f);
    glVertex3f(-2.5, 5.f, 8.f);
    glVertex3f(-2.5, 2.f, 8.f);
    glVertex3f(2.5, 2.f, 8.f);
    glVertex3f(2.5, 5.f, 8.f);

    glVertex3f(-2.5, 2.f, 8.f);
    glVertex3f(-2.5, 2.f, 0.f);
    glVertex3f(2.5, 2.f, 0.f);
    glVertex3f(2.5, 2.f, 8.f);
    glVertex3f(-2.5, 5.f, 8.f);
    glVertex3f(-2.5, 5.f, 0.f);
    glVertex3f(2.5, 5.f, 0.f);
    glVertex3f(2.5, 5.f, 8.f);

    glVertex3f(2.5, 5.f, 0.f);
    glVertex3f(2.5, 2.f, 0.f);
    glVertex3f(2.5, 2.f, 8.f);
    glVertex3f(2.5, 5.f, 8.f);
    glVertex3f(-2.5, 5.f, 0.f);
    glVertex3f(-2.5, 2.f, 0.f);
    glVertex3f(-2.5, 2.f, 8.f);
    glVertex3f(-2.5, 5.f, 8.f);
    glEnd();
    glPushMatrix();
    if (!doingShadows) glColor3f(1.0, 1.0, 1.0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-6.5f, 1.5f, 2.55f);

    GLUquadricObj *disk;
    disk = gluNewQuadric();
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, wheel_w, wheel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelimage);
        gluQuadricTexture(disk, GL_TRUE);
    }
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(5.5f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPushMatrix();
    if (!doingShadows) glColor3f(1.0, 1.0, 1.0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-6.5f, 1.5f, -2.55f);

    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, wheel_w, wheel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelimage);
        gluQuadricTexture(disk, GL_TRUE);
    }
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    glTranslatef(5.5f, 0, 0);
    glPushMatrix();
    glRotatef((world->trainU - n)*720, 0, 0, 1);
    gluDisk(disk, 0, 1.5, 15, 2);
    glPopMatrix();
    if (!doingShadows) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();
    gluDeleteQuadric(disk);
}

// this draws all of the stuff in the world
// NOTE: if you're drawing shadows, DO NOT set colors 
// (otherwise, you get colored shadows)
// this gets called twice per draw - once for the objects, once for the shadows
// TODO: if you have other objects in the world, make sure to draw them

void TrainView::drawStuff(bool doingShadows) {
    if (!tw->fog->value()) glDisable(GL_FOG);
    //Draw in reverse order to show correct water mirror image
    // draw the train
    // TODO: call your own train drawing code
    // don't draw the train if you're looking out the front window
    if (!tw->trainCam->value())
        drawTrain(doingShadows);
    float sumDis = 0;
    if (tw->carriagesNum->value()) {
        for (int i = 0; i < tw->carriagesNum->value(); i++) {
            drawCar(i, doingShadows);
        }
    }
    // draw the track
    // TODO: call your own track drawing code
    if (tw->trackBrowser->value() != 4)drawTrack(doingShadows);
    // draw the control points
    // don't draw the control points if you're driving 
    // (otherwise you get sea-sick as you drive through them)
    if (!tw->trainCam->value()) {
        for (size_t i = 0; i < world->points.size(); ++i) {
            if (!doingShadows) {
                if (((int) i) != selectedCube)
                    glColor3ub(240, 60, 60);
                else
                    glColor3ub(240, 240, 30);
            }
            world->points[i].draw();
        }
    }

    //start of the skipping things=>if snow,snowman else tree
    for (int k = 0; k < 10; k++) {
        glPushMatrix();
        GLUquadricObj *ball = gluNewQuadric();
        if (tw->snow->value()) {
            glTranslatef(treex[k], 5.f, treey[k]);
            if (!doingShadows) glColor3f(1.f, 1.f, 1.f);
            gluSphere(ball, 5.0f, 10, 10);
            glTranslatef(0, 5.f, 0);
            gluSphere(ball, 4.0f, 10, 10);
        } else {
            glTranslatef(treex[k], 0.f, treey[k]);
            if (!doingShadows) glColor4ub(59, 24, 3, 1);
            glPushMatrix();
            glRotatef(270, 1, 0, 0);
            gluCylinder(ball, 3, 1.5, 5, 10, 10);
            glTranslatef(0, 0, 5);

            if (!doingShadows) glColor4ub(48, 94, 2, 1);
            gluCylinder(ball, 3, 0, 5, 10, 10);
            glTranslatef(0, 0, 3);
            if (!doingShadows) glColor4ub(24, 191, 8, 1);

            gluCylinder(ball, 3, 0, 5, 10, 10);
            glTranslatef(0, 0, 3);

            if (!doingShadows) glColor4ub(98, 229, 112, 1);
            gluCylinder(ball, 3, 0, 5, 10, 10);
            glPopMatrix();
        }
        glPopMatrix();
   

	}

    //end of the objects that affected by track     
	    GLUquadricObj *ball = gluNewQuadric();
	
	glPushMatrix();
        glScalef(0.2f, 0.2f, 0.2f);
        glRotatef(270, 1, 0, 0);
        glRotatef(90, 0, 0, 1);
		glTranslatef(-(world->points[0].pos.z / 0.2f), -(15 + world->points[0].pos.x) / 0.2f, 0);
		glBegin(GL_QUADS);
		if (!doingShadows)
			glColor4ub(150, 50, 0, 1); //rightback
		glVertex3f(-20, 0, 150);
		glVertex3f(-20, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 150);
		if (!doingShadows)
			glColor4ub(100, 50, 0, 1);
		glVertex3f(0, 40, 140);
		glVertex3f(0, 40, 120);
		glVertex3f(5, 40, 120);
		glVertex3f(5, 40, 140);
		if (!doingShadows)
			glColor4ub(100, 80, 20, 1); //rightside
		glVertex3f(0, 0, 150);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 20, 0);
		glVertex3f(0, 20, 150);
		glVertex3f(0, 0, 150); //the top
		glVertex3f(0, 0, 115);
		glVertex3f(0, 130, 115);
		glVertex3f(0, 130, 150);
		glVertex3f(0, 110, 150); // leftside
		glVertex3f(0, 110, 0);
		glVertex3f(0, 130, 0);
		glVertex3f(0, 130, 150);
		if (!doingShadows)
			glColor4ub(150, 20, 0, 1);
		glVertex3f(5, 40, 140); // front
		glVertex3f(5, 40, 120);
		glVertex3f(5, 90, 120);
		glVertex3f(5, 90, 140);
		glEnd();
		glPushMatrix();
		glTranslatef(-0.1f, 65, 70);
		glRotatef(90, 0, 1, 0);
		gluPartialDisk(ball, 45, 65, 20, 20, 180, 180);
		glTranslatef(-50, 3, 10);
			if (!doingShadows)glColor4ub(130, 130, 130, 1);
		glPushMatrix();    //eyes
			if (!doingShadows)glColor4ub(225, 225, 225, 1);
		glTranslatef(-10, -8, -5);	
		glPushMatrix();
		if (eyecount == 0)
			glRotatef(eyerot = (float)(rand() % 360), 0, 0, 1);
		else
			glRotatef(eyerot, 0, 0, 1);
		eyecount = (eyecount+1) % 100;
		gluSphere(ball, 8, 20, 20);	
			if (!doingShadows)glColor4ub(0, 0, 0, 1);
		glTranslatef(-3.5, 0, 5);
		gluSphere(ball, 4, 20, 20);
		glPopMatrix();
		glPopMatrix();
			if (!doingShadows)glColor4ub(225, 225, 225, 1);
		glTranslatef(-10, 8, -5);
		glPushMatrix();
		glRotatef(eyerot, 0, 0, 1);
		eyecount = (eyecount + 1) % 100;
		gluSphere(ball, 8, 20, 20);
			if (!doingShadows)glColor4ub(0, 0, 0, 1);
		glTranslatef(-3.5, 0, 5);
		gluSphere(ball, 4, 20, 20);
		glPopMatrix();
		glPopMatrix();

		glTranslatef(0,0,0);
		glBegin(GL_QUADS);
		if (!doingShadows)
			glColor4ub(100, 80, 20, 1); //rightside

		glVertex3f(0, 0, 150);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 20, 0);
		glVertex3f(0, 20, 150);

		glVertex3f(0, 0, 150); //the top
		glVertex3f(0, 0, 115);
		glVertex3f(0, 130, 115);
		glVertex3f(0, 130, 150);

		glVertex3f(0, 110, 150); // leftside
		glVertex3f(0, 110, 0);
		glVertex3f(0, 130, 0);
		glVertex3f(0, 130, 150);
		glNormal3f(0, 0, 1);  // the second top
		glVertex3f(-20, 130, 150);
		glVertex3f(-20, 0, 150);
		glVertex3f(0, 0, 150);
		glVertex3f(0, 130, 150);
	
		if (!doingShadows)
			glColor4ub(150, 58, 0, 1); //roof
		glVertex3f(0, 90, 140);
		glVertex3f(0, 40, 140);
		glVertex3f(5, 40, 140);
		glVertex3f(5, 90, 140);


		glPushMatrix();

			if (!doingShadows)
			glColor4ub(150, 20, 0, 1);
		glVertex3f(5, 40, 140); // front
		glVertex3f(5, 40, 120);
		glVertex3f(5, 90, 120);
		glVertex3f(5, 90, 140);
		glEnd();
		glPushMatrix();		
		glTranslatef(-20,0,0);
		glTranslatef(-0.1f, 65, 70);
		glRotatef(90, 0, 1, 0);

		gluPartialDisk(ball, 45, 65, 20, 20, 180, 180);
		glTranslatef(-50, 3, 10);
			if (!doingShadows)glColor4ub(130, 130, 130, 1);

		glRotatef(180,1,0,0);
		glTranslatef(0,0,15);
		glPushMatrix();    //eyes

			if (!doingShadows)glColor4ub(225, 225, 225, 1);
		glTranslatef(-10, -8, -5);	
		glPushMatrix();
		glRotatef(eyerot, 0, 0, 1);
		gluSphere(ball, 8, 20, 20);	
			if (!doingShadows)glColor4ub(0, 0, 0, 1);
		glTranslatef(-3.5, 0, 5);
		gluSphere(ball, 4, 20, 20);
		glPopMatrix();

		glPopMatrix();
			if (!doingShadows)glColor4ub(225, 225, 225, 1);
		glTranslatef(-10, 8, -5);
		glPushMatrix();
		glRotatef(eyerot, 0, 0, 1);
		gluSphere(ball, 8, 20, 20);
			if (!doingShadows)glColor4ub(0, 0, 0, 1);
		glTranslatef(-3.5, 0, 5);
		gluSphere(ball, 4, 20, 20);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-20,0,0);
			glBegin(GL_QUADS);
		if (!doingShadows)
			glColor4ub(100, 80, 20, 1); //rightside

		glVertex3f(0, 0, 150);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 20, 0);
		glVertex3f(0, 20, 150);

		glVertex3f(0, 0, 150); //the top
		glVertex3f(0, 0, 115);
		glVertex3f(0, 130, 115);
		glVertex3f(0, 130, 150);

		glVertex3f(0, 110, 150); // leftside
		glVertex3f(0, 110, 0);
		glVertex3f(0, 130, 0);
		glVertex3f(0, 130, 150);

		glEnd();
		glPopMatrix();
		glBegin(GL_QUADS);
		if (!doingShadows)
			glColor4ub(66, 50, 38, 1); //leftback
		glVertex3f(0, 130, 150);
		glVertex3f(0, 130, 0);
		glVertex3f(-20, 130, 0);
		glVertex3f(-20, 130, 150);
		if (!doingShadows)
			glColor4ub(150, 58, 0, 1);
		glVertex3f(5, 90, 140);
		glVertex3f(5, 90, 120);
		glVertex3f(0, 90, 120);
		glVertex3f(0, 90, 140);
		if (!doingShadows)
			glColor4ub(66, 50, 38, 1);
		glEnd();
		glPopMatrix();



    gluSphere(ball, 0.05f, 5, 5);
    glPushMatrix(); //balloon
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, hotairballoon_w, hotairballoon_h, 0, GL_RGB, GL_UNSIGNED_BYTE, hotairballoonimage);
        gluQuadricTexture(ball, GL_TRUE);
    }

    glTranslatef(80, 10 + bolly, -80);
    bolly += bollv;
    if (bolly < 0 || bolly > 50) bollv = -bollv;
    glRotatef(90, 1, 0, 0); 
	if (!doingShadows) glColor3f(0.8f, 0.6f, 0.3f); 
	glPushMatrix();
	glTranslatef(0, 0, -7);
    gluCylinder(ball, 5, 3, 5, 7, 2);
    glTranslatef(0, 0, -5);
    gluCylinder(ball, 8, 5, 5, 7, 2);
    glTranslatef(0, 0, -7);
    gluSphere(ball, 11, 10, 10);
	glPopMatrix();
    if (!doingShadows) glColor3f(0.3f, 0.3f, 0.3f);
	glTranslatef(0,0,5);
    gluDisk(ball, 0, 3, 7, 2);
	glTranslatef(0,0,-5);
    gluCylinder(ball, 4, 3, 5, 7, 2);
    if (!doingShadows) glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    //start of house
    glPushMatrix();
    glTranslatef(80.f, 0.f, 80.f);
    glScalef(3, 3, 3);
    if (!doingShadows) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
        if (tw->snow->value())glTexImage2D(GL_TEXTURE_2D, 0, 3, houses_w, houses_h, 0, GL_RGB, GL_UNSIGNED_BYTE, housesimage);
        else glTexImage2D(GL_TEXTURE_2D, 0, 3, housen_w, housen_h, 0, GL_RGB, GL_UNSIGNED_BYTE, housenimage);
    }
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 45.f, 27.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.5, 9.f, 5.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5, 12.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5, 12.f, 0.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5, 9.f, 5.f);

    glNormal3f(0.0f, 45.f, -27.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(6.5, 12.f, 0.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.5, 9.f, -5.f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.5, 9.f, -5.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-2.5, 12.f, 0.f);
    glEnd();

    if (!doingShadows) glTexImage2D(GL_TEXTURE_2D, 0, 3, housen_w, housen_h, 0, GL_RGB, GL_UNSIGNED_BYTE, housenimage);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -90.f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.5f, 3.f, 5.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, 3.f, 5.f);

    glNormal3f(0.0f, 0.0f, -54.f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.5f, 9.f, -5.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, 9.f, -5.f);

    glNormal3f(0.0f, 0.0f, -54.f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.5f, 9.f, 5.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 3.f, 5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 3.f, 5.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, 9.f, 5.f);

    glNormal3f(-60.f, 0.0f, 0.0f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 3.f, 5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(6.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.5f, 9.f, -5.f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(6.5f, 9.f, 5.f);

    glNormal3f(60.f, 0.0f, 1.0f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-2.5f, 3.f, 5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 3.f, -5.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, 9.f, -5.f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.5f, 9.f, 5.f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, 12.f, 0.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-2.5f, 9.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5f, 9.f, -5.f);

    glTexCoord2f(1.f, 1.f);
    glVertex3f(-2.5, 9.f, 5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-2.5, 9.f, 0.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5, 12.f, 0.f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.5f, 12.f, 0.f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 9.f, 0.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(6.5f, 9.f, -5.f);

    glTexCoord2f(1.f, 1.f);
    glVertex3f(6.5f, 9.f, 5.f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(6.5f, 9.f, 0.f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(6.5f, 12.f, 0.f);
    glEnd();
    if (!doingShadows) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.8f, 0.5f, 0.5f);
    }
    glBegin(GL_QUADS);
    glVertex3f(-2.6f, 3.f, 1.f);
    glVertex3f(-2.6f, 3.f, -1.f);
    glVertex3f(-2.6f, 6.f, -1.f);
    glVertex3f(-2.6f, 6.f, 1.f);
    glEnd();
    GLUquadricObj *temp = gluNewQuadric();
    glPushMatrix();
    glTranslatef(6, 3, -4);
    glRotatef(90, 1, 0, 0);
    gluCylinder(temp, 0.3f, 0.3f, 3, 7, 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2, 3, -4);
    glRotatef(90, 1, 0, 0);
    gluCylinder(temp, 0.3f, 0.3f, 3, 7, 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(6, 3, 4);
    glRotatef(90, 1, 0, 0);
    gluCylinder(temp, 0.3f, 0.3f, 3, 7, 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2, 3, 4);
    glRotatef(90, 1, 0, 0);
    gluCylinder(temp, 0.3f, 0.3f, 3, 7, 2);
    glPopMatrix();
    gluDeleteQuadric(temp);
    glPopMatrix();
    //end of house
    glPushMatrix();
    //start of fountain
    glRotatef(-90.0, 1.0, 0, 0);


    glPushMatrix();
    if (!doingShadows) glColor4f(0.0f, 0.1f, 0.5f, 1.f);
    glScalef(1, 1, 5);
    glTranslatef(0, 0, 5);
    glPopMatrix();
    glScalef(1.f, 1.f, 0.1f);
    if (!doingShadows) glColor4f(0.0f, 0.4f, 0.5f, 1.0f);

    glPopMatrix();

    int i;
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0, 0);
    glPushMatrix();
    if (!doingShadows) glColor4f(1.0f, 0.4f, 0.5f, 1.0f);
    gluCylinder(ball, 10, 10, 5, 20, 10);
    glTranslatef(0, 0, 5);
    if (!doingShadows) glColor4f(0.4f, 1.f, 0.5f, 1.0f);
    gluCylinder(ball, 1.2, 0.5, 20, 10, 10);
    glTranslatef(0, 0, 0.1f);
    if (!doingShadows) glColor4f(1.0f, 0.4f, 0.5f, 1.0f);
    gluDisk(ball, 8, 10.f, 20, 2);
    if (!doingShadows) glColor4f(0.3f, 0.5f, 0.8f, 1.0f);
    gluDisk(ball, 0, 8.f, 20, 2);
    glTranslatef(0, 0, 10.f);
    gluDisk(ball, 0, 4.f, 10, 2);
    glTranslatef(0, 0, 5.f);
    gluDisk(ball, 0, 2.f, 10, 2);
    glPopMatrix();
    if (!doingShadows) glColor4f(0.0f, 0.4f, 0.5f, 1.0f);

    founta.acc = -0.5f;
    for (i = 0; i < fno; i++) {
        if (founta.life[i]&&(founta.fwy[i] * founta.fwy[i] + founta.fwx[i] * founta.fwx[i]) < 81.0) {
            founta.fwx[i] -= (founta.vx[i]);
            founta.fwy[i] -= (founta.vy[i]);
            founta.vz[i] += founta.acc;
            if ((founta.fwz[i] + founta.vz[i] <= 15.0 && (founta.fwy[i] * founta.fwy[i] + founta.fwx[i] * founta.fwx[i]) < 4.0) ||
                    (founta.fwz[i] + founta.vz[i] <= 10.0 && (founta.fwy[i] * founta.fwy[i] + founta.fwx[i] * founta.fwx[i]) < 16.0) ||
                    (founta.fwz[i] + founta.vz[i] <= 0.0)) {
                founta.vz[i] = -founta.vz[i]*0.2f; //rebound if touched disks
                founta.vy[i] = founta.vy[i]*0.2f;
                founta.vx[i] = founta.vx[i]*0.2f;
            } else
                founta.fwz[i] += (founta.vz[i]);
            founta.life[i] = (founta.life[i] - 1) % 20;
            glPushMatrix();
            glTranslatef(founta.fwx[i], founta.fwy[i], founta.fwz[i] + 5);
            gluSphere(ball, 0.1f, 5, 5);
            glPopMatrix();
        } else {
            founta.life[i] = ((rand() % 20));
            founta.fwx[i] = ((rand() % 20) - 10) / 10.0f;
            founta.fwy[i] = ((rand() % 20) - 10) / 10.0f;
            founta.fwz[i] = ((rand() % 10) + 200) / 10.0f;
            founta.vx[i] = ((rand() % 20) - 10) / 10.0f;
            founta.vy[i] = ((rand() % 20) - 10) / 10.0f;
            founta.vz[i] = ((rand() % 10)) / 10.0f;
        }
    }//end of fountain
    glPopMatrix();
    glPopMatrix();


    glPopMatrix();

    gluDeleteQuadric(ball);


}

// this tries to see which control point is under the mouse
// (for when the mouse is clicked)
// it uses OpenGL picking - which is always a trick
// TODO: if you want to pick things other than control points, or you
// changed how control points are drawn, you might need to change this

void TrainView::doPick() {
    make_current(); // since we'll need to do some GL stuff

    int mx = Fl::event_x(); // where is the mouse?
    int my = Fl::event_y();

    // get the viewport - most reliable way to turn mouse coords into GL coords
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    // set up the pick matrix on the stack - remember, FlTk is
    // upside down!
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix((double) mx, (double) (viewport[3] - my), 5, 5, viewport);

    // now set up the projection
    setProjection();

    // now draw the objects - but really only see what we hit
    GLuint buf[100];
    glSelectBuffer(100, buf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    // draw the cubes, loading the names as we go
    for (size_t i = 0; i < world->points.size(); ++i) {
        glLoadName((GLuint) (i + 1));
        world->points[i].draw();
    }

    // go back to drawing mode, and see how picking did
    int hits = glRenderMode(GL_RENDER);
    if (hits) {
        // warning; this just grabs the first object hit - if there
        // are multiple objects, you really want to pick the closest
        // one - see the OpenGL manual 
        // remember: we load names that are one more than the index
        selectedCube = buf[3] - 1;
    } else // nothing hit, nothing selected
        selectedCube = -1;

    printf("Selected Cube %d\n", selectedCube);
}

// CVS Header - if you don't know what this is, don't worry about it
// This code tells us where the original came from in CVS
// Its a good idea to leave it as-is so we know what version of
// things you started with
// $Header: /p/course/-gleicher/private/CVS/TrainFiles/TrainView.cpp,v 1.9 2008/10/21 14:46:45 gleicher Exp $
