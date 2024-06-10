#pragma once

#include <zephyr/types.h>
#include <vector>
#include <functional>

/** @brief Lidar Data Service UUID. */
#define BT_UUID_LDS_VAL BT_UUID_128_ENCODE(0x932ee1de, 0x547a, 0x4ef8, 0x93f7, 0x843ffa16afab)

/** @brief Lidar Data Characteristic UUID. */
#define BT_UUID_LDS_CHAR_VAL BT_UUID_128_ENCODE(0x932ee1df, 0x547a, 0x4ef8, 0x93f7, 0x843ffa16afab)

/** @brief Lidar Configuration Characteristic UUID. */
#define BT_UUID_LDS_CFG_VAL BT_UUID_128_ENCODE(0x932ee1e0, 0x547a, 0x4ef8, 0x93f7, 0x843ffa16afab)

#define BT_UUID_LDS BT_UUID_DECLARE_128(BT_UUID_LDS_VAL)
#define BT_UUID_LDS_CHAR BT_UUID_DECLARE_128(BT_UUID_LDS_CHAR_VAL)
#define BT_UUID_LDS_CFG BT_UUID_DECLARE_128(BT_UUID_LDS_CFG_VAL)

namespace LidarService{
        /** @brief Initialize the Lidar Data Service.
        *
        * This function allows to send notification with data
        *
        * @param[in] callback function to call when data is received
        * @throws std::runtime_error thrown when notify is not enabled 
        */
        void Notify(std::vector<uint8_t> data);
        void RegisterWrite(std::function<void(std::vector<uint8_t>)> callback);
};