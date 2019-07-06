#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {
        /**************\
         *    Bool    *
        \**************/

        bool parseBool(keg::YAMLNode value, OUT bool& val) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            val = value.data[0].as<bool>();

            return true;
        }

        /****************\
         *    String    *
        \****************/

        bool parseString(keg::YAMLNode value, OUT nString& str) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            str = value.data[0].as<nString>();

            return true;
        }

        /*****************\
         *    Vectors    *
        \*****************/

#define PARSE_VEC2(TYPE)                                        \
        bool parseVec2(keg::YAMLNode value, OUT TYPE##v2& vec) {    \
            if (keg::getType(&value) != keg::NodeType::SEQUENCE  \
                    || value.data.size() != 2) return false;   \
                                                                \
            vec[0] = value.data[0].as<TYPE>();                 \
            vec[1] = value.data[1].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

        PARSE_VEC2(i8)
        PARSE_VEC2(i16)
        PARSE_VEC2(i32)
        PARSE_VEC2(i64)
        PARSE_VEC2(ui8)
        PARSE_VEC2(ui16)
        PARSE_VEC2(ui32)
        PARSE_VEC2(ui64)
        PARSE_VEC2(f32)
        PARSE_VEC2(f64)

#define PARSE_VEC3(TYPE)                                        \
        bool parseVec3(keg::YAMLNode value, OUT TYPE##v3& vec) {    \
            if (keg::getType(&value) != keg::NodeType::SEQUENCE  \
                    || value.data.size() != 3) return false;   \
                                                                \
            vec[0] = value.data[0].as<TYPE>();                 \
            vec[1] = value.data[1].as<TYPE>();                 \
            vec[2] = value.data[2].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

        PARSE_VEC3(i8)
        PARSE_VEC3(i16)
        PARSE_VEC3(i32)
        PARSE_VEC3(i64)
        PARSE_VEC3(ui8)
        PARSE_VEC3(ui16)
        PARSE_VEC3(ui32)
        PARSE_VEC3(ui64)
        PARSE_VEC3(f32)
        PARSE_VEC3(f64)

#define PARSE_VEC4(TYPE)                                        \
        bool parseVec4(keg::YAMLNode value, OUT TYPE##v4& vec) {    \
            if (keg::getType(&value) != keg::NodeType::SEQUENCE  \
                    || value.data.size() != 4) return false;   \
                                                                \
            vec[0] = value.data[0].as<TYPE>();                 \
            vec[1] = value.data[1].as<TYPE>();                 \
            vec[2] = value.data[2].as<TYPE>();                 \
            vec[3] = value.data[3].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

        PARSE_VEC4(i8)
        PARSE_VEC4(i16)
        PARSE_VEC4(i32)
        PARSE_VEC4(i64)
        PARSE_VEC4(ui8)
        PARSE_VEC4(ui16)
        PARSE_VEC4(ui32)
        PARSE_VEC4(ui64)
        PARSE_VEC4(f32)
        PARSE_VEC4(f64)

        /********************\
         *    Text Align    *
        \********************/

        bool parsePartTextAlign(keg::YAMLNode value, OUT vg::TextAlign& textAlign, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "none") {
                textAlign = vg::TextAlign::NONE;
                return true;
            } else if (name == "left") {
                textAlign = vg::TextAlign::LEFT;
                return true;
            } else if (name == "top_left") {
                textAlign = vg::TextAlign::TOP_LEFT;
                return true;
            } else if (name == "top") {
                textAlign = vg::TextAlign::TOP;
                return true;
            } else if (name == "top_right") {
                textAlign = vg::TextAlign::TOP_RIGHT;
                return true;
            } else if (name == "right") {
                textAlign = vg::TextAlign::RIGHT;
                return true;
            } else if (name == "bottom_right") {
                textAlign = vg::TextAlign::BOTTOM_RIGHT;
                return true;
            } else if (name == "bottom") {
                textAlign = vg::TextAlign::BOTTOM;
                return true;
            } else if (name == "bottom_left") {
                textAlign = vg::TextAlign::BOTTOM_LEFT;
                return true;
            } else if (name == "center") {
                textAlign = vg::TextAlign::CENTER;
                return true;
            }

            return false;
        }

        bool parseTextAlign(keg::YAMLNode value, OUT vg::TextAlign& textAlign) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartTextAlign(value, textAlign);
        }

        /************************\
         *    Clipping State    *
        \************************/

        bool parsePartClippingState(keg::YAMLNode value, OUT vui::ClippingState& clippingState, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "visible") {
                clippingState = vui::ClippingState::VISIBLE;
                return true;
            } else if (name == "hidden") {
                clippingState = vui::ClippingState::HIDDEN;
                return true;
            } else if (name == "inherit") {
                clippingState = vui::ClippingState::INHERIT;
                return true;
            }

            return false;
        }

        bool parseClippingState(keg::YAMLNode value, OUT vui::ClippingState& clippingState) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartClippingState(value, clippingState);
        }

        /******************\
         *    Clipping    *
        \******************/

        bool parseClipping(keg::YAMLNode value, OUT vui::ClippingState* clipping) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 4) return false;

            if (!(parseClippingState(value.data["left"].as<nString>(),        clipping[0])
                    && parseClippingState(value.data["top"].as<nString>(),    clipping[1])
                    && parseClippingState(value.data["right"].as<nString>(),  clipping[2])
                    && parseClippingState(value.data["bottom"].as<nString>(), clipping[3]))) {
                return false;
            }

            return true;
        }

        /********************\
         *    Dock State    *
        \********************/

        bool parsePartDockState(keg::YAMLNode value, OUT vui::DockState& dockState, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "none") {
                dockState = vui::DockState::NONE;
                return true;
            } else if (name == "left") {
                dockState = vui::DockState::LEFT;
                return true;
            } else if (name == "right") {
                dockState = vui::DockState::RIGHT;
                return true;
            } else if (name == "bottom") {
                dockState = vui::DockState::BOTTOM;
                return true;
            } else if (name == "top") {
                dockState = vui::DockState::TOP;
                return true;
            } else if (name == "fill") {
                dockState = vui::DockState::FILL;
                return true;
            }

            return false;
        }

        bool parseDockState(keg::YAMLNode value, OUT vui::DockState& dockState) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartDockState(value, dockState);
        }

        /********************\
         *       Dock       *
        \********************/

        bool parseDock(keg::YAMLNode value, OUT vui::Dock& dock) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 2) return false;

            if (!value.data["state"] || !value.data["size"]) return false;

            if (!parseDockState({ value.data["state"] }, dock.state)) return false;

            dock.size = value.data["size"].as<f32>();
        }

        /***********************\
         *    Position Type    *
        \***********************/

        bool parsePartPositionType(keg::YAMLNode value, OUT vui::PositionType& positionType, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "static_to_window") {
                positionType = vui::PositionType::STATIC_TO_WINDOW;
                return true;
            } else if (name == "static_to_viewport") {
                positionType = vui::PositionType::STATIC_TO_VIEWPORT;
                return true;
            } else if (name == "static_to_parent") {
                positionType = vui::PositionType::STATIC_TO_PARENT;
                return true;
            } else if (name == "relative_to_window") {
                positionType = vui::PositionType::RELATIVE_TO_WINDOW;
                return true;
            } else if (name == "relative_to_viewport") {
                positionType = vui::PositionType::RELATIVE_TO_VIEWPORT;
                return true;
            } else if (name == "relative_to_parent") {
                positionType = vui::PositionType::RELATIVE_TO_PARENT;
                return true;
            }

            return false;
        }

        bool parsePositionType(keg::YAMLNode value, OUT vui::PositionType& positionType) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartPositionType(value, positionType);
        }

        /************************\
         *    Dimension Type    *
        \************************/

        bool parsePartDimensionType(keg::YAMLNode value, OUT vui::DimensionType& dimensionType, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "pixel") {
                dimensionType = vui::DimensionType::PIXEL;
                return true;
            } else if (name == "width_percentage") {
                dimensionType = vui::DimensionType::WIDTH_PERCENTAGE;
                return true;
            } else if (name == "height_percentage") {
                dimensionType = vui::DimensionType::HEIGHT_PERCENTAGE;
                return true;
            } else if (name == "min_percentage") {
                dimensionType = vui::DimensionType::MIN_PERCENTAGE;
                return true;
            } else if (name == "max_percentage") {
                dimensionType = vui::DimensionType::MAX_PERCENTAGE;
                return true;
            } else if (name == "parent_width_percentage") {
                dimensionType = vui::DimensionType::PARENT_WIDTH_PERCENTAGE;
                return true;
            } else if (name == "parent_height_percentage") {
                dimensionType = vui::DimensionType::PARENT_HEIGHT_PERCENTAGE;
                return true;
            } else if (name == "parent_min_percentage") {
                dimensionType = vui::DimensionType::PARENT_MIN_PERCENTAGE;
                return true;
            } else if (name == "parent_max_percentage") {
                dimensionType = vui::DimensionType::PARENT_MAX_PERCENTAGE;
                return true;
            } else if (name == "viewport_width_percentage") {
                dimensionType = vui::DimensionType::VIEWPORT_WIDTH_PERCENTAGE;
                return true;
            } else if (name == "viewport_height_percentage") {
                dimensionType = vui::DimensionType::VIEWPORT_HEIGHT_PERCENTAGE;
                return true;
            } else if (name == "viewport_min_percentage") {
                dimensionType = vui::DimensionType::VIEWPORT_MIN_PERCENTAGE;
                return true;
            } else if (name == "viewport_max_percentage") {
                dimensionType = vui::DimensionType::VIEWPORT_MAX_PERCENTAGE;
                return true;
            } else if (name == "window_width_percentage") {
                dimensionType = vui::DimensionType::WINDOW_WIDTH_PERCENTAGE;
                return true;
            } else if (name == "window_height_percentage") {
                dimensionType = vui::DimensionType::WINDOW_HEIGHT_PERCENTAGE;
                return true;
            } else if (name == "window_min_percentage") {
                dimensionType = vui::DimensionType::WINDOW_MIN_PERCENTAGE;
                return true;
            } else if (name == "window_max_percentage") {
                dimensionType = vui::DimensionType::WINDOW_MAX_PERCENTAGE;
                return true;
            }

            return false;
        }

        bool parseDimensionType(keg::YAMLNode value, OUT vui::DimensionType& dimensionType) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartDimensionType(value, dimensionType);
        }

        /**********************\
         *       Length       *
        \**********************/

        bool parseLength(keg::YAMLNode value, OUT vui::Length& length) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 2) return false;

            if (!(value.data["x"] && value.data["dim_x"])) return false;

            vui::DimensionType dimType;
            if (!parseDimensionType(value.data["dim_x"].as<nString>(), dimType)) return false;

            length.x = value.data["x"].as<f32>();
            length.dimension.x = dimType;

            return true;
        }

        bool parseLength2(keg::YAMLNode value, OUT vui::Length2& length) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 4) return false;

            if (!(value.data["x"] && value.data["dim_x"]
                    && value.data["y"] && value.data["dim_y"])) return false;

            vui::DimensionType dimType1, dimType2;
            if (!(parseDimensionType(value.data["dim_x"].as<nString>(), dimType1)
                    && parseDimensionType(value.data["dim_y"].as<nString>(), dimType2))) return false;

            length.x = value.data["x"].as<f32>();
            length.y = value.data["y"].as<f32>();
            length.dimension.x = dimType1;
            length.dimension.y = dimType2;

            return true;
        }

        bool parseLength4(keg::YAMLNode value, OUT vui::Length4& length) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 8) return false;

            if (!(value.data["x"] && value.data["dim_x"]
                    && value.data["y"] && value.data["dim_y"]
                    && value.data["z"] && value.data["dim_z"]
                    && value.data["w"] && value.data["dim_w"])) return false;

            vui::DimensionType dimType1, dimType2, dimType3, dimType4;
            if (!(parseDimensionType(value.data["dim_x"].as<nString>(), dimType1)
                    && parseDimensionType(value.data["dim_y"].as<nString>(), dimType2)
                    && parseDimensionType(value.data["dim_z"].as<nString>(), dimType3)
                    && parseDimensionType(value.data["dim_w"].as<nString>(), dimType4))) return false;

            length.x = value.data["x"].as<f32>();
            length.y = value.data["y"].as<f32>();
            length.z = value.data["z"].as<f32>();
            length.w = value.data["w"].as<f32>();
            length.dimension.x = dimType1;
            length.dimension.y = dimType2;
            length.dimension.z = dimType3;
            length.dimension.w = dimType4;

            return true;
        }

        /*******************\
         *      Color      *
        \*******************/

        bool parsePartColor(keg::YAMLNode value, OUT color4& color, size_t offset = 0) {
            color.r = value.data[offset + 0].as<ui8>();
            color.g = value.data[offset + 1].as<ui8>();
            color.b = value.data[offset + 2].as<ui8>();
            color.a = value.data[offset + 3].as<ui8>();

            return true;
        }

        bool parseColor(keg::YAMLNode value, OUT color4& color) {
            if (keg::getType(&value) != keg::NodeType::SEQUENCE
                    || value.data.size() != 4) return false;

            return parsePartColor(value, color);
        }

        /***********************\
         *    Gradient Type    *
        \***********************/

        bool parsePartGradientType(keg::YAMLNode value, OUT vg::GradientType& gradType, size_t offset = 0) {
            nString name = value.data[offset].as<nString>();

            if (name == "none") {
                gradType = vg::GradientType::NONE;
                return true;
            } else if (name == "horizontal") {
                gradType = vg::GradientType::HORIZONTAL;
                return true;
            } else if (name == "vertical") {
                gradType = vg::GradientType::VERTICAL;
                return true;
            } else if (name == "left_diagonal") {
                gradType = vg::GradientType::LEFT_DIAGONAL;
                return true;
            } else if (name == "right_diagonal") {
                gradType = vg::GradientType::RIGHT_DIAGONAL;
                return true;
            }

            return false;
        }

        bool parseGradientType(keg::YAMLNode value, OUT vg::GradientType& gradType) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            return parsePartGradientType(value, gradType);
        }

        /******************\
         *    Gradient    *
        \******************/

        bool parseGradient(keg::YAMLNode value, OUT color4& color1, OUT color4& color2, OUT vg::GradientType& gradType) {
            if (keg::getType(&value) != keg::NodeType::MAP
                    || value.data.size() != 3) return false;

            if (!value.data["color1"] || !value.data["color2"]
                    || !value.data["grad_type"]) return false;

            if (!parseColor({ value.data["color1"] }, color1)) return false;
            if (!parseColor({ value.data["color2"] }, color2)) return false;
            if (!parseGradientType({ value.data["grad_type"] }, gradType)) return false;

            return true;
        }
    }
}