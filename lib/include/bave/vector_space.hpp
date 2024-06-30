#pragma once
#include <bave/core/not_null.hpp>
#include <bave/graphics/extent_scaler.hpp>
#include <bave/graphics/render_view.hpp>

namespace bave {
class RenderDevice;

class VectorSpace {
  public:
	explicit VectorSpace(NotNull<RenderDevice const*> render_device);

	[[nodiscard]] auto get_render_device() const -> RenderDevice const& { return *m_render_device; }
	[[nodiscard]] auto get_viewport_scaler() const -> ExtentScaler;

	[[nodiscard]] auto get_framebuffer_size() const -> glm::vec2;
	[[nodiscard]] auto get_size() const -> glm::vec2 { return render_view.viewport; }

	[[nodiscard]] auto project(glm::vec2 fb_point) const -> glm::vec2;
	[[nodiscard]] auto unproject(glm::vec2 pointer) const -> glm::vec2;

	void sync_to_default_view();

	RenderView render_view{};

  private:
	NotNull<RenderDevice const*> m_render_device;
};
} // namespace bave
