#pragma once
#include "SharedParamFMOD.hpp"
#include "CLuaFmod.h"

FMOD::Studio::Bank* getBankFmod(const SString& bankDirrectory)
{
    // Поиск значения для заданного имени события.
    auto it = fMassive->mBanks.find(bankDirrectory);
    if (it != fMassive->mBanks.end())
    {
        // Значение найдено. Возвращаем экземпляр звукового события.
        return it->second;
    }

    // Значение не найдено. Возвращаем nullptr.
    return nullptr;
}

bool setBankFmod(const SString& bankDirrectory, FMOD::Studio::Bank* bank)
{
    if (getBankFmod(bankDirrectory))            // если такой банк уже существует,то мы его не записываем
    {
        return false;
    }
    // Проверка переданного указателя на nullptr.
    if (bank == nullptr)
    {
        return false;
    }

    fMassive->mBanks[bankDirrectory] = bank;
    return true;
}
///////////////////////////////////////////////////////////////////////////
//////////  Этот код выполняет логику банк файлов в системе  //////////////
///////////////////////////////////////////////////////////////////////////

FmodApiFunctionFormat<bool, SString> loadFmodBankFile(const SString& bankFile)
{
    FMOD::Studio::Bank* pBank;
    FMOD_RESULT         result = f_studio->loadBankFile(bankFile, FMOD_STUDIO_LOAD_BANK_NORMAL, &pBank);
    if (result == FMOD_OK)
    {
        return FmodMakeApiFunction(setBankFmod(bankFile, pBank), FMOD_ErrorString(result));            // вернет bool результат загрузки в массив, и код ошибки
    }
    return FmodMakeApiFunction(false, FMOD_ErrorString(result));
}

FmodApiFunctionFormat<bool, SString> unloadFmodBankFile(const SString& bankFile)
{
    auto pBank = getBankFmod(bankFile);
    if (pBank != nullptr)
    {
        FMOD_RESULT result = pBank->unload();
        if (result == FMOD_OK)
        {
            return FmodMakeApiFunction(true, FMOD_ErrorString(result));
        }
        return FmodMakeApiFunction(false, FMOD_ErrorString(result));
    }
    return FmodMakeApiFunction(false, SString("%s", "запрашиваемый банк файл - не найден"));
}

SString getFmodBankLoadingState(const SString& bankDir)
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_RESULT               result = fMassive->mBanks[bankDir]->getLoadingState(&state);
    if (FMOD_OK == result)
    {
        return SString("%s", getLocalizedLoadingStateBank[state].state);
    }
    return SString("[Ошибка -> %s]",FMOD_ErrorString(result));            // при загрузке ресурсов произошла ошибка
}
