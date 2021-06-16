#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <ngf/Graphics/Texture.h>
#include <sol/sol.hpp>
#include <CollisionResult.h>
#include <Graphics/Animation.h>
#include <Scripting/EntityManager.h>

struct PositionComponent {
  PositionComponent() = default;
  explicit PositionComponent(const glm::vec2 &p) : pos(p) {}
  explicit PositionComponent(const sol::table &t);

  void setPosition(const glm::vec2 &p);
  [[nodiscard]] glm::vec2 getPosition() const;

private:
  glm::vec2 pos{};
};

struct MotionComponent {
  glm::vec2 velocity{};

  MotionComponent() = default;
  explicit MotionComponent(const glm::vec2 &v) : velocity(v) {}
  explicit MotionComponent(const sol::table &t);
};

struct CollideComponent {
  glm::ivec2 size{};

  CollideComponent() = default;
  explicit CollideComponent(const glm::vec2 &s) : size(s) {}
  explicit CollideComponent(const sol::table &t);
};

struct GraphicComponent {
  std::shared_ptr<ngf::Texture> texture;
  ngf::irect frame;
  glm::vec2 origin{};
  bool visible{true};

  GraphicComponent() = default;
  explicit GraphicComponent(const sol::table &t);
};

struct AnimationComponent {
  std::string current;
  std::unordered_map<std::string, Animation> animations;
  std::size_t frameIndex{0};
  int loop{1};
  int delay{0};
  bool playing{false};

  AnimationComponent() = default;
  explicit AnimationComponent(const sol::table &t);

  void setAnim(const std::string &anim, int loop);
};

struct NameComponent {
  std::string name;

  NameComponent() = default;
  explicit NameComponent(std::string n) : name(std::move(n)) {}
  explicit NameComponent(const sol::table &t);
};

struct HierarchyComponent {
private:
  struct Child {
    Child(const glm::vec2 &o, EntityId id) : offset(o), entityId(id) {}

    glm::vec2 offset;
    EntityId entityId;
  };

public:
  HierarchyComponent() = default;
  explicit HierarchyComponent(const sol::table &t);

  [[nodiscard]] entt::entity getEntity() const;

  void addChild(Entity &entityChild);
  void removeChild(const Entity &entityChild);

  void updateParentOffset();
  void updateChildrenPosition(const glm::vec2& p);

  [[nodiscard]] bool hasParent() const {
    return m_parentId != entt::null;
  }

  [[nodiscard]] std::optional<std::reference_wrapper<Entity>> getParent();

private:
  void updateOffset(entt::entity e);
  [[nodiscard]] glm::vec2 getOffset(entt::entity entityChild) const;

private:
  EntityId m_parentId = entt::null;
  std::vector<Child> m_children;
};
