/***********************************************************************************
 ** DISCLAIMER. THIS SOFTWARE WAS WRITTEN BY EMPLOYEES OF THE U.S.
 ** GOVERNMENT AS A PART OF THEIR OFFICIAL DUTIES AND, THEREFORE, IS NOT
 ** PROTECTED BY COPYRIGHT. THE U.S. GOVERNMENT MAKES NO WARRANTY, EITHER
 ** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES
 ** OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, REGARDING THIS SOFTWARE.
 ** THE U.S. GOVERNMENT FURTHER MAKES NO WARRANTY THAT THIS SOFTWARE WILL NOT
 ** INFRINGE ANY OTHER UNITED STATES OR FOREIGN PATENT OR OTHER
 ** INTELLECTUAL PROPERTY RIGHT. IN NO EVENT SHALL THE U.S. GOVERNMENT BE
 ** LIABLE TO ANYONE FOR COMPENSATORY, PUNITIVE, EXEMPLARY, SPECIAL,
 ** COLLATERAL, INCIDENTAL, CONSEQUENTIAL, OR ANY OTHER TYPE OF DAMAGES IN
 ** CONNECTION WITH OR ARISING OUT OF COPY OR USE OF THIS SOFTWARE.
 ***********************************************************************************/


#include "Intrinsics_NEON_64block.h"


#define numrounds   42
#define numkeywords 3

#define R(U,V,k)   (V=XOR(V,ROL(U,2)), V=XOR(V,AND(ROL(U,1),ROL8(U))), V=XOR(V,k))

#define R1x4(U,V,k)  (R(U[0],V[0],k))
#define R1x8(U,V,k)  (R(U[0],V[0],k), R(U[1],V[1],k))
#define R1x12(U,V,k) (R(U[0],V[0],k), R(U[1],V[1],k), R(U[2],V[2],k))
#define R1x16(U,V,k) (R(U[0],V[0],k), R(U[1],V[1],k), R(U[2],V[2],k), R(U[3],V[3],k))

#define R2x4(X,Y,rk,r,s)  (R1x4(X,Y,rk[r][0]),  R1x4(Y,X,rk[s][0]))
#define R2x8(X,Y,rk,r,s)  (R1x8(X,Y,rk[r][0]),  R1x8(Y,X,rk[s][0]))
#define R2x12(X,Y,rk,r,s) (R1x12(X,Y,rk[r][0]), R1x12(Y,X,rk[s][0]))
#define R2x16(X,Y,rk,r,s) (R1x16(X,Y,rk[r][0]), R1x16(Y,X,rk[s][0]))

#define R2x32(X,Y,rk,r,s) (Y[2]=XOR(XOR(rk[r][2],Y[2]),XOR(AND(X[1],ROL8(X[2])),X[0])),\
                           Y[3]=XOR(XOR(rk[r][3],Y[3]),XOR(AND(X[2],ROL8(X[3])),X[1])),\
                           Y[4]=XOR(XOR(rk[r][4],Y[4]),XOR(AND(X[3],ROL8(X[4])),X[2])),\
                           Y[5]=XOR(XOR(rk[r][5],Y[5]),XOR(AND(X[4],ROL8(X[5])),X[3])),\
                           Y[6]=XOR(XOR(rk[r][6],Y[6]),XOR(AND(X[5],ROL8(X[6])),X[4])),\
                           Y[7]=XOR(XOR(rk[r][7],Y[7]),XOR(AND(X[6],ROL8(X[7])),X[5])),\
                           Y[0]=XOR(XOR(rk[r][0],Y[0]),ROL8(XOR(AND(X[7],X[0]),X[6]))),\
                           Y[1]=XOR(XOR(rk[r][1],Y[1]),XOR(AND(X[0],ROL8(X[1])),ROL8(X[7]))),\
                           X[2]=XOR(XOR(rk[s][2],X[2]),XOR(AND(Y[1],ROL8(Y[2])),Y[0])),\
                           X[3]=XOR(XOR(rk[s][3],X[3]),XOR(AND(Y[2],ROL8(Y[3])),Y[1])),\
                           X[4]=XOR(XOR(rk[s][4],X[4]),XOR(AND(Y[3],ROL8(Y[4])),Y[2])),\
                           X[5]=XOR(XOR(rk[s][5],X[5]),XOR(AND(Y[4],ROL8(Y[5])),Y[3])),\
                           X[6]=XOR(XOR(rk[s][6],X[6]),XOR(AND(Y[5],ROL8(Y[6])),Y[4])),\
                           X[7]=XOR(XOR(rk[s][7],X[7]),XOR(AND(Y[6],ROL8(Y[7])),Y[5])),\
                           X[0]=XOR(XOR(rk[s][0],X[0]),ROL8(XOR(AND(Y[7],Y[0]),Y[6]))),\
                           X[1]=XOR(XOR(rk[s][1],X[1]),XOR(AND(Y[0],ROL8(Y[1])),ROL8(Y[7]))))


