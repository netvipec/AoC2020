#include <bits/stdc++.h>

using ll = int64_t;
using tile_t = std::vector<std::string>;
using input_t = std::unordered_map<ll, tile_t>;
using result_t = ll;

// clang-format off
struct tiles_borde_group_t {
    ll up_r    = -1;       // 0
    ll right_r = -1;       // 1
    ll down_r  = -1;       // 2
    ll left_r  = -1;       // 3

    ll up_l    = -1;       // 4
    ll right_l = -1;       // 5
    ll down_l  = -1;       // 6
    ll left_l  = -1;       // 7
};
// clang-format on

union tiles_border_t {
    tiles_borde_group_t borders;
    std::array<ll, 8> borders_arr;
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
        auto& b = borders.borders;
        b.up_r = (b.up_r << 1) | ((tile.front()[i] == '#') ? 1 : 0);
        b.right_r = (b.right_r << 1) | ((tile[i].back() == '#') ? 1 : 0);
        b.down_r = (b.down_r << 1) | ((tile.back()[tile.back().size() - 1 - i] == '#') ? 1 : 0);
        b.left_r = (b.left_r << 1) | ((tile[tile.size() - 1 - i].front() == '#') ? 1 : 0);

        b.up_l = (b.up_l << 1) | ((tile.front()[tile.front().size() - 1 - i] == '#') ? 1 : 0);
        b.right_l = (b.right_l << 1) | ((tile[tile.size() - 1 - i].back() == '#') ? 1 : 0);
        b.down_l = (b.down_l << 1) | ((tile.back()[i] == '#') ? 1 : 0);
        b.left_l = (b.left_l << 1) | ((tile[i].front() == '#') ? 1 : 0);
    }
    return borders;
}

struct border_inverse_t {
    ll border = 0;
    ll tile_id = 0;
    ll type = 0;
};

