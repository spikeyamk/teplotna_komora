/*
 * DHT.h
 *
 *  Created on: Jun 28, 2020
 *      Author: Controllerstech.com
 */

#ifndef DHT_H_
#define DHT_H_
#include <optional>


struct DHT_DataTypedef
{
	float Temperature;
	float Humidity;
};


std::optional<DHT_DataTypedef> DHT_GetData();

#endif /* INC_DHT_H_ */
