//PINDEFINITION
#define pinPanelHomeButton 9 
#define pinAutomaticButton 10 
#define pinPanelRightButton 11
#define pinPanelLeftButton 12 

//VARIABLES
bool automaticButton = false;
bool panelLeftButton = false;
bool panelRightButton = false;
bool panelHomeButton = false;

void initButtons(){
  pinMode(pinAutomaticButton, INPUT);
  pinMode(pinPanelLeftButton, INPUT);
  pinMode(pinPanelRightButton, INPUT);
  pinMode(pinPanelHomeButton, INPUT);
  
  updateButtons();
  if (LOG_BUTTONS) Serial.println("initButtons done");
}

void updateButtons(){
  //automatic on/off switch
  automaticButton  = digitalRead(pinAutomaticButton);
  //directional switches left/right
  panelLeftButton = digitalRead(pinPanelLeftButton);
  panelRightButton = digitalRead(pinPanelRightButton);
  //home switch
  panelHomeButton = digitalRead(pinPanelHomeButton); 
  if (LOG_BUTTONS){
    Serial.print("updateButtons -" );
    Serial.print("automatic: "+ String(automaticButton));
    Serial.print(" left: "+String(panelLeftButton));
    Serial.print(" right: "+String(panelRightButton));
    Serial.println(" home: "+String(panelHomeButton));
  }
}
