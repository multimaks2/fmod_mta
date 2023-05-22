#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"

FMOD::Channel* getChannelFmod(int numChannel)
{
    // ����� �������� ��� ��������� ����� �������.
    auto it = fMassive->mChannels.find(numChannel);
    if (it != fMassive->mChannels.end())
    {
        // �������� �������. ���������� ��������� ��������� �������.
        return it->second;
    }

    // �������� �� �������. ���������� nullptr.
    return nullptr;
}

bool setChannelFmod(int numChannel, FMOD::Channel* channel)
{
    if (getChannelFmod(numChannel))            // ���� ����� ���� ��� ����������,�� �� ��� �� ����������
    {
        return false;
    }
    // �������� ����������� ��������� �� nullptr.
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


// ��������� ������� �������� ��� �������� f_system
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
