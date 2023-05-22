#pragma once
#include "CLuaDefs.h"

class CLuaFMOD : public CLuaDefs
{
public:
    static void LoadFunctions();
    static void AddClass(lua_State* luaVM);

    LUA_DECLARE(isInitMapFMOD);
    LUA_DECLARE(initMapFMOD);
    LUA_DECLARE(deleteMapFMOD);
    LUA_DECLARE(reInitMapFMOD);

    LUA_DECLARE(getFmodDirectory);            // получить путь до файлов

    // bank
    LUA_DECLARE(loadFmodBankFile);
    LUA_DECLARE(unloadFmodBankFile);
    LUA_DECLARE(getFmodBankLoadingState);

    // Создание и инициализация
    LUA_DECLARE(createFmodStudioCore);            // Создание ядра FMOD Studio
    LUA_DECLARE(createFmodCore);                  // Создание ядра FMOD
    LUA_DECLARE(initFmodCore);                    // Инициализация ядра FMOD
    LUA_DECLARE(initFmodStudio);                  // Инициализация FMOD Studio
    LUA_DECLARE(globalCloseFmod);                 // Освобождение всех ресурсов FMOD
    LUA_DECLARE(closeCoreFmod);                   // Закрытие ядра FMOD
    LUA_DECLARE(closeStudioFmod);                 // Закрытие FMOD Studio
    LUA_DECLARE(IsFmodCoreCreated);
    LUA_DECLARE(IsFmodStudioCreated);

    // обновление ядра
    LUA_DECLARE(FMOD_Update);

    //// Работа со слушателем и 3D звуком
    LUA_DECLARE(set3DListenerPosition);            // Установка позиции слушателя в пространстве

    // Функции для эвентов без тела
    LUA_DECLARE(loadFmodEvent);
    LUA_DECLARE(releaseFmodEvent);
    LUA_DECLARE(playFmodEvent);
    LUA_DECLARE(stopFmodEvent);
    LUA_DECLARE(setPauseFmodEvent);
    LUA_DECLARE(getPauseFmodEvent);
    LUA_DECLARE(setPosFmodEvent);
    LUA_DECLARE(getPosFmodEvent);
    LUA_DECLARE(setFmodEventParameter);
    LUA_DECLARE(getFmodEventParameter);
    LUA_DECLARE(setVolumeFmodEvent);
    LUA_DECLARE(getVolumeFmodEvent);

    // Функции обработки событий с телом
    LUA_DECLARE(loadFmodEventForElement);
    LUA_DECLARE(releaseEventInstanceForElement);
    LUA_DECLARE(playFmodEventForElement);
    LUA_DECLARE(stopFmodEventForElement);
    LUA_DECLARE(setPauseFmodEventForElement);
    LUA_DECLARE(getPauseFmodEventForElement);
    LUA_DECLARE(setFmodEvent3DPositionForElement);
    LUA_DECLARE(getFmodEvent3DPositionForElement);
    LUA_DECLARE(setFmodEventVolumeForElement);
    LUA_DECLARE(getFmodEventVolumeForElement);
    LUA_DECLARE(setFmodParameterForElement);
    LUA_DECLARE(getFmodParameterForElement);
    LUA_DECLARE(isEventInstanceExists);

    // Фунции звука
    LUA_DECLARE(createFmodSound);
    LUA_DECLARE(playFmodSound);
    LUA_DECLARE(releaseFmodSound);
    LUA_DECLARE(setFmodSound3DPosion);
    LUA_DECLARE(getFmodSound3DPosion);

    // Функции звукового канала
    LUA_DECLARE(setFmodMasterMute);
    LUA_DECLARE(getFmodMasterMute);

};
