#include "stdafx.h"
#include "ui/Panel.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"
#include "utils.h"

const int SLIDER_VAL_MAX = 10000;

vui::Panel::Panel() : Widget() {
    m_sliders[0] = new Slider;
    m_sliders[1] = new Slider;
    updateColor();
    addWidget(m_sliders[0]);
    addWidget(m_sliders[1]);
    m_sliders[0]->ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    m_sliders[1]->ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    updateSliders();
}

vui::Panel::Panel(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel() {
    m_name = name;
    setDestRect(destRect);
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
}

vui::Panel::Panel(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel(name, destRect) {
    parent->addWidget(this);
}

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);
    // Make copy
    m_drawnRect = m_drawableRect;


    m_sliders[0]->addDrawables(renderer);
    m_sliders[1]->addDrawables(renderer);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Panel::refreshDrawables));

}

void vui::Panel::removeDrawables() {
    Widget::removeDrawables();
    m_sliders[0]->removeDrawables();
    m_sliders[1]->removeDrawables();
}

bool vui::Panel::addWidget(Widget* child) {
    bool rv = IWidgetContainer::addWidget(child);
    updateSliders();
    return rv;
}

void vui::Panel::updatePosition() {
    f32v2 newPos = m_relativePosition;
    if (m_parent) newPos += m_parent->getPosition();
    m_position = newPos;

    if (m_parent) computeClipRect(m_parent->getClipRect());

    // Use child offset for auto-scroll
    m_position -= m_childOffset;
    // Update child positions
    for (auto& w : m_widgets) {
        w->updatePosition();
    }
    m_position += m_childOffset;

    if (m_autoScroll) {
        m_sliders[0]->updatePosition();
        m_sliders[1]->updatePosition();
    }

    std::cout << m_clipRect.x << " " << m_clipRect.y << " " << m_clipRect.z << " " << m_clipRect.w << std::endl;

    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    m_drawableRect.setClipRect(m_clipRect);
    refreshDrawables();
}

void vui::Panel::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);
    refreshDrawables();
}

void vui::Panel::setDestRect(const f32v4& destRect) {
    vui::Widget::setDestRect(destRect);
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(m_position);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(m_position.x);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(m_position.y);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setColor(const color4& color) {
    m_backColor = color;
    updateColor();
}

void vui::Panel::setHoverColor(const color4& color) {
    m_backHoverColor = color;
    updateColor();
}

void vui::Panel::setAutoScroll(bool autoScroll) {
    if (autoScroll != m_autoScroll) {
        m_autoScroll = autoScroll;
        updateSliders();
        updatePosition();
    }
}

void vui::Panel::updateColor() {
    if (m_isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
    refreshDrawables();
}

void vui::Panel::updateSliders() {
    bool needsHorizontal = false;
    bool needsVertical = false;
    // Check which scroll bars we need
    maxX = FLT_MIN;
    maxY = FLT_MIN;
    minX = FLT_MAX;
    minY = FLT_MAX;
    if (m_autoScroll) {     
        for (auto& w : m_widgets) {
            const f32v2& pos = w->getPosition();
            const f32v2& dims = w->getDimensions();
            if (pos.x < minX) {
                minX = pos.x;
            }
            if (pos.y < minY) {
                minY = pos.y;
            }
            if (pos.x + dims.x > maxX) {
                maxY = pos.x + dims.x;
            }
            if (pos.y + dims.y > maxY) {
                maxY = pos.y + dims.y;
            }
        }
        if ((maxX > m_position.x + m_dimensions.x) || (minX < m_position.x)) {
            needsVertical = true;
        }
        if ((maxY > m_position.y + m_dimensions.y) || (minY < m_position.y)) {
            needsHorizontal = true;
        }
    }
    if (minX > m_position.x) minX = m_position.x;
    if (maxX < m_position.x + m_dimensions.x) {
        maxX = m_position.x + m_dimensions.x;
    }
    if (minY > m_position.y) minY = m_position.y;
    if (maxY < m_position.y + m_dimensions.y) {
        maxY = m_position.y + m_dimensions.y;
    }

    if (needsHorizontal) {
        m_sliders[0]->enable();
        m_sliders[0]->setPosition(f32v2(0.0f, m_dimensions.y - m_sliderWidth));
        m_sliders[0]->setDimensions(f32v2(m_dimensions.x - m_sliderWidth, m_sliderWidth));
        m_sliders[0]->setSlideDimensions(f32v2(m_sliderWidth));
        m_sliders[0]->setRange(0, SLIDER_VAL_MAX);
        m_sliders[0]->setIsVertical(false);
    } else {
        m_sliders[0]->setDimensions(f32v2(0.0f));
        m_sliders[0]->setSlideDimensions(f32v2(0.0f));
        m_sliders[0]->disable();
    }
    if (needsVertical) {
        m_sliders[1]->enable();
        m_sliders[1]->setPosition(f32v2(m_dimensions.x - m_sliderWidth, 0));
        m_sliders[1]->setDimensions(f32v2(m_sliderWidth, m_dimensions.y - m_sliderWidth));
        m_sliders[1]->setSlideDimensions(f32v2(m_sliderWidth));
        m_sliders[1]->setRange(0, SLIDER_VAL_MAX);
        m_sliders[1]->setIsVertical(true);
    } else {
        m_sliders[1]->setDimensions(f32v2(0.0f));
        m_sliders[1]->setSlideDimensions(f32v2(0.0f));
        m_sliders[1]->disable();
    }
}

void vui::Panel::refreshDrawables() {
    m_drawnRect = m_drawableRect;
}

void vui::Panel::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
            updateColor();
        }
        MouseMove(e);
    } else {
        if (m_isMouseIn) {
            m_isMouseIn = false;
            MouseLeave(e);
            updateColor();
        }
    }
}

void vui::Panel::onMouseFocusLost(Sender s, const MouseEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
        updateColor();
    }
}

void vui::Panel::onSliderValueChange(Sender s, int v) {
    if (m_autoScroll) {
        f32 r = (f32)v / SLIDER_VAL_MAX;
        if ((Slider*)s == m_sliders[0]) {
            // Horizontal
            f32 range = maxX - minX;
            m_childOffset.x = minX + range * r - m_position.x;
        } else {
            // Vertical
            f32 range = maxY - minY;
            m_childOffset.y = minY + range * r - m_position.y;
        }
    }
    updatePosition();
}