#define f(x) ((LCS(x,1) & LCS(x,8)) ^ LCS(x,2))
#define R2(x,y,k1,k2) (y^=f(x), y^=k1, x^=f(y), x^=k2)

#define R2x1(x,y,k,r,s) (R2(x[0],y[0],k[r],k[s]))
#define R2x2(x,y,k,r,s) (R2(x[0],y[0],k[r],k[s]), R2(x[1],y[1],k[r],k[s]))

#define Enc(X,Y,rk,n) (R2x##n(X,Y,rk,0,1),   R2x##n(X,Y,rk,2,3),   R2x##n(X,Y,rk,4,5),   R2x##n(X,Y,rk,6,7),   R2x##n(X,Y,rk,8,9),\
		       R2x##n(X,Y,rk,10,11), R2x##n(X,Y,rk,12,13), R2x##n(X,Y,rk,14,15), R2x##n(X,Y,rk,16,17), R2x##n(X,Y,rk,18,19),\
		       R2x##n(X,Y,rk,20,21), R2x##n(X,Y,rk,22,23), R2x##n(X,Y,rk,24,25), R2x##n(X,Y,rk,26,27), R2x##n(X,Y,rk,28,29),\
		       R2x##n(X,Y,rk,30,31), R2x##n(X,Y,rk,32,33), R2x##n(X,Y,rk,34,35), R2x##n(X,Y,rk,36,37), R2x##n(X,Y,rk,38,39),\
		       R2x##n(X,Y,rk,40,41))



#define _D (u128)SET(0xffffffffffffffffLL,0xffffffffffffffffLL)
#define _C (u128)SET(0xffffff00ffffff00LL,0xffffff00ffffff00LL)

#define RKBS(rk,r,_V) (rk[r][7]= _D ^ rk[r-3][7] ^ ROR8(rk[r-1][2] ^ rk[r-1][3]), \
                       rk[r][6]= _D ^ rk[r-3][6] ^ ROR8(rk[r-1][1] ^ rk[r-1][2]), \
                       rk[r][5]= _D ^ rk[r-3][5] ^ ROR8(rk[r-1][0] ^ rk[r-1][1]), \
                       rk[r][4]= _D ^ rk[r-3][4] ^ rk[r-1][7] ^ ROR8(rk[r-1][0]), \
                       rk[r][3]= _D ^ rk[r-3][3] ^ rk[r-1][6] ^ rk[r-1][7], \
                       rk[r][2]= _D ^ rk[r-3][2] ^ rk[r-1][5] ^ rk[r-1][6], \
                       rk[r][1]= _C ^ rk[r-3][1] ^ rk[r-1][4] ^ rk[r-1][5], \
                       rk[r][0]= _V ^ rk[r-3][0] ^ rk[r-1][3] ^ rk[r-1][4])

#define EKBS(rk) (RKBS(rk,3,_D),  RKBS(rk,4,_C),  RKBS(rk,5,_D),  RKBS(rk,6,_C),  RKBS(rk,7,_D),  RKBS(rk,8,_D), \
                  RKBS(rk,9,_D),  RKBS(rk,10,_D), RKBS(rk,11,_C), RKBS(rk,12,_D), RKBS(rk,13,_D), RKBS(rk,14,_D), \
                  RKBS(rk,15,_C), RKBS(rk,16,_C), RKBS(rk,17,_C), RKBS(rk,18,_C), RKBS(rk,19,_C), RKBS(rk,20,_C), \
                  RKBS(rk,21,_D), RKBS(rk,22,_D), RKBS(rk,23,_C), RKBS(rk,24,_D), RKBS(rk,25,_C), RKBS(rk,26,_C), \
                  RKBS(rk,27,_D), RKBS(rk,28,_C), RKBS(rk,29,_C), RKBS(rk,30,_D), RKBS(rk,31,_D), RKBS(rk,32,_C), \
                  RKBS(rk,33,_C), RKBS(rk,34,_C), RKBS(rk,35,_D), RKBS(rk,36,_C), RKBS(rk,37,_D), RKBS(rk,38,_C), \
                  RKBS(rk,39,_C), RKBS(rk,40,_C), RKBS(rk,41,_C))


