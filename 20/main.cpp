#include <bits/stdc++.h>

using ll = int64_t;
using tile_t = std::vector<std::string>;
using input_t = std::unordered_map<ll, tile_t>;
using result_t = ll;

struct tiles_border_t {
    ll up = -1;
    ll right = -1;
    ll down = -1;
    ll left = -1;

    ll up2 = -1;
    ll right2 = -1;
    ll down2 = -1;
    ll left2 = -1;
};

input_t read_input() {
    input_t input_values;

    std::string line;
    tile_t tile;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        auto const idx = line.find("Tile ");
        assert(idx == 0);
        auto const id = std::stoi(line.substr(5));

        tile.clear();
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }

            tile.emplace_back(line);
        }

        input_values.emplace(id, tile);
    }
    return input_values;
}

tiles_border_t get_border(tile_t const& tile) {
    assert(tile.size() == tile.front().size());
    tiles_border_t borders{ 0, 0, 0, 0, 0, 0, 0, 0 };
    for (size_t i = 0; i < tile.size(); i++) {
        borders.up = (borders.up << 1) | ((tile.front()[i] == '#') ? 1 : 0);
        borders.right = (borders.right << 1) | ((tile[i].back() == '#') ? 1 : 0);
        borders.down = (borders.down << 1) | ((tile.back()[i] == '#') ? 1 : 0);
        borders.left = (borders.left << 1) | ((tile[i].front() == '#') ? 1 : 0);

        borders.up2 = (borders.up2 << 1) | ((tile.front()[tile.front().size() - 1 - i] == '#') ? 1 : 0);
        borders.right2 = (borders.right2 << 1) | ((tile[tile.size() - 1 - i].back() == '#') ? 1 : 0);
        borders.down2 = (borders.down2 << 1) | ((tile.back()[tile.back().size() - 1 - i] == '#') ? 1 : 0);
        borders.left2 = (borders.left2 << 1) | ((tile[tile.size() - 1 - i].front() == '#') ? 1 : 0);
    }
    return borders;
}

struct border_inverse_t {
    ll border;
    ll tile_id;
    ll type;
};

result_t solve1(input_t const& input_data) {
    std::vector<border_inverse_t> border_inv_info;
    for (auto const& t : input_data) {
        auto const border = get_border(t.second);
        border_inv_info.emplace_back(border_inverse_t{ border.up, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.down, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.left, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.right, t.first });

        border_inv_info.emplace_back(border_inverse_t{ border.up2, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.down2, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.left2, t.first });
        border_inv_info.emplace_back(border_inverse_t{ border.right2, t.first });
    }

    assert(input_data.size() == static_cast<ll>(sqrt(input_data.size())) * static_cast<ll>(sqrt(input_data.size())));
    std::sort(std::begin(border_inv_info), std::end(border_inv_info), [](auto const& lhs, auto const& rhs) {
        return std::tie(lhs.border, lhs.tile_id) < std::tie(rhs.border, rhs.tile_id);
    });

    std::map<ll, ll> tiles_border_info;
    for (auto bii_it = border_inv_info.cbegin(); bii_it != border_inv_info.cend(); bii_it++) {
        auto const c1 = std::count_if(border_inv_info.cbegin(), bii_it, [&](auto const& elem) {
            return elem.border == bii_it->border;
        });
        auto const c2 = std::count_if(bii_it + 1, border_inv_info.cend(), [&](auto const& elem) {
            return elem.border == bii_it->border;
        });
        if (c1 + c2 > 0) {
            tiles_border_info[bii_it->tile_id] += c1 + c2;
        }
    }

    std::vector<std::pair<ll, ll>> bi;
    std::for_each(std::begin(tiles_border_info), std::end(tiles_border_info), [&](auto const& elem) {
        return bi.emplace_back(elem.first, elem.second);
    });
    std::sort(std::begin(bi), std::end(bi), [](auto const& lhs, auto const& rhs) {
        return lhs.second < rhs.second;
    });

    result_t ans = 1;
    auto it = bi.cbegin();
    for (size_t i = 0; i < 4; i++, it++) {
        ans *= it->first;
    }

    return ans;
}

struct tile_pos_t {
    ll tile_id;
    ll x;
    ll y;
    ll pos;
};

