/**
 * Assignment 3 - csidell1
 *
 * I know this is a piss poor assignment to turn in. I have midterms
 * 	and other work I need to get through and openGL 2 isn't worth
 * 	my time to learn as it's been obsolete for 15 years.
 *
 * My resource list goes from the openGL man pages, stack overflow(numerous times),
 * 	lecture slides, lecture resources, 
 */

#include "walk.H"
#include <iostream>
#include <fstream>
#include <cstring>

#ifndef M_PI
#define	M_PI		3.14159265358979323846	/* pi */
#endif

// Global Variables
std::vector<Object> objects;
std::vector<Light> lights;
Camera camera, origCamera;
Viewport viewport;
bool SMOOTH_SHADING = false;
bool wMode = false;

///////////////////////////////////////////////////
// Begin Class Function Definitions
///////////////////////////////////////////////////

void Translate::apply() const {
  glTranslated(translation.x(),translation.y(),translation.z());
  //glPushMatrix
  // apply a translation to the OpenGL modelview matrix stack
  // see glTranslated
};

void Scale::apply() const {
  glScaled(scale.x(),scale.y(),scale.z());
  // apply a scale transformation to the OpenGL modelview matrix stack
  // see glScaled
};

void Rotate::apply() const {
  glRotated(angle,axis.x(),axis.y(),axis.z());
  // apply a rotation to the OpenGL modelview matrix stack
  // see glRotated
};

void Object::readObject(char *fname) {
  std::ifstream in(fname, std::ios::in);
  char c;
  SlVector3 pt;
  Triangle t;
  
  if (!in.good()) {
	std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
	abort();
  }
  
  while (in.good()) {
	in >> c;
	if (!in.good()) break;
	if (c == 'v') {
	  in >> pt[0] >> pt[1] >> pt[2];
	  vertices.push_back(pt);
	} else if (c == 'f') {
	  in >> t[0] >> t[1] >> t[2];
	  t[0]-=1; t[1]-=1; t[2]-=1;
	  triangles.push_back(t);
	}
  }
  // we've got the geometry, lets compute the normals
  computeNormals();
}

void Object::createDisplayList() {
  // If you choose to use display lists, this would be a reasonable place to add that code,
  // otherwise you can just leave this blank
}

void Object::drawObject() {
  //Enable standard GL things
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);

  //Save matrix
  glPushMatrix();

  //Apply all transformations
  for (int i = 0; i < transforms.size(); i++)
  {
    transforms[i]->apply();
  }

  //Wireframe mode
  if(wMode)
    glBegin(GL_LINES);  
  else
    glBegin(GL_TRIANGLES);

	//Define materials
  GLfloat ambient_color[4] = {ka*color.x(),ka*color.y(),ka*color.z(), 1.0f};
  GLfloat diffuse_color[4] = {kd*color.x(),kd*color.y(),kd*color.z(), 1.0f};
  GLfloat specular_color[4] = {ks*color.x(), ks*color.y(), ks*color.z(), 1.0f};

  //Draw all triangles
  for (int i = 0; i < triangles.size(); i++)
  {
    //Set color and material properties
    glColor3f(color.x(),color.y(),color.z());
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);

    glMaterialf(GL_FRONT, GL_SHININESS, ks);

    //Set normals
  	glNormal3d(faceNormals[i][0],faceNormals[i][1],faceNormals[i][2]);
  	
    //Build triangle
  	for (int j = 0; j < 3; ++j)
  	{
  		glVertex3f(vertices[triangles[i][j]][0],
  			vertices[triangles[i][j]][1],
  			vertices[triangles[i][j]][2]);
  	}
  }

  //Complete
  glEnd();

  //Restore matrix
  glPopMatrix();

  // here you draw the object
  // you will need to set material properties
  // and normals as well as actually draw the triangles
  // this is also where you call apply transformations
  // remember to push and pop your matrices
}

void Object::computeNormals() {
    //For all triangles
	 for (int i = 0; i < triangles.size(); i++)
	  {
      //Compute normals
	  	SlVector3 a = vertices[triangles[i][0]];
	  	SlVector3 b = vertices[triangles[i][1]];
	  	SlVector3 c = vertices[triangles[i][2]];
	  	SlVector3 normal = cross(b - a, c - a);
	  	faceNormals.push_back(normal);
	  }

  // here you compute normals
  // you should compute both face normals (cross product) for flat shading
  // and vertex normals (area-weighted averages of face normals) for
  // smooth shading
  // remember to normalize your normals
}

void Light::apply() {
  //GL Light things
	glLightfv(id,GL_AMBIENT,ambient);
	glLightfv(id,GL_DIFFUSE,diffuse);
	glLightfv(id,GL_SPECULAR,specular);
	glLightfv(id,GL_POSITION,pos);

  //Enable light
	glEnable(id);

	// this function tells openGL about the light
	// it is called from myDisplay().
	// Use glLightfv to set the different properties
	// of the light and glEnable() to turn the light on.
}

