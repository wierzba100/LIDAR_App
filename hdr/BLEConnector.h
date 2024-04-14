#pragma once

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/logging/log.h>
#include <stdexcept>
#include <string>
#include <sstream>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

// Randomly generated UUID for the service (via https://www.uuidgenerator.net/version4)
// 932ee1de-547a-4ef8-93f7-843ffa16afab
#define BT_UUID_LIDAR_SERVICE_VAL BT_UUID_128_ENCODE(0x932ee1de, 0x547a, 0x4ef8, 0x93f7, 0x843ffa16afab)
#define BT_UUID_LIDAR_SERVICE BT_UUID_DECLARE_128(BT_UUID_LIDAR_SERVICE_VAL)
#define BT_UUID_LIDAR_MEASUREMENT_CHARACTERISTIC_VAL BT_UUID_128_ENCODE(0x932ee1df, 0x547a, 0x4ef8, 0x93f7, 0x843ffa16afab)
#define BT_UUID_LIDAR_MEASUREMENT_CHARACTERISTIC BT_UUID_DECLARE_128(BT_UUID_LIDAR_MEASUREMENT_CHARACTERISTIC_VAL)

class BLEConnector{

private:
    static bt_conn *m_connection;
    static bool m_alreadyInitialized;
    struct bt_le_adv_param *m_advertisementParams;       
    static bool notificationEnabled;

public:
    BLEConnector();    

    // Data transfer functions    
    template<typename T> void Notify(T data);

    // Check functions
    bool IsConnected();
    bool IsNotifyEnabled();
    
    // Callbacks
    static void onConnect(bt_conn *connection, uint8_t error);
    static void onDisconnect(bt_conn *conn, uint8_t reason); 

    friend void lidarServiceConfigurationChanged(const struct bt_gatt_attr *attr, uint16_t value);

};

void lidarServiceConfigurationChanged(const struct bt_gatt_attr *attr, uint16_t value);

static const struct bt_data advertisingData[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data scanResponseData[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LIDAR_SERVICE_VAL),
};

BT_GATT_SERVICE_DEFINE(
	lidarService, BT_GATT_PRIMARY_SERVICE(BT_UUID_LIDAR_SERVICE),
	
	BT_GATT_CHARACTERISTIC(BT_UUID_LIDAR_MEASUREMENT_CHARACTERISTIC,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL,
			       NULL),

	BT_GATT_CCC(lidarServiceConfigurationChanged,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

);

template<typename T>
void BLEConnector::Notify(T data)
{
	std::stringstream errMessage;
    int err;

	if (!notificationEnabled) {
		errMessage <<"Notification not enabled (error " <<EACCES <<")" <<std::endl; 
		//LOG_ERR("%s",errMessage.str().c_str());
		throw new std::logic_error(errMessage.str());		
	}

	err = bt_gatt_notify(NULL, &lidarService.attrs[1], 
			      &data,
			      sizeof(T));
	if(err){
		errMessage <<"Notification error (error " <<err <<")" <<std::endl; 
		//LOG_ERR("%s",errMessage.str().c_str());
		throw new std::logic_error(errMessage.str());
	}
}