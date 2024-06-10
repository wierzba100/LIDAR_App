#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "../hdr/servo.hpp"
#include "../hdr/lidar.hpp"
#include "../hdr/Ble.h"

#include <vector>
#include <string.h>
#include <string>
#include <sstream>
#include <memory>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static volatile bool isConnected = false;

std::vector<uint8_t> StructToByteVector(void *structPointer, size_t sizeOfStruct)
{
	std::vector<uint8_t> returnVector;
	returnVector.resize(sizeOfStruct);
	memcpy(&returnVector[0],structPointer,sizeOfStruct); 
	
	return returnVector;
}

const std::vector<uint8_t> vector_to_be_received = {10, 11, 12, 13, 14, 15, 16, 17};

void configCallback(std::vector<uint8_t> data)
{        

	if(data.size() != 8){
			LOG_ERR("Wrong data size\n");
			return;
	}

	for (size_t i = 0; i < vector_to_be_received.size(); ++i)
	{
		if (vector_to_be_received[i] != data[i])
		{
			LOG_INF("Device Parred Incorrectly");
			return;
		}
		LOG_INF("Byte received: %d", data[i]);
	}

	LOG_INF("Device Parred Correctly");
	isConnected = true;
}

int main(void)
{
	pwm_dt_spec pwm_servo_x = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_x));
	std::unique_ptr<Servo> servo_x = std::make_unique<Servo>(pwm_servo_x);

	pwm_dt_spec pwm_servo_y = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_y));
	std::unique_ptr<Servo> servo_y = std::make_unique<Servo>(pwm_servo_y);

	const struct device *uart_device = DEVICE_DT_GET(DT_NODELABEL(uart1));
	std::unique_ptr<Lidar> lidar = std::make_unique<Lidar>(uart_device);

	servo_x->Callib();
	servo_y->GoTo(45);

	Ble::Initialize();
	std::function<void(std::vector<uint8_t>)> fun(configCallback);
	Ble::RegisterCallback(fun);

	struct dataToSend {
			uint8_t bytes[4] = {0,0,0,0};
	} data;

	k_msleep(1000);
	std::vector<uint8_t> vdata = StructToByteVector(&data, sizeof(dataToSend));

	std::stringstream msg;
	for(auto val : vdata){
			msg >>val;
	}

	uint16_t distance = 0, strength = 0;

	while(true)
	{
		while(isConnected)
		{	
			for(size_t i = 0; i < SERVO_Y_RANGE; i++)
			{
				for(size_t j = 0; j < SERVO_X_RANGE; j++)
				{
					if(lidar->read_data(&distance, &strength))
					{
						LOG_INF("Distance: %d cm\n", distance);
						LOG_INF("Strength: %d \n", strength);

						data.bytes[0] = (distance >> 8) & 0xFF;
						data.bytes[1] = distance & 0xFF;
						data.bytes[2] = (strength >> 8) & 0xFF;
						data.bytes[3] = strength & 0xFF;

						vdata = StructToByteVector(&data, sizeof(dataToSend));
						for(auto val : vdata){
							msg>>val;
						}

						servo_x->StepLeft();

						Ble::Notify(vdata);
						LOG_INF("Sending bytes...");
					}
					k_msleep(500);
				}
				servo_x->Callib();
				servo_y->StepLeft();
			}
			isConnected = false;
		}
		k_msleep(1000);
	}
}
