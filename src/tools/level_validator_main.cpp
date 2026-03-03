#include "core/config.h"
#include "level/level.h"
#include "level/level_validation.h"

#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv)
{
    if (argc < 3 || ((argc - 1) % 2) != 0) {
        std::fprintf(stderr, "Usage: %s <map-path> <level-number> [<map-path> <level-number> ...]\n", argv[0]);
        return 2;
    }

    bool allOk = true;

    for (int i = 1; i < argc; i += 2) {
        const char* mapPath = argv[i];
        const int levelNumber = std::atoi(argv[i + 1]);

        Level lvl;
        lvl.currentLevel = levelNumber;

        if (!loadLevel(lvl, mapPath, GameConfig::TILE_SIZE)) {
            std::fprintf(stderr, "[ValidLevel] ERRO: falha ao carregar mapa %s\n", mapPath);
            allOk = false;
            continue;
        }

        if (!validateLevel(lvl, levelNumber)) {
            allOk = false;
        }
    }

    return allOk ? 0 : 1;
}
