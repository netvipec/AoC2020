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

void fill_edges(std::unordered_map<ll, std::set<ll>> const& tiles_neighbors_count,
                std::unordered_map<ll, std::set<ll>> const& tiles_neighbors,
                std::vector<std::vector<ll>>& tile_matrix) {
    auto corner_tiles_it = tiles_neighbors_count.find(2);
    assert(corner_tiles_it != std::cend(tiles_neighbors_count));
    auto corner_tile_it = corner_tiles_it->second.cbegin();
    assert(corner_tile_it != std::cend(corner_tiles_it->second));

    auto edge_tiles_it = tiles_neighbors_count.find(3);
    assert(edge_tiles_it != std::cend(tiles_neighbors_count));

    ll last_tile_id = *corner_tile_it;
    tile_matrix[0][0] = last_tile_id;
    std::unordered_set<ll> used_tiles;
    used_tiles.insert(last_tile_id);
    std::cout << "put: " << last_tile_id << ", i: 0, d: 0" << std::endl;
    auto const square = static_cast<ll>(tile_matrix.size());
    for (ll d = 0; d < 4; d++) {
        for (ll i = 1; i < square - 1; i++) {
            auto const neighbors_it = tiles_neighbors.find(last_tile_id);
            assert(neighbors_it != std::cend(tiles_neighbors));
            auto neighbor_it =
                std::find_if(std::cbegin(neighbors_it->second), std::cend(neighbors_it->second), [&](auto const& elem) {
                    auto const used_it = used_tiles.find(elem);
                    auto const edge_it = edge_tiles_it->second.find(elem);
                    return used_it == std::cend(used_tiles) && edge_it != std::cend(edge_tiles_it->second);
                });

            assert(neighbor_it != std::cend(neighbors_it->second));

            last_tile_id = *neighbor_it;
            used_tiles.insert(last_tile_id);
            std::cout << "put: " << last_tile_id << ", i: " << i << ", d: " << d << std::endl;
            switch (d) {
                case 0:
                    tile_matrix[0][i] = last_tile_id;
                    break;
                case 1:
                    tile_matrix[i][square - 1] = last_tile_id;
                    break;
                case 2:
                    tile_matrix[square - 1][square - 1 - i] = last_tile_id;
                    break;
                case 3:
                    tile_matrix[square - 1 - i][0] = last_tile_id;
                    break;
                default:
                    assert(false);
                    break;
            }
        }

        if (d != 3) {
            auto const neighbors_c_it = tiles_neighbors.find(last_tile_id);
            assert(neighbors_c_it != std::cend(tiles_neighbors));
            auto const new_corner = std::find_if(std::cbegin(corner_tiles_it->second),
                                                 std::cend(corner_tiles_it->second),
                                                 [&](auto const& elem) {
                                                     auto const is_used_it = used_tiles.find(elem);
                                                     auto const is_neighbor_it = neighbors_c_it->second.find(elem);
                                                     return is_used_it == std::cend(used_tiles)
                                                            && is_neighbor_it != std::cend(neighbors_c_it->second);
                                                 });
            assert(new_corner != std::cend(corner_tiles_it->second));
            last_tile_id = *new_corner;
            used_tiles.insert(last_tile_id);
            std::cout << "put: " << last_tile_id << ", i: " << square - 1 << ", d: " << d << std::endl;
        }
        switch (d) {
            case 0:
                tile_matrix[0][square - 1] = last_tile_id;
                break;
            case 1:
                tile_matrix[square - 1][square - 1] = last_tile_id;
                break;
            case 2:
                tile_matrix[square - 1][0] = last_tile_id;
                break;
            case 3:
                break;
            default:
                assert(false);
                break;
        }
    }
}

template <class T, class Comp, class Alloc, class Predicate>
void discard_if(std::set<T, Comp, Alloc>& c, Predicate pred) {
    for (auto it{ c.begin() }, end{ c.end() }; it != end;) {
        if (pred(*it)) {
            it = c.erase(it);
        } else {
            ++it;
        }
    }
}

