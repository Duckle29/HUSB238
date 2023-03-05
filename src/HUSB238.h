#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

extern TwoWire Wire; /**< Forward declarations of Wire for board/variant
                        combinations that don't have a default 'Wire' */

class HUSB238
{
  public:
	HUSB238(TwoWire* myWire = &Wire);

	enum pd_response {
		PD_NO_RESPONSE = 0,
		PD_SUCCESS = 1,
		PD_INVALID = 3,
		PD_CMD_NOT_SUPPORTED = 4,
		PD_FAIL = 5
	};

	enum husb_registers
	{
		REG_PD_STATUS0 	= 0x00,
		REG_PD_STATUS1 	= 0x01,
		REG_SRC_PDO_5V 	= 0x02,
		REG_SRC_PDO_9V 	= 0x03,
		REG_SRC_PDO_12V	= 0x04,
		REG_SRC_PDO_15V	= 0x05,
		REG_SRC_PDO_18V	= 0x06,
		REG_SRC_PDO_20V	= 0x07,
		REG_SRC_PDO		= 0x08,
		REG_GO_COMMAND 	= 0x09
	};

	enum voltage_select
	{
		PDO_5V	= 0b0001,
		PDO_9V	= 0b0010,
		PDO_12V	= 0b0011,
		PDO_15V	= 0b1000,
		PDO_18V	= 0b1001,
		PDO_20V	= 0b1010
	};

	struct __attribute__ ((packed))
	{
		uint16_t ma_5V;
		uint16_t ma_9V;
		uint16_t ma_12V;
		uint16_t ma_15V;
		uint16_t ma_18V;
		uint16_t ma_20V;
	} capability;

	union
	{
		struct 
		{
			unsigned PD_SRC_CURRENT	: 4;
			unsigned PD_SRC_VOLTAGE	: 4;

			unsigned CURRENT_5V		: 2;
			unsigned VOLTAGE_5V		: 1;
			unsigned PD_RESPONSE	: 3;
			unsigned ATTACH			: 1;
			unsigned CC_DIR			: 1;
		};

		struct 
		{
			uint8_t PD_STATUS0;
			uint8_t PD_STATUS1;
		};

		uint16_t combined;

	} status;
  	TwoWire* m_wire;
	uint64_t begin();
  	uint64_t get_capabilities();
  	void set_voltage(voltage_select voltage);
  	uint8_t update_pd_status();
  	uint8_t get_pd_voltage();
  	uint16_t get_pd_current();

  private:

	const uint8_t m_i2c_address = 0x08;
	const uint8_t m_capability_reg_count = 6;
	const uint16_t m_src_current[16] = {
		500,
		700,
		1000,
		1250,
		1500,
		1750,
		2000,
		2250,
		2500,
		2750,
		3000,
		3250,
		3500,
		4000,
		4500,
		5000
	};
	const uint8_t m_src_voltage[7] = {
		0,
		5,
		9,
		12,
		15,
		18,
		20
	};
	const uint16_t m_apple_current[4] = {
		500,
		1500,
		2400,
		3000
	};

	uint8_t m_read_byte_from_register(uint8_t register);
};
