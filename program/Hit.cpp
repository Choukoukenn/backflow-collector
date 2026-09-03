#include "Main.h"
#include "Game.h"
#include "Hit.h"

//*********************************************************************************
//	３Ｄの当たり判定
//*********************************************************************************
//---------------------------------------------------------------------------------
//	ＸＺ座標だけの円同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckCircleXZHit( Float3 &center1, float radius1, Float3 &center2, float radius2 )
{
	Float3 pos1 = center1;
	Float3 pos2 = center2;

	pos1.y = 0.0f;
	pos2.y = 0.0f;

	float distance = GetFloat3Distance( pos1, pos2 );
	float radius = radius1 + radius2;
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	球同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckBallHit( Float3 &center1, float radius1, Float3 &center2, float radius2 )
{
	float distance = GetFloat3Distance( center1, center2 );
	float radius = radius1 + radius2;
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	ボックス同士の当たり判定
//	（座標 pos を中心にサイズ size 分プラスマイナスしたボックス：２Ｄのような左上からの大きさでの判定ではないので注意）
//---------------------------------------------------------------------------------
bool CheckBoxHit3D( Float3 &box_pos1, Float3 &box_size1, Float3 &box_pos2, Float3 &box_size2 )
{
	float right1 = box_pos1.x + box_size1.x;
	float left1  = box_pos1.x - box_size1.x;
	float up1    = box_pos1.y + box_size1.y;
	float down1  = box_pos1.y - box_size1.y;
	float front1 = box_pos1.z + box_size1.z;
	float back1  = box_pos1.z - box_size1.z;

	float right2 = box_pos2.x + box_size2.x;
	float left2  = box_pos2.x - box_size2.x;
	float up2    = box_pos2.y + box_size2.y;
	float down2  = box_pos2.y - box_size2.y;
	float front2 = box_pos2.z + box_size2.z;
	float back2  = box_pos2.z - box_size2.z;

	if( right1 >= left2 && left1 <= right2 ){
		if( up1 >= down2 && down1 <= up2 ){
			if( front1 >= back2 && back1 <= front2 ){
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	線分と点の一番近い座標を取得する
//---------------------------------------------------------------------------------
Float3 GetFloat3LinePointNearPosition( Float3 &line_start, Float3 &line_goal, Float3 &point )
{
	Float3 line_dir = line_goal - line_start;
	line_dir.Normalize();

	Float3 point_dir = point - line_start;

	float line_dot = GetFloat3Dot( line_dir, point_dir );

	Float3 near_dir = line_dir * line_dot;

	Float3 near_pos = line_start + near_dir;

	Float3 dir1 = line_start - near_pos;

	if( GetFloat3Dot( line_dir, dir1 ) > 0.0f ){
		near_pos = line_start;
	}

	Float3 dir2 = line_goal - near_pos;

	if( GetFloat3Dot( line_dir, dir2 ) < 0.0f ){
		near_pos = line_goal;
	}

	return near_pos;
}
//---------------------------------------------------------------------------------
//	線分と点の一番近い距離を取得する
//---------------------------------------------------------------------------------
float GetFloat3LinePointNearDistance( Float3 &line_start, Float3 &line_goal, Float3 &point )
{
	Float3 near_pos = GetFloat3LinePointNearPosition( line_start, line_goal, point );

	float near_distance = GetFloat3Distance( point, near_pos );

	return near_distance;
}
//---------------------------------------------------------------------------------
//	線分と球の当たり判定
//---------------------------------------------------------------------------------
bool CheckLineBallHit( Float3 &line_start, Float3 &line_goal, Float3 &ball_pos, float ball_radius )
{
	float near_distance = GetFloat3LinePointNearDistance( line_start, line_goal, ball_pos );

	if( near_distance <= ball_radius ){
		return true;
	}
	return false;
}

//*********************************************************************************
//	２Ｄの当たり判定
//*********************************************************************************
//---------------------------------------------------------------------------------
//	円同士の当たり判定（座標に Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckCircleHit( Float2 &center1, float radius1, Float2 &center2, float radius2 )
{
	float distance = GetFloat2Distance( center1, center2 );
	float radius = radius1 + radius2;
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	点と円の当たり判定（座標に Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckPointCircleHit( Float2 &point, Float2 &center, float radius )
{
	float distance = GetFloat2Distance( point, center );
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	四角同士の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckBoxHit( Float2 &box_pos1, Float2 &box_size1, Float2 &box_pos2, Float2 &box_size2 )
{
	if( box_pos1.x + box_size1.x >= box_pos2.x && box_pos1.x <= box_pos2.x + box_size2.x ){
		if( box_pos1.y + box_size1.y >= box_pos2.y && box_pos1.y <= box_pos2.y + box_size2.y ){
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	点と四角の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckPointBoxHit( Float2 &point, Float2 &box_pos, Float2 &box_size )
{
	if( point.x >= box_pos.x && point.x <= box_pos.x + box_size.x ){
		if( point.y >= box_pos.y && point.y <= box_pos.y + box_size.y ){
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	円と四角の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckCircleBoxHit( Float2 &circle, float radius, Float2 &box_pos, Float2 &box_size )
{
	Float2 near_pos;

	near_pos.x = circle.x;
	if( near_pos.x < box_pos.x )				near_pos.x = box_pos.x;
	if( near_pos.x > box_pos.x + box_size.x )	near_pos.x = box_pos.x + box_size.x;

	near_pos.y = circle.y;
	if( near_pos.y < box_pos.y )				near_pos.y = box_pos.y;
	if( near_pos.y > box_pos.y + box_size.y )	near_pos.y = box_pos.y + box_size.y;

	if( CheckPointCircleHit( near_pos, circle, radius ) ){
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------------
//	点と三角形の当たり判定
//---------------------------------------------------------------------------------
bool CheckPointTriangleHit( Float2 &point, Float2 &triangle_pos1, Float2 &triangle_pos2, Float2 &triangle_pos3 )
{
	Float2 dir1 = triangle_pos1 - point;
	Float2 dir2 = triangle_pos2 - point;
	Float2 dir3 = triangle_pos3 - point;

	float cross1 = GetFloat2Cross( dir1, dir2 );
	float cross2 = GetFloat2Cross( dir2, dir3 );
	float cross3 = GetFloat2Cross( dir3, dir1 );

	if( cross1 >= 0.0f && cross2 >= 0.0f && cross3 >= 0.0f ){
		return true;
	}
	if( cross1 <= 0.0f && cross2 <= 0.0f && cross3 <= 0.0f ){
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------------
//	線分と点の一番近い座標を取得する
//---------------------------------------------------------------------------------
Float2 GetFloat2LinePointNearPosition( Float2 &line_start, Float2 &line_goal, Float2 &point )
{
	Float2 line_dir = line_goal - line_start;
	line_dir.Normalize();

	Float2 point_dir = point - line_start;

	float line_dot = GetFloat2Dot( line_dir, point_dir );

	Float2 near_dir = line_dir * line_dot;

	Float2 near_pos = line_start + near_dir;

	Float2 dir1 = line_start - near_pos;

	if( GetFloat2Dot( line_dir, dir1 ) > 0.0f ){
		near_pos = line_start;
	}

	Float2 dir2 = line_goal - near_pos;

	if( GetFloat2Dot( line_dir, dir2 ) < 0.0f ){
		near_pos = line_goal;
	}

	return near_pos;
}
//---------------------------------------------------------------------------------
//	線分と点の一番近い距離を取得する
//---------------------------------------------------------------------------------
float GetFloat2LinePointNearDistance( Float2 &line_start, Float2 &line_goal, Float2 &point )
{
	Float2 near_pos = GetFloat2LinePointNearPosition( line_start, line_goal, point );

	float near_distance = GetFloat2Distance( point, near_pos );

	return near_distance;
}
//---------------------------------------------------------------------------------
//	線分と円の当たり判定
//---------------------------------------------------------------------------------
bool CheckLineCircleHit( Float2 &line_start, Float2 &line_goal, Float2 &circle_pos, float circle_radius )
{
	float near_distance = GetFloat2LinePointNearDistance( line_start, line_goal, circle_pos );

	if( near_distance <= circle_radius ){
		return true;
	}
	return false;
}

//*********************************************************************************
//	座標や四角の大きさに float を渡すバージョン
//*********************************************************************************
//---------------------------------------------------------------------------------
//	２点の長さを取得する関数
//---------------------------------------------------------------------------------
float GetDistance( float x1, float y1, float x2, float y2 )
{
	float x = x1 - x2;
	float y = y1 - y2;
	float distance = sqrtf( x * x + y * y );
	return distance;
}
//---------------------------------------------------------------------------------
//	円同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckCircleHit( float x1, float y1, float r1, float x2, float y2, float r2 )
{
	float distance = GetDistance( x1, y1, x2, y2 );
	float radius = r1 + r2;
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	点と円の当たり判定
//---------------------------------------------------------------------------------
bool CheckPointCircleHit( float point_x, float point_y, float circle_x, float circle_y, float circle_r )
{
	float distance = GetDistance( point_x, point_y, circle_x, circle_y );
	if( distance <= circle_r ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	四角同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckBoxHit( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 )
{
	if( x1 + w1 >= x2 && x1 <= x2 + w2 ){
		if( y1 + h1 >= y2 && y1 <= y2 + h2 ){
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	点と四角の当たり判定
//---------------------------------------------------------------------------------
bool CheckPointBoxHit( float point_x, float point_y, float box_x, float box_y, float box_w, float box_h )
{
	if( point_x >= box_x && point_x <= box_x + box_w ){
		if( point_y >= box_y && point_y <= box_y + box_h ){
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	円と四角の当たり判定
//---------------------------------------------------------------------------------
bool CheckCircleBoxHit( float circle_x, float circle_y, float circle_r, float box_x, float box_y, float box_w, float box_h )
{
	float near_x = circle_x;
	if( near_x < box_x )			near_x = box_x;
	if( near_x > box_x + box_w )	near_x = box_x + box_w;

	float near_y = circle_y;
	if( near_y < box_y )			near_y = box_y;
	if( near_y > box_y + box_h )	near_y = box_y + box_h;

	if( CheckPointCircleHit( near_x, near_y, circle_x, circle_y, circle_r ) ){
		return true;
	}

	return false;
}
