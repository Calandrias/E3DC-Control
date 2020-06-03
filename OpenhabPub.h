//
//  Restcall.h
//  RestApi call vor openhab
//
//  Created by Roland Biendl on 10.05.2020
//  Copyright © 2020 Roland Biendl. All rights reserved.
//
#ifndef OPENHAB_PUB
#define OPENHAB_PUB

#define MQTT_BROKER_IP "192.168.38.6"
#define MQTT_BROKER_PORT "1883"
#define MQTT_CLIENT "E3DC_Control"
#define MQTT_USER "e3dc"
#define MQTT_PWD ""
#define MQTT_TOPIC "A38/E3DC/"

#define MQTT_SEND_INTERVAL 10

#include <map>
#include <string>
#include <ctime>

using namespace std;

class Openhab_pub{

public:
    Openhab_pub ();

    virtual ~Openhab_pub();

    bool cyclicsend();

    bool updatevalue(string topic, string key, float value);

private:

    std::time_t last_send;
    map<string, float> Buffer_EMS;
    map<string, float> Buffer_PM;
    map<string, float> Buffer_BAT;
    map<string, float> Buffer_PVI;
};

#endif