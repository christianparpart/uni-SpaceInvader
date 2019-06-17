#include "quadtree.hpp"

#include <sgfx/color.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

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

void draw_outline(sgfx::canvas_view target, const offset_polygon& poly, sgfx::color::rgb_color col)
{
    for (std::size_t i = 0; i < poly.points.size() - 1; ++i)
        sgfx::line(target, poly.points[i] + poly.offset, poly.points[i + 1] + poly.offset, col);
    sgfx::line(target, poly.points.front() + poly.offset, poly.points.back() + poly.offset, col);
}

void draw_outline(sgfx::canvas_view target, const sgfx::rectangle& rect, sgfx::color::rgb_color col)
{
    sgfx::line(target, rect.top_left, rect.top_left + sgfx::vec{rect.size.w, 0}, col);
    sgfx::line(target, rect.top_left, rect.top_left + sgfx::vec{0, rect.size.h}, col);
    sgfx::line(target, rect.top_left + sgfx::vec{rect.size.w - 1, 0},
               rect.top_left + sgfx::vec{rect.size.w - 1, rect.size.h}, col);
    sgfx::line(target, rect.top_left + sgfx::vec{0, rect.size.h - 1},
               rect.top_left + sgfx::vec{rect.size.w, rect.size.h - 1}, col);
}

template <typename T, std::size_t max_depth>
void debug_draw(sgfx::canvas_view target, const quadtree<T, max_depth>& tree)
{
    for (const auto& n : tree.nodes())
    {
        if (n.elements().begin() != n.elements().end())
        {
            draw_outline(target, n.area(), sgfx::color::red);
        }
    }
}

int main(int argc, char* argv[])
{
    using namespace sgfx;

    window main_window{1024, 768, "Quadtree-Test"};

    std::vector<offset_polygon> polys;
    std::generate_n(std::back_inserter(polys), 100, [&]() {
        const auto poly = convex_hull(random_polygon({20, 20}, 20));
        const auto offset =
            sgfx::vec{random_uniform_int(main_window.width()), random_uniform_int(main_window.height())};
        return offset_polygon{poly, offset};
    });

    quadtree<offset_polygon, 10> qt(sgfx::rectangle{{0, 0}, {main_window.width(), main_window.height()}});

    for (const auto& poly : polys)
        qt.insert(poly);

    auto current_poly_iter = qt.begin();
    std::size_t framecount = 0, max_framecount = 120;

    while (main_window.handle_events() && !main_window.should_close())
    {
        clear(main_window, color::black);

        for (const auto& poly : polys)
            draw_outline(main_window, poly, color::white);

        debug_draw(main_window, qt);

        if (++framecount >= max_framecount)
        {
            if (++current_poly_iter == qt.end())
                current_poly_iter = qt.begin();
            framecount = 0;
        }

        for (const auto& poly : subtree(current_poly_iter))
        {
            draw_outline(main_window, poly, color::blue);
        }
        draw_outline(main_window, *current_poly_iter, color::red);

        main_window.show();
    }
    return 0;
}
