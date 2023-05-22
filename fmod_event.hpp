#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"

bool setDataEventInstanceForElement(const SString& elementName, const SString& eventName, FMOD::Studio::EventInstance* pEventInstance)
{
    // Проверка переданного указателя на nullptr.
    if (pEventInstance == nullptr)
    {
        return false;
    }

    // Запись значения в mElementEvent
    fMassive->mElementEvent[elementName][eventName] = pEventInstance;

    return true;
}

FMOD::Studio::EventInstance* getDataEventInstanceForElement(const SString& elementName, const SString& eventName)
{
    if (fMassive)
    {
        // Поиск элемента с заданным именем
        auto elementIt = fMassive->mElementEvent.find(elementName);
        if (elementIt != fMassive->mElementEvent.end())
        {
            // Элемент найден. Получение ссылки на его содержимое.
            auto& element = elementIt->second;

            // Поиск значения для заданного имени события.
            auto eventIt = element.find(eventName);
            if (eventIt != element.end())
            {
                // Экземпляр события найден. Возвращаем его.
                return eventIt->second;
            }
        }
    }

    // Элемент или экземпляр события не найдены, возвращаем nullptr.
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////
/////////////////////Эти функции пробросить в LUA//////////////////////////
///////////////////////////////////////////////////////////////////////////

bool loadFmodEventForElement(const SString& elementName, const SString& eventName)
{
    FMOD::Studio::EventDescription* pEventDescription = NULL;
    if (FMOD_OK == f_studio->getEvent(eventName, &pEventDescription))
    {
        if (pEventDescription)
        {
            FMOD::Studio::EventInstance* pEventInstance = NULL;
            pEventDescription->createInstance(&pEventInstance);
            if (pEventInstance)
            {
                return setDataEventInstanceForElement(elementName, eventName, pEventInstance);
            }
        }
    }
}

bool releaseEventInstanceForElement(const SString& elementName, const SString& eventName)
{
    if (fMassive)
    {
        auto it = fMassive->mElementEvent.find(elementName);
        if (it != fMassive->mElementEvent.end())
        {
            auto eventIt = it->second.find(eventName);
            if (eventIt != it->second.end())
            {
                // Удаление экземпляра звукового события
                FMOD::Studio::EventInstance* eventInstance = eventIt->second;
                if (eventInstance)
                {
                    eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);            // резкая остановка
                    eventInstance->release();
                    eventInstance = nullptr;
                }

                // Удаление элемента из контейнера
                it->second.erase(eventIt);

                if (it->second.empty())
                {
                    // Если контейнер пуст, удалить элемент из основного контейнера
                    fMassive->mElementEvent.erase(it);
                }

                return true;
            }
        }
    }
    // Элемент не найден
    return false;
}



bool isEventInstanceExists(const SString& elementName, const SString& eventName)
{
    if (fMassive)
    {
        auto elementIt = fMassive->mElementEvent.find(elementName);
        if (elementIt != fMassive->mElementEvent.end())
        {
            auto eventIt = elementIt->second.find(eventName);
            if (eventIt != elementIt->second.end() && eventIt->second != nullptr)
            {
                return true;
            }
        }
    }
    return false;
}


bool playFmodEventForElement(const SString& elementName, const SString& eventName)
{
    auto eventInst = getDataEventInstanceForElement(elementName, eventName);
    if (eventInst != nullptr)
    {
        if (eventInst->start() == FMOD_OK)            // Добавлена проверка успешного начала воспроизведения
        {
            return true;
        }
    }
    return false;
}

bool stopFmodEventForElement(const SString& elementName, const SString& eventName, bool bImmediate)
{
    FMOD_STUDIO_STOP_MODE eMode;
    eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
    auto event = getDataEventInstanceForElement(elementName, eventName);
    if (nullptr != event)            // если не содержит ошибки
    {
        if (event->stop(eMode))
            return true;
    }
    event = nullptr;
    return false;
}

bool setPauseFmodEventForElement(const SString& elementName, const SString& eventName, bool pState)
{
    auto event = getDataEventInstanceForElement(elementName, eventName);
    if (nullptr != event)            // если не содержит ошибки
    {
        event->setPaused(pState);
        return true;
    }
    event = nullptr;
    return false;
}

bool getPauseFmodEventForElement(const SString& elementName, const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstanceForElement(elementName, eventName);
    if (eventInstance != nullptr)
    {
        bool paused;
        eventInstance->getPaused(&paused);
        return paused;
    }

    // Возвращаем false, если указанное звуковое событие не найдено.
    return false;
}

