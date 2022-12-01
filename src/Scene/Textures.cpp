#include <SDL2/SDL.h>

#include <stdio.h>
#include <string>

class Textures
{
	public:
		Textures(SDL_Renderer* gRenderer, SDL_Window* gWindow);
		Textures(SDL_Renderer* gRenderer);

		~Textures();

		void load(std::string path);

		void executeShader(Uint32(*func)(Uint32));

		void free();
		
		void render(int x, int y, int w = 0, int h = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		void renderWithShader(
      int x, int y, int w, int h,
      Uint32(*func)(Uint32)
    );

		Uint32 color(Uint8 red, Uint8 green, Uint8 blue);

		int getWidth();
		int getHeight();

		bool lockTexture();
		bool unlockTexture();
		Uint32* getPixels();
		int getPitch();
    int getPixelCount();
    Uint32 getPixel(int x, int y);
    Uint32 getPixel(int xy);

	private:
		SDL_Texture* mTexture;
		void* mPixels;
		int mPitch;
    Uint32 format;
    SDL_PixelFormat* mappingFormat;

		int mWidth;
		int mHeight;

    SDL_Renderer *renderer;
};

Textures::Textures(SDL_Renderer* gRenderer, SDL_Window* gWindow)
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;

  renderer = gRenderer;

  format = SDL_GetWindowPixelFormat(gWindow);
  mappingFormat = SDL_AllocFormat(format);
}

Textures::Textures(SDL_Renderer* gRenderer)
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;

  renderer = gRenderer;

  format = SDL_PIXELFORMAT_RGBA8888;
  mappingFormat = SDL_AllocFormat(format);
}

Textures::~Textures()
{
  SDL_FreeFormat(mappingFormat);
  SDL_DestroyTexture(mTexture);

  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  mPixels = NULL;
  mPitch = 0;
}

void Textures::executeShader(Uint32(*func)(Uint32))
{

	lockTexture();
  int pixelCount = getPixelCount();
  Uint32* pixels = getPixels();

  
  for( int i = 0; i < pixelCount; ++i)
  {
    pixels[i] = func(pixels[i]);
  }
  
  unlockTexture();
}

void Textures::load(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, format, 0);
  newTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, loadedSurface->w, loadedSurface->h);

  SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);

  memcpy(mPixels, formattedSurface->pixels, mPitch * formattedSurface->h);

  SDL_UnlockTexture(newTexture);

  mPixels = NULL;
  mWidth = loadedSurface->w;
  mHeight = loadedSurface->h;
  
  SDL_FreeSurface(formattedSurface);
  SDL_FreeSurface(loadedSurface);
  
	mTexture = newTexture;
}

Uint32 Textures::color(Uint8 red, Uint8 green, Uint8 blue)
{
  return SDL_MapRGB(mappingFormat, red, green, blue);
}

void Textures::render(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  int rWidth = mWidth;
  int rHeight = mHeight;

  if (w != 0) {
    rWidth = w;
  }

  if (h != 0) {
    rHeight = h;
  }

	SDL_Rect renderQuad = { x, y, rWidth, rHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

void Textures::renderWithShader(int x, int y, int w, int h, Uint32(*func)(Uint32))
{
	SDL_Rect renderQuad = { x, y, w, h };

	lockTexture();
  Uint32* pixels = getPixels();  

  void* copyPixels;
  int copyPitch;
  SDL_Texture* copyTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);

  SDL_LockTexture(copyTexture, NULL, &copyPixels, &copyPitch);
  Uint32* copyPixelsInt = (Uint32*) copyPixels;

  for(int i = 0; i < (mWidth * mHeight); ++i)
  {
    copyPixelsInt[i] = func(pixels[i]);
  }

  SDL_UnlockTexture(copyTexture);
  unlockTexture();

	SDL_RenderCopy(renderer, copyTexture, NULL, &renderQuad);
}


int Textures::getWidth()
{
	return mWidth;
}

int Textures::getHeight()
{
	return mHeight;
}

bool Textures::lockTexture()
{
	if(mPixels != NULL)
	{
    return false;
	}

  SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch);
	return true;
}

bool Textures::unlockTexture()
{
	if(mPixels == NULL)
	{
    return true;
	}

  SDL_UnlockTexture(mTexture);
  mPixels = NULL;
  mPitch = 0;

  return true;
}

Uint32* Textures::getPixels()
{
	return (Uint32*) mPixels;
}

int Textures::getPitch()
{
	return mPitch;
}

int Textures::getPixelCount()
{
	return (getPitch() / 4) * getHeight();
}

Uint32 Textures::getPixel(int x, int y)
{
  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[(y * getPitch() / 4) + x];

	return pixel;
}

Uint32 Textures::getPixel(int xy)
{
  lockTexture();
  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[xy];
  unlockTexture();

	return pixel;
}
