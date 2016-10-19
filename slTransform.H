#include <iostream>
#include "slVector.H"

class SlVector4;

class SlTransform {

public:
  inline SlTransform();
  inline SlTransform(double a00, double a01, double a02, double a03,
	  double a10, double a11, double a12, double a13,
	  double a20, double a21, double a22, double a23,
	  double a30, double a31, double a32, double a33);
  inline SlTransform(const SlTransform &that);
  inline SlTransform &set(double a00, double a01, double a02, double a03,
	  double a10, double a11, double a12, double a13,
	  double a20, double a21, double a22, double a23,
	  double a30, double a31, double a32, double a33);
  inline double &operator()(unsigned int r, unsigned int c);
  inline double operator()(unsigned int r, unsigned int c) const;
  inline SlTransform &operator=(const SlTransform &that);
  inline SlTransform &operator+=(const SlTransform &that);
  inline SlTransform &operator-=(const SlTransform &that);
  double data[4][4];
};

inline SlTransform operator+(const SlTransform &a, const SlTransform &b);
inline SlTransform operator-(const SlTransform &a, const SlTransform &b);
// These are component-wise operations. 

inline SlTransform operator*(const SlTransform &a, const SlTransform &b); 
inline SlVector4   operator*(const SlTransform &a, const SlVector4   &b);
inline SlVector4   operator*(const SlVector4   &a, const SlTransform &b);

inline SlTransform transpose(const SlTransform &a);

std::istream &operator>>(std::istream &strm,      SlTransform &m);
std::ostream &operator<<(std::ostream &strm,const SlTransform &m);

inline SlTransform::SlTransform() {
  data[0][0] = 1; data[0][1] = 0; data[0][2] = 0; data[0][3] = 0;
  data[1][0] = 0; data[1][1] = 1; data[1][2] = 0; data[1][3] = 0;
  data[2][0] = 0; data[2][1] = 0; data[2][2] = 1; data[2][3] = 0;  
  data[3][0] = 0; data[3][1] = 0; data[3][2] = 0; data[3][3] = 1;  
}

inline SlTransform::SlTransform(double a00, double a01, double a02, double a03,
	double a10, double a11, double a12, double a13,
	double a20, double a21, double a22, double a23,
	double a30, double a31, double a32, double a33) {
  data[0][0] = a00; data[0][1] = a01; data[0][2] = a02; data[0][3] = a03;
  data[1][0] = a10; data[1][1] = a11; data[1][2] = a12; data[1][3] = a13;
  data[2][0] = a20; data[2][1] = a21; data[2][2] = a22; data[2][3] = a23;  
  data[3][0] = a30; data[3][1] = a31; data[3][2] = a32; data[3][3] = a33;  
}

inline SlTransform::SlTransform(const SlTransform &that) {
  data[0][0] = that.data[0][0];
  data[0][1] = that.data[0][1];
  data[0][2] = that.data[0][2];
  data[0][3] = that.data[0][3];

  data[1][0] = that.data[1][0];
  data[1][1] = that.data[1][1];
  data[1][2] = that.data[1][2];
  data[1][3] = that.data[1][3];

  data[2][0] = that.data[2][0];
  data[2][1] = that.data[2][1];
  data[2][2] = that.data[2][2];
  data[2][3] = that.data[2][3];

  data[3][0] = that.data[3][0];
  data[3][1] = that.data[3][1];
  data[3][2] = that.data[3][2];
  data[3][3] = that.data[3][3];
};


inline double &SlTransform::operator()(unsigned int r,unsigned int c) {
   return data[r][c];
}

inline double SlTransform::operator()(unsigned int r,unsigned int c) const { 
   return data[r][c];
}

inline SlTransform &SlTransform::set(double a00, double a01, double a02, double a03,
	double a10, double a11, double a12, double a13,
	double a20, double a21, double a22, double a23,
	double a30, double a31, double a32, double a33) {
  data[0][0] = a00; data[0][1] = a01; data[0][2] = a02; data[0][3] = a03;
  data[1][0] = a10; data[1][1] = a11; data[1][2] = a12; data[1][3] = a13;
  data[2][0] = a20; data[2][1] = a21; data[2][2] = a22; data[2][3] = a23;  
  data[3][0] = a30; data[3][1] = a31; data[3][2] = a32; data[3][3] = a33;  
  return *this;
}

inline SlTransform &SlTransform::operator=(const SlTransform &that) {
  data[0][0] = that.data[0][0];
  data[0][1] = that.data[0][1];
  data[0][2] = that.data[0][2];
  data[0][3] = that.data[0][3];

  data[1][0] = that.data[1][0];
  data[1][1] = that.data[1][1];
  data[1][2] = that.data[1][2];
  data[1][3] = that.data[1][3];

  data[2][0] = that.data[2][0];
  data[2][1] = that.data[2][1];
  data[2][2] = that.data[2][2];
  data[2][3] = that.data[2][3];

  data[3][0] = that.data[3][0];
  data[3][1] = that.data[3][1];
  data[3][2] = that.data[3][2];
  data[3][3] = that.data[3][3];
  return (*this);
};

inline SlTransform &SlTransform::operator+=(const SlTransform &that) {
  data[0][0] += that(0,0); data[1][0] += that(1,0); data[2][0] += that(2,0); data[3][0] += that(3,0);
  data[0][1] += that(0,1); data[1][1] += that(1,1); data[2][1] += that(2,1); data[3][1] += that(3,1);
  data[0][2] += that(0,2); data[1][2] += that(1,2); data[2][2] += that(2,2); data[3][2] += that(3,2);
  data[0][3] += that(0,3); data[1][3] += that(1,3); data[2][2] += that(2,3); data[3][3] += that(3,3);
  return (*this);
}
  
