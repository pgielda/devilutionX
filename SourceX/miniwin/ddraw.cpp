#include "devilution.h"
#include "miniwin/ddraw.h"
#include "stubs.h"

#include "DiabloUI/diabloui.h"

namespace dvl {

#if 0
SDL_Window *window;
SDL_Renderer *renderer;

/** Currently active palette */
SDL_Palette *palette;
SDL_Texture *texture;

/** 32-bit in-memory backbuffer surface */
SDL_Surface *surface;

/** 8-bit surface wrapper around #gpBuffer */
SDL_Surface *pal_surface;
#endif

bool bufferUpdated = false;

ULONG StubDraw::Release()
{
	DUMMY();
	/*
	SDL_FreeSurface(pal_surface);
	SDL_FreePalette(palette);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	*/

	return 0;
};

HRESULT StubDraw::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE *lplpDDPalette,
    IUnknown *pUnkOuter)
{
	DUMMY();
	*lplpDDPalette = new StubPalette();
/*
	palette = SDL_AllocPalette(256);
	if (palette == NULL) {
		SDL_Log(SDL_GetError());
		return (HRESULT)0x80000002L; //DDERR_OUTOFMEMORY
	}
*/
	return DVL_DS_OK;
}

HRESULT StubDraw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE *lplpDDSurface,
    IUnknown *pUnkOuter)
{
	DUMMY();

	if (!lpDDSurfaceDesc->dwWidth) {
		// TODO Get value from render/window
		lpDDSurfaceDesc->ddpfPixelFormat.dwFlags = SDL_PIXELFORMAT_RGBA8888;
		lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount = 32;

	gpBuffer = (BYTE*)malloc(1024*1024);

#if 0
		if (renderer) {
			if (SDL_GetRendererOutputSize(renderer, (int *)&lpDDSurfaceDesc->dwWidth, (int *)&lpDDSurfaceDesc->dwHeight) <= -1) {
				SDL_Log(SDL_GetError());
			}
			surface = SDL_CreateRGBSurfaceWithFormat(0, lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount, lpDDSurfaceDesc->ddpfPixelFormat.dwFlags);
		} else {
			SDL_GetWindowSize(window, (int *)&lpDDSurfaceDesc->dwWidth, (int *)&lpDDSurfaceDesc->dwHeight);
			surface = SDL_GetWindowSurface(window);
		}
		if (surface == NULL) {
			SDL_Log(SDL_GetError());
			return (HRESULT)0x80000002L; //DDERR_OUTOFMEMORY
		}
		lpDDSurfaceDesc->lpSurface = (BYTE *)surface->pixels;
		*lplpDDSurface = new StubSurface(lpDDSurfaceDesc);
		return DVL_DS_OK;
		#endif
	}

	#if 0
	printf("w = %d h=  %d\n", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight);
	pal_surface = SDL_CreateRGBSurfaceWithFormat(0, lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount, lpDDSurfaceDesc->ddpfPixelFormat.dwFlags);
	if (pal_surface == NULL) {
		SDL_Log(SDL_GetError());
		return (HRESULT)0x80000002L; //DDERR_OUTOFMEMORY
	}
	lpDDSurfaceDesc->lpSurface = (BYTE *)surface->pixels;
	#endif
	//lpDDSurfaceDesc->lpSurface = (BYTE*)malloc(1024*1024*4);
	*lplpDDSurface = new StubSurface(lpDDSurfaceDesc);

 
	//gpBuffer = (BYTE *)pal_surface->pixels; // Hack should happen in dx.cpp, but gives error
	#if 0
	if (SDL_SetSurfacePalette(pal_surface, palette) <= -1) {
		SDL_Log(SDL_GetError());
		return 1; //MAKE_HRESULT(130);//DVL_MAKE_HRESULT(130);
	}
	#endif

	return DVL_DS_OK;
}

HRESULT StubDraw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
	DUMMY();
	return DVL_DS_OK;
}

HRESULT StubDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
	DUMMY();

	width = dwWidth;
	height = dwHeight;

	return DVL_DS_OK;
}

HRESULT StubDraw::WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
	DUMMY_ONCE();
	return DVL_DS_OK;
}

StubSurface::StubSurface(LPDDSURFACEDESC lpDDSurfaceDesc)
{
}

