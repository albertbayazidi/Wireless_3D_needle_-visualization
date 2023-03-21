import processing.opengl.*;
import processing.serial.*;

BufferedReader reader;
String line;
float[] angles = new float[3];
int angleIndex = 0;
Serial arduino;
boolean serial = false; //Endre når vi bruker serial
byte[] buff;

//vars for cam movement
float camRotationAngle;
float camElevationAngle;

float centerX;
float centerY;
float centerZ;

float camX;
float camY;
float camZ;

int start = 0;
void setup() {

  size(1024, 768, P3D);
  background(0);
  pushMatrix();
  translate(1024/2, 768/2, 100);
  rod(500, 10);
  popMatrix();
  lights();
  smooth();
  if (!serial) {
    reader = createReader("data.txt");
  } else {
    arduino = new Serial(this, "COM5", 9600); //Fyll inn portnummer.
  }
  buff = new byte[128];
}

void draw() {
  if (keyPressed) {
    if (key == ENTER) {
      start = 1;
    }
  }
  if (start == 1) {
    background(0);
    try {
      line = reader.readLine();
      while (Float.isNaN(float(line))) {
        try {
          line = reader.readLine();
        }
        catch (IOException e) {
          e.printStackTrace();
          line = null;
        }
      }
      angles[angleIndex] =  float(line);
      angleIndex = (angleIndex + 1) % 2;

      line = reader.readLine();
      while (Float.isNaN(float(line))) {
        try {
          line = reader.readLine();
        }
        catch (IOException e) {
          e.printStackTrace();
          line = null;
        }
        angles[angleIndex] =  float(line);
        angleIndex = (angleIndex + 1) % 2;
      }
    } catch (IOException e) {
          e.printStackTrace();
          line = null;
    }

    // Draw rod and apply rotation
    println("angles:");
    printArray(angles);
    pushMatrix();
    translate(1024/2, 768/2, 100);
    //uncomment rest for 3D
    rotateX(radians(angles[1]));
    //rotateY(radians(angles[1]));
    rotateZ(radians(angles[0]));
    rod(500, 10);
    popMatrix();
    //change camera:
    //updateCamera();
  }
}

//change camera with mouse or gyro
void updateCamera() {
  if (serial) {
    camRotationAngle = map(mouseX, 0, width, -HALF_PI, HALF_PI);
    camElevationAngle = map(mouseY, 0, height, PI/4, 3*PI/4);
  } else {
    //read from serial...?
  }
  //centres camera on rod)
  camX = width/2;
  camY = height/2;
  camZ = (height/2.0) / tan(PI*30.0 / 180.0);
  //calculate angle
  centerZ = (-cos(camRotationAngle) * sin(camElevationAngle)) + camZ;
  centerX = (sin(camRotationAngle) * sin(camElevationAngle)) + camX;
  centerY = -cos(camElevationAngle) + camY;
  camera(camX, camY, camZ, centerX, centerY, centerZ, 0, 1, 0);
}

//draws rod from 0,0
void rod(float length, float radius) {
  pushMatrix();
  translate(0, 0, 0);
  box(length, radius, radius);
  pushMatrix();
  translate(length/2+radius, -radius/2, radius/2);

  float w = radius;
  float h = radius;
  float l = radius;
  beginShape(QUADS);
  vertex(0, 0, 0);
  vertex(w, 0, 0);
  vertex(w, h, 0);
  vertex(0, h, 0);

  vertex(0, 0, 0);
  vertex(0, h, 0);
  vertex(w / 2, h, l / 2);
  vertex(w / 2, 0, l / 2);

  vertex(w / 2, 0, l / 2);
  vertex(w / 2, h, l / 2);
  vertex(w, h, l);
  vertex(w, 0, l);

  vertex(w, 0, l);
  vertex(w, h, l);
  vertex(w, h, 0);
  vertex(w, 0, 0);

  vertex(w, h, 0);
  vertex(w, h, l);
  vertex(w / 2, h, l / 2);
  vertex(0, h, 0);
  rotateY(PI);
  endShape();
  popMatrix();
  popMatrix();
}
