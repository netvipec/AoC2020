#include <bits/stdc++.h>

using namespace std;

using ll = int64_t;
using input_line_t = std::map<std::string, std::string>;
using input_t = std::vector<input_line_t>;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         [](int ch) {
                             return !std::isspace(ch);
                         })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

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
    input_line_t p;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        if (line.empty()) {
            input_values.push_back(p);
            p.clear();
        }

        auto const elems = split(line, ' ');
        for (auto const& e : elems) {
            auto const b = split(e, ':');
            assert(b.size() == 2);
            p.insert(make_pair(b[0], b[1]));
        }
    }
    return input_values;
}

ll solve1(input_t const& input_data) {
    std::vector<std::string> req{ "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    ll ans = 0;
    for (auto const& p : input_data) {
        bool bad = false;
        for (auto const& r : req) {
            auto const it = p.find(r);
            if (it == p.cend()) {
                bad = true;
                break;
            }
        }
        if (!bad)
            ans++;
    }
    return ans;
}

bool check_size(std::string& d) {
    trim(d);
    return (d.size() == 4);
}

bool byr_check(std::string const& d) {
    auto dd = d;
    if (!check_size(dd)) {
        return false;
    }
    auto const n = std::stoi(dd);
    return 1920 <= n && n <= 2002;
}
bool iyr_check(std::string const& d) {
    auto dd = d;
    if (!check_size(dd)) {
        return false;
    }
    auto const n = std::stoi(dd);
    return 2010 <= n && n <= 2020;
}
bool eyr_check(std::string const& d) {
    auto dd = d;
    if (!check_size(dd)) {
        return false;
    }
    auto const n = std::stoi(dd);
    return 2020 <= n && n <= 2030;
}
bool hgt_check(std::string const& d) {
    auto dd = d;
    trim(dd);

    if (dd.size() < 2) {
        return false;
    }

    if (dd[dd.size() - 2] == 'c' && dd[dd.size() - 1] == 'm') {
        auto v = std::stoi(dd.substr(0, dd.size() - 2));
        return 150 <= v && v <= 193;
    } else if (dd[dd.size() - 2] == 'i' && dd[dd.size() - 1] == 'n') {
        auto v = std::stoi(dd.substr(0, dd.size() - 2));
        return 59 <= v && v <= 76;
    }

    return false;
}
bool hcl_check(std::string const& d) {
    if (d.size() != 7 || d[0] != '#') {
        return false;
    }
    auto it = std::find_if(d.cbegin() + 1, d.cend(), [](auto const& c) {
        return !(('0' <= c && c <= '9') || ('a' <= c && c <= 'f'));
    });
    return it == d.cend();
}
bool ecl_check(std::string const& d) {
    static std::set<std::string> color{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    return color.find(d) != color.cend();
}
bool pid_check(std::string const& d) {
    if (d.size() != 9)
        return false;
    return std::find_if(d.cbegin(),
                        d.cend(),
                        [](auto const& c) {
                            return !('0' <= c && c <= '9');
                        })
           == d.cend();
}

ll solve2(input_t const& input_data) {
    std::vector<std::string> req{ "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    std::vector<std::function<bool(std::string const&)>> req_f{ byr_check, iyr_check, eyr_check, hgt_check,
                                                                hcl_check, ecl_check, pid_check };

    ll ans = 0;
    for (auto const& p : input_data) {
        bool bad = false;
        for (auto const& r : req) {
            auto const it = p.find(r);
            if (it == p.cend()) {
                bad = true;
                break;
            }
        }
        if (!bad) {
            bad = false;
            for (int i = 0; i < req.size(); i++) {
                auto e = p.find(req[i]);
                if (!req_f[i](e->second)) {
                    // std::cout << i << " " << e->second << " " << e->second.size() << std::endl;
                    bad = true;
                    break;
                }
            }
            if (!bad) {
                ans++;
            }
        }
    }
    return ans;
}

int main() {
    std::cout << hcl_check("#c0946f") << std::endl;
    auto const input_data = read_input();
    std::for_each(std::cbegin(input_data), std::cbegin(input_data) + 1, [](auto const& elem) {
        std::for_each(std::cbegin(elem), std::cend(elem), [](auto const& e) {
            std::cout << e.first << ": " << e.second << ", ";
        });
        std::cout << std::endl;
    });
    std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}