#pragma once
#include <bave/graphics/shape.hpp>
#include <bave/ui/element.hpp>

namespace bave::ui {
class OutlineQuad : public IElement {
  public:
	OutlineQuad() { set_size(Quad::size_v); }

	void draw(Shader& shader) const final;

	[[nodiscard]] auto get_size() const -> glm::vec2 final { return m_quad.get_shape().size; }
	void set_size(glm::vec2 size);

	[[nodiscard]] auto get_position() const -> glm::vec2 final { return m_quad.transform.position; }
	void set_position(glm::vec2 position) final;

	void set_scale(glm::vec2 scale);

	void set_corner_ratio(float ratio);
	void set_corner_resolution(int resolution);
	void set_outline_width(float width);
	void set_outline_tint(Rgba tint);
	void set_tint(Rgba tint);

	[[nodiscard]] auto get_background() const -> RoundedQuadShape const& { return m_quad; }
	[[nodiscard]] auto get_outline() const -> RoundedQuadShape const& { return m_outline; }

  private:
	template <typename F>
	void set_quad_and_outline(F func);

	RoundedQuadShape m_quad{};
	RoundedQuadShape m_outline{};
	float m_outline_width{10.0f};
	float m_corner_ratio{0.0f};
};
} // namespace bave::ui
