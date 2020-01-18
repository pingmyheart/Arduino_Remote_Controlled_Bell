#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NTPClient.h>

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
int bellRelay = 1;

////////////////////////////////////////////////////

EthernetUDP udp;
unsigned int offset = 3600;
NTPClient timeClient(udp, "europe.pool.ntp.org", offset);

String ringMinute = "15";

void setup()
{
    while (Ethernet.begin(mac) == false)
    {
        //keep trying to connect
    }
    udp.begin(localport); //inizializza l'udp attraverso la porta scelta
    timeClient.begin();   //inizializza il client per la ricezione di segnale

    //SEZIONE DI DEFINIZIONE PINMODE
    pinMode(bellRelay, OUTPUT);
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
        if (minutestamp == ringMinute)
        {
            suona();
            operative = false;
        }
    }
    else if (minutestamp != ringMinute)
    {
        operative = true;
    }
    delay(1000);
}

void suona()
{
    digitalWrite(bellRelay, HIGH);
    delay(5000);
    digitalWrite(bellRelay, LOW);
}