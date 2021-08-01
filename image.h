#ifndef SDL_MAIN_HANDLED
#include "graphical.h"
#endif

// handles different graphical components in program
class image
{
	public:
		image() {}
		image(string pathr) // sets image size to default and loads image from path string
		{
			path = pathr;
			loadTexture();
		} 
		image(string pathr, int s) // loads image from path string and sets size to s
		{
			scale = s;
			path = pathr;
			loadTexture();
		} 
		void setPath(string p) // changes path the image is from
		{
			path = p;
		}
		void setColor( Uint8 red, Uint8 green, Uint8 blue ) // changes color value of image
		{
			//Modulate texture
			SDL_SetTextureColorMod(texture, red, green, blue );
		}
		void setAlpha(Uint8 alph) // changes alpha value of image
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(texture, alph);
		}
		void loadTexture() // loads texture with image information to be displayed
		{
			SDL_Renderer* gRenderer = main_window.gRenderer;
			//The final texture
			SDL_Texture* newTexture = NULL;

			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
			if( loadedSurface == NULL )
			{
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			}
			else
			{
				//Create texture from surface pixels
				newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
				if( newTexture == NULL )
				{
					printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				}
				else
				{
					width = loadedSurface->w;
					height = loadedSurface->h;
				}
				//Get rid of old loaded surface
				SDL_FreeSurface( loadedSurface );
			}
			texture = newTexture;
		}
		void setScale(int num) // changes the size of the image 
		{
			scale = num;
		}
		int getScale() // returns scale size value
		{
			return scale;
		}
		void renderRotate(int x, int y, SDL_Rect* clip=NULL, double angle=0.0, int size=0, SDL_Point* center=NULL, SDL_RendererFlip flip  = SDL_FLIP_NONE) // for rotating and flipping an image
		{
			SDL_Rect renderQuad;
			//Set rendering space and render to screen
			if(size == 0)
				renderQuad = { x, y, width*scale, height*scale };
			else
				renderQuad = { x, y, width*size, height*size};
			if(clip!=NULL && size==0)
			{
				renderQuad.w = clip->w*scale;
				renderQuad.h = clip->h*scale;	
			}
			else if(clip!=NULL && size!=0)
			{
				renderQuad.w = clip->w*size;
				renderQuad.h = clip->h*size;	
			}

			//Render to screen
			SDL_RenderCopyEx(main_window.gRenderer, texture, clip, &renderQuad, angle, center, flip );
		}
		void render( int x, int y, SDL_Rect* clip=NULL, int size=0) // displays an image, clip is a section of an image
		{
			SDL_Rect renderQuad;
			//Set rendering space and render to screen
			if(size == 0)
				renderQuad = { x, y, width*scale, height*scale };
			else
				renderQuad = { x, y, width*size, height*size};
			if(clip!=NULL && size==0)
			{
				renderQuad.w = clip->w*scale;
				renderQuad.h = clip->h*scale;	
			}
			else if(clip!=NULL && size!=0)
			{
				renderQuad.w = clip->w*size;
				renderQuad.h = clip->h*size;	
			}
			
			//Render to screen
			SDL_RenderCopy(main_window.gRenderer, texture, clip, &renderQuad );
		}
		string getPath() // gets the path string
		{
			return path;
		}
		
		~image() // deallocates memory
		{
			SDL_DestroyTexture(texture);
			x=0;
			y=0;
			height=0;
			width=0;
			scale = 1;
		}
		int getHeight() {return height;}
		int getWidth() {return width;}
	private:
		//Set rendering space and render to screen
		
		int scale = 1; // the scale an image is being displayed at with 1 being default
		int x = 0; // x and y coords of an image
		int y = 0;
		
		int height; // height and width of an image
		int width;
		
		string path; // the file path the image is taken from
		SDL_Texture* texture; // holds image information
};
