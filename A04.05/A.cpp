#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>


typedef std::vector<std::string> Field;

Field readField(std::istream& in) {
    size_t height, width;
    in >> height >> width;
    Field field(height);
    for (auto& row : field) {
        in >> row;
    }
    size_t startN;
    in >> startN;
    for (size_t x, y; startN; --startN) {
        in >> x >> y;
        field[x - 1][y - 1] = 's';
    }
    return field;
}


struct Ceil {
    int x, y;
    operator bool() const {
        return !(x == -1 && y == -1);
    }
    Ceil(): x(-1), y(-1) {}
    Ceil(int x, int y): x(x), y(y) {}
};

typedef std::vector<Ceil> Path;


Ceil findGift(const Field& field) {
    for (int x = 0; x < field.size(); ++x) {
        for (int y = 0; y < field[x].size(); ++y) {
            if (field[x][y] == '*') {
                return {x, y};
            }
        }
    }
    return {0, 0};
}

Path findNearestStart(const Field& field) {
    const size_t height = field.size();
    const size_t width = field.front().size();
    const Ceil from = findGift(field);
    std::queue<Ceil> queue;
    std::vector<std::vector<Ceil>> previous(height, std::vector<Ceil>(width));
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width));
    for (queue.push(from); !queue.empty(); queue.pop()) {
        const int x queue.front().x;
        const int y queue.front().y;

        if (field[x][y] == 's') {
            Path path;
            for (auto p = ceil; p; p = previous[p.x][p.y]) {
                path.push_back(p);
            }
            return path;
        }
        visited[x][y] = true;
        if (x > 0 && !visited[x - 1][y] && field[x - 1][y] != '1') {
            queue.emplace(x - 1, y);
            previous[x - 1][y] = ceil;
        }
        if (x + 1 < height && !visited[x + 1][y] && field[x + 1][y] != '1') {
            queue.emplace(x + 1, y);
            previous[x + 1][y] = ceil;
        }
        if (y > 0 && !visited[x][y - 1] && field[x ][y - 1] != '1') {
            queue.emplace(x, y - 1);
            previous[x][y - 1] = ceil;
        }
        if (y + 1 < width && !visited[x][y + 1] && field[x][y + 1] != '1') {
            queue.emplace(x, y + 1);
            previous[x][y + 1] = ceil;
        }
    }
    return {};
}

int main() {
    const Field field = readField(std::cin);
    const auto path = findNearestStart(field);
    if (path.empty()) {
        std::cout << -1 << std::endl;
    } else {
        std::cout << path.size() << std::endl;
        for (const auto& ceil : path) {
            std::cout << (ceil.x + 1) << " " << (ceil.y + 1) << std::endl;
        }
    }
    return 0;
}
