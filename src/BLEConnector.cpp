#include "../hdr/BLEConnector.h"

LOG_MODULE_REGISTER(blelog, LOG_LEVEL_INF);

int err;
std::stringstream errMessage;

bt_conn *BLEConnector::m_connection = NULL;
bool BLEConnector::m_alreadyInitialized = false;
bool BLEConnector::notificationEnabled = false;

bt_le_adv_param *defaultAdvertismentParameters = BT_LE_ADV_PARAM(
		(BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY), // connectable advertising and use identity address
		800, // min advertising interval 500ms (800*0.625ms)
		801, // max advertising interval 500.625ms (801*0.625ms)
		NULL); // undirected advertising

void lidarServiceConfigurationChanged(const struct bt_gatt_attr *attr, uint16_t value)
{
	BLEConnector::notificationEnabled = (value == BT_GATT_CCC_NOTIFY);
}

static struct bt_conn_cb connectionCallbacks; 

BLEConnector::BLEConnector(){

	connectionCallbacks.connected = onConnect;
	connectionCallbacks.disconnected = onConnect;

	bt_conn_cb_register(&connectionCallbacks);

	m_advertisementParams = defaultAdvertismentParameters;

	errMessage.str(std::string());

	if(!m_alreadyInitialized){

		m_alreadyInitialized = true;

		err = bt_enable(NULL);
		if (err) {
			errMessage <<"Bluetooth initialization failed (error %d)\n" <<err <<std::endl;
			LOG_ERR("%s",errMessage.str().c_str());
			throw new std::logic_error(errMessage.str());
		}

		LOG_INF("Bluetooth initialized\n");
	}   

	err = bt_le_adv_start(m_advertisementParams, advertisingData, ARRAY_SIZE(advertisingData), scanResponseData, ARRAY_SIZE(scanResponseData));
	if (err) {
		errMessage <<"Advertising failed to start (error " <<err <<")" <<std::endl; 
		LOG_ERR("%s",errMessage.str().c_str());
		throw new std::logic_error(errMessage.str());
	}

    LOG_INF("Advertising\n");	

}

void BLEConnector::onConnect(struct bt_conn *connection, uint8_t error){
	unsigned int key = irq_lock();

	if(m_connection == NULL)
		LOG_INF("BLE connection established");  
	else
		LOG_INF("BLE reconnected");  

	m_connection = bt_conn_ref(connection);	

	if (error) {
        LOG_ERR("Connection error %d", err);
        return;
    }
	
	irq_unlock(key);
}

void BLEConnector::onDisconnect(struct bt_conn *conn, uint8_t reason){		
	unsigned int key = irq_lock();

	switch(reason){
		case 0x13:
			LOG_INF("Disconnected: remote termination\n");  
			break;
		case 0x8:
			LOG_ERR("Disconnected: timeout\n");
			break;
		default:
			LOG_ERR("Disconnected: reason %d\n",reason);  
			break;
	}

	m_connection = NULL;

	irq_unlock(key);
}

bool BLEConnector::IsConnected(){

	return BLEConnector::m_connection != NULL;

}

bool BLEConnector::IsNotifyEnabled(){

	return notificationEnabled;

}