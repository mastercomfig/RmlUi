#include "../../../Include/RmlUi/Core/Elements/ElementFormControlSelect.h"
#include "../../../Include/RmlUi/Core/ElementText.h"
#include "../../../Include/RmlUi/Core/ElementUtilities.h"
#include "../../../Include/RmlUi/Core/Event.h"
#include "WidgetDropDown.h"

namespace Rml {

RMLUI_RTTI_Define(ElementFormControlSelect)

ElementFormControlSelect::ElementFormControlSelect(const String& tag) : ElementFormControl(tag), widget(nullptr)
{
	widget = new WidgetDropDown(this);
}

ElementFormControlSelect::~ElementFormControlSelect()
{
	delete widget;
}

String ElementFormControlSelect::GetValue() const
{
	return GetAttribute("value", String());
}

void ElementFormControlSelect::SetValue(const String& value)
{
	MoveChildren();

	SetAttribute("value", value);
}

void ElementFormControlSelect::SetSelection(int selection)
{
	MoveChildren();

	widget->SetSelection(widget->GetOption(selection));
}

int ElementFormControlSelect::GetSelection() const
{
	return widget->GetSelection();
}

Element* ElementFormControlSelect::GetOption(int index)
{
	MoveChildren();

	return widget->GetOption(index);
}

int ElementFormControlSelect::GetNumOptions()
{
	MoveChildren();

	return widget->GetNumOptions();
}

int ElementFormControlSelect::Add(const String& rml, const String& value, int before, bool selectable)
{
	MoveChildren();

	return widget->AddOption(rml, value, before, false, selectable);
}

int ElementFormControlSelect::Add(ElementPtr element, int before)
{
	MoveChildren();

	return widget->AddOption(std::move(element), before);
}

void ElementFormControlSelect::Remove(int index)
{
	MoveChildren();

	widget->RemoveOption(index);
}

void ElementFormControlSelect::RemoveAll()
{
	MoveChildren();

	widget->ClearOptions();
}

void ElementFormControlSelect::ShowSelectBox()
{
	widget->ShowSelectBox();
}

void ElementFormControlSelect::HideSelectBox()
{
	widget->HideSelectBox();
}
void ElementFormControlSelect::CancelSelectBox()
{
	widget->CancelSelectBox();
}

bool ElementFormControlSelect::IsSelectBoxVisible()
{
	return widget->IsSelectBoxVisible();
}

void ElementFormControlSelect::OnUpdate()
{
	ElementFormControl::OnUpdate();

	MoveChildren();

	widget->OnUpdate();
}

void ElementFormControlSelect::OnRender()
{
	ElementFormControl::OnRender();

	widget->OnRender();
}

void ElementFormControlSelect::OnLayout()
{
	widget->OnLayout();
}

void ElementFormControlSelect::OnChildAdd(Element* child)
{
	if (widget)
		widget->OnChildAdd(child);
}

void ElementFormControlSelect::OnChildRemove(Element* child)
{
	if (widget)
		widget->OnChildRemove(child);
}

void ElementFormControlSelect::MoveChildren()
{
	// Move any child elements into the widget (except for the three functional elements).
	while (Element* raw_child = GetFirstChild())
	{
		ElementPtr child = RemoveChild(raw_child);
		widget->AddOption(std::move(child), -1);
	}
}

bool ElementFormControlSelect::GetIntrinsicDimensions(Vector2f& intrinsic_dimensions, float& /*ratio*/)
{
	const float dp = ElementUtilities::GetDensityIndependentPixelRatio(this);
	float text_width = 0.0f;
	if (widget)
	{
		Element* option = widget->GetOption(widget->GetSelection());
		if (option)
		{
			String text;
			option->GetInnerRML(text);
			if (!text.empty())
			{
				text_width = (float)ElementUtilities::GetStringWidth(this, text);
			}
		}
	}

	if (text_width > 0.0f)
	{
		intrinsic_dimensions.x = text_width + 48.0f * dp;
	}
	else
	{
		intrinsic_dimensions.x = 128.0f * dp;
	}
	intrinsic_dimensions.y = GetLineHeight();

	return true;
}

void ElementFormControlSelect::OnAttributeChange(const ElementAttributes& changed_attributes)
{
	RMLUI_ASSERT(widget);

	ElementFormControl::OnAttributeChange(changed_attributes);

	auto it = changed_attributes.find("value");
	if (it != changed_attributes.end())
		widget->OnValueChange(it->second.Get<String>());
}

} // namespace Rml
