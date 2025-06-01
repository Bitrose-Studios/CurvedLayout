# Curved Panel – Advanced UMG Layout Plugin for Unreal Engine

**Author:** Peter Gyarmati ([@BitroseStudios](https://x.com/bitrosestudios))  
**Version:** 1.0  
**Engine Compatibility:** Unreal Engine 4.27+

---

## 📦 Overview

The **Curved Panel** plugin extends Unreal’s UMG layout system by introducing a flexible panel that positions child widgets in:

- 📐 **Curved horizontal arcs** (using a float curve for shaping)
- 🧭 **Radial/circular layouts** (full or partial)
- 🔄 **Fully configurable** alignment, spacing, rotation, and more

Perfect for radial menus, skill rings, dialogue wheels, stylized button bars, or emotion selectors.

---

## 🛠 Features

- 🔘 Curved Layout using a `UCurveFloat`
- 🔄 Radial Layout with configurable start/end angles
- ⚙️ Vertical alignment (Top, Middle, Bottom)
- ↕️ Curve magnitude and input scaling
- ↔️ Adjustable element spacing
- 🔁 Curve inversion support
- 🧭 Full radial rotation control
- 🎯 Optional per-widget rotation to match layout arc

---

## 📐 Layout Modes

### 1. **Curved Layout (Arc)**
- Child widgets are placed horizontally.
- Vertical position is offset using a `UCurveFloat`.

### 2. **Radial Layout**
- Widgets are distributed around a circle or arc using start/end angles.

---

## 🎛 Configurable Properties

All settings are available in the UMG Details panel when selecting a `CurvedLayoutPanel`.

> ⚠️ Some advanced properties (e.g. `StartAngle`, `EndAngle`) are hidden in the **Advanced** section.

| Property                 | Description                                                |
|--------------------------|------------------------------------------------------------|
| `Radius`                | Distance from panel center to each widget (radial mode)    |
| `StartAngle`            | Start of the arc (in degrees). Adjusted by `RadialRotation`. |
| `EndAngle`              | End of the arc (in degrees). Adjusted by `RadialRotation`.   |
| `RadialRotation`        | Rotates the entire arc (use -90° to place first at top)     |
| `bRotateTowards`        | Rotate widgets to face outward along the arc                |
| `RotateOffset`          | Extra angle offset per widget                               |
| `bUseCurve`             | Enables `UCurveFloat` vertical shaping                      |
| `Curve`                 | The curve asset controlling vertical offset                 |
| `CurveScale`            | Scales X input to the curve                                 |
| `CurveMagnitude`        | Scales Y output from the curve                              |
| `ElementSpacing`        | Horizontal space between widgets in curved mode            |
| `VerticalAlignmentMode`| Top / Middle / Bottom vertical anchoring                    |
| `bInvertCurve`          | Flips the curve upside-down                                |

---

## 🎨 How to Use

1. Add a `CurvedLayoutPanel` to a `CanvasPanel` in your UMG widget.
2. Add any number of child widgets (buttons, icons, etc.)
3. Configure layout using the exposed parameters.

### 💡 Tips:
- For radial menu: `bUseCurve = false`, `StartAngle = -90`, `EndAngle = 270`
- For smooth arc: Use a sin-like `UCurveFloat`, middle-aligned, with `CurveMagnitude` > 1
- To flip direction: toggle `bInvertCurve` or change `RadialRotation`

---

## 🔧 Advanced Usage

- Simulate full circles with `StartAngle = 0`, `EndAngle = 360`
- Use `VerticalAlignmentMode` to anchor arc vertically
- Use custom `UCurveFloat` assets to control shape

---

## 🚀 Roadmap

- 🧩 Per-widget slot overrides
- 🌀 Elliptical layouts (`FVector2D Radius`)
- 🧮 Explicit LayoutMode enum (Arc / Radial / Linear)
- 🖼 Design-time preview rendering in UMG

---

## 💬 Support & Feedback

- Twitter/X: [@BitroseStudios](https://x.com/bitrosestudios)
- GitHub issue page : [https://github.com/mrgyarmati/CurvedLayout/issues](https://github.com/mrgyarmati/CurvedLayout/issues)


---

**Happy arcing! 🎯**
