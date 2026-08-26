#ifndef MXUTILITIES_H
#define MXUTILITIES_H

#include "lego1_export.h"
#include "mxtypes.h"

#include <SDL3/SDL_endian.h>
#include <SDL3/SDL_stdinc.h>
#include <string.h>

struct LegoSdlEvents {
	Uint32 m_windowsMessage;
	Uint32 m_presenterProgress;
	Uint32 m_gameEvent;
};

LEGO1_EXPORT extern LegoSdlEvents g_legoSdlEvents;

class MxDSFile;
class MxDSObject;
class MxDSAction;
class MxCompositePresenterList;
class MxPresenter;

template <class T>
inline T Abs(T p_t)
{
	return p_t < 0 ? -p_t : p_t;
}

template <class T>
inline T Min(T p_t1, T p_t2)
{
	return p_t1 < p_t2 ? p_t1 : p_t2;
}

template <class T>
inline T Max(T p_t1, T p_t2)
{
	return p_t1 > p_t2 ? p_t1 : p_t2;
}

// SI stream data is little-endian on disk. MxSwapLE converts a scalar
// between the on-disk and host representation; no-ops on little-endian hosts.
inline MxU8 MxSwapLE(MxU8 p_value)
{
	return p_value;
}
inline MxS8 MxSwapLE(MxS8 p_value)
{
	return p_value;
}
inline MxU16 MxSwapLE(MxU16 p_value)
{
	return SDL_Swap16LE(p_value);
}
inline MxS16 MxSwapLE(MxS16 p_value)
{
	return (MxS16) SDL_Swap16LE((MxU16) p_value);
}
inline MxU32 MxSwapLE(MxU32 p_value)
{
	return SDL_Swap32LE(p_value);
}
inline MxS32 MxSwapLE(MxS32 p_value)
{
	return (MxS32) SDL_Swap32LE((MxU32) p_value);
}
inline float MxSwapLE(float p_value)
{
	return SDL_SwapFloatLE(p_value);
}
inline double MxSwapLE(double p_value)
{
	Uint64 bits;
	memcpy(&bits, &p_value, sizeof(bits));
	bits = SDL_Swap64LE(bits);
	memcpy(&p_value, &bits, sizeof(bits));
	return p_value;
}

// Read/write a scalar stored little-endian at a possibly unaligned address,
// converting to/from host byte order. Overload resolution on MxSwapLE
// restricts these to scalars with a defined on-disk byte order.
template <typename T>
T UnalignedRead(const MxU8* p_source)
{
	T value;
	memcpy(&value, p_source, sizeof(T));
	return MxSwapLE(value);
}

template <typename T>
void UnalignedWrite(MxU8* p_dest, T p_value)
{
	p_value = MxSwapLE(p_value);
	memcpy(p_dest, &p_value, sizeof(T));
}

template <class T>
inline void GetScalar(MxU8*& p_source, T& p_dest)
{
	p_dest = *(T*) p_source;
	p_source += sizeof(T);
}

template <class T>
inline T GetScalar(T*& p_source)
{
	T val = *p_source;
	p_source += 1;
	return val;
}

template <class T>
inline void GetDouble(MxU8*& p_source, T& p_dest)
{
	p_dest = *(double*) p_source;
	p_source += sizeof(double);
}

template <class T>
inline void GetString(MxU8*& p_source, char*& p_dest, T* p_obj, void (T::*p_setter)(const char*))
{
	(p_obj->*p_setter)((char*) p_source);
	p_source += strlen(p_dest) + 1;
}

MxBool GetRectIntersection(
	MxS32 p_rect1Width,
	MxS32 p_rect1Height,
	MxS32 p_rect2Width,
	MxS32 p_rect2Height,
	MxS32* p_rect1Left,
	MxS32* p_rect1Top,
	MxS32* p_rect2Left,
	MxS32* p_rect2Top,
	MxS32* p_width,
	MxS32* p_height
);

void MakeSourceName(char*, const char*);
void OmniError(const char* p_message, MxS32 p_status);
void SetOmniUserMessage(void (*p_omniUserMessage)(const char*, MxS32));
MxBool ContainsPresenter(MxCompositePresenterList& p_presenterList, MxPresenter* p_presenter);
void ApplyMask(MxDSAction* p_action, MxU32 p_mask, MxBool p_setFlags);
MxBool KeyValueStringParse(char*, const char*, const char*);

// TEMPLATE: BETA10 0x1012dfd0
// ?Max@@YAHHH@Z

// TEMPLATE: BETA10 0x1012dff0
// ?Min@@YAHHH@Z

#endif // MXUTILITIES_H
