typedef struct
{
    double x, y;
} Vec2;

typedef struct
{
    double x, y, z;
} Vec3;

typedef struct
{
    int x, y;
} Vec2i;

typedef struct
{
    int x, y, z;
} Vec3i;

#define vec2(x, y) (     \
    {                    \
        Vec2 v = {x, y}; \
        v;               \
    })

#define vec3(x, y, z) (     \
    {                       \
        Vec3 v = {x, y, z}; \
        v;                  \
    })

#define vec2i(x, y) (     \
    {                     \
        Vec2i v = {x, y}; \
        v;                \
    })

#define vec3i(x, y, z) (     \
    {                        \
        Vec3i v = {x, y, z}; \
        v;                   \
    })

#define vec3FromVec2(v) (        \
    {                            \
        Vec2 vec = v;            \
        vec3(vec.x, vec.y, 0.0); \
    })

#define vec2FromVec3(v) (   \
    {                       \
        Vec3 vec = v;       \
        vec2(vec.x, vec.y); \
    })

Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec3 vec3_add(Vec3 v1, Vec3 v2);

Vec2 vec2_sub(Vec2 v1, Vec2 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);

Vec2 vec2_mult(Vec2 v, double f);
Vec3 vec3_mult(Vec3 v, double f);

Vec2 vec2_div(Vec2 v, double f);
Vec3 vec3_div(Vec3 v, double f);

Vec2 vec2_norm(Vec2 v);
Vec3 vec3_norm(Vec3 v);

double vec2_length(Vec2 v);
double vec3_length(Vec3 v);

double vec2_length2(Vec2 v);
double vec3_length2(Vec3 v);

double vec2_distance(Vec2 v1, Vec2 v2);
double vec3_distance(Vec3 v1, Vec3 v2);

double vec2_distance2(Vec2 v1, Vec2 v2);
double vec3_distance2(Vec3 v1, Vec3 v2);

double vec2_dot(Vec2 v1, Vec2 v2);
double vec3_dot(Vec3 v1, Vec3 v2);

Vec3 vec3_cross(Vec3 v1, Vec3 v2);