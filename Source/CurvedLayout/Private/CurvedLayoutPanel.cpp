// Copyright Peter Gyarmati (@mrgyarmati)


#include "CurvedLayoutPanel.h"

#include "CurvedPanel.h"
#include "Widgets/SPanel.h"


void UCurvedLayoutPanel::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyPanel.Reset();
}

TSharedRef<SWidget> UCurvedLayoutPanel::RebuildWidget()
{
	StartAngle = RadialRotation;
	EndAngle = StartAngle + 360.f;
	
	MyPanel = SNew(CurvedPanel);
	MyPanel->SetRadius(Radius);
	MyPanel->SetStartAngle(StartAngle);
	MyPanel->SetEndAngle(EndAngle);
	MyPanel->SetRotateTowards(bRotateTowards);
	MyPanel->SetRotateOffset(RotateOffset);
	MyPanel->SetRadialRotation(RadialRotation);
	MyPanel->SetUseCurve(bUseCurve);
	MyPanel->SetCurve(Curve);
	MyPanel->SetCurveScale(CurveScale);
	MyPanel->SetElementSpacing(ElementSpacing);
	MyPanel->SetCurveMagnitude(CurveMagnitude);
	MyPanel->SetVerticalAlignmentMode(VerticalAlignmentMode);
	MyPanel->SetInvertCurve(bInvertCurve);

	for (const UPanelSlot* PanelSlot : Slots)
	{
		if (UWidget* Widget = PanelSlot->Content)
		{
			MyPanel->AddSlot(Widget->TakeWidget());
		}
	}

	return MyPanel.ToSharedRef();
}
