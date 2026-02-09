


#include "redis_exec_subscriber.hpp"
#include <hiredis/hiredis.h> 
#include <iostream> 
#include "executor.hpp"
#include "execmsg.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept> 

using json = nlohmann::json;

void execute_from_redis_queue(const std::string& sandbox_cfg_path, const std::string& ip_address, uint16_t port)
{
    redisContext* ctx = redisConnect(ip_address.c_str(), port);
    if (ctx->err)
    {
        throw std::runtime_error("Could not connect to Redis server on IP address " + ip_address + " on port " + std::to_string(port)
                                 + ": " + ctx->errstr);
    }

    Executor executor(sandbox_cfg_path);

    while (true)
    {
        redisReply* reply = (redisReply*)redisCommand(ctx, "BLPOP exec 0");  
        if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 2)
        { 
            json message = json::parse(reply->element[1]->str);  

            std::vector<ExecutionResult> results = executor.execute(execmsg::message_to_job(message));

            json response = execmsg::results_to_response(message, results);

#ifdef DEBUG_BUILD
            std::cout << "Pushing response " << response.dump(4) << std::endl;
#endif

            redisReply* push_reply = (redisReply*)redisCommand(ctx, "LPUSH exec_result %s", response.dump().c_str());
            freeReplyObject(push_reply);
        }

        freeReplyObject(reply);
    }
}






