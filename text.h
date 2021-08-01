#ifndef SDL_MAIN_HANDLED
#include "graphical.h"
#endif

// handles text output on the screen 
class text
{
	public:
		void loadLetters() // creates letters in memory to be accessed to when printing text to the screen
		{
			string text = " ";
			for(int i=0;i<71;i++)
			{
				// sets values for letters 
				if(i < 26) // upper case
					text[0] = (char)(65+i);
				else if(i >= 26 && i < 52)// lower case
					text[0] = (char)(97+(i-26));
				else if( i >= 52 && i < 62) // numbers
					text[0] = (char)(i-4);
				else if(i == 62) // extra characters
					text = " ";
				else if(i == 63)
					text = ".";
				else if(i == 64)
					text = "!";
				else if(i == 65)
					text = "?";
				else if(i == 66)
					text = ",";
				else if(i == 67)
					text = "'";
				else if(i == 68)
					text = ":";
				else if(i == 69)
					text = "\"";
				else if(i== 70)
					text = "/";
				SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, text.c_str(), textColor );
				if( textSurface == NULL )
				{
					printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
				}
				else
				{
					//Create texture from surface pixels
					mTexture[i] = SDL_CreateTextureFromSurface(main_window.gRenderer, textSurface );
					if(mTexture[i] == NULL )
					{
						printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
					}
					else
					{
						//Get image dimensions
						if(fontt.compare("fonts/Roman.ttf") == 0 || fontt.compare("fonts/PS3 STOLEN.ttf") == 0)
						{
							height = textSurface->h;
							width = textSurface->w*2;
						}
						else
						{
							width = textSurface->w;
							height = textSurface->h;
						}
					}
				
					//Get rid of old surface
					SDL_FreeSurface( textSurface );
				}
			}
		}
		text()
		{
		}
		text(string font, int seifer,Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255) // initializer
		{
			textColor = {red,red,red,255};
			sizer = seifer;
			fontt = font;
			mTexture = new SDL_Texture*[70];
			gFont = TTF_OpenFont(font.c_str(), sizer);
			loadLetters();
		}
		void setSize(int si) // deletes data and changes size
		{
			sizer = si;
			
			delete[] mTexture; 
			TTF_CloseFont(gFont); 
			
			mTexture = new SDL_Texture*[70];
			gFont = TTF_OpenFont(fontt.c_str(), sizer);
			loadLetters();
		}
		void setColor( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255) // sets color values
		{
			textColor.a = alpha;
			textColor.r = red;
			textColor.g = green;
			textColor.b = blue;
		}
		void setAlpha(Uint8 alph) // sets alpha values
		{
			textColor.a = alph;
		}
		void display(string a,int x, int y) // displays text through accessing the saved letters in memory
		{
			SDL_Rect renderQuad;
			int d;
			for(int i=0;i<a.length();i++)
			{
				renderQuad = {x+(i*width), y, width, height};
				
				if(a[i] == ' ') // loads extra characters
					d = 62;
				else if(a[i] == '.')
					d = 63;
				else if(a[i] == '?')
					d = 65;
				else if(a[i] == '!')
					d = 64;
				else if(a[i] == ',')
					d = 66;
				else if(a[i] == 39 )
					d = 67;
				else if(a[i] == ':')
					d = 68;
				else if(a[i] == '"')
					d = 69;
				else if(a[i] == '/')
					d = 70;
				
				else if((int)a[i] >= 65 && (int)a[i] < 91) // loads letters and numbers
					d = (int)a[i]-65;
				else if((int)a[i] >= 97 && (int)a[i] < 123)
					d = (int)a[i]-71;
				else if((int)a[i] >= 48 && (int)a[i] < 58)
					d = (int)a[i]+4;
				
				SDL_SetTextureColorMod(mTexture[d], textColor.r, textColor.g, textColor.b);
				SDL_SetTextureAlphaMod(mTexture[d], textColor.a);
				SDL_RenderCopy(main_window.gRenderer, mTexture[d], NULL, &renderQuad );
			}
		}
		// displays text at x and y which can be set to color values (r,g,b,a)
		void display(string a,int x, int y,  Uint8 red, Uint8 green, Uint8 blue, int alpha) 
		{
			SDL_Rect renderQuad;
			int d;
			// sets color and alpha
			textColor.r = red;
			textColor.g = green;
			textColor.b = blue;
			textColor.a = alpha;
			for(int i=0;i<a.length();i++)
			{
				renderQuad = {x+(i*width), y, width, height};
				
				if(a[i] == ' ')
					d = 62;
				else if(a[i] == '.')
					d = 63;
				else if(a[i] == '?')
					d = 65;
				else if(a[i] == '!')
					d = 64;
				else if(a[i] == ',')
					d = 66;
				else if(a[i] == 39)
					d = 67;
				else if(a[i] == ':')
					d = 68;
				else if(a[i] == '"')
					d = 69;
				
				else if((int)a[i] >= 65 && (int)a[i] < 91)
					d = (int)a[i]-65;
				else if((int)a[i] >= 97 && (int)a[i] < 123)
					d = (int)a[i]-71;
				else if((int)a[i] >= 48 && (int)a[i] < 58)
					d = (int)a[i]+4;
				
				SDL_SetTextureColorMod(mTexture[d], red, green, blue );
				SDL_SetTextureAlphaMod(mTexture[d], alpha);
				SDL_RenderCopy(main_window.gRenderer, mTexture[d], NULL, &renderQuad );
			}
		}
		void free() // deconstructor
		{
			delete[] mTexture;
			delete[] &textColor;
			TTF_CloseFont(gFont); 
		}
	private:
		
		SDL_Color textColor = {0,0,0}; // the color the text is set at 
		string fontt; // the path the font information is taken from
		int sizer = 1; // the size of the font
		
		int width; // the width and height information used for displaying
		int height;
		
		SDL_Texture ** mTexture; // holds texture of all font letters
		TTF_Font *gFont = NULL; // font information 
};
