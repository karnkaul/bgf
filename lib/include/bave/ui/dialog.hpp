#pragma once
#include <bave/ui/button.hpp>
#include <bave/ui/outline_quad.hpp>
#include <bave/ui/text.hpp>
#include <bave/ui/view.hpp>

namespace bave::ui {
struct DialogCreateInfo {
	struct ButtonInfo {
		std::string text{};
		std::function<void()> callback{};
	};

	glm::vec2 size{500.0f, 200.0f};
	std::string content_text{"Content text."};
	ButtonInfo main_button{.text = "OK"};
	ButtonInfo second_button{};
};

class Dialog : public ui::View {
  public:
	using CreateInfo = DialogCreateInfo;

	static constexpr auto text_height_v = TextHeight{static_cast<int>(TextHeight::eDefault) + 16};

	explicit Dialog(Services const& services, CreateInfo create_info = {});

  protected:
	DialogStyle m_style{};

	Ptr<OutlineQuad> m_background{};
	Ptr<Text> m_content_text{};
	std::array<Ptr<ui::Button>, 2> m_footer_buttons{};
};
} // namespace bave::ui