///////////////////////////////////////////////////
// End Class Definitions
///////////////////////////////////////////////////

// this function reads the input file
void parseInputFile(char *fname) {
  std::ifstream in(fname, std::ios::in);
  Object obj;
  char str[80];
  int numObjects,numLights,i;
  
  if (!in.good()) {
	std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
	abort();
  }
  
  in >> camera.fov >> camera.nearplane >> camera.farplane;
  in >> camera.eye[0] >> camera.eye[1] >> camera.eye[2];
  in >> camera.center[0] >> camera.center[1] >> camera.center[2];
  in >> camera.up[0] >> camera.up[1] >> camera.up[2];
  
  origCamera = camera;

  in >> numObjects;
  
  objects.resize(numObjects);
  
  for (i=0; i<numObjects; i++) {
	objects[i].dlid = (GLuint) i+1;
	in >> objects[i].color[0] >> objects[i].color[1] >> objects[i].color[2];
	in >> objects[i].ka >> objects[i].kd >> objects[i].ks;
	int done = 0;
	// this loop reads an arbitrary number of transformations
	// then reads the obj file name
	while (!done) {
	  in >> str;
	  if (std::strstr(str, "translate")) {
		SlVector3 translation;
		in >> translation[0] >> translation[1] >> translation[2];
		objects[i].transforms.push_back(new Translate(translation));
	  } else if (strstr(str, "rotate")) {
		double angle;
		SlVector3 axis;
		in >> angle >> axis[0] >> axis[1] >> axis[2];
		objects[i].transforms.push_back(new Rotate(angle, axis));
	  } else if (strstr(str, "scale")) {
		SlVector3 scale;
		in >> scale[0] >> scale[1] >> scale[2];
		objects[i].transforms.push_back(new Scale(scale));
	  } else {
		objects[i].readObject(str);
		done = 1;
	  }
	}
	objects[i].createDisplayList();
  }
  
  // read lights
  in >> numLights;
  lights.resize(numLights);
  
  for (i=0; i<numLights; i++) {
	in >> lights[i].ambient[0] >> lights[i].ambient[1] >> lights[i].ambient[2] >>
	  lights[i].ambient[3];
	in >> lights[i].diffuse[0] >> lights[i].diffuse[1] >> lights[i].diffuse[2] >>
	  lights[i].diffuse[3];
	in >> lights[i].specular[0] >> lights[i].specular[1] >> lights[i].specular[2] >>
	  lights[i].specular[3];
	in >> lights[i].pos[0] >> lights[i].pos[1] >> lights[i].pos[2] >> lights[i].pos[3];
	lights[i].id=GL_LIGHT0+i;
  }
}

// this routine computes the current time of day and 
// draws the clock, it assumes we're already in 2D drawing mode
void drawClock() {
  double theta;
  time_t mtime; // machine time
  tm *ltime;    // locale time
  time (&mtime);
  ltime = localtime(&mtime);
  
  double hours = ltime->tm_hour + ltime->tm_min/60.0 + ltime->tm_sec/3600.0;
  double minutes = ltime->tm_min + ltime->tm_sec/60.0;
  int seconds = ltime->tm_sec;

  float scaleDown[3] = {0.25,0.25,0.25};
  float translateDown[3] = {-1.0,-1.0,0};

  glMatrixMode(GL_MODELVIEW);

  //Draw Second Hand
  glPushMatrix();

  //Scale per-clock
  glScaled(0.9,0.9,0.9);
  //clock scale
  glScaled(scaleDown[0],scaleDown[1],scaleDown[2]);
  //Rotation
  glRotatef(-360 * (seconds/60.0), 0,0,1.0);

  //Draw hand
  glBegin(GL_TRIANGLES); 
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0, 1.0f);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(0.1f, 0.0f);
  glEnd();

  glPopMatrix();

  //Draw Minute Hand
  glPushMatrix();

  glScaled(0.75,0.75,0.75);
  glScaled(scaleDown[0],scaleDown[1],scaleDown[2]);
  glRotatef(-360 * (minutes/60.0), 0,0,1.0);

  glBegin(GL_TRIANGLES); 
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0, 1.0f);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(0.1f, 0.0f);
  glEnd();

  glPopMatrix();

  //Draw hour hand
  glPushMatrix();

  glScaled(0.6,0.6,0.6);
  glScaled(scaleDown[0],scaleDown[1],scaleDown[2]);
  glRotatef(-360 * (hours/12.0), 0,0,1.0);
  
  glBegin(GL_TRIANGLES); 
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0, 1.0f);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(0.1f, 0.0f);
  glEnd();

  glPopMatrix();
}

