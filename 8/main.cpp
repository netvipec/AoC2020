#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::pair<std::string, ll>>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string instruction;
        char sign;
        ll number;
        if (!(iss >> instruction >> sign >> number)) {
            break;
        } // error

        // std::cout << instruction << " " << sign << number << std::endl;
        if (sign == '-') {
            number = -number;
        }
        input_values.emplace_back(instruction, number);
    }
    return input_values;
}

result_t solve1(input_t const& input_data) {
    std::set<ll> inst_id;
    ll ip = 0;
    ll acc = 0;
    for (;;) {
        auto const it = inst_id.find(ip);
        if (it != std::cend(inst_id)) {
            break;
        }

        inst_id.insert(ip);

        if (input_data[ip].first == "acc") {
            acc += input_data[ip].second;
        } else if (input_data[ip].first == "jmp") {
            ip += input_data[ip].second;
            continue;
        }

        ip++;
    }

    return acc;
}

std::pair<ll, bool> run(input_t const& input_data) {
    std::set<ll> inst_id;
    ll ip = 0;
    ll acc = 0;
    bool finilize = false;
    for (; ip < input_data.size();) {
        auto const it = inst_id.find(ip);
        if (it != std::cend(inst_id)) {
            break;
        }

        inst_id.insert(ip);

        if (input_data[ip].first == "acc") {
            acc += input_data[ip].second;
        } else if (input_data[ip].first == "jmp") {
            ip += input_data[ip].second;
            continue;
        }

        ip++;
    }

    return std::make_pair(acc, ip == input_data.size());
}

result_t solve2(input_t const& input_data) {
    for (size_t i = 0; i < input_data.size(); i++) {
        if (input_data[i].first == "jmp" || input_data[i].first == "nop") {
            auto id = input_data;
            if (input_data[i].first == "jmp") {
                id[i].first = "nop";
            } else {
                id[i].first = "jmp";
            }

            auto const res = run(id);
            if (res.second) {
                return res.first;
            }
        }
    }
    return -1;
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