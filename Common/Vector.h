#pragma once

#ifndef _USE_MATH_DEFINES
#	ifdef _INC_MATH
#		error Included math.h without _USE_MATH_DEFINES!!!
#	else
#		define _USE_MATH_DEFINES
#	endif
#endif

#include <math.h>
#include <string>

#ifndef FLX_RAD_MATH
#define FLX_RAD_MATH
#define DegToRad(a)	(float(a)*(float(M_PI)/180.0f))
#define RadToDeg(a)	(float(a)*(180.0f/float(M_PI)))
#endif

class vec2 {
public:
  float X;
  float Y;

  //	Construktors
  vec2() : X(0.0f), Y(0.0f) {};
  vec2(const vec2& V1) : X(V1.X), Y(V1.Y) {};
  vec2(const float V1) : X(V1), Y(V1) {};
  vec2(const float* V1) : X(V1[0]), Y(V1[1]) {};
  vec2(const float x, const float y) : X(x), Y(y) {};
  void Set(const float x, const float y) {
    X = x;
    Y = y;
  };

  //	Operators
  void operator =(const vec2 &V1) {
    X = V1.X;
    Y = V1.Y;
  };
  void operator =(const float V1) {
    X = Y = V1;
  };
  void operator =(const float* V1) {
    X = V1[0];
    Y = V1[1];
  };

  vec2 operator +() {
    return *this;
  };
  vec2 operator +(const vec2& V1) {
    return vec2(X + V1.X, Y + V1.Y);
  };
  vec2 operator +(const float V1) {
    return vec2(X + V1, Y + V1);
  };
  vec2 operator +(const float* V1) {
    return vec2(X + V1[0], Y + V1[1]);
  };

  vec2 operator -() {
    return vec2(-X, -Y);
  };
  vec2 operator -(const vec2& V1) {
    return vec2(X - V1.X, Y - V1.Y);
  };
  vec2 operator -(const float V1) {
    return vec2(X - V1, Y - V1);
  };
  vec2 operator -(const float* V1) {
    return vec2(X - V1[0], Y - V1[1]);
  };

  vec2 operator *(const vec2& V1) {
    return vec2(X * V1.X, Y * V1.Y);
  };
  vec2 operator *(const float V1) {
    return vec2(X * V1, Y * V1);
  };
  vec2 operator *(const float* V1) {
    return vec2(X * V1[0], Y * V1[1]);
  };

  vec2 operator /(const vec2& V1) {
    return vec2(X / V1.X, Y / V1.Y);
  };
  vec2 operator /(const float V1) {
    return vec2(X / V1, Y / V1);
  };
  vec2 operator /(const float* V1) {
    return vec2(X / V1[0], Y / V1[1]);
  };

  void operator +=(const vec2& V1) {
    X += V1.X; Y += V1.Y;
  };
  void operator +=(const float V1) {
    X += V1; Y += V1;
  };
  void operator +=(const float* V1) {
    X += V1[0]; Y += V1[1];
  };

  void operator -=(const vec2& V1) {
    X -= V1.X; Y -= V1.Y;
  };
  void operator -=(const float V1) {
    X -= V1; Y -= V1;
  };
  void operator -=(const float* V1) {
    X -= V1[0]; Y -= V1[1];
  };

  void operator *=(const vec2& V1) {
    X *= V1.X; Y *= V1.Y;
  };
  void operator *=(const float V1) {
    X *= V1; Y *= V1;
  };
  void operator *=(const float* V1) {
    X *= V1[0]; Y *= V1[1];
  };

  void operator /=(const vec2& V1) {
    X /= V1.X; Y /= V1.Y;
  };
  void operator /=(const float V1) {
    X /= V1; Y /= V1;
  };
  void operator /=(const float* V1) {
    X /= V1[0]; Y /= V1[1];
  };

  float& operator [](int index) {
    switch (index) {
    case 0: return X;
    case 1: return Y;
    default: return X;
    }
  };

  bool operator ==(const vec2 &V1) {
    if (X == V1.X && Y == V1.Y)
      return true;
    return false;
  };
  bool operator ==(const float V1) {
    if (X == V1 && Y == V1)
      return true;
    return false;
  };
  bool operator ==(const float* V1) {
    if (X == V1[0] && Y == V1[1])
      return true;
    return false;
  };

  bool operator !=(const vec2 &V1) {
    return !(*this == V1);
  };
  bool operator !=(const float V1) {
    return !(*this == V1);
  };
  bool operator !=(const float* V1) {
    return !(*this == V1);
  };

