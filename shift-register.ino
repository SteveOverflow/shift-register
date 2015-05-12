const int clockPin = 8;//SH_CP
const int dataPin = 11; //DS
const int latchPin = 12;//ST_CP

int spreadArray[6] = {24, 36, 66, 129, 66, 36};
int allArray[2] = {255, 0};
int sweepArray[14] = {128, 64, 32, 16, 8, 4, 2, 1, 2, 4, 8, 16, 32, 64};
int reverseSweep[14] = {127, 191, 223, 239,247,251,253, 254, 253, 251, 247, 239, 223, 191};

//int curPattern;

class LightPattern{
  int curIndex = 0;
  int* pattern;
  int patternLength;
  int delayLength;
  unsigned long lastMillis;
  
  public:
  LightPattern(int p[] , int l, int d){
    pattern = p;
    patternLength = l;
    delayLength = d;
  }
  
  void resetIndex(){
    curIndex = 0;
    lastMillis = 0; 
  }
  
  void update(){
    unsigned long currentMillis = millis();
    
    if(currentMillis - lastMillis >= delayLength){
      if(curIndex >= patternLength){
        curIndex = 0; 
      }
      
      digitalWrite(latchPin,LOW); 
      shiftOut(dataPin,clockPin,MSBFIRST,pattern[curIndex]);
      digitalWrite(latchPin,HIGH);
      
      lastMillis = millis();
      curIndex++;
    }
  }
};

LightPattern spread(spreadArray, 6, 1000);
LightPattern all(allArray, 2, 1000);
LightPattern sweep(sweepArray, 14, 125);
LightPattern reverse(reverseSweep, 14, 125);

LightPattern* curPattern;

void setup (){
  Serial.begin(9600);
  
  //set pins to output
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  
  //Set intial pattern
  //curPattern = 1;
  curPattern = &all;
  curPattern->resetIndex();
}

void loop(){
  if(Serial.available()){
    char input = Serial.read();
   
   if(input == 'a'){
     curPattern = &all;
     curPattern->resetIndex();
   }
   if(input == 'b'){
      curPattern = &spread;
      curPattern->resetIndex();
   }
   if(input == 'c'){
      curPattern = &sweep;
      curPattern->resetIndex();
   }
   if(input == 'd'){
      curPattern = &reverse;
      curPattern->resetIndex();
   }
  }
  
  curPattern->update(); 
}
