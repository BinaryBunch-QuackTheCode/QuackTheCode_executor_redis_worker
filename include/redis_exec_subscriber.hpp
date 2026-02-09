

#pragma once

#include <string> 
#include <stdint.h> 

void execute_from_redis_queue(const std::string& sandbox_cfg_path, const std::string& ip_address, uint16_t port);
