/*
 * I2C_orgue.c
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#include "I2C_orgue.h"
#include "i2c.h"
#include "syscon.h"



void init_I2C()
{
	// Peripheral reset to the GPIO0 and pin interrupt modules. '0' asserts, '1' deasserts reset.
	LPC_SYSCON->PRESETCTRL0 &=  (GPIO0_RST_N & GPIOINT_RST_N);
	LPC_SYSCON->PRESETCTRL0 |= ~(GPIO0_RST_N & GPIOINT_RST_N);

	LPC_SYSCON->SYSAHBCLKCTRL0 |= IOCON | GPIO_INT;

	MCP23_write_reg(MCP23_OLAT_REG, 0x00);
	MCP23_write_reg(MCP23_IODIR_REG,0xF0);
	MCP23_write_reg(MCP23_GPPU_REG,0xFF);


}

void MCP23_write_reg(uint8_t reg_num,uint8_t valeur)
{
	uint8_t I2CMasterBuffer[3]; // ad, #reg, valeur
	uint8_t I2CWriteLength=2;
	I2CMasterBuffer[0]=MCP23_I2C_AD;
	I2CMasterBuffer[1]=reg_num;
	I2CMasterBuffer[2]=valeur;
	I2CmasterWrite(I2CMasterBuffer, I2CWriteLength );
}

uint8_t MCP23_read_reg(uint8_t reg_num)
{
	uint8_t I2CMasterBuffer[2]; // ad, #reg
	uint8_t I2CSlaveBuffer[1];
	uint8_t I2CWriteLength=1;
	uint8_t I2CReadLength=1;
	I2CMasterBuffer[0]=MCP23_I2C_AD;
	I2CMasterBuffer[1]=reg_num;
	I2CmasterWriteRead( I2CMasterBuffer, I2CSlaveBuffer, I2CWriteLength, I2CReadLength );
	// autre possibilité :
//	I2CmasterWrite(I2CMasterBuffer, I2CWriteLength );
//	I2CmasterRead( MCP23_I2C_AD, I2CSlaveBuffer, I2CReadLength );
	return I2CSlaveBuffer[0];
}

void MCP23_write_OD(uint8_t valeur)
{
	MCP23_write_reg(MCP23_GPIO_REG,valeur);
	MCP23_write_reg(MCP23_OLAT_REG, 0x00);
}

uint8_t Ftouche()
{
	// détection d'un front d'appui sur touche
	static uint8_t etat=0;
	uint8_t lecture;
	MCP23_write_OD(0x0f);					//xxxx1111
	//while (millis < t0 + 5);
	lecture=MCP23_read_reg(MCP23_GPIO_REG);	//lecture des xxxx
	MCP23_write_reg(MCP23_IODIR_REG,0x0F);	//SSSSEEEE
	MCP23_write_OD(0xf0);					//1111xxxx
	//while (millis < t0 + 5);
	lecture|=MCP23_read_reg(MCP23_GPIO_REG);//Lecture des xxxx
	MCP23_write_reg(MCP23_IODIR_REG,0xF0); 	//EEEESSSS
	switch(etat)
	{
		case 0:
			if (lecture != 0xff) //On a un appuie
				etat = 1;
			break;

		case 1:
			if (lecture != 0xff) 	//On a pas relaché
				lecture = 0xff;
			else					//On a relaché
				etat = 0;
			break;
	}

	return lecture;
	// compléter ici
	//
}

uint8_t decode_touche(uint8_t code)
{
	const uint8_t Tcodes[]={0x77,0x7b,0x7d,0x7e,
							0xb7,0xbb,0xbd,0xbe,
							0xd7,0xdb,0xdd,0xde,// compléter ici
							0xe7,0xeb,0xed,0xee};
	const uint8_t Tascii[]={'1','2','3','A',
							'4','5','6','B',
							'7','8','9','C',
							'*','0','#','D'};
	uint8_t i=0;
	while ((i<16)&&(Tcodes[i]!=code)) {
		i++;
	}
	return (i<16)?Tascii[i]:'?';
}
