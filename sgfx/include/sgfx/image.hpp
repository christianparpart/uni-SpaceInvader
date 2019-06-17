#ifndef SGFX_IMAGE_H
#define SGFX_IMAGE_H

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>

#include <string>

namespace sgfx {
canvas load_ppm(const std::string& path);
void save_ppm(canvas_view source, const std::string& path);

class rle_image;

rle_image load_rle(const std::string& path);
void save_rle(const rle_image& source, const std::string& path);
rle_image rle_encode(canvas_view source);
void draw(canvas_view target, const rle_image& source, point top_left);
void draw(canvas_view target, const rle_image& source, point top_left, color::rgb_color colorkey);

class rle_image {
  public:
    auto width() const { return width_; }
    auto height() const { return height_; }

  private:
    rle_image() = default;
    std::uint16_t width_, height_;
    std::vector<std::uint16_t> line_runs;
    struct run {
        std::uint8_t num;
        sgfx::color::rgb_color color;
    };
    std::vector<run> runs;

    friend rle_image sgfx::load_rle(const std::string& filename);
    friend rle_image sgfx::rle_encode(canvas_view src);
    friend void sgfx::save_rle(const rle_image& src, const std::string& fileame);

    friend void sgfx::draw(canvas_view target, const rle_image& source, point top_left);
    friend void sgfx::draw(canvas_view target, const rle_image& source, point top_left,
                           color::rgb_color colorkey);
};
}  // namespace sgfx

#endif
