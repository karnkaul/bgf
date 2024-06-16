#pragma once
#include <bave/core/is_positive.hpp>
#include <bave/graphics/render_device.hpp>
#include <bave/services/service.hpp>

namespace bave {
class IDisplay : public IService {
  public:
	[[nodiscard]] virtual auto get_render_device() const -> RenderDevice const& = 0;

	[[nodiscard]] virtual auto get_window_size() const -> glm::vec2 = 0;
	[[nodiscard]] virtual auto get_framebuffer_size() const -> glm::vec2 = 0;

	[[nodiscard]] virtual auto get_world_view() const -> RenderView const& = 0;
	[[nodiscard]] virtual auto get_default_view() const -> RenderView const& = 0;

	virtual void set_world_space(glm::vec2 size) = 0;

	[[nodiscard]] auto get_viewport_scaler() const -> ExtentScaler { return get_render_device().get_viewport_scaler(); }

	[[nodiscard]] auto get_world_space() const -> glm::vec2 { return get_world_view().viewport; }
	[[nodiscard]] auto get_default_space() const -> glm::vec2 { return get_default_view().viewport; }

	[[nodiscard]] auto project_to_world(glm::vec2 fb_point) const -> glm::vec2 { return get_render_device().project_to(get_world_view().viewport, fb_point); }
	[[nodiscard]] auto unproject_to_world(glm::vec2 pointer) const -> glm::vec2 { return unproject(get_world_view(), pointer); }

	[[nodiscard]] auto project_to_default(glm::vec2 fb_point) const -> glm::vec2 {
		return get_render_device().project_to(get_default_view().viewport, fb_point);
	}

	[[nodiscard]] auto unproject_to_default(glm::vec2 pointer) const -> glm::vec2 { return unproject(get_default_view(), pointer); }

	[[nodiscard]] auto unproject(RenderView const& view, glm::vec2 const pointer) const -> glm::vec2 {
		auto const fb_size = get_framebuffer_size();
		if (!is_positive(pointer) || !is_positive(fb_size)) { return pointer; }
		return view.unproject(pointer / fb_size);
	}
};
} // namespace bave