inline SlTransform &SlTransform::operator-=(const SlTransform &that) {
  data[0][0] -= that(0,0); data[1][0] -= that(1,0); data[2][0] -= that(2,0); data[3][0] -= that(3,0);
  data[0][1] -= that(0,1); data[1][1] -= that(1,1); data[2][1] -= that(2,1); data[3][1] -= that(3,1);
  data[0][2] -= that(0,2); data[1][2] -= that(1,2); data[2][2] -= that(2,2); data[3][2] -= that(3,2);
  data[0][3] -= that(0,3); data[1][3] -= that(1,3); data[2][2] -= that(2,3); data[3][3] -= that(3,3);
  return (*this);
}

inline SlTransform operator+(const SlTransform &a,const SlTransform &b) {
  return (SlTransform(a)+=b);
}
 
inline SlTransform operator-(const SlTransform &a,const SlTransform &b) {
  return (SlTransform(a)-=b);
}

inline SlTransform operator*(const SlTransform &a,const SlTransform &b) {
  SlTransform tmp(
	  a.data[0][0] * b.data[0][0] + a.data[0][1] * b.data[1][0] + a.data[0][2] * b.data[2][0] + a.data[0][3] * b.data[3][0],
	  a.data[0][0] * b.data[0][1] + a.data[0][1] * b.data[1][1] + a.data[0][2] * b.data[2][1] + a.data[0][3] * b.data[3][1],
	  a.data[0][0] * b.data[0][2] + a.data[0][1] * b.data[1][2] + a.data[0][2] * b.data[2][2] + a.data[0][3] * b.data[3][2],
	  a.data[0][0] * b.data[0][3] + a.data[0][1] * b.data[1][3] + a.data[0][2] * b.data[2][3] + a.data[0][3] * b.data[3][3],
	  a.data[1][0] * b.data[0][0] + a.data[1][1] * b.data[1][0] + a.data[1][2] * b.data[2][0] + a.data[1][3] * b.data[3][0],
	  a.data[1][0] * b.data[0][1] + a.data[1][1] * b.data[1][1] + a.data[1][2] * b.data[2][1] + a.data[1][3] * b.data[3][1],
	  a.data[1][0] * b.data[0][2] + a.data[1][1] * b.data[1][2] + a.data[1][2] * b.data[2][2] + a.data[1][3] * b.data[3][2],
	  a.data[1][0] * b.data[0][3] + a.data[1][1] * b.data[1][3] + a.data[1][2] * b.data[2][3] + a.data[1][3] * b.data[3][3],
	  a.data[2][0] * b.data[0][0] + a.data[2][1] * b.data[1][0] + a.data[2][2] * b.data[2][0] + a.data[2][3] * b.data[3][0],
	  a.data[2][0] * b.data[0][1] + a.data[2][1] * b.data[1][1] + a.data[2][2] * b.data[2][1] + a.data[2][3] * b.data[3][1],
	  a.data[2][0] * b.data[0][2] + a.data[2][1] * b.data[1][2] + a.data[2][2] * b.data[2][2] + a.data[2][3] * b.data[3][2],
	  a.data[2][0] * b.data[0][3] + a.data[2][1] * b.data[1][3] + a.data[2][2] * b.data[2][3] + a.data[2][3] * b.data[3][3],
	  a.data[3][0] * b.data[0][0] + a.data[3][1] * b.data[1][0] + a.data[3][2] * b.data[2][0] + a.data[3][3] * b.data[3][0],
	  a.data[3][0] * b.data[0][1] + a.data[3][1] * b.data[1][1] + a.data[3][2] * b.data[2][1] + a.data[3][3] * b.data[3][1],
	  a.data[3][0] * b.data[0][2] + a.data[3][1] * b.data[1][2] + a.data[3][2] * b.data[2][2] + a.data[3][3] * b.data[3][2],
	  a.data[3][0] * b.data[0][3] + a.data[3][1] * b.data[1][3] + a.data[3][2] * b.data[2][3] + a.data[3][3] * b.data[3][3]);
  return tmp;
}

inline SlVector4 operator*(const SlTransform &a,const SlVector4 &b) {
  return SlVector4(b.data[0]*a.data[0][0] + b.data[1]*a.data[0][1] + b.data[2]*a.data[0][2] + b.data[3]*a.data[0][3],
	  b.data[0]*a.data[1][0] + b.data[1]*a.data[1][1] + b.data[2]*a.data[1][2] + b.data[3]*a.data[1][3],
	  b.data[0]*a.data[2][0] + b.data[1]*a.data[2][1] + b.data[2]*a.data[2][2] + b.data[3]*a.data[2][3],
	  b.data[0]*a.data[3][0] + b.data[1]*a.data[3][1] + b.data[2]*a.data[3][2] + b.data[3]*a.data[3][3]);
}

inline SlVector4 operator*(const SlVector3 &a,const SlTransform &b) {
  return SlVector4(a[0]*b(0,0) + a[1]*b(1,0) + a[2]*b(2,0) + a[3]*b(3,0),
	  a[0]*b(0,1) + a[1]*b(1,1) + a[2]*b(2,1) + a[3]*b(3,1),
	  a[0]*b(0,2) + a[1]*b(1,2) + a[2]*b(2,2) + a[3]*b(3,2),
	  a[0]*b(0,3) + a[1]*b(1,3) + a[2]*b(2,3) + a[3]*b(3,3));
}

inline SlTransform transpose(const SlTransform &a) {
  return SlTransform(a(0,0),a(1,0),a(2,0),a(3,0),a(0,1),a(1,1),a(2,1),a(3,1),a(0,2),a(1,2),a(2,2),a(3,2),a(0,3),a(1,3),a(2,3),a(3,3));
}
  

