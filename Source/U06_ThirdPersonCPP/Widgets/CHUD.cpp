#include "CHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "/Game/Textures/T_Crosshair");
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();
	CheckFalse(bVisible);
	CheckNull(Texture);

	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;

	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);
	item.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;

	Canvas->DrawItem(item);
}
