#include "OpenhabPub.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

Openhab_pub::Openhab_pub(){

    this->last_send = std::time(0);

}
Openhab_pub::~Openhab_pub(){

}
bool Openhab_pub::updatevalue(string topic, string key, float value){

    if (topic.compare("PM")==0){
        Buffer_PM[key]=value;
    }
    else if (topic.compare("BAT")==0){
        Buffer_BAT[key]=value;
    }
    else if (topic.compare("EMS")==0){
        Buffer_EMS[key]=value;
    }
    else if (topic.compare("PVI")==0){
        Buffer_PVI[key]=value;
    }
    else
    {
         printf("\n error parsing openhab topic");
         return 1;
    }
    
    return 0;
}
//17:17:11 MQT: A38/STH16_S/tele/SENSOR = {"Time":"2020-02-08T17:17:11","DS18B20-1":{"Id":"031197790115","Temperature":19.6},"DS18B20-2":{"Id":"031497793BB5","Temperature":19.5},"SI7021":{"Temperature":20.6,"Humidity":31.9},"TempUnit":"C"}

string map2json(map<string,float> Buffer){
    string json;
    json.append("\"{");
    map<string, float>::iterator it = Buffer.begin();
    int count = 1; 
        while(it != Buffer.end())
        {
            char lbuffer[50];
            if(count > 1){
                json.append(",");
            }
            sprintf(lbuffer,"\\\"%s\\\":%0.1f",it->first.c_str(),it->second);
            json.append(lbuffer);
            //printf("\n %s",lbuffer);
            count++;
            it++;
        }
    json.append("}\"");  
    return json;
}


bool Openhab_pub::cyclicsend(){

    std::time_t now = std::time(0);
    int diff = now - this->last_send;
    int returnvalue = 0;
    if(0 == (now % MQTT_SEND_INTERVAL) || (diff -1) > MQTT_SEND_INTERVAL){

        // mosquitto_pub -h 192.168.1.1 -p 1885 -t sensors/temperature -m "1266193804 32"

        char cbuf[300];
        string pub;
        stringstream ss;
        ss << "mosquitto_pub -r -h " << MQTT_BROKER_IP << " -p " << MQTT_BROKER_PORT << " -i " << MQTT_CLIENT <<" -t " << MQTT_TOPIC;
        pub = ss.str();

        //publish PM values
        string pm = pub;
        pm.append("PM -m ");
        pm.append(map2json(Buffer_PM));
        //printf("\n %s",pm.c_str());
        returnvalue += system(pm.c_str());
        
       //publish EMS values
        string ems = pub;
        ems.append("EMS -m ");
        ems.append(map2json(Buffer_EMS));
        //printf("\n %s",pm.c_str());
        returnvalue += system(ems.c_str());

        //publish BAT values
        string bat = pub;
        bat.append("BAT -m ");
        bat.append(map2json(Buffer_BAT));
        //printf("\n %s",pm.c_str());
        returnvalue += system(bat.c_str());

        //publish PVI values
        string pvi = pub;
        pvi.append("PVI -m ");
        pvi.append(map2json(Buffer_PVI));
        //printf("\n %s",pm.c_str());
        returnvalue += system(pvi.c_str());
    }   
    if(returnvalue == 0){
    //systemcall ohne Fehler, ansonsten retry
    this->last_send = now;
    }
    return returnvalue;
}