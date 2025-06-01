// Copyright Peter Gyarmati (@mrgyarmati)

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SPanel.h"

/**
 * Enum representing vertical alignment behavior for curved layout.
 * - Bottom: Anchors the arc to the bottom of the panel
 * - Middle: Centers the arc vertically
 * - Top: Anchors the arc to the top of the panel
 */
UENUM(BlueprintType)
enum class EVerticalAlignmentMode : uint8
{
	Bottom		UMETA(DisplayName = "Bottom Alignment"),
	Middle		UMETA(DisplayName = "Middle Alignment"),
	Top			UMETA(DisplayName = "Top Alignment")
};

/**
 * Slot type used by the CurvedPanel to wrap child widgets.
 */
struct FCurvedSlot : public FSlotBase
{
	FCurvedSlot(const TSharedRef<SWidget>& InWidget)
		: FSlotBase(InWidget)
	{
	}
};

/**
 * A custom UMG panel that arranges its children in a curved or radial formation.
 * Supports vertical animation curves, spacing, alignment, and orientation options.
 */
class CurvedPanel : public SPanel
{
public:
	SLATE_BEGIN_ARGS(CurvedPanel)
		{
		}

	SLATE_END_ARGS()

	CurvedPanel();

	/** SWidget construction method (currently unused) */
	void Construct(const FArguments& InArgs)
	{
	};

	/** Radius used in radial layout mode */
	void SetRadius(const float InRadius) { Radius = InRadius; }

	/** Start angle in degrees for radial layout */
	void SetStartAngle(const float InStart) { StartAngle = InStart; }

	/** End angle in degrees for radial layout */
	void SetEndAngle(const float InEnd) { EndAngle = InEnd; }

	/** If true, child widgets are rotated to follow the curve */
	void SetRotateTowards(const bool InRotateTowards) { bRotateTowards = InRotateTowards; }

	/** Additional rotation offset (in degrees) applied to each child */
	void SetRotateOffset(const float InOffset) { RotateOffset = InOffset; }

	/** Additional rotation offset (in degrees) applied to each child */
	void SetRadialRotation(const float InOffset) { RadialRotation = InOffset; }

	/** Enable use of a UCurveFloat to modulate vertical offset */
	void SetUseCurve(const bool bInUseCurve) { bUseCurve = bInUseCurve; }

	/** Set the animation curve to control vertical displacement */
	void SetCurve(UCurveFloat* InCurve) { Curve = InCurve; }

	/** Scales the horizontal distance between curve samples */
	void SetCurveScale(const float InScale) { CurveScale = InScale; }

	/** Padding between elements along the X-axis */
	void SetElementSpacing(const float InSpacing) { ElementSpacing = InSpacing; }

	/** Magnitude of the curve effect along the Y-axis */
	void SetCurveMagnitude(const float InMagnitude) { CurveMagnitude = InMagnitude; }

	/** Vertical alignment mode: Top, Middle, or Bottom */
	void SetVerticalAlignmentMode(const EVerticalAlignmentMode InMode) { VerticalAlignmentMode = InMode; }

	/** If true, inverts the curve vertically */
	void SetInvertCurve(const bool bInInvertCurve) { bInvertCurve = bInInvertCurve; }

	/** Adds a new child slot to the panel */
	void AddSlot(const TSharedRef<SWidget>& InWidget)
	{
		Children.Add(new FCurvedSlot(InWidget));
	}

	/** Returns total layout size based on radius */
	virtual FVector2D ComputeDesiredSize(float) const override
	{
		return FVector2D(Radius * 2 + 100.f, Radius * 2 + 100.f);
	}

