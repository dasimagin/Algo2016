#include<algorithm>
#include<iostream>
#include<vector>


enum class Side {
    Left = 1,
    Right = 2,
};

bool operator<(Side lhs, Side rhs) {
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}

struct Board {
    size_t number;
    Side side;

    Board(size_t number, Side side): number(number), side(side) {}
};

bool operator<(const Board& lhs, const Board& rhs) {
    return (lhs.number < rhs.number) || (lhs.number == rhs.number && lhs.side < rhs.side);
}

std::vector<Board> getBoards(std::istream& in) {
    size_t size;
    in >> size;

    std::vector<Board> boards;
    boards.reserve(size);
    for (size_t number; size; --size) {
        in >> number;
        boards.emplace_back(number, Side::Left);
        in >> number;
        boards.emplace_back(number, Side::Right);
    }
    return boards;
}


int main() {
    std::vector<Board> boards = getBoards(std::cin);

    if (boards.empty()) {
        std::cout << 0 << std::endl;
        return 0;
    }

    std::sort(boards.begin(), boards.end());

    size_t result = 1;
    size_t counter = 1;
    for (size_t i = 1; i < boards.size(); ++i) {
        if (counter == 0) {
            ++result;
        }
        if (counter > 0) {
            result += boards[i].number - boards[i - 1].number;
        }
        if (boards[i].side == Side::Left) {
            ++counter;
        }
        if (boards[i].side == Side::Right) {
            --counter;
        }
    }

    std::cout << result << std::endl;

    return 0;
}
