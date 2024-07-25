#pragma once

#include <string>

/*
 *  Function in namespace for printing memory usage.
 *  It prints actual used RAM of this program in KB and MB.
*/
namespace MemoryUsage
{
	void printMemoryUsage(const std::string& name);
};
