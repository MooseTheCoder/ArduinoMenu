#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define GREEND  0x03E0
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// SYSTEM WIDE
#define COLOR_DARK  GREEND
#define COLOR_LIGHT GREEN

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Display Setup

void initDisplay(){
  tft.reset();
  uint16_t identifier = 0x9341;    //Need hardcode here (IC)
  tft.begin(identifier);
  tft.setRotation(3);
}
// Drawing functions
void color(int color){
    tft.setTextColor(color);
}
void clearScreen(int color){
  tft.fillScreen(color);  
}
void cpos(int x, int y){
  tft.setCursor(x,y);
}
void font(int textSize){
  tft.setTextSize(textSize);
}
void drawButton(int dx,int dy,int width,int height,int c,int b){
  tft.fillRect(dx,dy,width,height,c);
  tft.drawRect(dx,dy,width,height,b);
}
void echo(String echo){
  tft.println(echo);  
}
void border(){
  tft.drawRect(0,0,320,240,COLOR_DARK);
}
void itemBox(){
  tft.fillRect(150,20,155,205,COLOR_DARK);
}
void prep(){
  clearScreen(BLACK);
  clearScreen(WHITE);
  clearScreen(GREEN);
  clearScreen(GREEND);
  cpos(125,80);
  font(4);
  echo("MNS");
  font(1);
  cpos(100,125);
  echo("www.markamations.com");
  delay(1000);
}
// RUNTIME
String currentMenu = "MENU";
String seletedMenuTitle = "";
int seletedItemInt = 0;
int baseY=20;
int sel = 0;
void setup(){
  Serial.begin(9200);
  initDisplay();
  prep();
  menu(seletedItemInt,currentMenu);
}
void loop(){
  getInput();
}
void getInput(){
  int key;
  while(Serial.available()){
    key = Serial.read();
    if(key == 49){  
      seletedItemInt--;
      if(seletedItemInt < sel){
        seletedItemInt = 0;
      }
    }
    if(key == 50){
      seletedItemInt++;
    }
    if(key == 51){
      currentMenu = seletedMenuTitle;
      seletedItemInt = 0;
    }
    if(key == 48){
      currentMenu = "MENU";
      seletedItemInt = 0;
    }
    menu(seletedItemInt,currentMenu);
  }
}
void menu(int h,String m){
  //MENU ITEMS
  char menuItems[] = "TASKS,LOCATIONS,PEOPLE";
  
  // QUESTS MENU
  char quests[] = "Veriserv,SyNet,Mautic,WikiMedia";
  char box_quests[4][20] = {"Neil...","Link it all","Mautic Forms","Config 2"};
  // END QUESTS MENU

  //LOCATIONS MENU
  char locations[] = "dev-mark,salesboard,kanboard,tickets";
  char box_locations[4][15] = {"Development","Info Board","Projects","Ticket System"};
  //END LOCATIONS MENU

  //PEOPLE MENU
  char people[] = "Garry,Adohan,Jenny";
  char box_people[3][10] = {"216","UNKNOWN","203"};
  //END PEOPLE MENU
  
  char boxItem[]="A menu Item";
  //END MENU ITEMS
  Serial.flush();
  Serial.println("Current Menu => "+currentMenu);
  char * splitMenuItems;
  clearScreen(BLACK);
  border();
  cpos(10,0);
  font(2);
  color(COLOR_LIGHT);
  echo(m);
  itemBox();
  //MENU SWITCHER
  if(currentMenu == "MENU"){
    splitMenuItems = strtok(menuItems,",");
  }
  //
  if(currentMenu == "TASKS"){
    splitMenuItems = strtok(quests,",");
  }
  //
  if(currentMenu == "LOCATIONS"){
    splitMenuItems = strtok(locations,",");
  }
  //
  if(currentMenu == "PEOPLE"){
    splitMenuItems = strtok(people,",");
  }
  int sel=0;
  baseY = 20;
  while(splitMenuItems != NULL){
    if(h == sel){
      drawButton(10,baseY,125,25,COLOR_DARK,WHITE);
      seletedMenuTitle = splitMenuItems;
      Serial.println("|"+seletedMenuTitle+"|");
    }else{
      drawButton(10,baseY,125,25,COLOR_DARK,COLOR_LIGHT);
      Serial.println(splitMenuItems);
    }
    cpos(15,baseY+5);
    echo(splitMenuItems);
    baseY = baseY+30;
    sel++;
    splitMenuItems = strtok (NULL, ",");
  }
  //WRITE ITEM BOX INFO
  cpos(160,30);
  tft.println(seletedMenuTitle);
  cpos(160,60);
  if(currentMenu != "MENU"){
    if(currentMenu == "TASKS"){
      tft.println(box_quests[seletedItemInt]);
    }
    //
    if(currentMenu == "LOCATIONS"){
      tft.println(box_locations[seletedItemInt]);
    }
    //
    if(currentMenu == "PEOPLE"){
      tft.println(box_people[seletedItemInt]);
    }
  }else{
    tft.println(boxItem);
  }
}
