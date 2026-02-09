
#include "redis_exec_subscriber.hpp"
#include <iostream> 

int main(int argc, char** argv)
{

    std::string sandbox_path = argc < 2 ? "../QuackTheCode_executor/config/sandbox.cfg" : argv[1];

    std::cout << "REDIS Execution Worker running" << std::endl;

    execute_from_redis_queue(sandbox_path, "localhost", 6379);

    return 0; 
}


