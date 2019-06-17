#include <sgfx/image.hpp>
#include <sgfx/io_helper.hpp>

#include <algorithm>
#include <fstream>
#include <numeric>
#include <stdexcept>
#include <string>

#include <cstdint>

using namespace sgfx;

canvas sgfx::load_ppm(const std::string& path)
{
    std::ifstream in{path};
    in.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    std::string tmp;

    auto ignore_comments = [&]() -> bool {
        while (std::isspace(in.peek()) && in.get())
            ;
        while (in.peek() == '#' && std::getline(in, tmp))
            ;
        return !!in;
    };

    in >> tmp;
    if (tmp != "P3")
        throw std::runtime_error{"unsupported PPM type"};

    ignore_comments();

    int w, h;
    in >> w >> h;

    if (w < 0 || h < 0)
        throw std::runtime_error{"unsupported dimensions"};

    canvas ret_val{{(std::uint16_t) w, (std::uint16_t) h}};

    int max_val;
    in >> max_val;
    if (max_val <= 0)
        throw std::runtime_error{"maximum color value has to be bigger 0"};

    for (int i = 0; i < w * h; ++i)
    {
        int r, g, b;
        in >> r >> g >> b;
        r = 255 * r / max_val;
        g = 255 * g / max_val;
        b = 255 * b / max_val;
        ret_val.pixels()[i] = color::rgb_color(r, g, b);
    }

    return ret_val;
}

void sgfx::save_ppm(canvas_view img, const std::string& filename)
{
    std::ofstream out{filename};
    out.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    out << "P3\n# Comment\n";
    out << img.width() << ' ' << img.height() << '\n';
    out << "255\n";

    for (int i = 0; i < img.width() * img.height(); ++i)
    {
        out << (int) img.pixels()[i].red() << '\n';
        out << (int) img.pixels()[i].green() << '\n';
        out << (int) img.pixels()[i].blue() << '\n';
    }
}

rle_image sgfx::load_rle(const std::string& filename)
{
    std::ifstream in{filename, std::ios_base::binary};
    in.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    rle_image ret_val;

    ret_val.width_ = read_le<std::uint16_t>(in);
    ret_val.height_ = read_le<std::uint16_t>(in);

    for (std::uint16_t y = 0; y < ret_val.height_; ++y)
    {
        ret_val.line_runs.push_back(read_le<std::uint16_t>(in));

        for (std::uint16_t i = 0; i < ret_val.line_runs.back(); ++i)
        {
            ret_val.runs.push_back({});
            in.read((char*) &ret_val.runs.back().num, 1);

            auto r = read_le<std::uint8_t>(in);
            auto g = read_le<std::uint8_t>(in);
            auto b = read_le<std::uint8_t>(in);

            ret_val.runs.back().color = {r, g, b};
        }
    }

    return ret_val;
}

void sgfx::save_rle(const rle_image& src, const std::string& filename)
{
    std::ofstream out{filename, std::ios_base::binary};
    out.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    write_le(out, src.width_);
    write_le(out, src.height_);

    std::size_t run_offset = 0;
    for (std::uint16_t y = 0; y < src.height_; ++y)
    {
        write_le(out, src.line_runs[y]);

        for (std::uint16_t i = 0; i < src.line_runs[y]; ++i)
        {
            write_le(out, src.runs[run_offset + i].num);

            const auto c = src.runs[run_offset + i].color;
            const std::uint8_t r = c.red(), g = c.green(), b = c.blue();

            write_le(out, r);
            write_le(out, g);
            write_le(out, b);
        }

        run_offset += src.line_runs[y];
    }
}

rle_image sgfx::rle_encode(canvas_view src)
{
    rle_image ret_val;
    ret_val.width_ = src.width();
    ret_val.height_ = src.height();

    for (std::uint16_t y = 0; y < src.height(); ++y)
    {
        ret_val.line_runs.push_back(0);
        auto current_color = src.pixels()[y * src.width() + 0];
        rle_image::run current_run = {0, current_color};
        for (std::uint16_t x = 0; x < src.width(); ++x)
        {
            if (src.pixels()[y * src.width() + x] == current_run.color)
            {
                if (current_run.num == 255)
                {
                    ret_val.runs.push_back(current_run);
                    ++ret_val.line_runs.back();
                    current_run = {1, src.pixels()[y * src.width() + x]};
                }
                else
                    ++current_run.num;
            }
            else
            {
                ret_val.runs.push_back(current_run);
                ++ret_val.line_runs.back();
                current_run = {1, src.pixels()[y * src.width() + x]};
            }
        }

        ret_val.runs.push_back(current_run);
        ++ret_val.line_runs.back();
    }

    return ret_val;
}

void sgfx::draw(canvas_view target, const rle_image& source, point top_left)
{
    if (top_left.y >= target.height() || top_left.y + source.height_ < 0)
        return;
    if (top_left.x >= target.width() || top_left.x + source.width_ < 0)
        return;

    const auto y_start = std::max(0, -top_left.y);
    const auto y_end =
        top_left.y + source.height_ < target.height() ? source.height_ : (target.height() - top_left.y);

    std::size_t run_offset =
        std::accumulate(std::begin(source.line_runs), std::begin(source.line_runs) + y_start, 0);
    for (std::uint16_t y = y_start; y < y_end; ++y)
    {
        int x_offset = top_left.x;
        int y_offset = top_left.y;
        for (std::uint16_t r = 0; r < source.line_runs[y]; ++r)
        {
            if (x_offset >= target.width())
                break;

            if (x_offset + source.runs[run_offset + r].num >= 0)
            {
                const auto x_start = std::max(x_offset, 0);
                const auto x_end =
                    std::min(x_offset + source.runs[run_offset + r].num, static_cast<int>(target.width()));
                std::fill(&target.pixels()[(y_offset + y) * target.width() + x_start],
                          &target.pixels()[(y_offset + y) * target.width() + x_end],
                          source.runs[run_offset + r].color);
            }

            x_offset += source.runs[run_offset + r].num;
        }
        run_offset += source.line_runs[y];
    }
}

void sgfx::draw(canvas_view target, const rle_image& source, point top_left, color::rgb_color colorkey)
{
    if (top_left.y >= target.height() || top_left.y + source.height_ < 0)
        return;
    if (top_left.x >= target.width() || top_left.x + source.width_ < 0)
        return;

    const auto y_start = std::max(0, -top_left.y);
    const auto y_end =
        top_left.y + source.height_ < target.height() ? source.height_ : (target.height() - top_left.y);

    std::size_t run_offset =
        std::accumulate(std::begin(source.line_runs), std::begin(source.line_runs) + y_start, 0);
    for (std::uint16_t y = y_start; y < y_end; ++y)
    {
        int x_offset = top_left.x;
        int y_offset = top_left.y;
        for (std::uint16_t r = 0; r < source.line_runs[y]; ++r)
        {
            if (x_offset >= target.width())
                break;

            if (x_offset + source.runs[run_offset + r].num >= 0
                && source.runs[run_offset + r].color != colorkey)
            {
                const auto x_start = std::max(x_offset, 0);
                const auto x_end =
                    std::min(x_offset + source.runs[run_offset + r].num, static_cast<int>(target.width()));
                std::fill(&target.pixels()[(y_offset + y) * target.width() + x_start],
                          &target.pixels()[(y_offset + y) * target.width() + x_end],
                          source.runs[run_offset + r].color);
            }

            x_offset += source.runs[run_offset + r].num;
        }
        run_offset += source.line_runs[y];
    }
}
