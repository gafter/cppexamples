#include <iostream>

// An item has a name, value, and size.
struct Item {
    std::string name;
    double value;
    double size;
};
inline bool operator==(const Item& lhs, const Item& rhs) { return lhs.name == rhs.name; }
inline bool operator!=(const Item& lhs, const Item& rhs) { return !(lhs == rhs); }

void best_pack_core(
    const std::vector<Item>& items, int start,
    std::vector<Item>& items_so_far, double remaining_capacity,
    double value_so_far,
    std::vector<Item>& best_items,
    double& best_value) {
    if (start >= items.size()) {
        if (value_so_far > best_value) {
            best_value = value_so_far;
            best_items = items_so_far;
        }
        return;
    }

    // try without item at index start
    best_pack_core(items, start+1, items_so_far, remaining_capacity, value_so_far, best_items, best_value);

    // try with item at index start
    if (items[start].size <= remaining_capacity) {
        items_so_far.push_back(items[start]);
        value_so_far += items[start].value;
        remaining_capacity -= items[start].size;
        best_pack_core(items, start+1, items_so_far, remaining_capacity, value_so_far, best_items, best_value);
        items_so_far.pop_back();
    }
}

/*
 * Compute the best pack of a knapsack.
 *
 * You are given a vector of items `items` to choose from (one of each).
 * Each item has a name, a value, and a size.
 * You are also given a capacity for the knapsack.
 * Select some subset of the items that maximizes the sum of the values of that subset,
 * under the constraint that the sum of the sizes of that subset must be less than or
 * equal to the capacity of the knapsack.
 *
 * Return a pair of (1) a vector of the selected elements,
 * in the same order in which they appeared in `items`, and (2) a double giving the
 * total value of those items.
 */
std::pair<std::vector<Item>, double> best_pack(std::vector<Item>& items, double capacity) {
    std::vector<Item> items_so_far;
    std::vector<Item> best_items;
    double best_value = 0;
    best_pack_core(
        items, 0,
        items_so_far, capacity,
        0.0,
        best_items,
        best_value);
    return std::pair<std::vector<Item>, double>(best_items, best_value);
}

// TESTS

Item a = {"a", 1.53749, 1.22383};
Item b = {"b", 1.48996, 1.64679};
Item c = {"c", 1.37618, 1.79138};
Item d = {"d", 1.06667, 1.91386};
Item e = {"e", 1.65346, 1.64265};
Item f = {"f", 1.40055, 1.33684};
Item g = {"g", 1.19614, 1.6596};
Item h = {"h", 1.58644, 1.64236};
Item i = {"i", 1.23108, 1.3215};
Item j = {"j", 1.81625, 1.50874};
Item k = {"k", 1.38009, 1.69832};
Item l = {"l", 1.80603, 1.31304};
Item m = {"m", 1.01621, 1.33001};
Item n = {"n", 1.23472, 1.75226};
Item o = {"o", 1.34368, 1.88223};
Item p = {"p", 1.47654, 1.67125};

void test(
        std::vector<Item> items,
        double capacity,
        std::vector<Item> expected_pack,
        double expected_value) {
    std::cout << "testing { ";
    for (auto i : items) std::cout << i.name << ", ";
    std::cout << "} capacity " << capacity;
    auto[selected, value] = best_pack(items, capacity);
    // std::cout << std::endl;
    // std::cout << "capacity: " << capacity << std::endl;
    // std::cout << "value: " << value << std::endl;
    double packed_size = 0;
    double packed_value = 0;
    // std::cout << "{ ";
    for (auto item : selected) {
        // std::cout << item.name << ", ";
        packed_size += item.size;
        packed_value += item.value;
    }
    // std::cout << "}" << std::endl;
    // std::cout << "packed_value: " << packed_value << std::endl;
    // std::cout << "packed_size: " << packed_size << std::endl;

    assert(std::abs(value - expected_value) < 1e-4);
    assert(std::abs(packed_value - value) < 1e-4);
    assert(packed_size <= capacity);
    assert(selected == expected_pack);
    std::cout << std::endl;
}

void test1() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j },
        7.0,
        std::vector<Item>{ a, e, h, j },
        6.59364);
}

void test2() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j },
        8.0,
        std::vector<Item>{ a, b, e, h, j },
        8.0836);
}

void test3() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p },
        11.00,
        std::vector<Item>{ a, b, e, h, j, l, p },
        11.3662);
}

void test4() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p },
        12.96,
        std::vector<Item>{ a, b, e, f, h, j, l, p },
        12.7667);
}

void test5() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p },
        12.97,
        std::vector<Item>{ a, b, e, f, h, i, j, l, m },
        13.5375);
}

void test6() {
    test(
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p },
        120,
        std::vector<Item>{ a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p },
        22.6115);
}

void test7() {
    test(
        std::vector<Item>{},
        18,
        std::vector<Item>{},
        0.0);
}

Item q = {"q", 0.514498, 0.117363};
Item r = {"r", 0.750566, 0.961118};
Item s = {"s", 0.727592, 0.775994};
Item t = {"t", 0.518507, 0.20065};
Item u = {"u", 0.109512, 0.697764};
Item v = {"v", 0.676916, 0.81417};
Item w = {"w", 0.81935, 0.379059};
Item x = {"x", 0.655379, 0.989713};
Item y = {"y", 0.711705, 0.851085};
Item z = {"z", 0.932356, 0.379549};

void test8() {
    test(
        std::vector<Item>{ q, r, s, t, u, v, w, x, y, z },
        2.8,
        std::vector<Item>{ q, s, t, w, y, z },
        4.22401);
}

void test9() {
    test(
        std::vector<Item>{ q, r, s, t, u, v, w, x, y, z },
        3.0,
        std::vector<Item>{ q, r, s, t, w, z },
        4.26287);
}

void test10() {
    test(
        std::vector<Item>{ q, r, s, t, u, v, w, x, y, z },
        3.4,
        std::vector<Item>{ q, s, v, w, y, z },
        4.38242);
}

void test11() {
    test(
        std::vector<Item>{ q, r, s, t, u, v, w, x, y, z },
        3.6,
        std::vector<Item>{ q, s, t, v, w, y, z },
        4.90092);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();

    return 0;
}
