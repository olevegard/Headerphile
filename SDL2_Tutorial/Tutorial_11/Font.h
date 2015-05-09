// Font.h - headerphile.blogspot.com
// A simple class for handling TTF_Fonts
// Initlaisez the font hand handles the flags
#pragma once

enum class FontStyle
{
	Normal,
	Bold,
	Italic,
	Underline,
	Strikethrough
};
class Font
{
	public:
		Font()
			// Initialize the mask ( will be set to normal )
			:	mask( 0 )
		{
		}
		bool Init( const std::string &fontName, int32_t fontSize )
		{
			// Check if TTF was initlaized
			if ( TTF_WasInit() != 1 )
			{
				// TTF wasn't intialized, intialize it!
				if ( TTF_Init() == -1 )
				{
					std::cout << "Failed to init TTF! " << TTF_GetError() << std::endl;
					return false;
				}
			}

			// Load font
			font = TTF_OpenFont( fontName.c_str(), fontSize );

			if ( font == nullptr )
			{
				std::cout << "Failed to load font";
				return false;
			}

			return true;
		}
		// Adds a font style, will not remove any other flags
		void AddFontStyle( FontStyle style )
		{
			switch ( style )
			{
				case FontStyle::Normal:
					return;
				case FontStyle::Bold:
					mask |= TTF_STYLE_BOLD;
					break;
				case FontStyle::Italic:
					mask |= TTF_STYLE_ITALIC;
					break;
				case FontStyle::Underline:
					mask |= TTF_STYLE_UNDERLINE;
					break;
				case FontStyle::Strikethrough:
					mask |= TTF_STYLE_STRIKETHROUGH;
					break;
				default :
					break;
			}
			TTF_SetFontStyle( font, mask );

			ApplyFontStyle();
		}
		// Removes a font style, will only affect the one font style
		void RemFontStyle( FontStyle style )
		{
			switch ( style )
			{
				case FontStyle::Normal:
					break;
				case FontStyle::Bold:
					mask &= ~TTF_STYLE_BOLD;
					break;
				case FontStyle::Italic:
					mask &= ~TTF_STYLE_ITALIC;
					break;
				case FontStyle::Underline:
					mask &= ~TTF_STYLE_UNDERLINE;
					break;
				case FontStyle::Strikethrough:
					mask &= ~TTF_STYLE_STRIKETHROUGH;
					break;
				default :
					break;
			}

			ApplyFontStyle();
		}
		// Sets the font style, will remove all other styles
		void SetFontStyle( FontStyle style )
		{
			switch ( style )
			{
				case FontStyle::Normal:
					mask = TTF_STYLE_NORMAL;
					break;
				case FontStyle::Bold:
					mask = TTF_STYLE_BOLD;
					break;
				case FontStyle::Italic:
					mask = TTF_STYLE_ITALIC;
					break;
				case FontStyle::Underline:
					mask = TTF_STYLE_UNDERLINE;
					break;
				case FontStyle::Strikethrough:
					mask = TTF_STYLE_STRIKETHROUGH;
					break;
				default :
					break;
			}

			ApplyFontStyle();
		}
		// Checks if font style is set
		bool ChkFontStyle( FontStyle style )
		{
			switch ( style )
			{
				case FontStyle::Normal:
					return ( mask & TTF_STYLE_NORMAL );
				case FontStyle::Bold:
					return ( mask & TTF_STYLE_BOLD );
				case FontStyle::Italic:
					return ( mask & TTF_STYLE_ITALIC );
				case FontStyle::Underline:
					return ( mask & TTF_STYLE_UNDERLINE );
				case FontStyle::Strikethrough:
					return ( mask & TTF_STYLE_STRIKETHROUGH );
				default :
					return false;
			}

			ApplyFontStyle();
		}
		// Sets the mask. This enables us to do something lik 
		// SetFontStyle( TTF_STYLE_BOLD | TTF_STYLE_ITALIC )
		// Which will set font to be bold and italiz
		void SetFontStyleMask( int32_t newMask )
		{
			mask = newMask;
		}
		// Updates the font style based on what's in the mask
		void ApplyFontStyle()
		{
			// Checks if we are trying to set the same font style twice
			if ( prevMask == mask )
				return;
			
			TTF_SetFontStyle( font, mask );

			prevMask = mask;
		}
		TTF_Font* GetFont()
		{
			return font;
		}
		int32_t GetMask() const
		{
			return mask;
		}
	private:
		TTF_Font* font;

		int32_t prevMask;
		int32_t mask;
};
