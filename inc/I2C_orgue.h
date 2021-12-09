/*
 * I2C_orgue.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_I2C_ORGUE_H_
#define INC_I2C_ORGUE_H_

#include "stdint.h"

#define MCP23_I2C_AD	0x40
#define MCP23_IODIR_REG		0
#define MCP23_IPOL_REG		1
#define MCP23_GPINTEN_REG	2
#define MCP23_DEFVAL_REG	3
#define MCP23_INTCON_REG	4
#define MCP23_IOCON_REG		5
#define MCP23_GPPU_REG		6
#define MCP23_INTF_REG		7
#define MCP23_INTCAP_REG	8
#define MCP23_GPIO_REG		9
#define MCP23_OLAT_REG		10

void init_I2C();
void MCP23_write_reg(uint8_t reg_num,uint8_t valeur);
uint8_t MCP23_read_reg(uint8_t reg_num);
void MCP23_write_OD(uint8_t valeur);
uint8_t Ftouche();
uint8_t decode_touche(uint8_t code);

#endif /* INC_I2C_ORGUE_H_ */
