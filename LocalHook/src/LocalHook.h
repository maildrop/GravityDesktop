#pragma once
#if !defined( LOCAL_HOOK_HEADER_GUARD )
#define LOCAL_HOOK_HEADER_GUARD 1

#include <Windows.h>

/*
 ���̃}�N���́A__declspec( dllxport ) �� __declspec( dllimport ) ��؂�ւ��邽�߂ɂ���
 ���̂��߂ɁALocalHook �̃v���W�F�N�g �v���p�e�B 
 �\���v���p�e�B 
    C/C++
        �v���v���Z�b�T �� �v���v���Z�b�T�̒�` �ɂ�����
        EXPORT_ 
        ���`����K�v����B���̒�`�ɂ���āALocalHook.dll ���쐬����Ƃ��� __declspec(dllexport) ���g����

 ���̃w�b�_���g����ʂł́A�v���v���Z�b�T��` EXPORT_ �͕K�v����
 ���̏ꍇ __declspec( dllimport ) ���I�������B
*/

#ifdef EXPORT_
#define EXPORT_API_ __declspec(dllexport)
#else
#define EXPORT_API_ __declspec(dllimport)
#endif 

EXPORT_API_ LRESULT CALLBACK KeyHookProc(int, WPARAM, LPARAM);
EXPORT_API_ LRESULT CALLBACK MouseHookProc(int, WPARAM, LPARAM);
EXPORT_API_ bool SetHook(HWND hwnd, HWND target);
EXPORT_API_ bool ResetHook();

#endif /* !defined( LOCAL_HOOK_HEADER_GUARD ) */
