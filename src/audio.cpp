#include "audio.hpp"

void Audio::load(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filename)) {
        soundBuffers[name] = buffer;
        sounds[name].setBuffer(soundBuffers[name]);
        sounds[name].setLoop(true);
    } else {
        throw std::runtime_error("Failed to load sound effect: " + filename);
    }
}

void Audio::play(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    } else {
        throw std::runtime_error("Sound effect not found: " + name);
    }
}

void Audio::stop(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.stop();
    } else {
        throw std::runtime_error("Sound effect not found: " + name);
    }
}