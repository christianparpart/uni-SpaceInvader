#pragma once

#include "game.hpp"  // game_object::status

#include <sgfx/canvas.hpp>
#include <chrono>

class game_object;
class asteroid;
class bullet;
class player;
class scrolling_bg;
class spawn_point;

class game_object_visitor {
  public:
    virtual ~game_object_visitor() = default;

    virtual void visit(asteroid&) = 0;
    virtual void visit(bullet&) = 0;
    virtual void visit(player&) = 0;
    virtual void visit(scrolling_bg&) = 0;
    virtual void visit(spawn_point&) = 0;
};

class game_object_updater : public game_object_visitor {
  public:
    virtual ~game_object_updater() = default;

    static game_object::status update(game_proxy proxy, std::chrono::milliseconds delta, game_object& obj);

    void visit(asteroid&) override;
    void visit(bullet&) override;
    void visit(player&) override;
    void visit(scrolling_bg&) override;
    void visit(spawn_point&) override;

  private:
    game_object_updater(game_proxy _proxy, std::chrono::milliseconds _delta)
        : proxy_{std::move(_proxy)}, delta_{std::move(_delta)}, status_{game_object::status::dead}
    {
    }

    game_proxy proxy_;
    std::chrono::milliseconds delta_;
    game_object::status status_;
};

class game_object_drawer : public game_object_visitor {
  public:
    static void draw(sgfx::canvas_view target, game_object& obj);

    void visit(asteroid&) override;
    void visit(bullet&) override;
    void visit(player&) override;
    void visit(scrolling_bg&) override;
    void visit(spawn_point&) override;

  private:
    explicit game_object_drawer(sgfx::canvas_view target) : target_{target} {}

    sgfx::canvas_view target_;
};
