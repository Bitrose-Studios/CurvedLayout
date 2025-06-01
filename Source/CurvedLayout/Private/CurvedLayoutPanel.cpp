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

			for (int32 i = 0; i < Slots.Num(); ++i)
			{
				UWidget* slotContent = Slots[i]->Content;

				// Calculate the angle for this slot and apply it to the widget
				// This helps with the rotation of the widget content to upward facing
				float AngleStep = (EndAngle - StartAngle) / Slots.Num();
				float ComputedAngle = StartAngle + i * AngleStep;
				float FinalComputedAngle = bRotateTowards ? ComputedAngle + 180 + RotateOffset - 90.f : ComputedAngle;
				if (UUserWidget* UserWidget = Cast<UUserWidget>(slotContent))
				{
					float SlotRotation = FinalComputedAngle;
					UProperty* RotationProp = UserWidget->GetClass()->FindPropertyByName(FName("SlotRotationAngle"));
					if (RotationProp && RotationProp->IsA<FFloatProperty>())
					{
						FFloatProperty* FloatProp = CastField<FFloatProperty>(RotationProp);
						FloatProp->SetPropertyValue_InContainer(UserWidget, SlotRotation);
					}
				}
			}
		}
	}

	return MyPanel.ToSharedRef();
}
