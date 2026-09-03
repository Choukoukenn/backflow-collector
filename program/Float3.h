#pragma once
#include"DxLib.h"

//---------------------------------------------------------------------------------
//	Float3 クラス（ x y z があるクラス）
//---------------------------------------------------------------------------------
class Float3
{
public:
	float	x;
	float	y;
	float	z;

	Float3();									//	コンストラクタ
	Float3( float x, float y, float z );
	Float3( VECTOR v );

	void Clear();								//	ゼロ初期化
	void Set( float x, float y, float z );		//	値のセット
	void Set( Float3 &f3 );

	float GetLength();							//	長さ取得
	void Normalize();							//	正規化
	void SetLength( float length );				//	長さ設定

	Float3& operator = ( const Float3 &f3 );	//	= 演算子のオーバーロード

	Float3& operator += ( const Float3 &f3 );	//	+= 演算子のオーバーロード
	Float3& operator -= ( const Float3 &f3 );	//	-= 演算子のオーバーロード
	Float3& operator *= ( const float f );		//	*= 演算子のオーバーロード
	Float3& operator /= ( const float f );		//	/= 演算子のオーバーロード

	VECTOR VGet();								//	DXライブラリで使う VECTOR を返す
	void VSet( VECTOR v );						//	DXライブラリで使う VECTOR の値を取得して設定
};

//	+ 演算子のオーバーロード
Float3 operator + ( const Float3 &f3_1, const Float3 &f3_2 );
//	- 演算子のオーバーロード
Float3 operator - ( const Float3 &f3_1, const Float3 &f3_2 );
//	* 演算子のオーバーロード
Float3 operator * ( const Float3 &f3, const float f );
//	/ 演算子のオーバーロード
Float3 operator / ( const Float3 &f3, const float f );

//	２つの Float3 の距離を求める
float GetFloat3Distance( Float3 &pos1, Float3 &pos2 );
//	２つの Float3 の内積を求める
float GetFloat3Dot( Float3 &v1, Float3 &v2 );
//	２つの Float3 の外積を求める
Float3 GetFloat3Cross( Float3 &v1, Float3 &v2 );
//	Float3 を MATRIX で変換したものを返す
Float3 GetFloat3VTransform( Float3 &v, MATRIX &mat );
