#pragma once

#include <functional>
#include <vector>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

namespace Ble {

    enum Characteristic{
        LidarData,
        LidarConfig
    };

    /** @brief Initialize BLE.  *
    *     
    * @throws std::runtime_error thrown when blutooth failed to enable
    * or advertising couldn't start 
    */
    void Initialize();

    /** @brief Send BLE notification to client
    *     
    * @param[in] data vector with bytes to send
    * @param[in] service service with
    */
    void Notify(std::vector<uint8_t> data, Characteristic characteristic = LidarData);

    /** @brief Register a callback function to receive configuration data
    *     
    * @param[in] callback function that will be called when configuration
    * data is received
    */
    void RegisterCallback(std::function<void(std::vector<uint8_t>)> callback, Characteristic characteristic = LidarConfig);
    
}