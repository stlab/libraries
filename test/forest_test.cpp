/**************************************************************************************************/

// stdc++
#include <iostream>

// boost
#include <boost/test/unit_test.hpp>

// stlab
#include <stlab/forest.hpp>

/**************************************************************************************************/

using namespace stlab;

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(empty_forest) {
    forest<int> f;
    BOOST_CHECK(f.empty());
    BOOST_CHECK(f.size() == 0);
    BOOST_CHECK(f.begin() == f.end());
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(single_node_forest) {
    forest<int> f;

    auto il = f.insert(f.end(), 42);
    BOOST_CHECK(il.edge() == forest_edge::leading);
    BOOST_CHECK(*il == 42);
    BOOST_CHECK(!f.empty());

    BOOST_CHECK(f.begin() != f.end());
    BOOST_CHECK(f.size() == 1);

    auto it = trailing_of(il);
    BOOST_CHECK(it.edge() == forest_edge::trailing);
    BOOST_CHECK(*it == *il);
}

/**************************************************************************************************/

namespace {

/**************************************************************************************************/

auto big_test_forest() {
    forest<std::string> f;

    auto a_iter = trailing_of(f.insert(f.end(), "A"));

    auto b_iter = trailing_of(f.insert(a_iter, "B"));

    auto c_iter = trailing_of(f.insert(b_iter, "C"));
    auto d_iter = trailing_of(f.insert(b_iter, "D"));
    auto e_iter = f.insert(b_iter, "E");

    f.insert(c_iter, "F");
    f.insert(c_iter, "G");
    f.insert(c_iter, "H");

    f.insert(d_iter, "I");
    f.insert(d_iter, "J");
    f.insert(d_iter, "K");

    BOOST_CHECK(f.size() == 11);

    BOOST_CHECK(has_children(a_iter));
    BOOST_CHECK(has_children(b_iter));
    BOOST_CHECK(has_children(c_iter));
    BOOST_CHECK(has_children(d_iter));
    BOOST_CHECK(!has_children(e_iter));

    return f;
}

/**************************************************************************************************/

template <typename R>
inline auto to_string(const R& r) {
    std::string result;
    for (const auto& x : r) {
        result += x;
    }
    return result;
}

template <typename I>
inline auto to_string(I first, I last) {
    std::string result;
    while (first != last) {
        result += *first++;
    }
    return result;
}

/**************************************************************************************************/

template <typename Iterator>
void test_fullorder_traversal(Iterator first, Iterator last, const std::string& expected) {
    BOOST_CHECK(to_string(first, last) == expected);
}

/**************************************************************************************************/

template <typename Iterator, forest_edge Edge, typename Forest>
auto test_edge_traversal(Forest& f, Iterator fi, Iterator li) {
    std::string expected;

    {
        Iterator first{fi};
        Iterator last{li};
        while (first != last) {
            if (first.edge() == Edge)
                expected += *first;
            ++first;
        }
        BOOST_CHECK(expected.size() == f.size());
    }

    {
        edge_iterator<Iterator, Edge> first(fi);
        edge_iterator<Iterator, Edge> last(li);
        std::string result{to_string(first, last)};
        BOOST_CHECK(result == expected);
    }

    return expected;
}

/**************************************************************************************************/

using iterator_t = forest<std::string>::iterator;
using const_iterator_t = forest<std::string>::const_iterator;
using reverse_iterator_t = forest<std::string>::reverse_iterator;
using const_reverse_iterator_t = forest<std::string>::const_reverse_iterator;

/**************************************************************************************************/

} // namespace

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(forward_traversal) {
    auto f{big_test_forest()};
    auto first{std::begin(f)};
    auto last{std::end(f)};

    /*fullorder*/ {
        static const auto expected{"ABCFFGGHHCDIIJJKKDEEBA"};
        test_fullorder_traversal<iterator_t>(first, last, expected);
        test_fullorder_traversal<const_iterator_t>(first, last, expected);

#if 0 // fullorder_range doesn't exist?
        BOOST_CHECK(range_value(fullorder_range(f)) == expected);
#endif
    }

    /*preorder*/ {
        auto a = test_edge_traversal<iterator_t, forest_edge::leading>(f, first, last);
        auto b = test_edge_traversal<const_iterator_t, forest_edge::leading>(f, first, last);
        BOOST_CHECK(a == b);
        BOOST_CHECK(to_string(preorder_range(f)) == a);
    }

    /*postorder*/ {
        auto a = test_edge_traversal<iterator_t, forest_edge::trailing>(f, first, last);
        auto b = test_edge_traversal<const_iterator_t, forest_edge::trailing>(f, first, last);
        BOOST_CHECK(a == b);
        BOOST_CHECK(to_string(postorder_range(f)) == a);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(reverse_traversal) {
    auto f{big_test_forest()};
    auto rfirst{std::rbegin(f)};
    auto rlast{std::rend(f)};

    /*fullorder*/ {
        static const auto expected{"ABEEDKKJJIIDCHHGGFFCBA"};
        test_fullorder_traversal<reverse_iterator_t>(rfirst, rlast, expected);
        test_fullorder_traversal<const_reverse_iterator_t>(rfirst, rlast, expected);
        BOOST_CHECK(to_string(reverse_fullorder_range(f)) == expected);
    }

    /*preorder*/ {
        auto a = test_edge_traversal<reverse_iterator_t, forest_edge::leading>(f, rfirst, rlast);
        auto b = test_edge_traversal<const_reverse_iterator_t, forest_edge::leading>(f, rfirst, rlast);
        BOOST_CHECK(a == b);
    }

    /*postorder*/ {
        auto a = test_edge_traversal<reverse_iterator_t, forest_edge::trailing>(f, rfirst, rlast);
        auto b = test_edge_traversal<const_reverse_iterator_t, forest_edge::trailing>(f, rfirst, rlast);
        BOOST_CHECK(a == b);
    }
}

/**************************************************************************************************/

template <typename Forest, typename T>
auto find_node(Forest& f, const T& x) {
    return std::find_if(f.begin(), f.end(), [&](const auto& v){ return v == x; });
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(child_traversal) {
    auto f{big_test_forest()};
    auto parent{find_node(f, "B")};
    std::string expected;

    BOOST_CHECK(*parent == "B");

    {
        auto first{child_begin(parent)};
        auto last{child_end(parent)};
        expected = to_string(first, last);
    }

    BOOST_CHECK(to_string(child_range(parent)) == expected);

    if constexpr (false) { // I'm not sure reverse_child_iterator ever worked.
        forest<std::string>::reverse_child_iterator first{child_begin(parent)};
        forest<std::string>::reverse_child_iterator last{child_end(parent)};
        std::string result{to_string(first, last)};
        BOOST_CHECK(result == expected);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(test_find_parent) {
    auto f{big_test_forest()};

    {
        auto child{find_node(f, "B")};
        BOOST_CHECK(*child == "B");
        auto parent{find_parent(child)};
        BOOST_CHECK(*parent == "A");
    }

    {
        auto child{find_node(f, "J")};
        BOOST_CHECK(*child == "J");
        auto parent{find_parent(child)};
        BOOST_CHECK(*parent == "D");
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(test_has_children) {
    auto f{big_test_forest()};

    /*pass*/ {
        auto node{find_node(f, "B")};
        BOOST_CHECK(*node == "B");
        BOOST_CHECK(has_children(node));
    }

    /*fail*/ {
        auto node{find_node(f, "J")};
        BOOST_CHECK(*node == "J");
        BOOST_CHECK(!has_children(node));
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(erase) {
    auto f{big_test_forest()};

    /*single node*/ {
        auto node{find_node(f, "J")};
        BOOST_CHECK(*node == "J");
        auto erased_result = f.erase(node);
        std::string result{to_string(preorder_range(f))};
        BOOST_CHECK(result == "ABCFGHDIKE");
        BOOST_CHECK(*erased_result == "K");
    }

    /*multiple nodes*/ {
        auto node{find_node(f, "D")};
        BOOST_CHECK(*node == "D");
        auto erased_result = f.erase(leading_of(node), std::next(trailing_of(node)));
        std::string result{to_string(preorder_range(f))};
        BOOST_CHECK(result == "ABCFGHE");
        BOOST_CHECK(*erased_result == "E");
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(construction) {
    auto f{big_test_forest()};

    /* copy construction */ {
        auto f2 = f;
        BOOST_CHECK(f2 == f);
    }

    /* move construction */ {
        auto f_size{f.size()};
        auto f2 = std::move(f);
        BOOST_CHECK(f2 != f);
        BOOST_CHECK(f2.size() == f_size);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(assignment) {
    auto f{big_test_forest()};

    /* copy assignment */ {
        decltype(f) f2;
        BOOST_CHECK(f2.empty());
        f2 = f;
        BOOST_CHECK(f2 == f);
    }

    /* move assignment */ {
        auto f_size{f.size()};
        decltype(f) f2;
        f2 = std::move(f);
        BOOST_CHECK(f2 != f);
        BOOST_CHECK(f2.size() == f_size);
    }

    /* self-move assignment */ {
        auto f{big_test_forest()};
        auto* pf{&f}; // We use a pointer here to get around a clang error when moving to self.
        auto f_size{f.size()};
        f = std::move(*pf);
        BOOST_CHECK(f.size() == f_size);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(swap) {
    auto f1{big_test_forest()};
    auto f2{f1};
    f2.pop_back();
    f2.pop_front();
    auto f1_sz{f1.size()};
    auto f2_sz{f2.size()};

    BOOST_CHECK(f1_sz != f2_sz);
    BOOST_CHECK(f1 != f2);

    std::swap(f1, f2);

    BOOST_CHECK(f2.size() == f1_sz);
    BOOST_CHECK(f1.size() == f2_sz);

    f1.swap(f2);

    BOOST_CHECK(f1.size() == f1_sz);
    BOOST_CHECK(f2.size() == f2_sz);
}

/**************************************************************************************************/
