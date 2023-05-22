// #pragma once
// #include "SharedPatamFMOD.hpp"            // helper :)
// #include "fmod_init.hpp"
//
//
//
//
//
//
// bool fmodAllUpdate()
//{
//     if (::IsFmodCoreInit() == true)
//     {
//         f_system->update();
//     }
//     if (::IsFmodStudioInit() == true)
//     {
//         f_studio->update();
//     }
//     return true;
// }
//
// SString set3DListenerPosition(CVector position, CVector forward, CVector up, CVector velocity)
//{
//     FMOD_3D_ATTRIBUTES listenerAttributes = {{0}};
//
//     listenerAttributes.position = ToFMOD_VECTOR(position);
//     listenerAttributes.velocity = ToFMOD_VECTOR(velocity);
//     auto Forw = forward;
//     Forw.Normalize();
//     listenerAttributes.forward = ToFMOD_VECTOR(Forw);
//     auto lup = up;
//     lup.Normalize();
//     listenerAttributes.up = ToFMOD_VECTOR(lup);
//     result =
//         f_system->set3DListenerAttributes(0, &listenerAttributes.position, &listenerAttributes.velocity, &listenerAttributes.forward,
//         &listenerAttributes.up);
//     // f_system->update();
//     return FMOD_ErrorString(result);
// }
//
////std::pair<bool, SString> loadSound(SString element, SString soundDirectory, SString flagSound)
////{
////    FMOD::Sound* pSound = nullptr;
////    result = f_system->createSound(soundDirectory, getFmodFlagSound(flagSound), 0, &pSound);
////    if (result == FMOD_OK)
////    {
////        ArrayMap->mSounds[element][soundDirectory] = pSound;
////        // pSound->setMode(FMOD_LOOP_NORMAL);
////        // pSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
////        // unsigned int msLength = 0;
////        // pSound->getLength(&msLength, FMOD_TIMEUNIT_MS);
////        return std::make_pair(true, FMOD_ErrorString(result));
////    }
////    return std::make_pair(false, FMOD_ErrorString(result));
////}
//
//
