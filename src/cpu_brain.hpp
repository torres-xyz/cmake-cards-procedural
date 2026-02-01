#pragma once

struct Player;

struct CpuPlayerOptions
{
    float actionDelay{0.25f};
    bool autoStartNewRound{false};
};

void RunCpuBrain(Player &player, const CpuPlayerOptions &options);
