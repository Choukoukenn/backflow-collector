#pragma once

#include "Float3.h"

//---------------------------------------------------------------------------------
//	‚R‚c‚Ì“–‚½‚è”»’è
//---------------------------------------------------------------------------------
bool CheckCircleXZHit( Float3 &center1, float radius1, Float3 &center2, float radius2 );
bool CheckBallHit( Float3 &center1, float radius1, Float3 &center2, float radius2 );
bool CheckBoxHit3D( Float3 &box_pos1, Float3 &box_size1, Float3 &box_pos2, Float3 &box_size2 );

Float3 GetFloat3LinePointNearPosition( Float3 &line_start, Float3 &line_goal, Float3 &point );
float GetFloat3LinePointNearDistance( Float3 &line_start, Float3 &line_goal, Float3 &point );
bool CheckLineBallHit( Float3 &line_start, Float3 &line_goal, Float3 &ball_pos, float ball_radius );

#include "Float2.h"

//---------------------------------------------------------------------------------
//	‚Q‚c‚Ì“–‚½‚è”»’è
//---------------------------------------------------------------------------------
//	À•W‚âlŠp‚Ì‘å‚«‚³‚É Float2 ‚ğ“n‚·ƒo[ƒWƒ‡ƒ“
bool CheckCircleHit( Float2 &center1, float radius1, Float2 &center2, float radius2 );
bool CheckPointCircleHit( Float2 &point, Float2 &center, float radius );
bool CheckBoxHit( Float2 &box_pos1, Float2 &box_size1, Float2 &box_pos2, Float2 &box_size2 );
bool CheckPointBoxHit( Float2 &point, Float2 &box_pos, Float2 &box_size );
bool CheckCircleBoxHit( Float2 &circle, float radius, Float2 &box_pos, Float2 &box_size );

bool CheckPointTriangleHit( Float2 &point, Float2 &triangle_pos1, Float2 &triangle_pos2, Float2 &triangle_pos3 );
Float2 GetFloat2LinePointNearPosition( Float2 &line_start, Float2 &line_goal, Float2 &point );
float GetFloat2LinePointNearDistance( Float2 &line_start, Float2 &line_goal, Float2 &point );
bool CheckLineCircleHit( Float2 &line_start, Float2 &line_goal, Float2 &circle_pos, float circle_radius );

//	À•W‚âlŠp‚Ì‘å‚«‚³‚É float ‚ğ“n‚·ƒo[ƒWƒ‡ƒ“
float GetDistance( float x1, float y1, float x2, float y2 );
bool CheckCircleHit( float x1, float y1, float r1, float x2, float y2, float r2 );
bool CheckPointCircleHit( float point_x, float point_y, float circle_x, float circle_y, float circle_r );
bool CheckBoxHit( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 );
bool CheckPointBoxHit( float point_x, float point_y, float box_x, float box_y, float box_w, float box_h );
bool CheckCircleBoxHit( float circle_x, float circle_y, float circle_r, float box_x, float box_y, float box_w, float box_h );
