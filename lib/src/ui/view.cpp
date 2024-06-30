#include <bave/ui/view.hpp>

namespace bave::ui {
void View::push(std::unique_ptr<IElement> element) {
	if (!element) { return; }
	m_elements.push_back(std::move(element));
	if (auto* widget = dynamic_cast<IWidget*>(m_elements.back().get())) { m_widgets.push_back(widget); }
}

void View::on_move(PointerMove const& pointer_move) {
	for (auto const& widget : m_widgets) { widget->on_move(pointer_move); }
}

void View::on_tap(PointerTap const& pointer_tap) {
	for (auto const& widget : m_widgets) { widget->on_tap(pointer_tap); }
}

void View::tick(Seconds const dt) {
	for (auto const& widget : m_widgets) { widget->tick(dt); }
}

void View::render(Shader& shader) const {
	if (m_elements.empty()) { return; }
	for (auto const& element : m_elements) { element->draw(shader); }
}
} // namespace bave::ui