  //	Methods
  float Length() {
    return sqrtf(X*X + Y*Y);
  };
  static float Length(vec2& V1, vec2& V2) {
    return (V2 - V1).Length();
  };
  float LengthSq() {
    return X*X + Y*Y;
  };
  static float LengthSq(vec2& V1, vec2& V2) {
    return (V2 - V1).LengthSq();
  }
  void Normalize() {
    float l = Length();
    if (l != 0.0f) l = 1.0f / l;
    X *= l;
    Y *= l;
  };
  static vec2 Normalize(const vec2& v) {
    vec2 a = v;
    a.Normalize();
    return a;
  };
  float Dot(const vec2& V1) {
    return (X * V1.X + Y * V1.Y);
  };
  float GetAngle() {
    float f = RadToDeg(atan2f(X, Y));
    if (fabsf(f) < 0.0001f) return 0.0f;
    return f;
  };
  void SetAngle(float ang, float r) {
    X = sinf(DegToRad(ang)) * r;
    Y = cosf(DegToRad(ang)) * r;
    if (fabs(X) < 0.0001f) X = 0.0f;
    if (fabs(Y) < 0.0001f) Y = 0.0f;
  };
  void ToFloat(float* V1) {
    V1[0] = X;
    V1[1] = Y;
  };
  float* ToFloat() {
    return &X;
  };
  std::string ToStr() {
    int len = _scprintf("V( %.4f, %.4f )", X, Y);
    char* buf = new char[len + 1];
    sprintf_s(buf, len + 1, "V( %.4f, %.4f )", X, Y);
    std::string ret = buf;
    delete[] buf;
    return ret;
  };
  std::string ToStr(char* str) {
    int len = _scprintf("V( %.4f, %.4f )", X, Y);
    sprintf_s(str, len + 1, "V( %.4f, %.4f )", X, Y);
    return std::string(str);
  }
};

class vec3 {
public:
  float X;
  float Y;
  float Z;

  void Set(const float x, const float y, const float z) {
    X = x; Y = y; Z = z;
  };
  //	Construktors
  vec3() : X(0.0f), Y(0.0f), Z(0.0f) {};
  vec3(const vec3& V1) : X(V1.X), Y(V1.Y), Z(V1.Z) {};
  vec3(const float V1) : X(V1), Y(V1), Z(V1) {};
  vec3(const float* V1) : X(V1[0]), Y(V1[1]), Z(V1[2]) {};
  vec3(const vec2& V1) : X(V1.X), Y(V1.Y), Z(0.0f) {};
  vec3(const vec2& V1, float z) : X(V1.X), Y(V1.Y), Z(z) {};
  vec3(const float x, const float y, const float z) : X(x), Y(y), Z(z) {};

  //	Operators
  void operator =(const vec3 &V1) {
    X = V1.X; Y = V1.Y; Z = V1.Z;
  };
  void operator =(const float V1) {
    X = Y = Z = V1;
  };
  void operator =(const float* V1) {
    X = V1[0]; Y = V1[1]; Z = V1[2];
  };

  vec3 operator +() {
    return *this;
  };
  vec3 operator +(const vec3& V1) {
    return vec3(X + V1.X, Y + V1.Y, Z + V1.Z);
  };
  vec3 operator +(const float V1) {
    return vec3(X + V1, Y + V1, Z + V1);
  };
  vec3 operator +(const float* V1) {
    return vec3(X + V1[0], Y + V1[1], Z + V1[2]);
  };

  vec3 operator -() {
    return vec3(-X, -Y, -Z);
  };
  vec3 operator -(const vec3& V1) {
    return vec3(X - V1.X, Y - V1.Y, Z - V1.Z);
  };
  vec3 operator -(const float V1) {
    return vec3(X - V1, Y - V1, Z - V1);
  };
  vec3 operator -(const float* V1) {
    return vec3(X - V1[0], Y - V1[1], Z - V1[2]);
  };

  vec3 operator *(const vec3& V1) {
    return vec3(X * V1.X, Y * V1.Y, Z * V1.Z);
  };
  vec3 operator *(const float V1) {
    return vec3(X * V1, Y * V1, Z * V1);
  };
  vec3 operator *(const float* V1) {
    return vec3(X * V1[0], Y * V1[1], Z * V1[2]);
  };

  vec3 operator /(const vec3& V1) {
    return vec3(X / V1.X, Y / V1.Y, Z / V1.Z);
  };
  vec3 operator /(const float V1) {
    return vec3(X / V1, Y / V1, Z / V1);
  };
  vec3 operator /(const float* V1) {
    return vec3(X / V1[0], Y / V1[1], Z / V1[2]);
  };

  void operator +=(const vec3& V1) {
    X += V1.X; Y += V1.Y; Z += V1.Z;
  };
  void operator +=(const float V1) {
    X += V1; Y += V1; Z += V1;
  };
  void operator +=(const float* V1) {
    X += V1[0]; Y += V1[1]; Z += V1[2];
  };

  void operator -=(const vec3& V1) {
    X -= V1.X; Y -= V1.Y; Z -= V1.Z;
  };
  void operator -=(const float V1) {
    X -= V1; Y -= V1; Z -= V1;
  };
  void operator -=(const float* V1) {
    X -= V1[0]; Y -= V1[1]; Z -= V1[2];
  };

  void operator *=(const vec3& V1) {
    X *= V1.X; Y *= V1.Y; Z *= V1.Z;
  };
  void operator *=(const float V1) {
    X *= V1; Y *= V1; Z *= V1;
  };
  void operator *=(const float* V1) {
    X *= V1[0]; Y *= V1[1]; Z *= V1[2];
  };

