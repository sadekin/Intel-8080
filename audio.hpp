#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <map>

class Audio {
public:
    void load(const std::string& name, const std::string& filename);
    void play(const std::string& name);
    void stop(const std::string& name);

private:
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
};