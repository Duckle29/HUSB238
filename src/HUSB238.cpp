#include "HUSB238.h"

HUSB238::HUSB238(TwoWire* myWire = &Wire)
{
    m_wire = myWire;
}

uint64_t HUSB238::begin()
{
    m_wire->begin();
    return get_capabilities();
}

uint64_t HUSB238::get_capabilities()
{
    uint64_t return_codes = 0;

    for(int i=0; i<m_capability_reg_count; i++)
    {
        // As the capability struct is packed together, we can iterate over them.
        uint16_t* capability_address = &capability.ma_5V + i;
        // As the pdo registers are consecutive, we can iterate through them
        uint8_t pdo_register = REG_SRC_PDO_5V + i;

        m_wire->beginTransmission(m_i2c_address);
        uint8_t response = m_read_byte_from_register(pdo_register);
        return_codes |= (m_wire->endTransmission() << i*8);
        
        if (!bitRead(response, 7))
        {
            // PSU doesn't support this voltage
            *capability_address = 0;
        }
        else
        {
            // Voltage is supported at the given current
            uint8_t current_code = response & 0x0F;
            *capability_address = m_src_current[current_code];
        }

        
    }
    return return_codes;
}

void HUSB238::set_voltage(voltage_select voltage)
{
    m_wire->beginTransmission(m_i2c_address);
    m_wire->write(REG_SRC_PDO);
    m_wire->write(voltage);
    m_wire->endTransmission();

    m_wire->beginTransmission(m_i2c_address);
    m_wire->write(REG_GO_COMMAND);
    m_wire->write(1);
    m_wire->endTransmission();
}

uint8_t HUSB238::update_pd_status()
{
    m_wire->beginTransmission(m_i2c_address);
    status.PD_STATUS0 = m_read_byte_from_register(REG_PD_STATUS0);
    status.PD_STATUS1 = m_read_byte_from_register(REG_PD_STATUS0);
    return m_wire->endTransmission();
}

uint8_t HUSB238::get_pd_voltage()
{
    update_pd_status();
    return m_src_voltage[status.PD_SRC_VOLTAGE];
}

uint16_t HUSB238::get_pd_current()
{
    update_pd_status();

    if (status.VOLTAGE_5V)
    {
        return m_apple_current[status.CURRENT_5V];
    }
    else
    {
        return m_src_current[status.PD_SRC_CURRENT];
    }
}


uint8_t HUSB238::m_read_byte_from_register(uint8_t device_register)
{
    m_wire->write(device_register);
    m_wire->requestFrom(m_i2c_address, 1);
    uint8_t response = m_wire->read();
    return response;
}