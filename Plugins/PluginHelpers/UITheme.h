#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Toolbars.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

class CUIThemeFile;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class UITheme
			{
			public:
				UITheme();
				UITheme(const UITHEME* pTheme);

				enum class RenderStyle 
				{ 
					Gradient, 
					Glass, 
					GlassWithGradient 
				};

				RenderStyle GetRenderStyle();

				enum class AppColor 
				{ 
					AppBackDark, 
					AppBackLight, 
					AppLinesDark, 
					AppLinesLight,
					AppText,
					MenuBack,
					ToolbarDark,
					ToolbarLight,
					ToolbarHot,
					StatusBarDark,
					StatusBarLight,
					StatusBarText,
					Weekends,
					NonWorkingHours,
				};

				Windows::Media::Color GetAppMediaColor(AppColor color);
				Windows::Media::Color GetAppMediaColor(AppColor color, unsigned char opacity);

				Drawing::Color GetAppDrawingColor(AppColor color);
				Drawing::Color GetAppDrawingColor(AppColor color, unsigned char opacity);

			private:
				CUIThemeFile* m_pTheme;

				UInt32 GetColor(AppColor color);
				void InitTheme(const UITHEME* pTheme);
			};

			// --------------------------------------------------------------

			public ref class UIThemeToolbarRenderer : BaseToolbarRenderer
			{
			public:
				UIThemeToolbarRenderer();

				void SetUITheme(UITheme^ theme);

			protected:
				virtual void OnRenderButtonBackground(Windows::Forms::ToolStripItemRenderEventArgs^ e) override;
				virtual void OnRenderMenuItemBackground(Windows::Forms::ToolStripItemRenderEventArgs^ e) override;
				virtual void OnRenderDropDownButtonBackground(Windows::Forms::ToolStripItemRenderEventArgs^ e) override;
				virtual void DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow) override;

			private:
				Drawing::Color m_HotFillColor;
				Drawing::Color m_HotBorderColor;
				Drawing::Color m_PressedFillColor;
				Drawing::Color m_BkgndLightColor;
				Drawing::Color m_BkgndDarkColor;

				UITheme::RenderStyle m_Style;

				bool ValidColours();
				bool RenderButtonBackground(Windows::Forms::ToolStripItemRenderEventArgs^ e);
			};

		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

