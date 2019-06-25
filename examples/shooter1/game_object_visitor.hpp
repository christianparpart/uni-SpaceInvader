#pragma once

#include <sgfx/canvas.hpp>

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
