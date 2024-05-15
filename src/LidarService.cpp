#include <zephyr/bluetooth/gatt.h>

#include <zephyr/logging/log.h>

#include <stdexcept>
#include <sstream>
#include <vector>
#include <functional>

#include "LidarService.h"

LOG_MODULE_REGISTER(ble_lidar_service, LOG_LEVEL_INF);

static bool notifyEnabled = false;
static std::function<void(std::vector<uint8_t>)> configurationCallback;

static void CccConfigChanged(const struct bt_gatt_attr *attr, uint16_t value)
{
	notifyEnabled = (value == BT_GATT_CCC_NOTIFY);
}

static ssize_t WriteCofigurationCallback(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags){

    LOG_INF("Configuration was updated by the client\n");

    std::vector<uint8_t> byteData;
    byteData.resize(len);

    for(int i=0; i<len; i++){
        byteData[i] = ((uint8_t*)buf)[i];
    }

    if(configurationCallback) configurationCallback(byteData);

    return len;
}

/* Service Declaration */
BT_GATT_SERVICE_DEFINE(lidar_service, BT_GATT_PRIMARY_SERVICE(BT_UUID_LDS),
            BT_GATT_CHARACTERISTIC(BT_UUID_LDS_CHAR,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ, NULL, NULL, NULL),

            BT_GATT_CCC(CccConfigChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

            BT_GATT_CHARACTERISTIC(BT_UUID_LDS_CFG,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                        BT_GATT_PERM_WRITE, NULL, WriteCofigurationCallback, NULL), );            

void LidarService::Notify(std::vector<uint8_t> data){
    
    if (!notifyEnabled)
		throw std::runtime_error("Notify is not enabled");	

	int error = bt_gatt_notify(NULL, &lidar_service.attrs[2], &data[0], data.size()*sizeof(uint8_t));
    if(error != 0)
        LOG_ERR("Notify failed (Error %d)",-error);

}

void  LidarService::RegisterWrite(std::function<void(std::vector<uint8_t>)> callback){
    
    configurationCallback = callback;

}