bool setFmodEvent3DPositionForElement(const SString& elementName, const SString& eventName, const CVector& pos)
{
    if (isnan(pos.fX) || isnan(pos.fY) || isnan(pos.fZ))            // проверяем наличие NaN
    {
        // Если хотя бы одна координата содержит NaN, то сразу же возвращаем false.
        return false;
    }

    auto event = getDataEventInstanceForElement(elementName, eventName);
    if (nullptr != event)            // если не содержит ошибки
    {
        FMOD_3D_ATTRIBUTES attributes = {};
        /*       memset(&attributes, 0, sizeof(attributes));*/

        attributes.forward.z = 1.0f;
        attributes.up.y = 1.0f;
        // f_studio->setListenerAttributes(0, &attributes);

        attributes.position.x = (float)pos.fX;
        attributes.position.y = (float)pos.fY;
        attributes.position.z = (float)pos.fZ;

        FMOD_RESULT result = event->set3DAttributes(&attributes);
        // return FMOD_ErrorString(result);
        if (FMOD_OK == result)
        {
            return true;
        }
    }
    // return FMOD_ErrorString(result);
    return false;
}

CVector getFmodEvent3DPositionForElement(const SString& elementName, const SString& eventName)
{
    CVector pos;
    auto    event = getDataEventInstanceForElement(elementName, eventName);
    if (nullptr != event)            // если не содержит ошибки
    {
        FMOD_3D_ATTRIBUTES attributes;
        memset(&attributes, 0, sizeof(attributes));
        if (FMOD_OK == event->get3DAttributes(&attributes))
        {
            pos = ToCVector(attributes.position);            // преобразуем координаты из FMOD_VECTOR в CVector
        }
    }
    return pos;
}

// Установить громкость звука

bool setFmodEventVolumeForElement(const SString& elementName, const SString& eventName, float volume)
{
    auto event = getDataEventInstanceForElement(elementName, eventName);
    if (event != nullptr)                    // если экземпляр события создан успешно
    {
        event->setVolume(volume);            // установить громкость звука
        return true;
    }
    event = nullptr;
    return false;
}

// Получить текущую громкость звука
float getFmodEventVolumeForElement(const SString& elementName, const SString& eventName)
{
    float volume = 0.0f;
    auto  event = getDataEventInstanceForElement(elementName, eventName);
    if (event != nullptr)                     // если экземпляр события создан успешно
    {
        event->getVolume(&volume);            // получить текущую громкость звука
    }
    return volume;
}

bool setFmodParameterForElement(const SString& elementName, const SString& eventName, const char* paramName, float value)
{
    FMOD::Studio::EventInstance* eventInst = getDataEventInstanceForElement(elementName, eventName);
    if (!eventInst)
    {
        // обработка ошибки получения экземпляра эвента
        return false;
    }

    FMOD_RESULT result = eventInst->setParameterByName(paramName, value);
    if (result == FMOD_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;            // ну а вдруг
}

float getFmodParameterForElement(const SString& elementName, const SString& eventName, const char* paramName)
{
    FMOD::Studio::EventInstance* eventInst = getDataEventInstanceForElement(elementName, eventName);
    if (!eventInst)
    {
        // обработка ошибки получения экземпляра эвента
        return 0;
    }

    float       value;
    FMOD_RESULT result = eventInst->getParameterByName(paramName, &value);
    if (result != FMOD_OK)
    {
        return 0;
    }

    return value;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////Этот код выполняет обработку событий без владельца//////////////
///////////////////////////////////////////////////////////////////////////
bool setDataEventInstance(const SString& eventName, FMOD::Studio::EventInstance* pEventInstance)
{
    // Проверка переданного указателя на nullptr.
    if (pEventInstance == nullptr)
    {
        return false;
    }

    // Поиск значения для заданного имени события.
    auto it = fMassive->mEvents.find(eventName);
    if (it != fMassive->mEvents.end())
    {
        // Старое значение найдено. Удаление экземпляра звукового события.
        auto& oldEventInstance = it->second;
        if (oldEventInstance && oldEventInstance != pEventInstance)
        {
            oldEventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
            oldEventInstance->release();
        }
    }
    // Запись нового значения для заданного имени события.
    fMassive->mEvents[eventName] = pEventInstance;
    return true;
}

FMOD::Studio::EventInstance* getDataEventInstance(const SString& eventName)
{
    // Поиск значения для заданного имени события.
    auto it = fMassive->mEvents.find(eventName);
    if (it != fMassive->mEvents.end())
    {
        // Значение найдено. Возвращаем экземпляр звукового события.
        return it->second;
    }

    // Значение не найдено. Возвращаем nullptr.
    return nullptr;
}

bool loadFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventDescription* eventDescription = nullptr;
    result = f_studio->getEvent(eventName, &eventDescription);
    if (result != FMOD_OK || eventDescription == nullptr)
    {
        // Обработка ошибки.
        return false;
    }

    FMOD::Studio::EventInstance* eventInstance = nullptr;
    eventDescription->createInstance(&eventInstance);
    if (eventInstance == nullptr)
    {
        // Обработка ошибки.
        return false;
    }

    return setDataEventInstance(eventName, eventInstance);
}

bool releaseFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstance(eventName);
    if (eventInstance != nullptr)
    {
        eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        eventInstance->release();
        return true;
    }
    // Возвращаем false, если указанное звуковое событие не найдено.
    return false;
}

