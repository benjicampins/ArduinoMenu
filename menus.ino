///////////////////////////////////////////////////////////////////////////
//  Menú con submenús para Arduino y LCD Keypad shield                  ///
//  Funciona como una máquina de estados finitos del tipo               ///
//  Autómata de Moore. Versión sin delay()                              ///
//  Con el boton SELECT se enciende y apaga el display                  ///
//  Autor: Benji Campins                                                ///
//  Versión: 1.0  Fecha: 16/09/2020                                     ///
///////////////////////////////////////////////////////////////////////////


#include <LiquidCrystal.h>
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define btnPIN    A0
#define keyDELAY 200

#define LCDlightPIN 10


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Los distintos estados, MENU_1_1 es submenú de MENU_1, etc.
enum state {MENU_1, MENU_2, MENU_3, MENU_1_1, MENU_1_2, MENU_1_3, MENU_2_1, MENU_2_2, MENU_1_1_1, MENU_1_2_1, MENU_1_3_1, LCD_ON, LCD_OFF};

// Variables que almacenan el estado actual y el siguiente estado
state pr_state,nx_state;

unsigned long currentMillis;
bool LCDlightState = true;      // Estado inicial de la iluminación del display - true = encendido
int variableGlobal1 = 0;

void setup() {
  lcd.begin(16, 2);              // Inicializar el LCD
  lcd.setCursor(0,0);
  Serial.begin(9600);
  pr_state = MENU_1;
  nx_state = MENU_1;
  pinMode(LCDlightPIN, OUTPUT);
  digitalWrite(LCDlightPIN, LCDlightState); 
}
                                                                 

void loop() {

  currentMillis = millis();

  digitalWrite(LCDlightPIN, LCDlightState);
  
  switch(pr_state){
     int button;
  
    // PRIMER NIVEL DE LOS MENÚS
    
    case MENU_1:
      printMenu(">MENU 1         ", "MENU 2          ");
      button = readLCDButtons();
      switch(button){
        case btnDOWN:
          nx_state = MENU_2;
          break;
        case btnRIGHT:
          nx_state = MENU_1_1;
          break;
      }
      break;

    case MENU_2:
      printMenu(">MENU 2         ", "MENU 3        ");
      button = readLCDButtons();
      switch(button){
        case btnUP:
          nx_state = MENU_1;
          break;
        case btnDOWN:
          nx_state = MENU_3;
          break;
        case btnRIGHT:
          nx_state = MENU_2_1;
          break;

      }
      break;

    case MENU_3:
      printMenu(">MENU 3         ", "                ");
      button = readLCDButtons();
      switch(button){
        case btnUP:
          nx_state = MENU_2;
          break;
      }
      break;
    
    // SEGUNDO NIVEL DE LOS MENUÚS
    
      case MENU_1_1:
      printMenu(">MENU 1-1     ", "MENU 1-2           ");
       button = readLCDButtons();
       switch(button){
        case btnDOWN:
          nx_state = MENU_1_2;
          break;
        case btnLEFT:
          nx_state = MENU_1;
          break;
        case btnRIGHT:
          nx_state = MENU_1_1_1;
          break;
      }
      break;
      
      case MENU_1_2:
      printMenu(">MENU 1_2         ", "MENU 1-3         ");
      button = readLCDButtons();
       switch(button){
        case btnDOWN:
          nx_state = MENU_1_3;
          break;
        case btnLEFT:
          nx_state = MENU_1;
          break;
        case btnUP:
          nx_state = MENU_1_1;
          break;
        case btnRIGHT:
          nx_state = MENU_1_2_1;
          break;
    
      }
      break;
   
      case MENU_1_3:
      printMenu(">MENU 1-3     ", "              ");
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_1;
          break;
        case btnUP:
          nx_state = MENU_1_2;
          break;
        case btnRIGHT:
          nx_state = MENU_1_3_1;
          break;
       }
       
      break;
      
      case MENU_2_1:
      printMenu(">MENU 2-1     ", "MENU 2-2      ");
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_2;
          break;
        case btnDOWN:
          nx_state = MENU_2_2;
          break;
       }
       
      break;
      
      case MENU_2_2:
      printMenu(">MENU 2-2     ", "              ");
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_2;
          break;
        case btnUP:
          nx_state = MENU_2_1;
          break;
       }
       
      break;
      
   
    case MENU_1_1_1:
     
      String S = "Val: ";
      S += (String)variableGlobal1;
      printMenu(">VALOR     ", S);
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_1_1;
          break;
        case btnUP:
          lcd.clear();
          variableGlobal1++;
          break;
        case btnDOWN:
          lcd.clear();
          variableGlobal1--;
          break;
          
      }
      break;
    
    case MENU_1_2_1:
      printMenu(">MENU 1-2-1     ", "                ");
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_1_2;
          break;
      }
      break;
      
    case MENU_1_3_1:
      printMenu(">MENU 1-3-1     ", "                ");
      button = readLCDButtons();
       switch(button){
        case btnLEFT:
          nx_state = MENU_1_2;
          break;
      }
      break;
      
    default:
      nx_state = pr_state;
  }

// Avanza la máquina de estados al estado siguiente
pr_state = nx_state;

}

int readLCDButtons()  {
    static unsigned long previousMillis;
    int adc_key_in = analogRead(btnPIN);      // Leemos A0
    // Mis botones dan:  0, 133, 307,481,721
    // Y ahora los comparamos con un margen comodo
    if (adc_key_in > 900 && currentMillis > previousMillis + keyDELAY) {
      return btnNONE;     // Ningun boton pulsado 
    }
    if (adc_key_in < 50 && currentMillis > previousMillis + keyDELAY) {
         previousMillis = currentMillis;
         LCDlightState = true;            // Enciende la luz de fondo del display
         return btnRIGHT; 
    }
    if (adc_key_in < 200  && currentMillis > previousMillis + keyDELAY) {
         previousMillis = currentMillis;
         LCDlightState = true;            // Enciende la luz de fondo del display
         return btnUP;
    }
    if (adc_key_in <350  && currentMillis > previousMillis + keyDELAY)  {
         previousMillis = currentMillis;
         LCDlightState = true;            // Enciende la luz de fondo del display
         return btnDOWN;
    }
    if (adc_key_in < 550  && currentMillis > previousMillis + keyDELAY) {
         previousMillis = currentMillis;
         LCDlightState = true;            // Enciende la luz de fondo del display
         return btnLEFT;
    }
    if (adc_key_in < 800  && currentMillis > previousMillis + keyDELAY) {
         previousMillis = currentMillis;
         LCDlightState = !LCDlightState;   // Cambia el estado de la luz de fondo
         return btnSELECT; 
    }
    return btnNONE;  // Por si todo falla
  }

void printMenu(String L1, String L2)  {
     
      lcd.setCursor(0,0);
      lcd.print(L1);
      lcd.setCursor(0,1);
      lcd.print(L2);
     
 
}
