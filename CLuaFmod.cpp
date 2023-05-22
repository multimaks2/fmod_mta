#pragma once
#include "StdInc.h"
#include "SharedParamFMOD.hpp"
#include "fmod_event.hpp"
#include "fmod_init.hpp"
#include "fmod_bank.hpp"

#include "fmod_channel.hpp"
#include "fmod_sound.hpp"

void CLuaFMOD::LoadFunctions()
{
    constexpr static const std::pair<const char*, lua_CFunction> functions[]{
        // Функции для работы массивом
        {"isInitMapFMOD", isInitMapFMOD},
        {"initMapFMOD", initMapFMOD},
        {"deleteMapFMOD", deleteMapFMOD},
        {"reInitMapFMOD", reInitMapFMOD},

        // Функции для работы с ядром FMOD
        {"createFmodCore", createFmodCore},
        {"createFmodStudioCore", createFmodStudioCore},
        {"initFmodCore", initFmodCore},
        {"initFmodStudio", initFmodStudio},
        {"IsFmodCoreCreated", IsFmodCoreCreated},
        {"IsFmodStudioCreated", IsFmodStudioCreated},
        {"closeCoreFmod", closeCoreFmod},
        {"closeStudioFmod", closeStudioFmod},
        {"globalCloseFmod", globalCloseFmod},
        {"FMOD_Update", FMOD_Update},

        // Функции для работы с банками звуковых ресурсов FMOD
        {"loadFmodBankFile", loadFmodBankFile},
        {"unloadFmodBankFile", unloadFmodBankFile},
        {"getFmodBankLoadingState", getFmodBankLoadingState},

        // Функция для получения директории FMOD
        {"getFmodDirectory", getFmodDirectory},

        // Функции для установки позиции слушателя в 3D-пространстве
        {"set3DListenerPosition", set3DListenerPosition},

        // Функции для обработки событий event без хозяина
        {"loadFmodEvent", loadFmodEvent},
        {"releaseFmodEvent", releaseFmodEvent},
        {"playFmodEvent", playFmodEvent},
        {"stopFmodEvent", stopFmodEvent},
        {"setPauseFmodEvent", setPauseFmodEvent},
        {"getPauseFmodEvent", getPauseFmodEvent},
        {"setPosFmodEvent", setPosFmodEvent},
        {"getPosFmodEvent", getPosFmodEvent},
        {"setFmodEventParameter", setFmodEventParameter},
        {"getFmodEventParameter", getFmodEventParameter},
        {"setVolumeFmodEvent", setVolumeFmodEvent},
        {"getVolumeFmodEvent", getVolumeFmodEvent},

        // Функции обработки событий с телом
        {"loadFmodEventForElement", loadFmodEventForElement},
        {"releaseEventInstanceForElement", releaseEventInstanceForElement},
        {"playFmodEventForElement", playFmodEventForElement},
        {"stopFmodEventForElement", stopFmodEventForElement},
        {"setPauseFmodEventForElement", setPauseFmodEventForElement},
        {"getPauseFmodEventForElement", getPauseFmodEventForElement},
        {"setFmodEvent3DPositionForElement", setFmodEvent3DPositionForElement},
        {"getFmodEvent3DPositionForElement", getFmodEvent3DPositionForElement},
        {"setFmodEventVolumeForElement", setFmodEventVolumeForElement},
        {"getFmodEventVolumeForElement", getFmodEventVolumeForElement},
        {"setFmodParameterForElement", setFmodParameterForElement},
        {"getFmodParameterForElement", getFmodParameterForElement},
        {"isEventInstanceExists", isEventInstanceExists},

        // Функции для работы со звуком
        {"createFmodSound", createFmodSound},
        {"playFmodSound", playFmodSound},
        {"releaseFmodSound", releaseFmodSound},
        {"setFmodSound3DPosion", setFmodSound3DPosion},
        {"getFmodSound3DPosion", getFmodSound3DPosion},

        {"setFmodMasterMute", setFmodMasterMute},
        {"getFmodMasterMute", getFmodMasterMute},

    };

    // Add functions
    for (const auto& [name, func] : functions)
        CLuaCFunctions::AddFunction(name, func);
}

