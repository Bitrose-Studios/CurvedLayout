// Copyright Peter Gyarmati (@mrgyarmati)

#pragma once

#include "CoreMinimal.h"
#include "CurvedPanel.h"
#include "Components/PanelWidget.h"
#include "CurvedLayoutPanel.generated.h"

/**
 * UCurvedLayoutPanel is a widget container class for creating a custom
 * curved layout for child widgets. It arranges its children along a
 * circular or curved path based on various configurable parameters.
 */
UCLASS(meta = (DisplayName = "Curved Layout Panel"))
class CURVEDLAYOUT_API UCurvedLayoutPanel : public UPanelWidget
{
	GENERATED_BODY()

public:
	// Radial layout parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout",
		meta = (ToolTip = "Distance from center to each widget in radial mode."))
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout",
		meta=(ClampMin = "-360", ClampMax = "360", ToolTip =
			"Radial rotation in degrees. This rotates the arc as a whole and shifts StartAngle and EndAngle internally."
		))
	float RadialRotation = -90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Curved Layout",
		meta = (ToolTip = "Starting angle of the arc (in degrees) (modified by RadialRotation)."))
	float StartAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Curved Layout",
		meta = (ToolTip = "Ending angle of the arc (in degrees) (modified by RadialRotation)."))
	float EndAngle = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout",
		meta = (ToolTip = "If enabled, widgets will rotate to face along the arc."))
	bool bRotateTowards = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout",
		meta = (ToolTip = "Additional rotation (in degrees) applied to each child widget."))
	float RotateOffset = 0.f;


	// Curved layout parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "Enable curved layout using a float curve for vertical displacement."))
	bool bUseCurve = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "If enabled, flips the curve vertically."))
	bool bInvertCurve = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "The curve that defines vertical offset for each widget."))
	UCurveFloat* Curve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "Scales the curve’s input (X-axis), stretching or compressing the arc shape."))
	float CurveScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "Multiplies the curve’s output (Y-axis), controlling the height of the arc."))
	float CurveMagnitude = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "Horizontal spacing between child elements in curved mode."))
	float ElementSpacing = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curved Layout|Animation Curve",
		meta = (ToolTip = "Controls whether the arc aligns to the top, middle, or bottom of the layout."))
	EVerticalAlignmentMode VerticalAlignmentMode = EVerticalAlignmentMode::Middle;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<class CurvedPanel> MyPanel;
};
