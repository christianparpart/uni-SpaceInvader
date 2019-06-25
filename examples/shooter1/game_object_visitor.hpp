#pragma once

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