bool playFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstance(eventName);
    if (eventInstance != nullptr)
    {
        eventInstance->start();
        return true;
    }
    return false;
}

bool stopFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstance(eventName);
    if (eventInstance != nullptr)
    {
        eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        return true;
    }

    // Возвращаем false, если указанное звуковое событие не найдено.
    return false;
}

bool setPauseFmodEvent(const SString& eventName, bool paused)
{
    FMOD::Studio::EventInstance* event = getDataEventInstance(eventName);
    if (event != nullptr)
    {
        event->setPaused(paused);
        return true;
    }
    // Возвращаем false, если указанное звуковое событие не найдено.
    return false;
}

bool getPauseFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstance(eventName);
    if (eventInstance != nullptr)
    {
        bool paused;
        eventInstance->getPaused(&paused);
        return paused;
    }

    // Возвращаем false, если указанное звуковое событие не найдено.
    return false;
}

SString setPosFmodEvent(const SString& eventName, CVector pos /*, CVector forw*/)
{
    if (isnan(pos.fX) || isnan(pos.fY) || isnan(pos.fZ))            // проверяем наличие NaN
    {
        // Если хотя бы одна координата содержит NaN, то сразу же возвращаем false.
        return false;
    }

    auto event = getDataEventInstance(eventName);
    if (nullptr != event)            // если не содержит ошибки
    {
        FMOD_3D_ATTRIBUTES attributes = {};
        /*       memset(&attributes, 0, sizeof(attributes));*/

        attributes.forward.z = 1.0f;
        attributes.up.y = 1.0f;
        // f_studio->setListenerAttributes(0, &attributes);

        attributes.position.x = (float)pos.fX;
        attributes.position.y = (float)pos.fY;
        attributes.position.z = (float)pos.fZ;

        FMOD_RESULT result = event->set3DAttributes(&attributes);
        return FMOD_ErrorString(result);
        // if (FMOD_OK == result)
        //{
        //     return true;
        // }
    }
    return FMOD_ErrorString(result);
    // return false;
}

CVector getPosFmodEvent(const SString& eventName)
{
    FMOD::Studio::EventInstance* eventInstance = getDataEventInstance(eventName);
    if (eventInstance != nullptr)
    {
        FMOD_3D_ATTRIBUTES attributes;
        memset(&attributes, 0, sizeof(attributes));

        eventInstance->get3DAttributes(&attributes);

        return ToCVector(attributes.position);
    }

    // Возвращаем пустой вектор, если указанное звуковое событие не найдено.
    return CVector();
}

bool setFmodEventParameter(const SString& eventName, const SString& paramName, float value)
{
    FMOD::Studio::EventInstance* eventInst = getDataEventInstance(eventName);
    if (!eventInst)
    {
        // обработка ошибки получения экземпляра эвента
        return false;
    }

    FMOD_RESULT result = eventInst->setParameterByName(paramName, value);
    if (result == FMOD_OK)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;            // ну а вдруг
}

float getFmodEventParameter(const SString& eventName, const SString& paramName)
{
    FMOD::Studio::EventInstance* eventInst = getDataEventInstance(eventName);
    if (!eventInst)
    {
        // обработка ошибки получения экземпляра эвента
        return 0;
    }

    float       value;
    FMOD_RESULT result = eventInst->getParameterByName(paramName, &value);
    if (result != FMOD_OK)
    {
        return 0;
    }

    return value;
}

bool setVolumeFmodEvent(const SString& eventName, float volume)
{
    auto event = getDataEventInstance(eventName);
    if (event != nullptr)                    // если экземпляр события создан успешно
    {
        event->setVolume(volume);            // установить громкость звука
        return true;
    }
    return false;
}

// Получить текущую громкость звука
float getVolumeFmodEvent(const SString& eventName)
{
    float volume = 0.0f;
    auto  event = getDataEventInstance(eventName);
    if (event != nullptr)                     // если экземпляр события создан успешно
    {
        event->getVolume(&volume);            // получить текущую громкость звука
    }
    return volume;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
