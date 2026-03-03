#include "level/level_validation.h"

#include <cstdio>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {

using VisitGrid = std::vector<std::vector<bool>>;

bool isBlockedTile(char c, bool doorOpen)
{
    if (c == '1' || c == '2') return true;
    if (c == 'D') return !doorOpen;
    return false;
}

VisitGrid makeVisitGrid(const MapLoader& map)
{
    VisitGrid visited(map.getHeight());
    const auto& data = map.data();
    for (int z = 0; z < map.getHeight(); ++z) {
        visited[z].assign(data[z].size(), false);
    }
    return visited;
}

VisitGrid bfsReachable(const MapLoader& map, bool doorOpen)
{
    const auto& data = map.data();
    const int height = map.getHeight();
    VisitGrid visited = makeVisitGrid(map);

    const int spawnX = static_cast<int>(map.getPlayerStartX());
    const int spawnZ = static_cast<int>(map.getPlayerStartZ());
    if (spawnZ < 0 || spawnZ >= height || spawnX < 0 || spawnX >= static_cast<int>(data[spawnZ].size())) {
        std::printf("[ValidLevel] AVISO: spawn do jogador invalido no MapLoader!\n");
        return visited;
    }

    std::queue<std::pair<int, int>> q;
    q.push({spawnZ, spawnX});
    visited[spawnZ][spawnX] = true;

    const int dz[] = {-1, 1, 0, 0};
    const int dx[] = {0, 0, -1, 1};

    while (!q.empty()) {
        const auto [cz, cx] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            const int nz = cz + dz[i];
            const int nx = cx + dx[i];
            if (nz < 0 || nz >= height) continue;
            if (nx < 0 || nx >= static_cast<int>(data[nz].size())) continue;
            if (visited[nz][nx]) continue;
            if (isBlockedTile(data[nz][nx], doorOpen)) continue;

            visited[nz][nx] = true;
            q.push({nz, nx});
        }
    }

    return visited;
}

bool isVisited(const VisitGrid& visited, int x, int z)
{
    return z >= 0 &&
           z < static_cast<int>(visited.size()) &&
           x >= 0 &&
           x < static_cast<int>(visited[z].size()) &&
           visited[z][x];
}

bool canInteractWithDoor(const MapLoader& map, const VisitGrid& visited, int doorX, int doorZ)
{
    const int dz[] = {-1, 1, 0, 0};
    const int dx[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        const int nz = doorZ + dz[i];
        const int nx = doorX + dx[i];
        if (nz < 0 || nz >= map.getHeight()) continue;
        if (nx < 0 || nx >= static_cast<int>(map.data()[nz].size())) continue;
        if (isVisited(visited, nx, nz)) return true;
    }

    return false;
}

struct ValidationState {
    int batteriesTotal = 0;
    int batteriesReachableBeforeDoor = 0;
    int keysTotal = 0;
    int keysReachableBeforeDoor = 0;
    int healthTotal = 0;
    bool doorFound = false;
    bool doorReachableBeforeOpen = false;
};

void logTileStatus(const char* label, int x, int z, bool reachable, bool beforeDoor)
{
    if (reachable) {
        std::printf("[ValidLevel]   OK  %s em (%d,%d)%s\n",
                    label, x, z, beforeDoor ? " antes da porta" : "");
    } else {
        std::printf("[ValidLevel] AVISO: %s em (%d,%d) inatingivel%s!\n",
                    label, x, z, beforeDoor ? " antes da porta" : "");
    }
}

} // namespace

bool validateLevel(const Level& lvl, int levelNumber)
{
    const auto& map = lvl.map;
    const auto& data = map.data();
    const int height = map.getHeight();

    std::printf("[ValidLevel] Validando Level %d...\n", levelNumber);

    VisitGrid reachableBeforeDoor = bfsReachable(map, false);
    VisitGrid reachableAfterDoor = bfsReachable(map, true);

    ValidationState s;
    bool allOk = true;

    for (int z = 0; z < height; ++z) {
        const std::string& row = data[z];
        for (int x = 0; x < static_cast<int>(row.size()); ++x) {
            const char c = row[x];

            if (c == 'V') {
                ++s.batteriesTotal;
                const bool reachable = isVisited(reachableBeforeDoor, x, z);
                if (reachable) ++s.batteriesReachableBeforeDoor;
                logTileStatus("Bateria (V)", x, z, reachable, true);
                allOk = allOk && reachable;
            } else if (c == 'Y') {
                ++s.keysTotal;
                const bool reachable = isVisited(reachableBeforeDoor, x, z);
                if (reachable) ++s.keysReachableBeforeDoor;
                logTileStatus("Chave (Y)", x, z, reachable, true);
                allOk = allOk && reachable;
            } else if (c == 'H') {
                ++s.healthTotal;
                const bool reachable = isVisited(reachableAfterDoor, x, z);
                logTileStatus("Kit Saude (H)", x, z, reachable, false);
                allOk = allOk && reachable;
            } else if (c == 'D') {
                s.doorFound = true;
                s.doorReachableBeforeOpen = canInteractWithDoor(map, reachableBeforeDoor, x, z);
                if (s.doorReachableBeforeOpen) {
                    std::printf("[ValidLevel]   OK  Porta (D) interagivel em (%d,%d)\n", x, z);
                } else {
                    std::printf("[ValidLevel] AVISO: Porta (D) em (%d,%d) nao pode ser alcancada/interagida!\n", x, z);
                    allOk = false;
                }
            }
        }
    }

    if (!s.doorFound) {
        std::printf("[ValidLevel] AVISO: o level nao possui porta de saida (D).\n");
        allOk = false;
    }

    if (s.batteriesReachableBeforeDoor != s.batteriesTotal) {
        std::printf("[ValidLevel] AVISO: nem todas as baterias necessarias para abrir a porta sao alcancaveis antes da porta.\n");
        allOk = false;
    }

    if (s.keysTotal == 0) {
        std::printf("[ValidLevel] AVISO: o level nao possui chave (Y).\n");
        allOk = false;
    } else if (s.keysReachableBeforeDoor == 0) {
        std::printf("[ValidLevel] AVISO: nenhuma chave alcancavel antes da porta; o level nao pode ser concluido.\n");
        allOk = false;
    }

    if (allOk) {
        std::printf("[ValidLevel] Level %d: progressao valida (spawn -> itens obrigatorios -> porta). ✓\n", levelNumber);
    } else {
        std::printf("[ValidLevel] Level %d: progressao invalida. Verifique os avisos acima.\n", levelNumber);
    }

    return allOk;
}