void CLuaFMOD::AddClass(lua_State* luaVM)
{
    lua_newclass(luaVM);
    lua_classfunction(luaVM, "isInitMapFMOD", "isInitMapFMOD");
    lua_classfunction(luaVM, "initMapFMOD", "initMapFMOD");
    lua_classfunction(luaVM, "deleteMapFMOD", "deleteMapFMOD");
    lua_classfunction(luaVM, "reInitMapFMOD", "reInitMapFMOD");
    lua_classfunction(luaVM, "set3DListenerPosition", "set3DListenerPosition");

    lua_registerclass(luaVM, "FMOD", "Element");
}

int CLuaFMOD::isInitMapFMOD(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::isInitMapFMOD());
    return 1;
}

int CLuaFMOD::initMapFMOD(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::initMapFMOD());
    return 1;
}

int CLuaFMOD::deleteMapFMOD(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::deleteMapFMOD());
    return 1;
}

int CLuaFMOD::reInitMapFMOD(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::reInitMapFMOD());
    return 1;
}

int CLuaFMOD::createFmodCore(lua_State* luaVM)
{
    bool    bState;
    SString bResult;
    ReadDataFmodFunction(bState, bResult) = ::createFmodCore();
    lua_pushboolean(luaVM, bState);
    lua_pushstring(luaVM, bResult);
    return 2;
}

int CLuaFMOD::createFmodStudioCore(lua_State* luaVM)
{
    bool    bState;
    SString bResult;
    ReadDataFmodFunction(bState, bResult) = ::createFmodStudioCore();
    lua_pushboolean(luaVM, bState);
    lua_pushstring(luaVM, bResult);
    return 2;
}