#define _c 0xfffffffc
#define _d 0xfffffffd


#define RKNBS(c0,c1,c2,A,B,C,rk,key,i) (A^=c0^(RCS(C,3)^RCS(C,4)), key[i]=A,   SET1(rk[i][0],A), \
                                        B^=c1^(RCS(A,3)^RCS(A,4)), key[i+1]=B, SET1(rk[i+1][0],B), \
                                        C^=c2^(RCS(B,3)^RCS(B,4)), key[i+2]=C, SET1(rk[i+2][0],C))


#define EKNBS(A,B,C,rk,key) (SET1(rk[0][0],A), key[0]=A, SET1(rk[1][0],B), key[1]=B, SET1(rk[2][0],C), key[2]=C, \
			     RKNBS(_d,_c,_d,A,B,C,rk,key,3),  RKNBS(_c,_d,_d,A,B,C,rk,key,6),  RKNBS(_d,_d,_c,A,B,C,rk,key,9),  RKNBS(_d,_d,_d,A,B,C,rk,key,12), \
			     RKNBS(_c,_c,_c,A,B,C,rk,key,15), RKNBS(_c,_c,_c,A,B,C,rk,key,18), RKNBS(_d,_d,_c,A,B,C,rk,key,21), RKNBS(_d,_c,_c,A,B,C,rk,key,24), \
			     RKNBS(_d,_c,_c,A,B,C,rk,key,27), RKNBS(_d,_d,_c,A,B,C,rk,key,30), RKNBS(_c,_c,_d,A,B,C,rk,key,33), RKNBS(_c,_d,_c,A,B,C,rk,key,36), \
			     RKNBS(_c,_c,_c,A,B,C,rk,key,39))


#define mask4  (u128)SET(0x0f0f0f0f0f0f0f0fLL,0x0f0f0f0f0f0f0f0fLL)
#define mask2  (u128)SET(0x3333333333333333LL,0x3333333333333333LL)
#define mask1  (u128)SET(0x5555555555555555LL,0x5555555555555555LL)

#define Transpose(M) (W[0] = AND(XOR(SR(M[0],4),M[4]),  mask4), M[4] = XOR(M[4], W[0]), W[0] = SL(W[0],4), M[0] = XOR(M[0], W[0]), \
                      W[1] = AND(XOR(SR(M[1],4),M[5]),  mask4), M[5] = XOR(M[5], W[1]), W[1] = SL(W[1],4), M[1] = XOR(M[1], W[1]), \
                      W[2] = AND(XOR(SR(M[2],4),M[6]),  mask4), M[6] = XOR(M[6], W[2]), W[2] = SL(W[2],4), M[2] = XOR(M[2], W[2]), \
                      W[3] = AND(XOR(SR(M[3],4),M[7]),  mask4), M[7] = XOR(M[7], W[3]), W[3] = SL(W[3],4), M[3] = XOR(M[3], W[3]), \
                      W[0] = AND(XOR(SR(M[0],2),M[2]),  mask2), M[2] = XOR(M[2], W[0]), W[0] = SL(W[0],2), M[0] = XOR(M[0], W[0]), \
                      W[1] = AND( XOR(SR(M[1],2),M[3]), mask2), M[3] = XOR(M[3], W[1]), W[1] = SL(W[1],2), M[1] = XOR(M[1], W[1]), \
                      W[2] = AND( XOR(SR(M[4],2),M[6]), mask2), M[6] = XOR(M[6], W[2]), W[2] = SL(W[2],2), M[4] = XOR(M[4], W[2]), \
                      W[3] = AND( XOR(SR(M[5],2),M[7]), mask2), M[7] = XOR(M[7], W[3]), W[3] = SL(W[3],2), M[5] = XOR(M[5], W[3]), \
                      W[0] = AND(XOR(SR(M[0],1),M[1]),  mask1), M[1] = XOR(M[1], W[0]), W[0] = SL(W[0],1), M[0] = XOR(M[0], W[0]), \
                      W[1] = AND( XOR(SR(M[2],1),M[3]), mask1), M[3] = XOR(M[3], W[1]), W[1] = SL(W[1],1), M[2] = XOR(M[2], W[1]), \
                      W[2] = AND( XOR(SR(M[4],1),M[5]), mask1), M[5] = XOR(M[5], W[2]), W[2] = SL(W[2],1), M[4] = XOR(M[4], W[2]), \
                      W[3] = AND( XOR(SR(M[6],1),M[7]), mask1), M[7] = XOR(M[7], W[3]), W[3] = SL(W[3],1), M[6] = XOR(M[6], W[3]))
