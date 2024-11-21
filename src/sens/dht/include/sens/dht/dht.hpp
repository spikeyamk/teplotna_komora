/*
 * DHT.h
 *
 *  Created on: Jun 28, 2020
 *      Author: Controllerstech.com
 */


#include <optional>

namespace sens {
namespace dht {
	struct DHT_DataTypedef {
		float Temperature;
		float Humidity;
	};

	std::optional<DHT_DataTypedef> get_data();
}
}