std::map<ll, ll>
more_close_border(std::vector<border_inverse_t> const& bii, ll tile_id, std::vector<tile_pos_t> const& used_tiles) {
    std::map<ll, ll> border_c;
    for (size_t i = 0; i < bii.size() - 1; i++) {
        if ((bii[i].tile_id == tile_id || bii[i + 1].tile_id == tile_id) && (bii[i].border == bii[i + 1].border)) {
            auto other_tile_id = bii[i].tile_id;
            if (other_tile_id == tile_id) {
                other_tile_id = bii[i + 1].tile_id;
            }
            if (other_tile_id == tile_id) {
                continue;
            }
            auto const it = std::find_if(std::cbegin(used_tiles), std::cend(used_tiles), [=](auto const& t) {
                return t.tile_id == other_tile_id;
            });
            if (it != std::cend(used_tiles)) {
                continue;
            }

            border_c[other_tile_id]++;
        }
    }
    return border_c;
}

result_t solve2(input_t const& input_data) {
    std::vector<border_inverse_t> border_inv_info;
    for (auto const& t : input_data) {
        auto const border = get_border(t.second);
        border_inv_info.emplace_back(border_inverse_t{ border.up, t.first, 0 });
        border_inv_info.emplace_back(border_inverse_t{ border.down, t.first, 1 });
        border_inv_info.emplace_back(border_inverse_t{ border.left, t.first, 2 });
        border_inv_info.emplace_back(border_inverse_t{ border.right, t.first, 3 });

        border_inv_info.emplace_back(border_inverse_t{ border.up2, t.first, 4 });
        border_inv_info.emplace_back(border_inverse_t{ border.down2, t.first, 5 });
        border_inv_info.emplace_back(border_inverse_t{ border.left2, t.first, 6 });
        border_inv_info.emplace_back(border_inverse_t{ border.right2, t.first, 7 });
    }

    assert(input_data.size() == static_cast<ll>(sqrt(input_data.size())) * static_cast<ll>(sqrt(input_data.size())));
    std::sort(std::begin(border_inv_info), std::end(border_inv_info), [](auto const& lhs, auto const& rhs) {
        return std::tie(lhs.border, lhs.tile_id, lhs.type) < std::tie(rhs.border, rhs.tile_id, rhs.type);
    });

    std::map<ll, ll> tiles_border_info;
    for (auto bii_it = border_inv_info.cbegin(); bii_it != border_inv_info.cend(); bii_it++) {
        auto const c1 = std::count_if(border_inv_info.cbegin(), bii_it, [&](auto const& elem) {
            return elem.border == bii_it->border;
        });
        auto const c2 = std::count_if(bii_it + 1, border_inv_info.cend(), [&](auto const& elem) {
            return elem.border == bii_it->border;
        });
        if (c1 + c2 > 0) {
            tiles_border_info[bii_it->tile_id] += c1 + c2;
        }
    }

    std::vector<std::pair<ll, ll>> bi;
    std::for_each(std::begin(tiles_border_info), std::end(tiles_border_info), [&](auto const& elem) {
        return bi.emplace_back(elem.first, elem.second);
    });
    std::sort(std::begin(bi), std::end(bi), [](auto const& lhs, auto const& rhs) {
        return lhs.second < rhs.second;
    });

    std::vector<ll> corners;
    result_t ans = 1;
    auto it = bi.cbegin();
    for (size_t i = 0; i < 4; i++, it++) {
        ans *= it->first;
        corners.emplace_back(it->first);
    }

    // std::for_each(std::cbegin(bi), std::cend(bi), [](auto const& elem) {
    //     std::cout << "(" << elem.second << "->" << elem.first << "), " << std::endl;
    // });
    // std::cout << std::endl;
    // std::for_each(std::cbegin(border_inv_info), std::cend(border_inv_info), [](auto const& elem) {
    //     std::cout << "(" << elem.border << "->" << elem.tile_id << ", " << elem.type << "), " << std::endl;
    // });

    std::vector<tile_pos_t> used_tiles{ tile_pos_t{ corners.front(), 0, 0, -1 } };
    auto const square = static_cast<ll>(sqrt(input_data.size()));
    for (ll i = 0; i < square; i++) {
        for (ll j = 1; j < square; j++) {
            // auto const most_common = more_close_border()
        }
    }

    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem.first << " -> " << elem.second.front() << std::endl;
    // });
    // std::cout << std::endl;
    std::cout << "Read Input Time = "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_input - begin_input).count() << "[ms]"
              << std::endl;

    auto begin_s1 = std::chrono::steady_clock::now();
    auto const s1 = solve1(input_data);
    auto end_s1 = std::chrono::steady_clock::now();
    std::cout << "Solution Part1: " << s1
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s1 - begin_s1).count()
              << "[ms]" << std::endl;

    auto begin_s2 = std::chrono::steady_clock::now();
    auto const s2 = solve2(input_data);
    auto end_s2 = std::chrono::steady_clock::now();
    std::cout << "Solution Part2: " << s2
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s2 - begin_s2).count()
              << "[ms]" << std::endl;

    return 0;
}