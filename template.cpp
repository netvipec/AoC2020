#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<ll>;
using result_t = ll;

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        ll a;
        if (!(iss >> a)) {
            break;
        } // error

        input_values.emplace_back(a);
    }
    return input_values;
}

result_t solve1(input_t const& input_data) {
    result_t ans = -1;
    // TODO: insert code
    return ans;
}

result_t solve2(input_t const& input_data) {
    result_t ans = -1;
    // TODO: insert code
    return ans;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}