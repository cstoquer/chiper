#include "RenderingContext.h"

RenderingContext::RenderingContext(SDL_Surface* ctx) {
	context = ctx;
	bgColor = SDL_MapRGB(context->format, 0, 0, 0);
	_transparency = {0, 0, 0};
}

SDL_Surface* RenderingContext::getContext() {
	return context;
}

void RenderingContext::backgroundColor(int r, int g, int b) {
	bgColor = SDL_MapRGB(context->format, r, g, b);
}

void RenderingContext::transparency(int r, int g, int b) {
	_transparency = {r, g, b};
}

void RenderingContext::clear() {
	SDL_FillRect(context, 0, bgColor);
}

void RenderingContext::update() {
	SDL_Flip(context);
}

void RenderingContext::drawImage(SDL_Surface* img) {
	// TODO: do we need this?
	translate.x = 0;
	translate.y = 0;

	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, _transparency[0], _transparency[1], _transparency[2]));
	SDL_BlitSurface(img, NULL, context, &translate); // draw bitmap
}

void RenderingContext::drawImage(SDL_Surface* img, int x, int y) {
	translate.x = x;
	translate.y = y;

	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, _transparency[0], _transparency[1], _transparency[2]));
	SDL_BlitSurface(img, NULL, context, &translate); // draw bitmap
}

void RenderingContext::drawImage(SDL_Surface* img, int sx, int sy, int sw, int sh, int tx, int ty) {
	clip.x = sx;
	clip.y = sy;
	clip.w = sw;
	clip.h = sh;

	translate.x = tx;
	translate.y = ty;

	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, _transparency[0], _transparency[1], _transparency[2]));
	SDL_BlitSurface (img, &clip, context, &translate );
}
