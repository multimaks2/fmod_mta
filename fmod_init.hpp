#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"



bool is_fmod_core_created = false;
bool IsFmodCoreCreated()
{
    return is_fmod_core_created;
}

bool is_fmod_studio_created = false;
bool IsFmodStudioCreated()
{
    return is_fmod_studio_created;
}

FmodApiFunctionFormat<bool, SString> createFmodCore()
{
    if (IsFmodCoreCreated())
    {
        return FmodMakeApiFunction(false, SString("%s", "Core - давно создано"));
    }
    else
    {
        FMOD_RESULT result = FMOD::System_Create(&f_system);
        if (result != FMOD_OK)
        {
            return FmodMakeApiFunction(false, FMOD_ErrorString(result));
        }
        else
        {
            is_fmod_core_created = true;
            f_system->setSoftwareFormat(AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0);
            f_system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
            f_system->getMasterChannelGroup(&mastergroup);
            f_system->update();
            return FmodMakeApiFunction(true, FMOD_ErrorString(result));
        }
    }
    return FmodMakeApiFunction(false, SString("%s", "не удалось создать core!"));
}

FmodApiFunctionFormat<bool, SString> createFmodStudioCore()
{
    if (IsFmodStudioCreated())
    {
        return FmodMakeApiFunction(false, SString("%s", "Studio - давно создано"));
    }
    FMOD_RESULT result = FMOD::Studio::System::create(&f_studio);
    if (result == FMOD_OK)
    {
        is_fmod_studio_created = true;
        f_studio->update();
        return FmodMakeApiFunction(true, FMOD_ErrorString(result));
    }
    else
    {
        return FmodMakeApiFunction(false, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, SString("%s", "не удалось создать Studio!"));
}

// FMOD API INIT CORE
FmodApiFunctionFormat<bool, SString> initFmodCore(int mxchannel, const SString& flagName)
{
    if (::IsFmodCoreCreated())
    {
        FMOD_RESULT result = f_system->init(mxchannel, getFmodCoreFlag(flagName), nullptr);
        f_system->update();
        if (result == FMOD_OK)
        {
            return FmodMakeApiFunction(true, FMOD_ErrorString(result));
        }
        return FmodMakeApiFunction(false, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, SString("%s", "Core - было инициализировано"));
}

// FMOD API INIT STUDIO CORE
FmodApiFunctionFormat<bool, SString> initFmodStudio(int mxchannel, const SString& flagName, const SString& flag)
{
    if (IsFmodStudioCreated())
    {
        FMOD_RESULT result = f_studio->initialize(mxchannel, getFmodStudioFlag(flagName), getFmodCoreFlag(flag), extraDriverData);
        if (result == FMOD_OK)
        {
            f_studio->update();
            return FmodMakeApiFunction(true, FMOD_ErrorString(result));
        }
        return FmodMakeApiFunction(false, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, SString("%s", "Studio - было инициализировано"));
}

FmodApiFunctionFormat<bool, SString> closeCoreFmod()
{
    if (f_system == nullptr)
    {
        return FmodMakeApiFunction(false, SString("%s", "f_system - не существует"));
    }

    if (IsFmodCoreCreated())
    {
        f_system->update();
        FMOD_RESULT result = f_system->close();
        if (result != FMOD_OK)
        {
            return FmodMakeApiFunction(false, SString("Ошибка close: {%s}", FMOD_ErrorString(result)));
        }
        f_system->update();
        result = f_system->release();
        if (result != FMOD_OK)
        {
            return FmodMakeApiFunction(false, SString("Ошибка release: {%s}", FMOD_ErrorString(result)));
        }
        is_fmod_core_created = false;
        f_system = nullptr;
        return FmodMakeApiFunction(true, SString("%s", "fmod core - успешно закрылся"));
    }

    return FmodMakeApiFunction(true, SString("%s", "fmod core - не смог закрыться"));
}

FmodApiFunctionFormat<bool, SString> closeStudioFmod()
{
    if (f_studio == nullptr)
    {
        return FmodMakeApiFunction(false, SString("%s", "f_studio - не существует"));
    }

    if (IsFmodStudioCreated())
    {
        f_studio->update();

        FMOD_RESULT result = f_studio->unloadAll();
        f_studio->update();
        if (result != FMOD_OK)
        {
            // return FmodMakeApiFunction(false, SString("Ошибка unloadAll: {%s}", FMOD_ErrorString(result)));
        }
        f_studio->update();
        result = f_studio->release();
        if (result != FMOD_OK)
        {
            // return FmodMakeApiFunction(false, SString("Ошибка release: {%s}", FMOD_ErrorString(result)));
        }
        is_fmod_studio_created = false;
        f_studio = nullptr;
        return FmodMakeApiFunction(true, SString("%s", "fmod studio - успешно закрылся"));
    }
    return FmodMakeApiFunction(true, SString("%s", "fmod studio - не смог закрыться"));
}

bool globalCloseFmod()
{
    if (closeCoreFmod().first && closeStudioFmod().first)
    {
        return deleteMapFMOD();
    }
    return false;
}

bool FMOD_Update()
{
    if (IsFmodCoreCreated())
    {
        f_system->update();
    }
    if (IsFmodStudioCreated())
    {
        f_studio->update();
    }
    return true;
}
