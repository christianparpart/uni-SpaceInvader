#include <sgfx/color.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <algorithm>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

template <typename Container, typename Dest, typename F>
void transform(Container const& container, Dest dest, F f)
{
    std::transform(begin(container), end(container), std::move(dest), std::move(f));
}

template <typename Container, typename Init, typename F>
auto accumulate(Container const& container, Init init, F f)
{
    return std::accumulate(cbegin(container), cend(container), std::move(init), std::move(f));
}

int random_uniform_int(int max)
{
    static std::mt19937 random_gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, max - 1);
    return dist(random_gen);
}

std::vector<sgfx::point> convex_hull(std::vector<sgfx::point> points)
{
    // Graham scan, see: https://en.wikipedia.org/wiki/Graham_scan

    std::vector<sgfx::point> ret_val;

    enum class direction { clockwise, counterclockwise };
    const auto compute_turn = [](sgfx::point first, sgfx::point second, sgfx::point third) {
        if (((second.x - first.x) * (third.y - first.y) - (second.y - first.y) * (third.x - first.x)) <= 0)
            return direction::clockwise;
        return direction::counterclockwise;
    };

    const auto min_it =
        std::min_element(std::begin(points), std::end(points), [](const auto& lhs, const auto& rhs) {
            return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
        });
    std::swap(*min_it, points.front());

    std::sort(std::begin(points) + 1, std::end(points), [&](const auto& lhs, const auto& rhs) {
        return lhs != rhs && compute_turn(points.front(), lhs, rhs) == direction::counterclockwise;
    });

    for (const auto& p : points)
    {
        while (ret_val.size() > 2
               && compute_turn(ret_val[ret_val.size() - 2], ret_val[ret_val.size() - 1], p)
                      == direction::clockwise)
            ret_val.pop_back();

        ret_val.push_back(p);
    }

    return ret_val;
}

std::vector<sgfx::point> random_polygon(sgfx::dimension max_size, std::size_t vertices)
{
    std::vector<sgfx::point> points;
    std::generate_n(std::back_inserter(points), vertices, [&]() {
        return sgfx::point{random_uniform_int(max_size.w), random_uniform_int(max_size.h)};
    });

    return points;
}

struct offset_polygon {
    std::vector<sgfx::point> points;
    sgfx::vec offset;
};

std::vector<sgfx::vec> edges(offset_polygon const& poly)
{
    auto result = std::vector<sgfx::vec>{};
    for (size_t i = 0; i < poly.points.size() - 1; ++i)
        result.emplace_back(poly.points[i + 1] - poly.points[i]);
    result.emplace_back(poly.points.back() - poly.points.front());
    return result;
}

std::vector<sgfx::vec> axes(offset_polygon const& poly)
{
    auto axes = std::vector<sgfx::vec>{};
    transform(edges(poly), back_inserter(axes), [](auto& edge) { return perpendicular(edge); });
    return axes;
}

sgfx::vec project(offset_polygon const& poly, sgfx::vec const& axis)
{
    auto const edges = ::edges(poly);

    // auto minimum = edges[0] * axis;
    // auto maximum = minimum;
    // for (size_t i = 1; i < edges.size(); ++i)
    //{
    //	auto p = axis * edges[i];
    //	minimum = std::min(minimum, p);
    //	maximum = std::max(maximum, p);
    //}
    // return {minimum, maximum};

    auto const minimum =
        accumulate(edges, edges[0] * axis, [&](auto a, auto e) { return std::min(a, e * axis); });

    auto const maximum =
        accumulate(edges, edges[0] * axis, [&](auto a, auto e) { return std::max(a, e * axis); });

    return {minimum, maximum};
}

void draw_outline(sgfx::canvas_view target, const offset_polygon& poly, sgfx::color::rgb_color col)
{
    for (std::size_t i = 0; i < poly.points.size() - 1; ++i)
        sgfx::line(target, poly.points[i] + poly.offset, poly.points[i + 1] + poly.offset, col);
    sgfx::line(target, poly.points.front() + poly.offset, poly.points.back() + poly.offset, col);
}

constexpr bool overlap(sgfx::vec v, sgfx::vec w)
{
    return false; // TODO return (w - v) < 0;
}

bool sat_collide(const offset_polygon& poly0, const offset_polygon& poly1)
{
    using namespace std;
    using sgfx::vec;

    auto const axes0 = axes(poly0);
    auto const axes1 = axes(poly1);

    for (auto const axes : {axes0, axes1})
    {
        for (auto const axis : axes)
        {
			auto const p1 = project(poly0, axis);
			auto const p2 = project(poly1, axis);
			if (overlap(p1, p2))
				return true;
		}
    }

    return false;
}

int main(int argc, char* argv[])
{
    using namespace sgfx;

    window main_window{1024, 768, "SAT-Test"};

    offset_polygon poly0{convex_hull(random_polygon({300, 300}, 10)), {0, 0}};
    offset_polygon poly1{convex_hull(random_polygon({300, 300}, 10)), {400, 0}};

    while (main_window.handle_events() && !main_window.should_close())
    {
        if (main_window.is_pressed(key_id{'w'}))
            poly0.offset.y -= 1;
        if (main_window.is_pressed(key_id{'s'}))
            poly0.offset.y += 1;
        if (main_window.is_pressed(key_id{'a'}))
            poly0.offset.x -= 1;
        if (main_window.is_pressed(key_id{'d'}))
            poly0.offset.x += 1;

        if (main_window.is_pressed(key::up))
            poly1.offset.y -= 1;
        if (main_window.is_pressed(key::down))
            poly1.offset.y += 1;
        if (main_window.is_pressed(key::left))
            poly1.offset.x -= 1;
        if (main_window.is_pressed(key::right))
            poly1.offset.x += 1;

        clear(main_window, color::black);

        const auto col = sat_collide(poly0, poly1) ? color::red : color::white;

        draw_outline(main_window, poly0, col);
        draw_outline(main_window, poly1, col);

        main_window.show();
    }
    return 0;
}
