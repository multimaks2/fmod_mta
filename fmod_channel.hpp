#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"

FMOD::Channel* getChannelFmod(int numChannel)
{
    // Поиск значения для заданного имени события.
    auto it = fMassive->mChannels.find(numChannel);
    if (it != fMassive->mChannels.end())
    {
        // Значение найдено. Возвращаем экземпляр звукового события.
        return it->second;
    }

    // Значение не найдено. Возвращаем nullptr.
    return nullptr;
}

bool setChannelFmod(int numChannel, FMOD::Channel* channel)
{
    if (getChannelFmod(numChannel))            // если такой банк уже существует,то мы его не записываем
    {
        return false;
    }
    // Проверка переданного указателя на nullptr.
    if (channel == nullptr)
    {
        return false;
    }
    fMassive->mChannels[numChannel] = channel;
    return true;
}

int getFmodFreeIdChannel()
{
    return fMassive->mnNextChannelId++;
}


// Настройки мастера получили при создании f_system
bool setFmodMasterMute(bool mState)
{
    (mastergroup->setMute(mState));
    muted = mState;
    return muted;
}

bool getFmodMasterMute()
{
    return muted;
}
