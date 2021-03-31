﻿/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/*                                                                            */
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include <ss6player_system.h>
#include "application.h"

#if defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS)
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#else
#endif	/* defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS) */
#include <stdio.h>
#include <time.h>
#include <vector>

#include <GLFW/glfw3.h>

/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
/* 定数群 */
enum Constants	{
	VERSION_OPENGL_MAJOR = 4,
	VERSION_OPENGL_MINOR = 5,

	SCREEN_WIDTH = 1280,
	SCREEN_HEIGHT = 720,

	SWAP_INTERVAL = 1,
};
// #define NAME_RUNTIME_FILE_DIRECTORY	u8"../../DataRuntime/OpenGL/"
#define NAME_RUNTIME_FILE_DIRECTORY	u8"/User/Program/WebTechnology/SS6PlayerForCpp/DataRuntime/OpenGL/"

enum ErrorCode	{
	APPLICATION_ERROR = -1,
	APPLICATION_SUCCESS = 0,
};

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
static GLFWwindow* HandleWindowGLFW = nullptr;

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static bool BootUpOpenGL(void);
static void ShutDownOpenGL(void);

static void GLFWCallbackWindow(GLFWwindow* window);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! エントリーポイント
/*!
@param	なし
 
@return	なし
 
アプリケーションのエントリーポイントです。
*/
int main(void)
{
#if defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS)
	/* デバグ用ヒープ監視 */
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#endif	/* defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS) */

	/* 初期化 */
	{
		/* OpenGL関係 */
		if(false == BootUpOpenGL())	{	/* 失敗 */
			return(ErrorCode::APPLICATION_ERROR);
		}

		/* SS6Player関係 */
		SpriteStudio6::SettingSystem settingSystem;
		sprintf(settingSystem.FilePathBase, NAME_RUNTIME_FILE_DIRECTORY);
		settingSystem.RendererLimitBufferChunkDelayDelete = 1024;
		if(false == SpriteStudio6::SystemBootUp(settingSystem))	{	/* 終了 */
			return(ErrorCode::APPLICATION_ERROR);
		}

		/* アプリケーション */
		if(false == ApplicationBootUp())	{	/* 失敗 */
			return(ErrorCode::APPLICATION_ERROR);
		}
	}

	/* メインループ */
	{
		clock_t clockPrevious = 0;
		clock_t clockNow = 0;
		clock_t clockElapsed = 0;
		float timeElapseed = 0.0f;
		bool flagProcessMain = true;
		for(clockNow=0; flagProcessMain==true ; clockNow=clock())	{
			/* SS6Playerの定常ループ先頭処理 */
			SpriteStudio6::SystemProcessMainPre();

			/* 前フレームからの経過時間を取得 */
			clockElapsed = clockNow - clockPrevious;
			clockPrevious = clockNow;

			/* アプリケーションの定常更新 */
			timeElapseed = (float)clockElapsed / CLOCKS_PER_SEC;
			flagProcessMain = ApplicationUpdate(timeElapseed);

			/* SS6Playerの定常ループ末尾処理 */
			SpriteStudio6::SystemProcessMainPost();

			/* 描画 */
			/* MEMO: ApplicationUpdateが終わっている場合において、描画は別スレッドで行うことも可能です。 */
			/* MEMO: ApplicationDrawの引数は、OpenGL実装では使用しませんので、何の値を渡しても構いません。 */
			/*       ただし、nullptr以外の値を渡してください（内部処理でnullptrはエラーと判定するため）。  */
			uintptr_t commandList = UINTPTR_MAX;
			ApplicationDraw((SpriteStudio6::TypeDrawCommandList)commandList);

			/* フレームを進める */
			glfwSwapBuffers(HandleWindowGLFW);
			glfwPollEvents();

			/* 定常更新以外での終了判定 */
			if(GLFW_FALSE != glfwWindowShouldClose(HandleWindowGLFW))	{	/* ウィンドウが閉じられた */
				flagProcessMain = false;
			}

			/* SS6Playerの描画コマンドリスト作成スレッドの末尾処理 */ 
			/* MEMO: 基本的に、描画定義が終わった後に更新することを推奨します。 */
			/*       ※メインスレッドが描画コマンド構築スレッドと完全非同期で走 */
			/*         る（これは1描画に対して複数回メインスレッドが走ることを  */
			/*         意味せず、メインスレッドでの主処理部分と描画コマンド構築 */
			/*         スレッドとの同期がとられていないことを示します）場合には */
			/*         それを前提とした特段処理を行う必要がある点に注意してくだ */
			/*         さい。                                                   */
			SpriteStudio6::SystemProcessRenderPost();
		}
	}

	/* 終了 */
	{
		/* アプリケーション */
		ApplicationShutDown();

		/* SS6Player関係 */
		SpriteStudio6::SystemShutDown();

		/* OpenGL関係 */
		ShutDownOpenGL();
	}

	return(ErrorCode::APPLICATION_SUCCESS);
}

/* ********************************************************* */
//! OpenGLの初期化関連
/*!
@param	なし
 
@return	なし

*/
static bool BootUpOpenGL(void)
{
	/* GLFW: 初期化 */
	if(GLFW_FALSE == glfwInit())	{	/* 失敗 */
		return(false);
	}

	/* GLFW: OpenGLのバージョン指定 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_OPENGL_MINOR);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	/* 前方互換プロファイル */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	/* プロファイル */

	/* GLFW: ウィンドウ設置 */
	HandleWindowGLFW = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SS6Player for CPP", NULL, NULL);
	if(nullptr == HandleWindowGLFW)	{
		glfwTerminate();
		return(false);
	}
	glfwMakeContextCurrent(HandleWindowGLFW);
	glfwSwapInterval(SWAP_INTERVAL);	/* スワップのインターバル */

	/* GLEW: 初期化 */
	glewExperimental = true;	/* 部分的サポート */
	if(GLEW_OK != glewInit())	{	/* 失敗 */
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! OpenGLの終了関連
/*!
@param	なし
 
@return	なし

*/
static void ShutDownOpenGL(void)
{
	/* GLFW: 終了 */
	glfwTerminate();
}
