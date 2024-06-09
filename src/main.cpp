#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <memory>
#include "../hdr/servo.hpp"
#include "../hdr/lidar.hpp"
#include "../hdr/Ble.h"

#include <vector>
#include <string.h>
#include <string>
#include <sstream>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

struct configData {        
        uint8_t data[8];
};

struct configData cfg; //= {0.0,0};

std::vector<uint8_t> StructToByteVector(void *structPointer, size_t sizeOfStruct){

        std::vector<uint8_t> returnVector;
        returnVector.resize(sizeOfStruct);
        memcpy(&returnVector[0],structPointer,sizeOfStruct); 

        LOG_INF("Size of struct: %zu %d",sizeOfStruct,returnVector.size()); 
        
        return returnVector;

}

void configCallback(std::vector<uint8_t> data){
        
        if(data.size() != 8){
                LOG_ERR("Wrong data size\n");
                return;
        }
/*
        memcpy(&cfg,&data[0],data.size());
        LOG_INF("Config received: %f %d",cfg.db_data,cfg.int16_data); */

        for(auto val : data)
                LOG_INF("Bytes received: %d",val);
}

int main(void)
{
        Ble::Initialize();
        std::function<void(std::vector<uint8_t>)> fun(configCallback);
        Ble::RegisterCallback(fun);

        struct dataToSend {
                uint8_t bytes[8] = {1,2,3,4,5,6,7,8};
        } data;

        k_msleep(1000);
        std::vector<uint8_t> vdata = StructToByteVector(&data, sizeof(dataToSend));

        LOG_INF("Size of struct: %d",vdata.size()); 

        std::stringstream msg;
        for(auto val : vdata){
                msg >>val;
        }

        //vdata.push_back(data.val);

        while(true){
                Ble::Notify(vdata);
                //vdata[0]++;
                //LOG_INF("Sending %u",vdata[0]); 
                LOG_INF("Sending bytes..."); 
                //LOG_INF("Config data: %lf %d",cfg.db_data,cfg.int16_data); 
                k_msleep(1000);
        }
}