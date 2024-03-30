#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Skin {
private:
    int skinId;
    string skinName;
    Sprite skinSprite;
    IntRect frameRect;
    bool unlocked;
    static Texture skinTextures;
public:
    Skin(int skinId, string skinName, IntRect zone, bool unlocked) {
        this->skinId = skinId;
        this->skinName = skinName;
        frameRect = zone;
        skinTextures.loadFromFile("skins.png");
        skinSprite.setTexture(skinTextures);
        skinSprite.setTextureRect(frameRect);
        this->unlocked = unlocked;
    }

    int getSkinId() {
        return skinId;
    }
    string getSkinName() {
        return skinName;
    }
    Sprite& getSkinSprite() {
        return skinSprite;
    }
    bool isUnlocked() {
        return unlocked;
    }

    void unlock() {
        unlocked = true;
    }
};
Texture Skin::skinTextures;

class Player {
private:
    Sprite playerSprite;
    Texture skinTextures;
public:
    Player() {
        skinTextures.loadFromFile("skins.png");
        playerSprite.setTexture(skinTextures);
        playerSprite.scale(4, 4);;
    }

    void setSkin(int selectedSkin, vector<Skin>& skins) {
        for (auto& skin : skins) {
            if (skin.getSkinId() == selectedSkin) {
                if (skin.isUnlocked()) {
                    playerSprite.setTextureRect(skin.getSkinSprite().getTextureRect());
                    break;
                }
                else playerSprite.setTextureRect(skins[0].getSkinSprite().getTextureRect());
            }
        }
    }

    void show(RenderWindow& window) {
        window.draw(playerSprite);
    }
};

vector<Skin> loadSkins(string filename) {
    vector<Skin> skins;

    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        int skinId;
        string skinName;
        IntRect zone;

        int startPos = 0;
        for (int i = 0; i < 5; i++) {
            int endPos = line.find(';', startPos);
            string token = line.substr(startPos, endPos - startPos);
            switch (i) {
            case 0:
                skinId = stoi(token);
                break;
            case 1:
                skinName = token;
                break;
            case 2:
                zone.left = stoi(token);
                break;
            case 3:
                zone.top = stoi(token);
                break;
            case 4:
                zone.width = stoi(token);
                break;
            }
            startPos = endPos + 1;
        }

        zone.height = stoi(line.substr(startPos));
        skins.emplace_back(Skin(skinId, skinName, zone, false));
    }
    file.close();

    return skins;
}

int main() {
    RenderWindow window(VideoMode(600, 600), "Skins!");
    Player player;
    vector<Skin> skins = loadSkins("skins.txt");

    for (auto& skin : skins) {
        if (skin.getSkinId() == 0) {
            skin.unlock();
            break;
        }
    }

    player.setSkin(1, skins);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        player.show(window);
        window.display();
    }
}