#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <sgfx/primitives.hpp>

#include <list>

class collider;

class collision_manager {
  public:
    class handle {
      public:
        handle(const handle&) = delete;
        handle& operator=(const handle&) = delete;

        handle(handle&& other) : parent_{other.parent_}, it_{other.it_} { other.parent_ = nullptr; };

        handle& operator=(handle&& other)
        {
            if (this != &other)
            {
                if (parent_)
                    parent_->colliders_.erase(it_);

                parent_ = other.parent_;
                it_ = other.it_;
                other.parent_ = nullptr;
            }
            return *this;
        }

        handle() = default;

        ~handle()
        {
            if (parent_)
                parent_->colliders_.erase(it_);
        }

      private:
        collision_manager* parent_ = nullptr;
        std::list<collider*>::iterator it_;

        handle(collision_manager* parent, std::list<collider*>::iterator it) : parent_{parent}, it_{it} {}

        friend class collision_manager;
    };

    collision_manager() = default;
    collision_manager(const collision_manager&) = delete;
    collision_manager& operator=(const collision_manager&) = delete;

    handle register_collider(collider& new_collider);
    void handle_collisions();

  private:
    std::list<collider*> colliders_;
};

class collider {
  public:
    virtual sgfx::rectangle bounds() const = 0;
    virtual void hit() = 0;

  protected:
    ~collider() = default;
};

#endif
