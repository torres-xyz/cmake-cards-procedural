#include "audio.hpp"
#include <map>
#include <string>
#include "raylib-cpp.hpp"

void PlaySound(const GameSound sound)
{
    GetSoundFromLibrary(sound).Play();
}

raylib::Sound &GetSoundFromLibrary(GameSound sound)
{
    static const std::map<GameSound, std::string> gameSoundToPathMap
    {
        {GameSound::buttonPress01, "resources/audio/sounds/click_01.ogg"},
        {GameSound::cardPlace01, "resources/audio/sounds/card_place_01.ogg"},
    };
    static std::map<GameSound, raylib::Sound> gameSoundToSoundMap{};

    if (!gameSoundToSoundMap.contains(sound))
    {
        gameSoundToSoundMap.insert(
            {
                sound,
                raylib::Sound(gameSoundToPathMap.at(sound))
            });
    }
    return gameSoundToSoundMap.at(sound);
}


void PlayMusic(const GameMusic music)
{
    const raylib::Music &musicToPlay = GetMusicFromLibrary(music);
    if (!IsMusicStreamPlaying(musicToPlay))
    {
        PlayMusicStream(musicToPlay);
        return;
    }
    UpdateMusicStream(musicToPlay);
}

raylib::Music &GetMusicFromLibrary(GameMusic music)
{
    static const std::map<GameMusic, std::string> m_gameMusicToPathMap
    {
        {GameMusic::start, "resources/audio/music/homework.ogg"},
        {GameMusic::playing, "resources/audio/music/morning_walk.ogg"},
        {GameMusic::gameOver, "resources/audio/music/after.ogg"},
    };

    static std::map<GameMusic, raylib::Music> gameMusicToMusicMap{};

    if (!gameMusicToMusicMap.contains(music))
    {
        gameMusicToMusicMap.insert(
            {
                music,
                raylib::Music(m_gameMusicToPathMap.at(music))
            });
    }
    return gameMusicToMusicMap.at(music);
}
