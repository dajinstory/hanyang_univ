#include <math.h>
#define M1 259200
#define IA1 7141
#define IC1 54773
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349

void nrerror(error_text)
char error_text[];
/* Numerical Recipes standard error handler */
{
	void _exit();

	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	_exit(1);
}

float ran1(idum)
int *idum;
{
	static int ix1,ix2,ix3;
	static float r[98];
	float temp;
	static int iff=0;
	int j;
	void nrerror();

	if (*idum < 0 || iff == 0) {
		iff=1;
		ix1=(IC1-(*idum)) % M1;
		ix1=(IA1*ix1+IC1) % M1;
		ix2=ix1 % M2;
		ix1=(IA1*ix1+IC1) % M1;
		ix3=ix1 % M3;
		for (j=1;j<=97;j++) {
			ix1=(IA1*ix1+IC1) % M1;
			ix2=(IA2*ix2+IC2) % M2;
			r[j]=(ix1+ix2*RM2)*RM1;
		}
		*idum=1;
	}
	ix1=(IA1*ix1+IC1) % M1;
	ix2=(IA2*ix2+IC2) % M2;
	ix3=(IA3*ix3+IC3) % M3;
	j=1 + ((97*ix3)/M3);
	if (j > 97 || j < 1) nrerror("RAN1: This cannot happen.");
	temp=r[j];
	r[j]=(ix1+ix2*RM2)*RM1;
	return temp;
}

float gasdev(int *idum)
{
	static int iset=0;
	static float gset;
	float fac,rsq,v1,v2;

	if  (iset == 0) {
		do {
			v1=2.0*ran1(idum)-1.0;
			v2=2.0*ran1(idum)-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	} else {
		iset=0;
		return gset;
	}
}
