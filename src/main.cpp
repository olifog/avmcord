#include <iostream>
#include "env.h"
#include "string_hash.h"
#include "dpp/dpp.h"

#include "join.h"

int main()
{
    const std::string token = safe_getenv("DISCORD_API_TOKEN");
    dpp::cluster bot(token);

    uint8_t* recorded_audio = nullptr;
    size_t recorded_size = 0;
    FILE* fd = nullptr;

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand(
        [&bot, &fd, &recorded_audio, &recorded_size](const dpp::slashcommand_t& event)
        {
            const std::string command_name = event.command.get_command_name();
            switch (hash(command_name))
            {
            case "join"_hash:
                {
                    join_user_vc(event);
                    break;
                }
            case "leave"_hash:
                {
                    if (event.from()->get_voice(event.command.guild_id))
                    {
                        event.from()->disconnect_voice(event.command.guild_id);
                        event.reply("Left your voice channel.");
                    }
                    else
                    {
                        event.reply("I'm not in a voice channel.");
                    }
                }
                break;
            case "record"_hash:
                {
                    join_user_vc(event, false);
                    fd = fopen("recording.pcm", "wb");
                    if (fd == nullptr)
                    {
                        event.reply("Failed to open file for recording.");
                    }
                    else
                    {
                        event.reply("Recording...");
                    }
                }
                break;
            case "stop"_hash:
                if (fd != nullptr)
                {
                    fclose(fd);
                    fd = nullptr;
                }

                if (recorded_audio)
                {
                    delete[] recorded_audio;
                    recorded_audio = nullptr;
                    recorded_size = 0;
                }

                if (std::ifstream input("recording.pcm", std::ios::in | std::ios::binary | std::ios::ate); input.
                    is_open())
                {
                    recorded_size = input.tellg();
                    recorded_audio = new uint8_t[recorded_size];
                    input.seekg(0, std::ios::beg);
                    input.read(reinterpret_cast<char*>(recorded_audio), recorded_size);
                    input.close();
                }

                event.reply("Stopped recording.");

                break;
            case "play"_hash:
                {
                    bool exit = false;
                    switch (join_user_vc(event, false))
                    {
                    case 0:
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        }
                        break;
                    case 1:
                        {
                            event.reply("Joining failed. Are you sure you're in a voice channel?");
                            exit = true;
                        }
                        break;
                    case 2:
                    default:
                        break;
                    }
                    if (exit) break;

                    const auto v= event.from()->get_voice(event.command.guild_id);
                    if (!v || !v->voiceclient || !v->voiceclient->is_ready())
                    {
                        event.reply("There was an issue getting the voice client.");
                        break;
                    }

                    v->voiceclient->send_audio_raw(
                        reinterpret_cast<uint16_t*>(recorded_audio),
                        recorded_size);
                    event.reply("Playing audio.");
                }
                break;
            default:
                break;
            }
        }
    );

    bot.on_voice_receive([&bot, &fd](const dpp::voice_receive_t& event)
    {
        if (fd != nullptr)
        {
            fwrite((char*)event.audio, 1, event.audio_size, fd);
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event)
    {
        if (dpp::run_once<struct register_bot_commands>())
        {
            dpp::slashcommand join_command("join", "Joins your voice channel.", bot.me.id);
            dpp::slashcommand leave_command("leave", "Leaves your voice channel.", bot.me.id);
            dpp::slashcommand record_command("record", "Records audio from your voice channel.", bot.me.id);
            dpp::slashcommand stop_command("stop", "Stops recording.", bot.me.id);
            dpp::slashcommand play_command("play", "Plays the recorded audio.", bot.me.id);

            bot.global_bulk_command_create({join_command, leave_command, record_command, stop_command, play_command});
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}