	/** Core layout logic: positions and rotates each child based on layout settings */
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override
	{
		// Determine the center of the available panel space
		const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;

		// Get the number of child widgets to arrange
		const int32 Count = Children.Num();
		if (Count == 0) return; // Nothing to arrange

		// Calculate how far apart each element should be spread in degrees (for radial layout mode)
		const float AngleStep = (EndAngle - StartAngle) / Count;

		// Iterate over all children
		for (int32 i = 0; i < Count; ++i)
		{
			// Determine the angle in degrees and radians for this element
			float AngleDeg = StartAngle + i * AngleStep;
			const float AngleRad = FMath::DegreesToRadians(AngleDeg);

			float EffectiveRadius = Radius;
			FVector2D Offset;

			// Default curve scale is 1.0 (no curve offset applied)
			float CurveValue = 1.f;
			if (bUseCurve && Curve)
			{
				// Normalized alpha across curve (0–1) per element
				const float NormalizedAlpha = (Count > 1) ? ((float)i / (Count - 1)) : 0.5f;
				// Sample the curve to get height offset, scaled and clamped
				CurveValue = FMath::Clamp(Curve->GetFloatValue(NormalizedAlpha * CurveScale), 0.05f, 1.f);
			
				// Apply magnitude to the sampled curve height
				EffectiveRadius *= CurveValue * CurveMagnitude;

				float VerticalOffset = 0.f;

				// Adjust vertical alignment of the entire arc (bottom, middle, top)
				switch (VerticalAlignmentMode)
				{
				case EVerticalAlignmentMode::Bottom:
					VerticalOffset = GetDesiredSize().Y * .5f;
					break;
				case EVerticalAlignmentMode::Middle:
					VerticalOffset = 0.f;
					break;
				case EVerticalAlignmentMode::Top:
					VerticalOffset = -GetDesiredSize().Y * .5f;
					break;
				}

				// Horizontal positioning: equally space children based on size and spacing
				const FVector2D ChildSize = Children[0].GetWidget()->GetDesiredSize();
				const float HorizontalSpacing = ChildSize.X + ElementSpacing;
				const float TotalWidth = (Count > 1) ? (Count - 1) * HorizontalSpacing : 0.f;
				const float XOffset = (Count > 1) ? (-TotalWidth / 2.f + i * HorizontalSpacing) : 0.f;

				// Calculate the final vertical position
				float Y = VerticalOffset - EffectiveRadius;
				if (bInvertCurve)
				{
					Y = VerticalOffset + EffectiveRadius;
				}

				// Offset includes both X and Y placement
				Offset = FVector2D(XOffset, Y); // horizontally spread, vertically curved
			}
			else
			{
				// Fallback to full radial placement using polar coordinates
				Offset = Count > 1 ? FVector2D(FMath::Cos(AngleRad) * EffectiveRadius, FMath::Sin(AngleRad) * EffectiveRadius) : FVector2D(0.f);
				// UE_LOG(LogTemp, Warning, TEXT("Offset: %s"), *Offset.ToString());
			}

			// Final position in widget space
			FVector2D Pos = Center + Offset;

			// Fetch the actual child widget
			const FCurvedSlot& Slot = Children[i];
			const TSharedRef<SWidget>& Widget = Slot.GetWidget();

			// Get its desired size and adjust so it's centered around Pos
			FVector2D Size = Widget->GetDesiredSize();
			FVector2D WidgetPos = Pos - Size * 0.5f;

			// Optionally rotate to face outward along the curve
			float RotationDeg = bRotateTowards ? (AngleDeg + RotateOffset - 90.f) : RotateOffset;
			FSlateRenderTransform SlateRenderTransform(FQuat2D(FMath::DegreesToRadians(RotationDeg)));

			// Create layout geometry for the widget
			FGeometry ChildGeometry = AllottedGeometry.MakeChild(
				Size,
				FSlateLayoutTransform(WidgetPos),
				SlateRenderTransform,
				FVector2D(0.5f, 0.5f) // pivot from center
			);

			// Add the arranged widget to the layout system
			ArrangedChildren.AddWidget(EVisibility::Visible, FArrangedWidget(Widget, ChildGeometry));
		}
	}

	/** Provides access to panel children for rendering and hit-testing */
	virtual FChildren* GetChildren() override { return &Children; }

private:
	/** Array of layout-aware child widgets */
	TPanelChildren<FCurvedSlot> Children;

	/** Radial layout radius */
	float Radius = 300.f;

	/** Angle in degrees where the layout starts */
	float StartAngle = 0.f;

	/** Angle in degrees where the layout ends */
	float EndAngle = 360.f;

	/** Whether to rotate widgets to face along the layout arc */
	bool bRotateTowards = false;

	/** Additional rotation offset applied per widget */
	float RotateOffset = 0.f;

	/** Default rotation offset in degrees applied to child widgets in the radial layout. */
	float RadialRotation = -90.f;

	/** Enables use of the animation curve */
	bool bUseCurve = false;

	/** Whether to invert the curve vertically */
	bool bInvertCurve = false;

	/** Curve asset to shape the layout vertically */
	UCurveFloat* Curve = nullptr;

	/** Scale applied to the curve's X input (stretching the curve) */
	float CurveScale = 1.f;

	/** Padding between child elements */
	float ElementSpacing = 20.f;

	/** Strength of curve's vertical displacement */
	float CurveMagnitude = 1.f;

	/** How to align the curve vertically in the panel */
	EVerticalAlignmentMode VerticalAlignmentMode = EVerticalAlignmentMode::Middle;
};