void myReshape(int w, int h) {
  glViewport (0,0,w,h);
  viewport.w = w;
  viewport.h = h;
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(SMOOTH_SHADING)
    glShadeModel(GL_SMOOTH);

  //Enable depth
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //Setup perspective
  gluPerspective(camera.fov, viewport.w/viewport.h, camera.nearplane, camera.farplane);
  gluLookAt(camera.eye.x(),camera.eye.y(),camera.eye.z(),
    camera.center.x(),camera.center.y(),camera.center.z(),
    camera.up.x(),camera.up.y(),camera.up.z());

  // apply all the lights
  glMatrixMode(GL_MODELVIEW);
  for (int i = 0; i < 8 &&  i < lights.size() ; ++i)
  {
    lights[i].apply();
  }

  // draw all the objects
  for (int i = 0; i < objects.size(); ++i)
  {
    objects[i].drawObject();
  }

  // switch to 2d (gluOrtho2D) and draw the clock hands
  glPushMatrix();
    glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
    glLoadIdentity();                 // Reset Projection matrix
    gluOrtho2D(-0.2, 2.0, -0.2, 2.0);
    drawClock();
  glPopMatrix();
    
  //Restore perspective view
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera.fov, viewport.w/viewport.h, camera.nearplane, camera.farplane);

  //Enable lighting and materials
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  // you will need to enable various GL features (like GL_LIGHTING) here
  // this is probably the hardest function to fill in, but
  // it shouldn't be very long or complicated, it will be hard
  // to figure all the OpenGL things you have to do, it may help 
  // to look for some other OpenGL code on the web

  glutSwapBuffers();
}

// handle "special" (non-ascii) keyboard events
void mySpecial(int key, int x, int y) {
  double theta = 0.087266463;
  SlVector3 dir, ndir;
  switch(key) {
  case GLUT_KEY_UP:
  	camera.eye += SlVector3(0.0,0.0,0.1);
  	camera.center += SlVector3(0.0,0.0,0.1);
	break;
  case GLUT_KEY_DOWN:
  	camera.eye += SlVector3(0.0,0.0,-0.1);
    camera.center += SlVector3(0.0,0.0,-0.1);
	break;
  case GLUT_KEY_LEFT:
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) 
	{
		camera.eye += SlVector3(0.1,0.0,0.0);
    camera.center += SlVector3(0.1,0.0,0.0);
	}
	else
  {
    //Couldn't get this to actually work...
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    GLfloat mat44[16];

    mat44[0] = camera.eye.x();
    mat44[5] = camera.eye.y();
    mat44[10] = camera.eye.z();
    mat44[15] = 1.0f;

    glLoadIdentity();
    glMultMatrixf(mat44);
    glRotated(-5.0,0.0,camera.eye.y(),0.0);

    GLfloat resp[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, resp);

    camera.eye.set(resp[0],resp[5],resp[10]);

    glPopMatrix();
  }
	break;
  case GLUT_KEY_RIGHT:
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) 
	{
		camera.eye += SlVector3(-0.1,0.0,0.0);
    camera.center += SlVector3(-0.1,0.0,0.0);
	}
	else
  {
    //Couldn't get this to actually work...
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    GLfloat mat44[16];

    mat44[0] = camera.center.x();
    mat44[5] = camera.center.y();
    mat44[10] = camera.center.z();
    mat44[15] = 1.0f;

    glLoadIdentity();
    glMultMatrixf(mat44);
    glRotated(5.0,0.0,camera.eye.y(),0.0);

    GLfloat resp[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, resp);

    camera.center.set(resp[0],resp[5],resp[10]);

    glPopMatrix();
  }
	break;
  default:
	std::cerr<<"That key is not recognized"<<std::endl;
	break;
  }
  glutPostRedisplay();
}

// handle ascii keyboard events
void myKeyboard(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
  case 'Q':
	exit(0);
	break;
  case 'o':
  case 'O':
	std::cout<<"o key pressed"<<std::endl;
	camera = origCamera;
	// here you want to set the current camera to the original camera
	// this is a one-liner
	break;
  case 's':
  case 'S':
	std::cout<<"s key pressed"<<std::endl;
  SMOOTH_SHADING = !SMOOTH_SHADING;
  case 'w':
  case 'W':
    wMode = !wMode; 
	break;
  default:
	std::cerr<<"key "<<key<<" not supported"<<std::endl;
  }
  glutPostRedisplay();
}

// timer function so that the clock is updated at least every second
void myTimerFunc(int id) {
  glutPostRedisplay();
  glutTimerFunc(1000, myTimerFunc, 0);
}

// the usual stuff, nothing exciting here
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);
  
  glutDisplayFunc(myDisplay);
  glutReshapeFunc(myReshape);
  glutKeyboardFunc(myKeyboard);
  glutSpecialFunc(mySpecial);
  glutTimerFunc(1000, myTimerFunc, 0);
  
  parseInputFile(argv[1]);
  
  glutMainLoop();
  
  return 0;
}