  void operator /=(const vec3& V1) {
    X /= V1.X; Y /= V1.Y; Z /= V1.Z;
  };
  void operator /=(const float V1) {
    X /= V1; Y /= V1; Z /= V1;
  };
  void operator /=(const float* V1) {
    X /= V1[0]; Y /= V1[1]; Z /= V1[2];
  };

  float& operator [](int index) {
    switch (index) {
    case 0: return X;
    case 1: return Y;
    case 2: return Z;
    default: return X;
    }
  };

  bool operator ==(const vec3 &V1) {
    if (X == V1.X && Y == V1.Y && Z == V1.Z)
      return true;
    return false;
  };
  bool operator ==(const float V1) {
    if (X == V1 && Y == V1 && Z == V1)
      return true;
    return false;
  };
  bool operator ==(const float* V1) {
    if (X == V1[0] && Y == V1[1] && Z == V1[2])
      return true;
    return false;
  };

  bool operator !=(const vec3 &V1) {
    return !(*this == V1);
  };
  bool operator !=(const float V1) {
    return !(*this == V1);
  };
  bool operator !=(const float* V1) {
    return !(*this == V1);
  };

  //	Methods
  vec3 Cross(const vec3 &V1) {
    return vec3((Y * V1.Z) - (V1.Y * Z),
      (V1.X * Z) - (X * V1.Z),
                (X * V1.Y) - (V1.X * Y));
  };
  float Dot(const vec3 &V1) {
    return X * V1.X + Y * V1.Y + Z * V1.Z;
  };
  float Length() {
    return sqrtf(X*X + Y*Y + Z*Z);
  };
  static float Length(vec3& V1, vec3& V2) {
    return (V2 - V1).Length();
  };
  float LengthSq() {
    return X*X + Y*Y + Z*Z;
  };
  static float LengthSq(vec3& V1, vec3& V2) {
    return (V2 - V1).LengthSq();
  };
  void Normalize() {
    float l = Length();
    if (l != 0.0f) l = 1.0f / l;
    X *= l;
    Y *= l;
    Z *= l;
  };
  static vec3 Normalize(vec3& v) {
    vec3 a = v;
    a.Normalize();
    return a;
  };
  void ToFloat(float* V1) {
    V1[0] = X;
    V1[1] = Y;
    V1[2] = Z;
  };
  float* ToFloat() {
    return &X;
  };
  std::string ToStr() {
    int len = _scprintf("V( %.4f, %.4f, %.4f )", X, Y, Z);
    char* buf = new char[len + 1];
    sprintf_s(buf, len + 1, "V( %.4f, %.4f, %.4f )", X, Y, Z);
    std::string ret = buf;
    delete[] buf;
    return ret;
  };
  std::string ToStr(char* str) {
    int len = _scprintf("V( %.4f, %.4f, %.4f )", X, Y, Z);
    sprintf_s(str, len + 1, "V( %.4f, %.4f, %.4f )", X, Y, Z);
    return std::string(str);
  };
};

class vec4 {
public:
  vec3	V;
  float	W;

  vec4() : W(1.0f) {};
  vec4(const float v) : V(v), W(v) {};
  vec4(const float* V1) : V(V1), W(V1[3]) {};
  vec4(const vec4& v) : V(v.V), W(v.W) {};
  vec4(const vec2& V1) : V(V1), W(1.0f) {};
  vec4(const vec2& V1, const vec2& V2) : V(V1, V2.X), W(V2.Y) {};
  vec4(const vec3& V1) : V(V1), W(1.0f) {};
  vec4(const vec3& V1, const float w) : V(V1), W(w) {};
  vec4(const float x, const float y, const float z) : V(x, y, z), W(1.0f) {};
  vec4(const float x, const float y, const float z, const float w) : V(x, y, z), W(w) {};

  void Set(const float x, const float y, const float z, const float w) {
    V.Set(x, y, z);
    W = w;
  };

  void operator =(const vec4& V1) {
    V = V1.V;
    W = V1.W;
  };
  void operator =(const float* V1) {
    V = V1;
    W = V1[3];
  };
  void operator =(const float V1) {
    V = V1;
    W = V1;
  };

  vec4 operator +() {
    return *this;
  };
  vec4 operator +(const vec4& V1) {
    return vec4(V + V1.V, W);
  };

  vec4 operator -() {
    return vec4(-V, W);
  };
  vec4 operator -(const vec4& V1) {
    return vec4(V - V1.V, W);
  };

  vec4 operator *(const vec4& V1) {
    return vec4(V * V1.V, W);
  };

  vec4 operator /(const vec4& V1) {
    return vec4(V / V1.V, W);
  };

  float& operator[](int index) {
    switch (index) {
    case 0: return V.X;
    case 1: return V.Y;
    case 2: return V.Z;
    case 3: return W;
    default: return V.X;
    };
  };
  void ToFloat(float* V1) {
    V.ToFloat(V1);
    V1[3] = W;
  };
  float* ToFloat() {
    return &V.X;
  };
};