void fill_inside(std::unordered_map<ll, std::set<ll>> const& tiles_neighbors_count,
                 std::unordered_map<ll, std::set<ll>> const& tiles_neighbors,
                 std::vector<std::vector<ll>>& tile_matrix) {
    auto inside_tiles_it = tiles_neighbors_count.find(4);
    assert(inside_tiles_it != std::cend(tiles_neighbors_count));

    std::unordered_set<ll> used_tiles;
    auto const square = static_cast<ll>(tile_matrix.size());
    for (ll i = 1; i < square - 1; i++) {
        for (ll j = 1; j < square - 1; j++) {
            auto neighbors1_it = tiles_neighbors.find(tile_matrix[i - 1][j]);
            assert(neighbors1_it != std::cend(tiles_neighbors));
            auto neighbors2_it = tiles_neighbors.find(tile_matrix[i][j - 1]);
            assert(neighbors2_it != std::cend(tiles_neighbors));
            std::set<ll> f;
            std::set_intersection(std::cbegin(neighbors1_it->second),
                                  std::cend(neighbors1_it->second),
                                  std::cbegin(neighbors2_it->second),
                                  std::cend(neighbors2_it->second),
                                  std::inserter(f, f.end()));

            discard_if(f, [&](auto const& elem) {
                auto const is_used_it = used_tiles.find(elem);
                return is_used_it != std::cend(used_tiles)
                       || inside_tiles_it->second.find(elem) == std::cend(inside_tiles_it->second);
            });

            assert(f.size() == 1);
            tile_matrix[i][j] = *f.cbegin();
            used_tiles.insert(*f.cbegin());
            std::cout << "put: " << tile_matrix[i][j] << ", i: " << i << ", j: " << j << std::endl;
        }
    }
}

result_t solve2(input_t const& input_data) {
    std::vector<border_inverse_t> border_inv_info;
    for (auto const& t : input_data) {
        auto const border = get_border(t.second);
        for (ll i = 0; i < static_cast<ll>(border.borders_arr.size()); i++) {
            border_inv_info.emplace_back(border_inverse_t{ border.borders_arr[i], t.first, i });
        }
    }

    auto const square = static_cast<ll>(sqrt(input_data.size()));
    assert(input_data.size() == square * square);

    std::cout << "Matrix size: " << square << "x" << square << std::endl;

    std::sort(std::begin(border_inv_info), std::end(border_inv_info), [](auto const& lhs, auto const& rhs) {
        return std::tie(lhs.border, lhs.tile_id, lhs.type) < std::tie(rhs.border, rhs.tile_id, rhs.type);
    });

    for (ll i = 1; i < static_cast<ll>(border_inv_info.size()) - 1;) {
        if (border_inv_info[i - 1].border != border_inv_info[i].border
            && border_inv_info[i].border != border_inv_info[i + 1].border) {
            border_inv_info.erase(border_inv_info.begin() + i);
        } else {
            i++;
        }
    }
    if (border_inv_info[border_inv_info.size() - 2].border != border_inv_info[border_inv_info.size() - 1].border) {
        border_inv_info.erase(border_inv_info.end() - 1);
    }
    if (border_inv_info[0].border != border_inv_info[1].border) {
        border_inv_info.erase(border_inv_info.begin());
    }

    std::unordered_map<ll, std::vector<std::pair<ll, ll>>> neighbors_info;
    std::for_each(std::cbegin(border_inv_info), std::cend(border_inv_info), [&](auto const& elem) {
        neighbors_info[elem.border].emplace_back(elem.tile_id, elem.type);
    });

    std::unordered_map<ll, std::set<ll>> tiles_neighbors;
    std::for_each(std::cbegin(neighbors_info), std::cend(neighbors_info), [&](auto const& elem) {
        std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& e1) {
            std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& e2) {
                if (e1.first != e2.first) {
                    tiles_neighbors[e1.first].insert(e2.first);
                    tiles_neighbors[e2.first].insert(e1.first);
                }
            });
        });
    });

    std::unordered_map<ll, std::set<ll>> tiles_neighbors_count;
    std::for_each(std::cbegin(tiles_neighbors), std::cend(tiles_neighbors), [&](auto const& elem) {
        tiles_neighbors_count[static_cast<ll>(elem.second.size())].insert(elem.first);
    });

    // std::for_each(std::cbegin(tiles_neighbors), std::cend(tiles_neighbors), [&](auto const& elem) {
    //     std::cout << elem.first << ": ";
    //     std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& e) {
    //         std::cout << e << ",";
    //     });
    //     std::cout << std::endl;
    // });
    std::for_each(std::cbegin(tiles_neighbors_count), std::cend(tiles_neighbors_count), [&](auto const& elem) {
        std::cout << elem.first << ": ";
        std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& e) {
            std::cout << e << ",";
        });
        std::cout << std::endl;
    });

    std::vector<std::vector<ll>> tile_matrix(square, std::vector<ll>(square));
    auto inside_tiles_it = tiles_neighbors_count.find(4);
    assert(inside_tiles_it != std::cend(tiles_neighbors_count));

    fill_edges(tiles_neighbors_count, tiles_neighbors, tile_matrix);
    fill_inside(tiles_neighbors_count, tiles_neighbors, tile_matrix);

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