#include<SPI.h>
#include<Ethernet.h>
#include<EthernetUdp.h>
#include<NTPClient.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned int localport = 8888;

bool operative = true;

//SEZIONE DI DEFINIZIONE STRINGHE
String formatteddate;
String daystamp;
String timestamp;
String minutestamp;

///////////////////////////////////////////////////

//SEZIONE DI DEFINIZIONE PIN
int relaycampana = 1;

////////////////////////////////////////////////////

EthernetUDP udp;
unsigned int offset = 3600;
NTPClient timeClient(udp, "europe.pool.ntp.org", offset);

void setup()
{
  while(Ethernet.begin(mac)==false){
      //keep trying to connect
  }
  udp.begin(localport);//inizializza l'udp attraverso la porta scelta
  timeClient.begin();//inizializza il client per la ricezione di segnale

  //SEZIONE DI DEFINIZIONE PINMODE
  pinMode(relaycampana, OUTPUT);
}

void loop()
{
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }


  formatteddate = timeClient.getFormattedTime();

  int splitT = formatteddate.indexOf("T");

  daystamp = formatteddate.substring(0, splitT);
  timestamp = formatteddate.substring(splitT + 1, formatteddate.length() - 1);
  minutestamp = formatteddate.substring(splitT + 4, splitT + 5);
    //CODICE PER FAR SUONARE UNA VOLTA LA CAMPANELLA AL MINUTO INDICATO
    if (operative)
    {
      if (minutestamp == "15")
      {
        suona();
        operative = false;
      }
    }
    else 
    {
      if (minutestamp != "15")
      {
        operative = true;
      }
    }
  else
  {
    digitalWrite(ledverde, LOW);
    digitalWrite(ledrosso, HIGH);
  }
  delay(1000);
}

void suona()
{
  digitalWrite(relaycampana, HIGH);
  delay(5000);
  digitalWrite(relaycampana, LOW);
}
/*
MATERIALE UTILE:
Arduino Uno
Shield Ethernet
Dispay 16X2 con modulo I2C
Cavo Ethernet
Alimentatore Arduino
Jumper
*/