result_t solve1(input_t const& input_data) {
    std::vector<border_inverse_t> border_inv_info;
    for (auto const& t : input_data) {
        auto const border = get_border(t.second);
        for (ll i = 0; i < static_cast<ll>(border.borders_arr.size()); i++) {
            border_inv_info.emplace_back(border_inverse_t{ border.borders_arr[i], t.first });
        }
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
    std::for_each(std::cbegin(tiles_border_info), std::cend(tiles_border_info), [&](auto const& elem) {
        return bi.emplace_back(elem.first, elem.second);
    });
    std::sort(std::begin(bi), std::end(bi), [](auto const& lhs, auto const& rhs) {
        return lhs.second < rhs.second;
    });

    result_t ans = std::accumulate(std::cbegin(bi), std::cbegin(bi) + 4, 1ll, [](auto const& base, auto const& elem) {
        return base * elem.first;
    });
    return ans;
}

/*
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

ll square;
std::map<ll, tiles_border_t> border_data;

struct bb_t {
    ll dir;
    ll type;
};

void get_bi(tiles_border_t const& b0, ll dir, tiles_border_t const& b, std::vector<bb_t>& bi) {
    for (ll i = 0; i < 8; i++) {
        for (ll j = 0; j < 8; j++) {
            if (b0.tb_index[i] == b.tb_index[j]) {
                bi.emplace_back(bb_t{ dir, i });
            }
        }
    }
}

void get_solution(std::vector<tile_pos_t>& used_tiles, std::vector<border_inverse_t> const& border_inv_info) {
    for (ll i = 0; i < square * square; i++) {
        ll x = i / square;
        ll y = i % square;

        auto const b0_it = border_data.find(used_tiles[i].tile_id);
        assert(b0_it != std::cend(border_data));

        std::vector<bb_t> borders_i;
        if (x != 0) {
            auto const bu_it = border_data.find(used_tiles[i - square].tile_id);
            assert(bu_it != std::cend(border_data));
            get_bi(b0_it->second, 0, bu_it->second, borders_i);
        }
        if (x != square - 1) {
            auto const bd_it = border_data.find(used_tiles[i + square].tile_id);
            assert(bd_it != std::cend(border_data));
            get_bi(b0_it->second, 2, bd_it->second, borders_i);
        }

        if (y != 0) {
            auto const bl_it = border_data.find(used_tiles[i - 1].tile_id);
            assert(bl_it != std::cend(border_data));
            get_bi(b0_it->second, 3, bl_it->second, borders_i);
        }
        if (y != square - 1) {
            auto const br_it = border_data.find(used_tiles[i + 1].tile_id);
            assert(br_it != std::cend(border_data));
            get_bi(b0_it->second, 1, br_it->second, borders_i);
        }

        std::for_each(std::cbegin(borders_i), std::cend(borders_i), [](auto const& elem) {
            std::cout << "(dir: " << elem.dir << ", type: " << elem.type << ")" << std::endl;
        });
        std::cout << std::endl;
    }
}

void get_arrange(std::vector<tile_pos_t>& used_tiles,
                 std::vector<ll> const& corners,
                 std::vector<border_inverse_t> const& border_inv_info,
                 ll idx = 1) {
    if (idx == square * square) {
        std::cout << "SOLUTION" << std::endl;
        std::for_each(std::cbegin(used_tiles), std::cend(used_tiles), [](auto const& elem) {
            std::cout << elem.tile_id << " -> (" << elem.x << "," << elem.y << ")," << std::endl;
        });
        std::cout << std::endl;
        get_solution(used_tiles, border_inv_info);
    }
    ll i = idx / square;
    ll j = idx % square;

    auto search_tile_id = used_tiles.back().tile_id;
    if (j == 0) {
        search_tile_id = used_tiles[used_tiles.size() - square].tile_id;
    }
    auto const most_common = more_close_border(border_inv_info, search_tile_id, used_tiles);
    if (most_common.size() == 0) {
        return;
    }

    if ((j == square - 1 && (i == 0 || i == square - 1)) || (j == 0 && i == square - 1)) {
        for (auto const& mc : most_common) {
            auto const cit = std::find(std::cbegin(corners), std::cend(corners), mc.first);
            if (cit == std::cend(corners)) {
                continue;
            }
            used_tiles.emplace_back(tile_pos_t{ mc.first, i, j, -1 });

            // std::cout << "put " << used_tiles.back().tile_id << ", i: " << i << ", j: " << j << std::endl;
            get_arrange(used_tiles, corners, border_inv_info, idx + 1);
            // std::cout << "remove " << used_tiles.back().tile_id << ", i: " << i << ", j: " << j << std::endl;

            used_tiles.pop_back();
        }
    } else {
        for (auto const& mc : most_common) {
            auto const cit = std::find(std::cbegin(corners), std::cend(corners), mc.first);
            if (cit != std::cend(corners)) {
                continue;
            }
            used_tiles.emplace_back(tile_pos_t{ mc.first, i, j, -1 });

            std::cout << "put " << used_tiles.back().tile_id << ", i: " << i << ", j: " << j << std::endl;
            get_arrange(used_tiles, corners, border_inv_info, idx + 1);
            std::cout << "remove " << used_tiles.back().tile_id << ", i: " << i << ", j: " << j << std::endl;

            used_tiles.pop_back();
        }
    }
}
*/
result_t solve2(input_t const& input_data) { /*
     std::vector<border_inverse_t> border_inv_info;
     for (auto const& t : input_data) {
         auto const border = get_border(t.second);
         border_data.emplace(t.first, border);
         border_inv_info.emplace_back(border_inverse_t{ border.tb.up, t.first, 0 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.down, t.first, 1 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.left, t.first, 2 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.right, t.first, 3 });

         border_inv_info.emplace_back(border_inverse_t{ border.tb.up2, t.first, 4 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.down2, t.first, 5 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.left2, t.first, 6 });
         border_inv_info.emplace_back(border_inverse_t{ border.tb.right2, t.first, 7 });
     }

     assert(input_data.size() == static_cast<ll>(sqrt(input_data.size())) * static_cast<ll>(sqrt(input_data.size())));
     square = static_cast<ll>(sqrt(input_data.size()));
     std::sort(std::begin(border_inv_info), std::end(border_inv_info), [](auto const& lhs, auto const& rhs) {
         return std::tie(lhs.border, lhs.tile_id, lhs.type) < std::tie(rhs.border, rhs.tile_id, rhs.type);
     });

     std::cout << "Matrix size: " << square << "x" << square << std::endl;

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
     get_arrange(used_tiles, corners, border_inv_info);

     return ans;*/
    return -1;
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