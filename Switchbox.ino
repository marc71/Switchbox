#include <ClickEncoder.h>
#include <TimerOne.h>

int KpinNo; 
int Koutpin;
byte _terminator;

String KoldpinStateSTR, KpinStateSTR, Kstringnewstate,Kstringoldstate;

ClickEncoder *encoder;
ClickEncoder *encoder2;
int16_t last, value, last2, value2;

void timerIsr() {
  encoder->service();
  encoder2->service();
}

void setup() {
  Kstringoldstate = "111111111111111111111111111111111111111111111111111111111111111111111";
  
  _terminator = 0x4;
  
  for (int KoutPin = 2; KoutPin < 13; KoutPin++)// Get all the pins ready for "Keys"  
  {
    pinMode(KoutPin, INPUT);
    digitalWrite(KoutPin, HIGH);  
  }
  
  Serial.begin(115200);
  encoder2 = new ClickEncoder(A0, A1, A2, 4);
  encoder = new ClickEncoder(A3, A4, A5, 4);

  encoder->setAccelerationEnabled(false);
  encoder2->setAccelerationEnabled(false);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = -1;
  last2 = -1;
}

void loop() {
  {KEYS();} //Check the "keys" section  
  value += encoder->getValue();
  value2 += encoder2->getValue();

  if (value != last) {
    if (value > last) {
      Serial.println("A57");
    } 
    else 
    {
      Serial.println("A58");
    }
    value = 0;
    last = value;

    // Serial.println(value);
  }

  if (value2 != last2) {
    if (value2 > last2) {
      Serial.println("A56");
    } 
    else 
    {
      Serial.println("A55");
    }
    value2 = 0;
    last2 = value2;
  }
  
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::Clicked:
          Serial.println("E01C");
      break;
      case ClickEncoder::DoubleClicked:
          Serial.println("E01D");
      break;
    }
  }

  ClickEncoder::Button b2 = encoder2->getButton();
  if (b2 != ClickEncoder::Open) {
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b2) {
      case ClickEncoder::Clicked:
          Serial.println("E02C");
      break;
      case ClickEncoder::DoubleClicked:
          Serial.println("E02D");
      break;
    }
  }

}

void KEYS() 
{
  Kstringnewstate = "";
  for (int KpinNo = 2; KpinNo < 13; KpinNo++){
    KpinStateSTR = String(digitalRead(KpinNo)); 
    KoldpinStateSTR = String(Kstringoldstate.charAt(KpinNo - 2));
    if (KpinStateSTR != KoldpinStateSTR)
    {
      switch (KpinNo) 
      {
        case 2:
            if (KpinStateSTR == "1") 
            {
              Serial.println ("E05");
            } 
            else 
            {
              Serial.println ("");
            }
            break;
        case 3:
            if (KpinStateSTR == "1") 
            {
              Serial.println ("E18");
            } 
            else 
            {
              Serial.println ("E17");
            }
            break;
       case 11:
            if (KpinStateSTR == "1") 
            {
              Serial.println ("B03");
            } 
            else 
            {
              Serial.println ("B02");
            }
            break;
       case 10: // Pitot-Heat, Pin 10
            if (KpinStateSTR == "1") 
            {
              Serial.println ("C05");
            } 
            else 
            {
              Serial.println ("C06");
            }
            break;
         case 4:
           Serial.print ("F02");
           Serial.println (KpinStateSTR);
           break;
        case 5:
         Serial.print ("C41");
         Serial.println (KpinStateSTR);
         break;
        case 6:
         Serial.print ("C43");
         Serial.println (KpinStateSTR);
         break;
         case 7:
         Serial.print ("C44");
         Serial.println (KpinStateSTR);
         break;
          case 8:
         Serial.print ("C45");
         Serial.println (KpinStateSTR);
         break;
           case 9:
         Serial.print ("C42");
         Serial.println (KpinStateSTR);
         break;
         
       default:
        if (KpinNo != 13)
        {
          Serial.print ("D"); 
          if (KpinNo < 10) Serial.print ("0");
          Serial.print (KpinNo);
          Serial.println (KpinStateSTR); 
        }
        break;  
      }
    }
    Kstringnewstate += KpinStateSTR;
  }
  Kstringoldstate = Kstringnewstate;
}