StubSurface::~StubSurface()
{
}

ULONG StubSurface::Release()
{
	DUMMY();
	return 0;
};

#include <X11/Xlib.h>
#include <X11/Xutil.h>


typedef struct {
        Window window;
        XImage *image;
        int width;
        int height;
        char *data;
} _internal_surface;

#define MAX_SURFACES 255

static Display *display = NULL;
static _internal_surface surfaces[MAX_SURFACES];
int surface_count = 0;

static int windowid = -1;

int mousex = 0;
int mousey = 0;

int active_events() {
	 XEvent event;
	  if (windowid == -1) return 0;

 if (XCheckWindowEvent(display, surfaces[windowid].window, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask, &event)) {
 	printf("event, putting it back!\n");
 	XPutBackEvent(display, &event);
	return 1;
 }
 return 0;
}

int process_events(uint64_t *ev)
{
printf("process events!\n");
 if (windowid == -1) return 0;
 XEvent event;
 if (XCheckWindowEvent(display, surfaces[windowid].window, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask, &event)) {
 		printf("event.type = %X\n", event.type);
 		if (event.type == MotionNotify) {
			uint8_t *evv = (uint8_t*)ev;
			evv[0] = 0x6;
			uint16_t *ms = (uint16_t*)ev;
			mousex = event.xbutton.x;
			mousey = event.xbutton.y;
			ms[2] = mousex;
			ms[3] = mousey;
			return 1;
		}
		if (event.type == ButtonPress) {
			printf("button press!\n");
			uint8_t *evv = (uint8_t*)ev;
			evv[0] = 0x10;
			uint16_t *ms = (uint16_t*)ev;
			ms[2] = event.xbutton.x; 
			ms[3] = event.xbutton.y;
			return 1;
		} else if (event.type == ButtonRelease) {
			printf("button release\n");
                        uint8_t *evv = (uint8_t*)ev;
                        evv[0] = 0x11;
                        uint16_t *ms = (uint16_t*)ev;
                        ms[2] = event.xbutton.x;
                        ms[3] = event.xbutton.y;
			return 1;
		} else if (event.type == KeyPress) {
			printf("key press!\n");
			uint8_t *evv = (uint8_t*)ev;
			evv[0] = 0x12;
			uint32_t *ms = (uint32_t*)ev;
			ms[1] = event.xkey.keycode;
			return 1;
		} else if (event.type == KeyRelease) {
		        uint8_t *evv = (uint8_t*)ev;
			printf("key release!\n");
                        evv[0] = 0x13;
                        uint32_t *ms = (uint32_t*)ev;
                        ms[1] = event.xkey.keycode;
			return 1;
		}
 }
 return 0;
}

void get_mouse_position(int*x, int*y)
{
if (windowid == -1) return;
*x = mousex;
*y = mousey;
}


// get framebuffer data
uint32_t *fb_getdata(int surface_id) {
        return (uint32_t*)surfaces[surface_id].image->data;
}

// partial redraw
void fb_redraw_rect(int surface_id, int x, int y, int w, int h) {
        XPutImage(display,surfaces[surface_id].window,DefaultGC(display,DefaultScreen(display)),surfaces[surface_id].image,x,y,x,y,w,h);
        XFlush(display);
}

// redraw whole fb
void fb_redraw(int surface_id) {
        fb_redraw_rect(surface_id, 0, 0, surfaces[surface_id].width,surfaces[surface_id].height);
}

int create_fb_window(int w, int h) {
        if (!display) {
                display = XOpenDisplay(NULL);
        }
        Window root = RootWindow(display,DefaultScreen(display));
        surfaces[surface_count].width = w;
        surfaces[surface_count].height = h;
        surfaces[surface_count].window = XCreateSimpleWindow(display,root,0,0,surfaces[surface_count].width,surfaces[surface_count].height,1,0,0); // TODO: should be centered?
        XSelectInput(display,surfaces[surface_count].window, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);
        XMapWindow(display,surfaces[surface_count].window);
        surfaces[surface_count].data = (char*)malloc(w*h*4);
        surfaces[surface_count].image = XCreateImage(display,DefaultVisual(display,DefaultScreen(display)),DefaultDepth(display,DefaultScreen(display)),ZPixmap, 0,surfaces[surface_count].data,w,h,32,0);


        char data[1] = {0};
	XColor col;
        Pixmap bogus = XCreateBitmapFromData(display, root, data, 1, 1);
        XDefineCursor(display, surfaces[surface_count].window, XCreatePixmapCursor(display, bogus, bogus, &col, &col, 0, 0));

        surface_count++;
        return (surface_count - 1);
}


uint32_t get_color(uint8_t pxl) {
	uint32_t result = system_palette[pxl].peRed << 16;
	result |= system_palette[pxl].peBlue;
	result |= system_palette[pxl].peGreen << 8;
	return result;
}

HRESULT StubSurface::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans)
{
	DUMMY_ONCE();

	assert(lpDDSrcSurface == lpDDSBackBuf); // is src pal_surface

	int w = lpSrcRect->right - lpSrcRect->left + 1;
	int h = lpSrcRect->bottom - lpSrcRect->top + 1;

	if (windowid == -1) {
		windowid = create_fb_window(640, 480);
	}

	uint8_t *pixels = (uint8_t*)gpBuffer;

	uint32_t *dt = fb_getdata(windowid);

	for (int x = lpSrcRect->left; x < (lpSrcRect->left + w); x++)
	for (int y = lpSrcRect->top; y < (lpSrcRect->top + h); y++) {
		int posX = x - lpSrcRect->left + dwX;
		int posY = y - lpSrcRect->top + dwY;
		if (posX >= 640) continue;
		if (posY >= 480) break;
		uint8_t pixel = pixels[x + y * 768];
		uint32_t new_pixel = get_color(pixel);
		dt[posX + posY * 640] = new_pixel;
	}

	fb_redraw_rect(windowid, dwX, dwY, w, h);

	return DVL_S_OK;
}

HRESULT StubSurface::GetCaps(LPDDSCAPS lpDDSCaps)
{
	DUMMY();
	return DVL_S_OK;
}

HRESULT StubSurface::GetDC(HDC *lphDC)
{
	DUMMY_ONCE();
	return DVL_S_OK;
}

HRESULT StubSurface::GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
{
	lpDDPixelFormat->dwFlags = SDL_PIXELFORMAT_INDEX8;
	lpDDPixelFormat->dwRGBBitCount = 8;
	return DVL_S_OK;
}

HRESULT StubSurface::IsLost()
{
	DUMMY_ONCE();
	return DVL_S_OK;
}

HRESULT StubSurface::Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
	DUMMY_ONCE();
	lpDDSurfaceDesc->lpSurface = (BYTE *)gpBuffer;
	//pal_surface->pixels;
	return DVL_S_OK;
}

HRESULT StubSurface::ReleaseDC(HDC hDC)
{
	DUMMY_ONCE();
	return DVL_S_OK;
}

HRESULT StubSurface::Restore()
{
	UNIMPLEMENTED();
	return DVL_S_OK;
}

HRESULT StubSurface::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
	DUMMY();
	return DVL_S_OK;
}

HRESULT StubSurface::Unlock(LPVOID lpSurfaceData)
{
	return DVL_S_OK;
}

ULONG StubPalette::Release()
{
	DUMMY();
	return 0;
};

HRESULT StubPalette::GetCaps(LPDWORD lpdwCaps)
{
	UNIMPLEMENTED();
	return DVL_S_OK;
}

HRESULT StubPalette::GetEntries(DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
	for (int i = 0; i < dwNumEntries; i++) {
		lpEntries[i].peFlags = 0;
		lpEntries[i].peRed = system_palette[i].peRed;
		lpEntries[i].peGreen = system_palette[i].peGreen;
		lpEntries[i].peBlue = system_palette[i].peBlue;
	}
	return DVL_S_OK;
}

HRESULT StubPalette::SetEntries(DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
	for (int i = 0; i < dwCount; i++) {
		system_palette[i].peFlags = 0;
		system_palette[i].peRed = lpEntries[i].peRed;
		system_palette[i].peGreen = lpEntries[i].peGreen;
		system_palette[i].peBlue = lpEntries[i].peBlue;
	}
//	palette_update();
	return DVL_S_OK;
}

} // namespace dvl
