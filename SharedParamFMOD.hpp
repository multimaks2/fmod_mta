#pragma once
#include "CLuaFmod.h"
// #include "StdInc.h"
// #include "CLuaDefs.h" // для видимости g_pClientGame класса
#include <utility>

#define ReadDataFmodFunction          std::tie
#define FmodApiFunctionFormat         std::pair
#define FmodApiFunctionFormatReadData std::tie
#define FmodMakeApiFunction           std::make_pair
#define FmodCameraFunct               std::tuple
#define FmodCameraReturn              std::make_tuple

FMOD_RESULT result;                            // Other
void*       extraDriverData = NULL;            // Other

FMOD::System*         f_system;
FMOD::Studio::System* f_studio;

static const int    AUDIO_SAMPLE_RATE = 44100;
const float         DISTANCEFACTOR = 1.0f;
FMOD_VECTOR         listenerpos = {0.0f, 0.0f, -1.0f * DISTANCEFACTOR};
FMOD::ChannelGroup* mastergroup = 0;
FMOD::Reverb3D*     reverb;
FMOD_VECTOR         revPos = {0.0f, 0.0f, 0.0f};
float               revMinDist = 10.0f, revMaxDist = 50.0f;
bool                muted = false;

struct PositionVelocity
{
    float posX;
    float posY;
    float posZ;
    float velX;
    float velY;
    float velZ;
};

// Конвертировать CVector в FMOD_VECTOR
FMOD_VECTOR ToFMOD_VECTOR(const CVector& v)
{
    // if (isnan(v.fX) || isnan(v.fY) || isnan(v.fZ))
    //{
    //     return {0.0f, 0.0f, 0.0f};
    // }
    return {v.fX, v.fY, v.fZ};
}

// Конвертировать FMOD_VECTOR в CVector
CVector ToCVector(const FMOD_VECTOR& v)
{
    // if (isnan(v.x) || isnan(v.y) || isnan(v.z))
    //{
    //     return {0.0f, 0.0f, 0.0f};
    // }
    return {v.x, v.y, v.z};
}

#define mFmodMap std::map
struct fmod_struct
{
    int                                                     mnNextChannelId;
    typedef mFmodMap<SString, FMOD::Studio::Bank*>          BankMap;               // содержит экземпляр банк файла
    typedef mFmodMap<SString, FMOD::Sound*>                 SoundMap;              // хранит экземпляр звука
    typedef mFmodMap<int, FMOD::Channel*>                   ChannelMap;            // хранит айди канала
    typedef mFmodMap<SString, FMOD::Studio::EventInstance*> EventMap;

    typedef mFmodMap<SString, mFmodMap<SString, FMOD::Studio::EventInstance*> > ElementEventMap;
    ElementEventMap mElementEvent;              // страшно блять)

    BankMap    mBanks;
    EventMap   mEvents;
    SoundMap   mSounds;
    ChannelMap mChannels;
};
fmod_struct* fMassive = nullptr;            // структура к успеху шла

