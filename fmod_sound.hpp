#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"
#include "fmod_channel.hpp"            // ����� ������� �������

FMOD::Sound* getSoundFmod(const SString& soundDirrectory)
{
    // ����� �������� ��� ��������� ����� �������.
    auto it = fMassive->mSounds.find(soundDirrectory);
    if (it != fMassive->mSounds.end())
    {
        // �������� �������. ���������� ��������� ��������� �������.
        return it->second;
    }

    // �������� �� �������. ���������� nullptr.
    return nullptr;
}

bool setSoundFmod(const SString& soundDirrectory, FMOD::Sound* sound)
{
    if (getSoundFmod(soundDirrectory))            // ���� ����� ���� ��� ����������,�� �� ��� �� ����������
    {
        return false;
    }
    // �������� ����������� ��������� �� nullptr.
    if (sound == nullptr)
    {
        return false;
    }
    fMassive->mSounds[soundDirrectory] = sound;
    return true;
}

FmodApiFunctionFormat<bool, SString> createFmodSound(const SString& soundDirectory, SString flagSound)
{
    FMOD::Sound* pSound = nullptr;
    result = f_system->createSound(soundDirectory, getFmodFlagSound(flagSound), 0, &pSound);
    if (result == FMOD_OK)
    {
        setSoundFmod(soundDirectory, pSound);
        // pSound->setMode(FMOD_LOOP_NORMAL);
        // pSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
        return FmodMakeApiFunction(true, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, FMOD_ErrorString(result));
}

FmodApiFunctionFormat<int, SString> playFmodSound(const SString& soundDirectory)            // ������ ����� -1 � ������
{
    auto pSound = getSoundFmod(soundDirectory);
    if (pSound != nullptr)
    {
        int            nChannelID = getFmodFreeIdChannel();
        FMOD::Channel* pChannel = nullptr;
        FMOD_RESULT    result = f_system->playSound(pSound, nullptr, false, &pChannel);
        if (result == FMOD_OK)
        {
            bool channelSet = setChannelFmod(nChannelID, pChannel);
            if (channelSet)
            {
                return FmodMakeApiFunction(nChannelID, FMOD_ErrorString(result));
            }
            FmodMakeApiFunction(nChannelID, SString("%s", "�� ������� �������� ��������� ������"));
        }
        return FmodMakeApiFunction(-1, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(-1, SString("%s", "���� - �� ���� �����������"));
}

FmodApiFunctionFormat<bool, SString> releaseFmodSound(const SString& soundDirectory)            // ������ ����� -1 � ������
{
    auto pSound = getSoundFmod(soundDirectory);
    if (pSound != nullptr)
    {
        FMOD_RESULT result = pSound->release();
        
        if (result == FMOD_OK)
        {
            return FmodMakeApiFunction(true, FMOD_ErrorString(result));
        }
        return FmodMakeApiFunction(false, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, SString("%s", "���� - �� ������"));
}

bool setFmodSound3DPosion(int channel, CVector position) // ����� �������� �����������  CVector forward
{
    FMOD::Channel* fChannel = getChannelFmod(channel);
    if (fChannel != nullptr)
    {
        FMOD_3D_ATTRIBUTES listenerAttributes = {{0}};
        result = fChannel->set3DAttributes(&ToFMOD_VECTOR(position), NULL);
        //fChannel->set3DMinMaxDistance
        if (result == FMOD_OK)
        {
            return true;
        }
    }
    return false;
}

CVector getFmodSound3DPosion(int channel)            // ����� �������� �����������  CVector forward
{
    FMOD::Channel* fChannel = getChannelFmod(channel);
    if (fChannel != nullptr)
    {
        FMOD_3D_ATTRIBUTES attributes = {};
        result = fChannel->get3DAttributes(&attributes.position,0);
        if (result == FMOD_OK)
        {
            return ToCVector(attributes.position);
        }
    }
    return CVector();
}
