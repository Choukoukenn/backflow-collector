#pragma once

//---------------------------------------------------------------------------------
//	Float2 クラス
//---------------------------------------------------------------------------------
class Float2
{
public:
	float x;
	float y;

	Float2();									//	コンストラクタ
	Float2( float x, float y );

	void Clear();								//	ゼロ初期化
	void Set( float x, float y );				//	値のセット
	void Set( Float2 &f2 );

	float GetLength();							//	長さ取得
	void Normalize();							//	正規化
	void SetLength( float length );				//	長さ設定

	Float2& operator = ( const Float2 &f2 );	//	= 演算子のオーバーロード

	Float2& operator += ( const Float2 &f2 );	//	+= 演算子のオーバーロード
	Float2& operator -= ( const Float2 &f2 );	//	-= 演算子のオーバーロード
	Float2& operator *= ( const float f );		//	*= 演算子のオーバーロード
	Float2& operator /= ( const float f );		//	/= 演算子のオーバーロード
};

//	+ 演算子のオーバーロード
Float2 operator + ( const Float2 &f2_1, const Float2 &f2_2 );
//	- 演算子のオーバーロード
Float2 operator - ( const Float2 &f2_1, const Float2 &f2_2 );
//	* 演算子のオーバーロード
Float2 operator * ( const Float2 &f2, const float f );
//	/ 演算子のオーバーロード
Float2 operator / ( const Float2 &f2, const float f );

//	マウス座標を Float2 で取得
Float2 GetMouseFloat2();
//	マウスの移動量を Float2 で取得
Float2 GetMouseMoveFloat2();
//	２つの Float2 の距離を求める
float GetFloat2Distance( Float2 &pos1, Float2 &pos2 );
//	２つの Float2 の内積を求める
float GetFloat2Dot( Float2 &v1, Float2 &v2 );
//	２つの Float2 の外積を求める
float GetFloat2Cross( Float2 &v1, Float2 &v2 );
