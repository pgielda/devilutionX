#include "devilution.h"
#include "miniwin/dsound.h"
#include "stubs.h"
#include <SDL.h>

namespace dvl {

ULONG DirectSound::Release()
{
	return 0;
};

HRESULT DirectSound::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOute)
{
	if (pcDSBufferDesc->dwFlags != DVL_DSBCAPS_PRIMARYBUFFER) { // Creating primery buffer isn't needed and breaks Music
		*ppDSBuffer = new DirectSoundBuffer();
	}

	return DVL_DS_OK;
};

HRESULT DirectSound::GetCaps(LPDSCAPS pDSCaps)
{
	return DVL_DS_OK;
};

/**
 * @brief SDL handels this for us when using Mix_PlayChannel(-1);
 */
HRESULT DirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate)
{
	UNIMPLEMENTED();
	return DVL_DS_OK;
};

HRESULT DirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return DVL_DS_OK;
};

///// DirectSoundBuffer /////

ULONG DirectSoundBuffer::Release()
{
	return 0;
};

/**
 * @brief Only used for handeling duplicates 
 */
HRESULT DirectSoundBuffer::GetStatus(LPDWORD pdwStatus)
{
	return DVL_DSERR_INVALIDPARAM;
};

HRESULT DirectSoundBuffer::Lock(DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
    LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags)
{
	*pdwAudioBytes1 = dwBytes;
	*ppvAudioPtr1 = malloc(dwBytes);

	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::Play(DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags)
{
	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::SetFormat(LPCWAVEFORMATEX pcfxFormat)
{
	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::SetVolume(LONG lVolume)
{
	volume = pow(10, lVolume / 2000.0) * MIX_MAX_VOLUME;

	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::SetPan(LONG lPan)
{
	pan = copysign(pow(10, -abs(lPan) / 2000.0) * 255, lPan);

	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::Stop()
{
	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::Unlock(LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2)
{
	return DVL_DS_OK;
};

HRESULT DirectSoundBuffer::Restore()
{
	return DVL_DS_OK;
};

} // namespace dvl
