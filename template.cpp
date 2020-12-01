#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<int>;

input_t read_input(std::string const& file_path) {
    input_t input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int a;
        if (!(iss >> a)) {
            break;
        } // error

        input_values.push_back(a);
    }
    return input_values;
}

ll solve1(input_t const& input_data) {
    return -1;
}

ll solve2(input_t const& input_data) {
    return -1;
}

int main() {
    auto const input_data = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){ std::cout << elem << ", "; });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}