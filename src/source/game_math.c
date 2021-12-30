#include "game_math.h"
#include <math.h>

Vec2 vec2_add(Vec2 a, Vec2 b) { return vec2(a.x + b.x, a.y + b.y); }
Vec3 vec3_add(Vec3 a, Vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }

Vec2 vec2_sub(Vec2 a, Vec2 b) { return vec2(a.x - b.x, a.y - b.y); }
Vec3 vec3_sub(Vec3 a, Vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }

Vec2 vec2_mult(Vec2 v, double f) { return vec2(v.x * f, v.y * f); }
Vec3 vec3_mult(Vec3 v, double f) { return vec3(v.x * f, v.y * f, v.z * f); }

Vec2 vec2_div(Vec2 v, double f) { return vec2(v.x / f, v.y / f); }
Vec3 vec3_div(Vec3 v, double f) { return vec3(v.x / f, v.y / f, v.z / f); }

Vec2 vec2_norm(Vec2 v) { return vec2_div(v, vec2_length(v)); }
Vec3 vec3_norm(Vec3 v) { return vec3_div(v, vec3_length(v)); }

double vec2_length(Vec2 v) { return sqrt(vec2_length2(v)); }
double vec3_length(Vec3 v) { return sqrt(vec3_length2(v)); }

double vec2_length2(Vec2 v) { return v.x * v.x + v.y * v.y; }
double vec3_length2(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

double vec2_distance(Vec2 a, Vec2 b) { return vec2_length(vec2_sub(a, b)); }
double vec3_distance(Vec3 a, Vec3 b) { return vec3_length(vec3_sub(a, b)); }

double vec2_distance2(Vec2 a, Vec2 b) { return vec2_length2(vec2_sub(a, b)); }
double vec3_distance2(Vec3 a, Vec3 b) { return vec3_length2(vec3_sub(a, b)); }

double vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
double vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}