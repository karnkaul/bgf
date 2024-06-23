#include <bave/graphics/render_device.hpp>
#include <bave/vector_space.hpp>

namespace bave {
VectorSpace::VectorSpace(NotNull<RenderDevice const*> render_device) : render_view(render_device->get_default_view()), m_render_device(render_device) {}

auto VectorSpace::get_framebuffer_size() const -> glm::vec2 {
	auto const ret = m_render_device->get_swapchain_extent();
	return glm::vec2{ret.width, ret.height};
}

auto VectorSpace::get_viewport_scaler() const -> ExtentScaler { return get_render_device().get_viewport_scaler(); }

auto VectorSpace::project(glm::vec2 const fb_point) const -> glm::vec2 { return get_render_device().project_to(render_view.viewport, fb_point); }

auto VectorSpace::unproject(RenderView const& render_view, glm::vec2 const pointer) const -> glm::vec2 {
	auto const fb_size = get_framebuffer_size();
	if (!is_positive(pointer) || !is_positive(fb_size)) { return pointer; }
	return render_view.unproject(pointer / fb_size);
}

void VectorSpace::sync_to_default_view() { render_view = get_render_device().get_default_view(); }
} // namespace bave
