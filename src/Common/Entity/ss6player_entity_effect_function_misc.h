﻿/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO: 【注意!!】                                                           */
/* 本ヘッダはentity_effect.hの一部です（その他・未分類関数群）。              */
/* 単独でincludeしないでください（一応コンパイルエラー防護はしているものの、  */
/* 内容が正常に適用されません）。                                             */
/* ※EntityEffectクラスの機能が大きい巨大なため、各区分の機能を見通しやすくす */
/*   るために（C#のpartial的に）ファイルを分割してあるだけです。              */
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#if defined(__SS6PLAYER_ENTITY_CLASS__)

/* -------------------------------------------------------------------------- */
/*                                                  Class Functions (Partial) */
/* -------------------------------------------------------------------------- */
// namespace SpriteStudio6	{
// class Entity	{
	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! エンティティが使用可能かの状態を取得
	/*!
	@param	なし

	@retval	関数値
		true == BootUpが終了している（使用可能）
		false == BootUpされていない（or 失敗）

	エンティティがBootUpして使用可能な状態にあるかを取得します。
	ただし、本関数はBootUpが終了しているかだけをチェックしますので、
		PackSetの実行後か（アニメーションデータの設定も終わって再生
		可能な状態にあるか）を取得したい場合は、StatusGetValidPlay
		関数を使用してください。
	※つまり、本関数は「PackSet」を実行することが可能であるかの
		取得に使用します。
	*/
	bool StatustGetValidBootUp(void);

	/* ********************************************************* */
	//! エンティティが再生可能かの状態を取得
	/*!
	@param	なし

	@retval	関数値
		true == PackSetが終了している（再生可能）
		false == PackSetされていない（or 失敗）

	エンティティがBootUpして使用可能な状態にあるかを取得します。
	ただし、本関数はBootUpとPackSetが終了しているかをチェックします。
	本関数がtrueを返した場合、StatusGetValidBootUpも同時にtrueである
		ことを意味します。
	※つまり、本関数は「AnimationPlay」を実行することが可能であるかの
		取得に使用します。
	*/
	bool StatusGetValidPlay(void);

	/* ********************************************************* */
	//! 動的パーツカラーパラメータの生成
	/*!
	@param	なし

	@retval	関数値
		動的パーツカラーのポインタ

	動的パーツカラーのバッファを作成して、そのポインタを返します。
	原則として、動的パーツカラーは、各アニメーションエンティティ毎
		に1つのみ持つことが可能で・エンティティの種類によって挙動が
		若干異なります（ただし、本関数のflagInvolveChildrenにtrueを
		与えた場合、子エンティティ群とパラメータを共有するような
		設定を行います）。

	- Entityクラス（アニメーション）
		アニメーションが持つすべてのパーツに本値がパーツカラーとして
			適用されます。
		アニメーションデータの「パーツカラー」を持っているパーツの場
			合には、そのデータへの上書き（差し替え）で適用されます。
		データと本値が同時に効果するわけではありませんので、注意して
		ください。
	- EntityEffectクラス（エフェクト）
		エフェクトが発生させている全てのパーティクルに本値が乗算適用
			されます（つまり、「なし」か「乗算」しか有効ではありませ
			ん）。
		また、色の設定は頂点毎には行えず・SetOverallのみでの設定にな
			ります（SetVertexで設定した場合、左上の色が適用されます）。
		※本機能については、現状非対応です。

	動的パーツカラーが必要なくなった場合、AdditionalColorReleaseで
		パラメータを解放してください。
	一時的に使用しない場合はAdditionalColorReleaseではなく、本パラ
		メータの合成方法にNONを与えて設定することで一時的に動的パー
		ツカラーの効果を無効にできます。

	動的パーツカラーを使い続ける限り、本関数が返すパラメータは静的に
		確保されており、ポインタが変わることはありません。

	本関数は複数回アクセスしても、（作成済かつ解放されていなければ）
		同じパラメータ（ポインタ）を返します。
	*/
	Library::Control::AdditionalColor* AdditionalColorCreate(void);

	/* ********************************************************* */
	//! 動的パーツカラーパラメータの解放
	/*!
	@param	なし

	@retval	なし

	AddtionalColorCreateで取得したパラメータを解放（破棄）します。
	子エンティティ群で同じパラメータを共用しているものも同時に
		解放します。
	必ず、AdditionalColorCreateを行ったエンティティから行ってくだ
		さい（エンティティが異なる場合、メモリの解放ミスを起こし
		ますので、最悪例外やハングアップなどの障害が出る可能性が
		あります）。

	AdditionalColorCreateで最初に作成したパラメータは、最終的に
		本関数で解放しないと、メモリリークを起こしますので注意して
		ください。
	※解放タイミングについては、原則的にPackSet(nullptr)の前です。
		ただし、AdditionalColorCreate(true)で取得した場合には、
		最親エンティティのShutDown()前に廃棄されていれば問題あり
		ません。
	*/
	void AdditionalColorRelease(void);

// };	/* EntityEffect */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */