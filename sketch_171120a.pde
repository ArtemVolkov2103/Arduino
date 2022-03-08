import processing.serial.*; // imports library for serial communication
//import java.awt.event.KeyEvent; // imports library for reading the data from the serial port
//import java.io.IOException;

Serial myPort; // defines Object Serial
float angle=0;
int dist=0;
int dist2=0;
int ang=0;
//int mas[]; 
int i=0;

int[] inStr = {0,0,0};

void setup() {
 size (1280, 720, P2D);
  myPort = new Serial(this,"COM3", 9600); // starts the serial communication
  myPort.bufferUntil('\n');
  //mas=new int[10];
  //printArray(Serial.list());
frameRate(15);
}

void draw() {
  //background(0,0,angle);
  strokeWeight(1);
  stroke(1);
  line(0,350,1280,350);
  line(640,0,640,720);
  
  fill(0, 190, 255);                                  //blue text
  textAlign(LEFT, CENTER);
  text("0", 1200-10, 350);                          //"0" degrees

  textAlign(LEFT, BOTTOM);
  text("45", 1200-250, 640*0.2);             //"45" degrees

  textAlign(LEFT, CENTER);
  text("90",1200/2+5, 5);                         //"90" degrees

  textAlign(RIGHT, BOTTOM);
  text("135", 250, 640*0.2);          //"135" degrees

  textAlign(LEFT, CENTER);
  text("180", 70, 350);                       //"180" degrees
  
  fill(255); 
  textSize(20);
  textAlign(LEFT, BOTTOM);
  text("100cm", 600, width/2*0.15);                      //"180cm"
  text("50", 600, width/2*0.33);                    //"100cm"
  text("25", 600, width/2*0.45);                     //"50cm"
  text("12", 600, width/2*0.5);                    //"25cm"
  
  fill(color(random(0,255),random(0,255),random(0,255)));
  circle(abs(640+(dist*(cos(ang*0.01745))*3)), abs(350-dist*sin(ang*0.01745)*3),5);
  circle(abs(640-(dist2*(cos(ang*0.01745))*3)), abs(350+dist2*sin(ang*0.01745)*3),5);
  //delay(100);
  textAlign(CENTER);
  textSize(25);
  text("1st value: "+inStr[0]+" 2nd value: "+inStr[1],width/2,30);  
  text("x: "+int(dist*cos(ang))+" y: "+int(dist*sin(ang)),width/2,70); 
   text("x1: "+int(640+dist*cos(ang))+" y1: "+int(350+dist*sin(ang)),width/2,100); 
}

void serialEvent(Serial myPort) {
try {
String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);
    inStr = int(split(inString,','));
    println(inStr);
    ang=inStr[0];
    dist=inStr[1];
    dist2=inStr[2];
  }
}
  catch(RuntimeException e) {
    e.printStackTrace();
  }
}
