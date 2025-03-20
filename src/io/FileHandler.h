#pragma once

#include <windows.h>
#include <shobjidl.h>
#include <string>

#include <json.hpp>
#include <fstream>
#include <iostream>

namespace FileHandler {

	void SaveFileJSON(nlohmann::json json);
	nlohmann::json LoadFileJSON();
}