bool initMapFMOD()
{
    fMassive = new fmod_struct();            // я родился
    if (fMassive)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool deleteMapFMOD()            // имей в виду что ты должен релизнуть все события, и все остальное!
{
    if (fMassive)
    {
        // Удаление элементов из словарей
        fMassive->mBanks.clear();
        fMassive->mEvents.clear();
        fMassive->mElementEvent.clear();
        fMassive->mSounds.clear();
        fMassive->mChannels.clear();
        // Освобождение памяти, выделенной под объект fmod_struct
        delete fMassive;
        fMassive = nullptr;
        return true;
    }
    else
    {
        return false;
    }
}

bool reInitMapFMOD()            // перезагрузка массива
{
    return deleteMapFMOD() && initMapFMOD();
}

bool isInitMapFMOD()            // перезагрузка массива
{
    if (fMassive)
    {
        return true;
    }
    return false;
}

SString set3DListenerPosition(const CVector& position, const CVector& forward, const CVector& up, const CVector& velocity)
{
    FMOD_3D_ATTRIBUTES listenerAttributes = {{0}};
    listenerAttributes.position = ToFMOD_VECTOR(position);
    listenerAttributes.velocity = ToFMOD_VECTOR(velocity);
    auto Forw = forward;
    Forw.Normalize();
    listenerAttributes.forward = ToFMOD_VECTOR(Forw);
    auto lup = up;
    lup.Normalize();
    listenerAttributes.up = ToFMOD_VECTOR(lup);
    result =
        f_system->set3DListenerAttributes(0, &listenerAttributes.position, &listenerAttributes.velocity, &listenerAttributes.forward, &listenerAttributes.up);
    f_studio->setListenerAttributes(0, &listenerAttributes);
    return FMOD_ErrorString(result);
}

FmodCameraFunct<FMOD_VECTOR, FMOD_VECTOR, FMOD_VECTOR, FMOD_VECTOR> get3DListenerPosition()
{
    FMOD_3D_ATTRIBUTES listenerAttributes = {0};
    f_system->get3DListenerAttributes(0, &listenerAttributes.position, &listenerAttributes.velocity, &listenerAttributes.forward, &listenerAttributes.up);
    FMOD_VECTOR position = (listenerAttributes.position);
    FMOD_VECTOR velocity = (listenerAttributes.velocity);
    FMOD_VECTOR forward = (listenerAttributes.forward);
    FMOD_VECTOR up = (listenerAttributes.up);
    return FmodCameraReturn(position, forward, up, velocity);
}

SString getFmodDirectory(const char* resourceName, const char* file)
{
    return SString("%s\\resources\\%s\\%s", g_pClientGame->GetFileCacheRoot(), resourceName, file);
}

SString FMODErrorCheck(FMOD_RESULT result, const char* funcName)
{
    return SString("%s [function -> %s]", FMOD_ErrorString(result), funcName);
}

const char* const_to_string(const char* text)
{
    return SString("%s", text);
}

struct localized_state_bank
{
    unsigned int id;
    SString      state;
};

static const SFixedArray<localized_state_bank, 5> getLocalizedLoadingStateBank = {{
    {0, "происходит выгрузка ресурсов"},
    {1, "ресурсы выгружены и освобождены"},
    {2, "загрузка ресурсов в память"},
    {3, "загружен в память - без ошибок"},
    {4, "при загрузке ресурсов произошла ошибка"},
}};

struct localized_event_playing
{
    unsigned int id;
    SString      state;
};

static const SFixedArray<localized_event_playing, 5> getLocalizedEventPlaying = {{
    {0, "Воспроизведение"},
    {1, "Удерживание"},
    {2, "Остановлено"},
    {3, "Запуск"},
    {4, "Остановка"},
}};

struct flagfmodstudioinit
{
    const char*  Flag_Name;
    unsigned int flag;
};

static const SFixedArray<flagfmodstudioinit, 7> getFmodStudioFlags = {{
    {"FMOD_STUDIO_INIT_NORMAL", FMOD_STUDIO_INIT_NORMAL},
    {"FMOD_STUDIO_INIT_LIVEUPDATE", FMOD_STUDIO_INIT_LIVEUPDATE},
    {"FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS", FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS},
    {"FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE", FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE},
    {"FMOD_STUDIO_INIT_DEFERRED_CALLBACKS", FMOD_STUDIO_INIT_DEFERRED_CALLBACKS},
    {"FMOD_STUDIO_INIT_LOAD_FROM_UPDATE", FMOD_STUDIO_INIT_LOAD_FROM_UPDATE},
    {"FMOD_STUDIO_INIT_MEMORY_TRACKING", FMOD_STUDIO_INIT_MEMORY_TRACKING},
}};

unsigned int getFmodStudioFlag(const char* flagName)
{
    for (unsigned int i = 0; i <= (6); i++)
    {
        if (stricmp(flagName, getFmodStudioFlags[i].Flag_Name) == 0 ||
            (getFmodStudioFlags[i].Flag_Name && stricmp(flagName, getFmodStudioFlags[i].Flag_Name) == 0))
        {
            return getFmodStudioFlags[i].flag;
        }
    }
    return getFmodStudioFlags[0].flag;
}

struct flagfmodinit
{
    SString      Flag_Name;
    unsigned int flag;
};

static const SFixedArray<flagfmodinit, 13> getFmodFlags = {{
    {"FMOD_INIT_NORMAL", FMOD_INIT_NORMAL},
    {"FMOD_INIT_STREAM_FROM_UPDATE", FMOD_INIT_STREAM_FROM_UPDATE},
    {"FMOD_INIT_MIX_FROM_UPDATE", FMOD_INIT_MIX_FROM_UPDATE},
    {"FMOD_INIT_3D_RIGHTHANDED", FMOD_INIT_3D_RIGHTHANDED},
    {"FMOD_INIT_CHANNEL_LOWPASS", FMOD_INIT_CHANNEL_LOWPASS},
    {"FMOD_INIT_CHANNEL_DISTANCEFILTER", FMOD_INIT_CHANNEL_DISTANCEFILTER},
    {"FMOD_INIT_PROFILE_ENABLE", FMOD_INIT_PROFILE_ENABLE},
    {"FMOD_INIT_VOL0_BECOMES_VIRTUAL", FMOD_INIT_VOL0_BECOMES_VIRTUAL},
    {"FMOD_INIT_GEOMETRY_USECLOSEST", FMOD_INIT_GEOMETRY_USECLOSEST},
    {"FMOD_INIT_PREFER_DOLBY_DOWNMIX", FMOD_INIT_PREFER_DOLBY_DOWNMIX},
    {"FMOD_INIT_THREAD_UNSAFE", FMOD_INIT_THREAD_UNSAFE},
    {"FMOD_INIT_PROFILE_METER_ALL", FMOD_INIT_PROFILE_METER_ALL},
    {"FMOD_INIT_MEMORY_TRACKING", FMOD_INIT_MEMORY_TRACKING},
}};

unsigned int getFmodCoreFlag(const char* flagName)
{
    for (unsigned int i = 0; i <= (12); i++)
    {
        if (stricmp(flagName, getFmodFlags[i].Flag_Name) == 0 || (getFmodFlags[i].Flag_Name && stricmp(flagName, getFmodFlags[i].Flag_Name) == 0))
        {
            return getFmodFlags[i].flag;
        }
    }
    return getFmodFlags[0].flag;
}

struct flagfmodSound
{
    SString      Flag_Name;
    unsigned int soundFlag;
};

static const SFixedArray<flagfmodSound, 29> getFmodSoundFlags = {{
    {"FMOD_DEFAULT", FMOD_DEFAULT},
    {"FMOD_LOOP_OFF", FMOD_LOOP_OFF},
    {"FMOD_LOOP_NORMAL", FMOD_LOOP_NORMAL},
    {"FMOD_LOOP_BIDI", FMOD_LOOP_BIDI},
    {"FMOD_2D", FMOD_2D},
    {"FMOD_3D", FMOD_3D},
    {"FMOD_CREATESTREAM", FMOD_CREATESTREAM},
    {"FMOD_CREATESAMPLE", FMOD_CREATESAMPLE},
    {"FMOD_CREATECOMPRESSEDSAMPLE", FMOD_CREATECOMPRESSEDSAMPLE},
    {"FMOD_OPENUSER", FMOD_OPENUSER},
    {"FMOD_OPENMEMORY", FMOD_OPENMEMORY},
    {"FMOD_OPENMEMORY_POINT", FMOD_OPENMEMORY_POINT},
    {"FMOD_OPENRAW", FMOD_OPENRAW},
    {"FMOD_OPENONLY", FMOD_OPENONLY},
    {"FMOD_ACCURATETIME", FMOD_ACCURATETIME},
    {"FMOD_MPEGSEARCH", FMOD_MPEGSEARCH},
    {"FMOD_NONBLOCKING", FMOD_NONBLOCKING},
    {"FMOD_UNIQUE", FMOD_UNIQUE},
    {"FMOD_3D_HEADRELATIVE", FMOD_3D_HEADRELATIVE},
    {"FMOD_3D_WORLDRELATIVE", FMOD_3D_WORLDRELATIVE},
    {"FMOD_3D_INVERSEROLLOFF", FMOD_3D_INVERSEROLLOFF},
    {"FMOD_3D_LINEARROLLOFF", FMOD_3D_LINEARROLLOFF},
    {"FMOD_3D_LINEARSQUAREROLLOFF", FMOD_3D_LINEARSQUAREROLLOFF},
    {"FMOD_3D_INVERSETAPEREDROLLOFF", FMOD_3D_INVERSETAPEREDROLLOFF},
    {"FMOD_3D_CUSTOMROLLOFF", FMOD_3D_CUSTOMROLLOFF},
    {"FMOD_3D_IGNOREGEOMETRY", FMOD_3D_IGNOREGEOMETRY},
    {"FMOD_IGNORETAGS", FMOD_IGNORETAGS},
    {"FMOD_LOWMEM", FMOD_LOWMEM},
    {"FMOD_VIRTUAL_PLAYFROMSTART", FMOD_VIRTUAL_PLAYFROMSTART},
}};

unsigned int getFmodFlagSound(const char* flagName)
{
    for (unsigned int i = 0; i <= (28); i++)
    {
        if (stricmp(flagName, getFmodSoundFlags[i].Flag_Name) == 0 ||
            (getFmodSoundFlags[i].Flag_Name && stricmp(flagName, getFmodSoundFlags[i].Flag_Name) == 0))
        {
            return getFmodSoundFlags[i].soundFlag;
        }
    }
    return getFmodSoundFlags[0].soundFlag;
}
