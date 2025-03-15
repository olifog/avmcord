#include <iostream>
#include "env.h"
#include "string_hash.h"
#include "dpp/dpp.h"

int main()
{
    const std::string token = safe_getenv("DISCORD_API_TOKEN");
    dpp::cluster bot(token);

    uint8_t* recorded_audio = nullptr;
    size_t recorded_size = 0;
    FILE *fd = nullptr;

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand(
        [&bot, &fd, &recorded_audio, &recorded_size](const dpp::slashcommand_t& event)
        {
            const std::string command_name = event.command.get_command_name();
            switch (hash(command_name))
            {
                case "join"_hash:
                    break;
                case "leave"_hash:
                    break;
                case "record"_hash:
                    break;
                case "stop"_hash:
                    break;
                case "play"_hash:
                    break;
                default:
                    break;
            }
        }
    );

    bot.on_ready([&bot](const dpp::ready_t & event)
    {
        if (dpp::run_once<struct register_bot_commands>())
        {
            // TODO: create slash commands here
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}
