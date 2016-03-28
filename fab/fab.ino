// This #include statement was automatically added by the Particle IDE.
#include "httpsclient-particle/httpsclient-particle.h"

int bottomSensorPin = D5;
int topSensorPin = D6;
int sensorState = 0;
int previousSensorState = 0;
int photonId = 3;

const bool g_https_trace = true;  // This controls debug info print to Serial
const char host [] = "project.cmi.hr.nl";
const char endpoint0 [] = "/2015_2016/mtnll_mt2b_t1/api/sensor2.php?waterlevel=1&id=3";
const char endpoint1 [] = "/2015_2016/mtnll_mt2b_t1/api/sensor2.php?waterlevel=2&id=3";
const char endpoint2 [] = "/2015_2016/mtnll_mt2b_t1/api/sensor2.php?waterlevel=3&id=3";
const int g_port = 443; // DoNOT change this unless you know what's up
static unsigned int freemem;
bool g_https_complete;
uint32 g_bytes_received;

TCPClient client;

unsigned char httpRequestContent[] = "GET %s HTTP/1.0\r\n"
  "User-Agent: MatrixSSL/" MATRIXSSL_VERSION "\r\n"
  "Host: project.cmi.hr.nl\r\n"
  "Accept: */*\r\n"
  "Content-Type: applcation/json\r\n"
  "Content-Length: %d\r\n\r\n%s";

void setup() {
     if (g_https_trace) {
    Serial.begin(9600);
  }
    // initialize the sensor pin as an input:
    pinMode(bottomSensorPin, INPUT);
    // initialize the sensor pin as an input:
    pinMode(topSensorPin, INPUT);
    
}

int g_connected;

void loop() {
    delay(200);
    
    if (digitalRead(bottomSensorPin) == HIGH) {
        sensorState = 2;
    }
    else if (digitalRead(topSensorPin) == HIGH) {
        sensorState = 1;
    }
    else {
        sensorState = 0;
    }
    
    if (previousSensorState != sensorState) {
        // check if the sensor is active.
        // if it is, the sebsorstate is HIGH:
        if (sensorState == 2) {
            httpsclientSetup(host, endpoint2);
        } 
        else if (sensorState == 1) {
            httpsclientSetup(host, endpoint1);
        } 
        else if (sensorState == 0) {
            httpsclientSetup(host, endpoint0);
        }
        
        
        
        g_connected = client.connect(host, g_port);
        if (!g_connected) {
        client.stop();
        // If TCP Client can't connect to host, exit here.
        return;
        }
        g_https_complete = false;
        g_bytes_received = 0;
        if (g_https_trace) {
        freemem = System.freeMemory();
        Serial.print("free memory: ");
        Serial.println(freemem);
        }
        int32 rc;
        if ((rc = httpsClientConnection(httpRequestContent, 0, NULL) < 0)) {
        // TODO: When massive FAIL
        httpsclientCleanUp();
        } else {
        }
        client.stop();
        
        previousSensorState = sensorState;
        delay(5000);
    }
    
}