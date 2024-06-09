#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <zephyr/logging/log.h>

#include <stdexcept>

#include "../hdr/Ble.h"
#include "../hdr/LidarService.h"

LOG_MODULE_REGISTER(ble, LOG_LEVEL_INF);

static const struct bt_data advertisingData[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data serviceData[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LDS_VAL),
};

static void Connected(struct bt_conn *conn, uint8_t err) {

	if (err) {
		LOG_ERR("Connection failed (err %u)\n", err);
		return;
	}

	LOG_INF("Connected\n");

}

static void Disconnected(struct bt_conn *conn, uint8_t reason){

	LOG_INF("Disconnected (reason %u)\n", reason);

}

BT_CONN_CB_DEFINE(connectionCallbacks) = {
	.connected = Connected,
	.disconnected = Disconnected,
};

struct bt_le_adv_param *advertisingParameters = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY), // connectable advertising and use identity address
    800, // min advertising interval 500ms (800*0.625ms)
    801, // max advertising interval 500.625ms (801*0.625ms)
    NULL);

void Ble::Initialize(){

    int error;

    error = bt_enable(NULL);
    if(error)
        throw std::runtime_error("BLE was not enabled");

    LOG_INF("Bluetooth enabled");      

    error = bt_le_adv_start(advertisingParameters, advertisingData, ARRAY_SIZE(advertisingData), serviceData, ARRAY_SIZE(serviceData));
    if(error)
        throw std::runtime_error("Advertising did not start");

    LOG_INF("Advertising started");

}

void Ble::Notify(std::vector<uint8_t> data, Characteristic characteristic){

    switch(characteristic){
        case LidarData:
            try{
                LidarService::Notify(data);
            }
            catch(std::runtime_error e){
                LOG_ERR("%s",e.what());
            }
            break;

        default:
            LOG_ERR("Characteristic notifcation not implemented");
            break;
    }

}

void Ble::RegisterCallback(std::function<void(std::vector<uint8_t>)> callback, Characteristic characteristic){

    LOG_INF("Registering callback"); 

    switch(characteristic){
        case LidarConfig:            
            LidarService::RegisterWrite(callback);            
            break;

        default:
            LOG_ERR("Characteristic callback not implemented");
            break;
    }

}