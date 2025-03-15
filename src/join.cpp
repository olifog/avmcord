//
// Created by Oliver Fogelin on 15/03/2025.
//

#include "../include/join.h"
#include "dpp/dpp.h"

int join_user_vc (const dpp::slashcommand_t& event, bool send_replies) {
    dpp::guild* g = dpp::find_guild(event.command.guild_id);
    const dpp::voiceconn* current_vc = event.from()->get_voice(event.command.guild_id);
    const auto users_vc = g->voice_members.find(event.command.get_issuing_user().id);

    if (current_vc) {
        if (users_vc != g->voice_members.end() && current_vc->channel_id == users_vc->second.channel_id) {
            if (send_replies) event.reply("I'm already in your voice channel bozo.");
            return 2;
        } else {
            event.from()->disconnect_voice(event.command.guild_id);
        }
    }

    if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
        if (send_replies) event.reply("Joining failed. Are you sure you're in a voice channel?");
        return 1;
    }

    if (send_replies) event.reply("Joined your voice channel.");
    return 0;
}