int CLuaFMOD::initFmodCore(lua_State* luaVM)
{
    int              channel;
    bool             bState;
    SString          bResult, flagName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadNumber(channel);
    argStream.ReadString(flagName);
    if (!argStream.HasErrors())
    {
        ReadDataFmodFunction(bState, bResult) = ::initFmodCore(channel, flagName);
        lua_pushboolean(luaVM, bState);
        lua_pushstring(luaVM, bResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::initFmodStudio(lua_State* luaVM)
{
    int              channel;
    bool             bState;
    SString          bResult, flagName, flag;
    CScriptArgReader argStream(luaVM);
    argStream.ReadNumber(channel);
    argStream.ReadString(flagName);
    argStream.ReadString(flag);
    if (!argStream.HasErrors())
    {
        ReadDataFmodFunction(bState, bResult) = ::initFmodStudio(channel, flagName, flag);
        lua_pushboolean(luaVM, bState);
        lua_pushstring(luaVM, bResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::IsFmodCoreCreated(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::IsFmodCoreCreated());
    return 1;
}

int CLuaFMOD::IsFmodStudioCreated(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::IsFmodStudioCreated());
    return 1;
}

int CLuaFMOD::closeCoreFmod(lua_State* luaVM)
{
    bool    bState;
    SString bResult;
    ReadDataFmodFunction(bState, bResult) = ::closeCoreFmod();
    lua_pushboolean(luaVM, bState);
    lua_pushstring(luaVM, bResult);
    return 2;
}

int CLuaFMOD::closeStudioFmod(lua_State* luaVM)
{
    bool    bState;
    SString bResult;
    ReadDataFmodFunction(bState, bResult) = ::closeStudioFmod();
    lua_pushboolean(luaVM, bState);
    lua_pushstring(luaVM, bResult);
    return 2;
}

int CLuaFMOD::globalCloseFmod(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::globalCloseFmod());
    return 1;
}

int CLuaFMOD::FMOD_Update(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::FMOD_Update());
    return 1;
}

int CLuaFMOD::loadFmodBankFile(lua_State* luaVM)
{
    SString          bankDirectory = "";
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(bankDirectory);
    if (!argStream.HasErrors())
    {
        bool    bState;
        SString bResult;
        ReadDataFmodFunction(bState, bResult) = ::loadFmodBankFile(bankDirectory);
        lua_pushboolean(luaVM, bState);
        lua_pushstring(luaVM, bResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::unloadFmodBankFile(lua_State* luaVM)
{
    SString          bankDirectory = "";
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(bankDirectory);
    if (!argStream.HasErrors())
    {
        bool    bState;
        SString bResult;
        ReadDataFmodFunction(bState, bResult) = ::unloadFmodBankFile(bankDirectory);
        lua_pushboolean(luaVM, bState);
        lua_pushstring(luaVM, bResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::set3DListenerPosition(lua_State* luaVM)
{
    CVector          position, forward, up, velocity;
    CScriptArgReader argStream(luaVM);
    argStream.ReadVector3D(position);
    argStream.ReadVector3D(forward);
    argStream.ReadVector3D(up);
    argStream.ReadVector3D(velocity);
    if (!argStream.HasErrors())
    {
        lua_pushstring(luaVM, ::set3DListenerPosition(position, forward, up, velocity));
        lua_pushstring(luaVM, SString("[Position: %f, %f, %f]\n[Forward: %f, %f, %f]\n[Up: %f, %f, %f]\n[Velocity: %f, %f, %f]", position.fX, position.fY,
                                      position.fZ, forward.fX, forward.fY, forward.fZ, up.fX, up.fY, up.fZ, velocity.fX, velocity.fY, velocity.fZ));
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::getFmodDirectory(lua_State* luaVM)
{
    SString          resourceName;
    SString          file;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(resourceName);
    argStream.ReadString(file);
    if (!argStream.HasErrors())
    {
        auto returnData = ::getFmodDirectory(resourceName, file);
        if (returnData)
        {
            lua_pushstring(luaVM, returnData);
            return 1;
        }
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodBankLoadingState(lua_State* luaVM)
{
    SString          bankDir;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(bankDir);
    if (!argStream.HasErrors())
    {
        lua_pushstring(luaVM, ::getFmodBankLoadingState(bankDir));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::loadFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::loadFmodEvent(eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::releaseFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::releaseFmodEvent(eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::playFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::playFmodEvent(eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::stopFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::stopFmodEvent(eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setPauseFmodEvent(lua_State* luaVM)
{
    bool             paused;
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    argStream.ReadBool(paused);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setPauseFmodEvent(eventName, paused));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getPauseFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::getPauseFmodEvent(eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setPosFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CVector          position, forward;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    argStream.ReadVector3D(position);
    // argStream.ReadVector3D(forward);
    if (!argStream.HasErrors())
    {
        lua_pushstring(luaVM, ::setPosFmodEvent(eventName, position /*, forward*/));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getPosFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushvector(luaVM, ::getPosFmodEvent(eventName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setFmodEventParameter(lua_State* luaVM)
{
    float            value;
    SString          eventName, paramName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    argStream.ReadString(paramName);
    argStream.ReadNumber(value);

    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodEventParameter(eventName, paramName, value));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodEventParameter(lua_State* luaVM)
{
    SString          eventName, paramName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    argStream.ReadString(paramName);

    if (!argStream.HasErrors())
    {
        lua_pushnumber(luaVM, ::getFmodEventParameter(eventName, paramName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setVolumeFmodEvent(lua_State* luaVM)
{
    float            value;
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    argStream.ReadNumber(value);

    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setVolumeFmodEvent(eventName, value));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getVolumeFmodEvent(lua_State* luaVM)
{
    SString          eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushnumber(luaVM, ::getVolumeFmodEvent(eventName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::createFmodSound(lua_State* luaVM)
{
    bool             sState;
    SString          sDir, sFlag, sResult;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(sDir);
    argStream.ReadString(sFlag);

    if (!argStream.HasErrors())
    {
        ReadDataFmodFunction(sState, sResult) = ::createFmodSound(sDir, sFlag);
        lua_pushboolean(luaVM, sState);
        lua_pushstring(luaVM, sResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::playFmodSound(lua_State* luaVM)
{
    int              sID;
    SString          sDir, sResult;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(sDir);

    if (!argStream.HasErrors())
    {
        ReadDataFmodFunction(sID, sResult) = ::playFmodSound(sDir);
        lua_pushnumber(luaVM, sID);
        lua_pushstring(luaVM, sResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::releaseFmodSound(lua_State* luaVM)
{
    bool             sState;
    SString          sDir, sResult;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(sDir);
    if (!argStream.HasErrors())
    {
        ReadDataFmodFunction(sState, sResult) = ::releaseFmodSound(sDir);
        lua_pushboolean(luaVM, sState);
        lua_pushstring(luaVM, sResult);
        return 2;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}

int CLuaFMOD::setFmodSound3DPosion(lua_State* luaVM)
{
    int              sChannel;
    CVector          position;
    CScriptArgReader argStream(luaVM);
    argStream.ReadNumber(sChannel);
    argStream.ReadVector3D(position);

    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodSound3DPosion(sChannel, position));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodSound3DPosion(lua_State* luaVM)
{
    int              sChannel;
    CScriptArgReader argStream(luaVM);
    argStream.ReadNumber(sChannel);
    if (!argStream.HasErrors())
    {
        lua_pushvector(luaVM, ::getFmodSound3DPosion(sChannel));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushvector(luaVM, CVector());            // вернем пустой вектор
    return 1;
}

int CLuaFMOD::setFmodMasterMute(lua_State* luaVM)
{
    bool             mState;
    CScriptArgReader argStream(luaVM);
    argStream.ReadBool(mState);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodMasterMute(mState));
        return 1;
    }
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodMasterMute(lua_State* luaVM)
{
    lua_pushboolean(luaVM, ::getFmodMasterMute());
    return 1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////Эвенты имеют тело///////////////////////////////

int CLuaFMOD::loadFmodEventForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::loadFmodEventForElement(Element, eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::releaseEventInstanceForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::releaseEventInstanceForElement(Element, eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::playFmodEventForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::playFmodEventForElement(Element, eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::stopFmodEventForElement(lua_State* luaVM)
{
    bool             bImmediate; // FMOD_STUDIO_STOP_ALLOWFADEOUT с затуханием // FMOD_STUDIO_STOP_IMMEDIATE резкая остановка
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadBool(bImmediate);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::stopFmodEventForElement(Element, eventName, bImmediate));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setPauseFmodEventForElement(lua_State* luaVM)
{
    bool             paused;
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadBool(paused);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setPauseFmodEventForElement(Element, eventName, paused));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getPauseFmodEventForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::getPauseFmodEventForElement(Element, eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setFmodEvent3DPositionForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CVector          position, forward;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadVector3D(position);
    // argStream.ReadVector3D(forward);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodEvent3DPositionForElement(Element, eventName, position /*, forward*/));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodEvent3DPositionForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushvector(luaVM, ::getFmodEvent3DPositionForElement(Element, eventName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setFmodEventVolumeForElement(lua_State* luaVM)
{
    float            value;
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadNumber(value);

    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodEventVolumeForElement(Element, eventName, value));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodEventVolumeForElement(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushnumber(luaVM, ::getFmodEventVolumeForElement(Element, eventName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::setFmodParameterForElement(lua_State* luaVM)
{
    float            value;
    SString          Element, eventName, paramName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadString(paramName);
    argStream.ReadNumber(value);

    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::setFmodParameterForElement(Element, eventName, paramName, value));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}

int CLuaFMOD::getFmodParameterForElement(lua_State* luaVM)
{
    SString          Element, eventName, paramName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    argStream.ReadString(paramName);

    if (!argStream.HasErrors())
    {
        lua_pushnumber(luaVM, ::getFmodParameterForElement(Element, eventName, paramName));
        return 1;
    }
    else
        g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}


int CLuaFMOD::isEventInstanceExists(lua_State* luaVM)
{
    SString          Element, eventName;
    CScriptArgReader argStream(luaVM);
    argStream.ReadString(Element);
    argStream.ReadString(eventName);
    if (!argStream.HasErrors())
    {
        lua_pushboolean(luaVM, ::isEventInstanceExists(Element, eventName));
        return 1;
    }
    g_pClientGame->GetScriptDebugging()->LogCustom(luaVM, argStream.GetFullErrorMessage());
    lua_pushboolean(luaVM, false);
    return 1;
}
