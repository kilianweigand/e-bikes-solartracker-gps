/* handles rtc - clock and calculates current azimuth and altitude of sun */
int sunrise[64][3] =  {
  {330,55,0},
{345,58,2},
{360,60,4},
{375,63,7},
{390,65,9},
{405,68,11},
{420,70,14},
{435,73,16},
{450,75,19},
{465,78,21},
{480,80,24},
{495,83,26},
{510,86,29},
{525,88,31},
{540,91,34},
{555,94,37},
{570,97,39},
{585,100,42},
{600,103,44},
{615,106,47},
{630,110,49},
{645,114,52},
{660,118,54},
{675,122,56},
{690,127,58},
{705,133,60},
{720,139,62},
{735,146,64},
{750,153,65},
{765,161,66},
{780,169,67},
{795,178,67},
{810,187,67},
{825,196,66},
{840,204,66},
{855,211,64},
{870,218,63},
{885,225,61},
{900,231,59},
{915,236,57},
{930,240,55},
{945,245,53},
{960,249,50},
{975,253,48},
{990,256,45},
{1005,259,43},
{1020,262,40},
{1035,265,38},
{1050,268,35},
{1065,271,32},
{1080,273,30},
{1095,276,27},
{1110,279,25},
{1125,281,22},
{1140,284,20},
{1155,286,17},
{1170,289,15},
{1185,291,12},
{1200,294,10},
{1215,296,8},
{1230,299,5},
{1245,301,3},
{1260,304,1},
{1275,307,1}
};

int currentpos[3];
int lastupdatetime= -1;

void initSunPos() {
  
}

void updateSunPos(){
    updateSunPosInternal(getGPSHour(), getGPSMinute());
}

void updateSunPosInternal(int hour, int minutes) {
  int currenttime = hour * 60 + minutes;
  //only update if time changed
  if(lastupdatetime == currenttime) return;
  if(LOG_SUNPOS) Serial.println("currenttime: "+String(currenttime)); 
  
  //init with first pos
  currentpos[0] = sunrise[0][0];
  currentpos[1] = sunrise[0][1];
  currentpos[2] = sunrise[0][2];
  
  for (byte i = 0; i < (sizeof(sunrise) / sizeof(sunrise[0])); i++) {
    int tmpTime = sunrise[i][0];
    //Serial.println(tmpTime);
    if(tmpTime > currenttime) break;
    currentpos[0] = sunrise[i][0];
    currentpos[1] = sunrise[i][1];
    currentpos[2] = sunrise[i][2];
  }
  lastupdatetime = currenttime;
}

int getAzimuth(){
  return currentpos[1];
}

int getAltitude(){
  return currentpos[2];
}
