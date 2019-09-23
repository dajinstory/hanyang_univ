#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_IT_BIS 40 		// rtbis 	Bisection
#define MAX_IT_LI 30 		// rtflsp 	Linear Interpolation
#define MAX_IT_SEC 30 		// rtsec 	Secant
#define MAX_IT_NT 20 		// rtnewt	Newton-Raphson
#define MAX_IT_NT_BR 100 	// rtsafe	Newton with bracketing

void nrerror(char error_text[]) {
    printf("[calculating error] %s\n", error_text);
    exit(1);
}

void zbrak(float (*fx)(float), float x1, float x2, int n, float xb1[], float xb2[], int *nb);
float rtbis(float (*func)(float), float x1, float x2, float xacc, int* iter);
float rtflsp(float (*func)(float), float x1, float x2, float xacc, int* iter);
float rtsec(float (*func)(float), float x1, float x2, float xacc, int* iter);
float rtnewt(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float rtsafe(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);

void zbrak(float (*fx)(float), float x1, float x2, int n, float xb1[], float xb2[], int *nb)
{
   int nbb,i;
   float x,fp,fc,dx;

   nbb=0;
   dx=(x2-x1)/n;
   fp=(*fx)(x=x1);
   for (i=1;i<=n;i++) {
      fc=(*fx)(x += dx);
      if (fc*fp <= 0.0) {
         xb1[++nbb]=x-dx;
         xb2[nbb]=x;
         if(*nb == nbb) return;

      }
      fp=fc;
   }
   *nb = nbb;
}

// Bisection
float rtbis(float (*func)(float), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float dx,f,fmid,xmid,rtb;

   f=(*func)(x1);
   fmid=(*func)(x2);
   if (f*fmid >= 0.0) nrerror("Root must be bracketed for bisection in rtbis");
   rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
   for (j=1;j<=MAX_IT_BIS;j++) {
      *iter = j;
	  fmid=(*func)(xmid=rtb+(dx *= 0.5));
      if (fmid <= 0.0) rtb=xmid;
      if (fabs(dx) < xacc || fmid == 0.0) {
            return rtb;
        }
   }
   nrerror("Too many bisections in rtbis");
   return 0.0;
}

// Linear interpolation
float rtflsp(float (*func)(float), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float fl,fh,xl,xh,swap,dx,del,f,rtf;

   fl=(*func)(x1);
   fh=(*func)(x2);
   if (fl*fh > 0.0) nrerror("Root must be bracketed in rtflsp");
   if (fl < 0.0) {
      xl=x1;
      xh=x2;
   } else {
      xl=x2;
      xh=x1;
      swap=fl;
      fl=fh;
      fh=swap;
   }
   dx=xh-xl;
   for (j=1;j<=MAX_IT_LI;j++) {
   	*iter = j;
      rtf=xl+dx*fl/(fl-fh);
      f=(*func)(rtf);
      if (f < 0.0) {
         del=xl-rtf;
         xl=rtf;
         fl=f;
      } else {
         del=xh-rtf;
         xh=rtf;
         fh=f;
      }
      dx=xh-xl;
      if (fabs(del) < xacc || f == 0.0) {
            return rtf;
        }
   }
   nrerror("Maximum number of iterations exceeded in rtflsp");
   return 0.0;
}

// Secant
float rtsec(float (*func)(float), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float fl,f,dx,swap,xl,rts;

   fl=(*func)(x1);
   f=(*func)(x2);
   if (fabs(fl) < fabs(f)) {
      rts=x1;
      xl=x2;
      swap=fl;
      fl=f;
      f=swap;
   } else {
      xl=x1;
      rts=x2;
   }
   for (j=1;j<=MAX_IT_SEC;j++) {
      *iter = j;
	  dx=(xl-rts)*f/(f-fl);
      xl=rts;
      fl=f;
      rts += dx;
      f=(*func)(rts);
      if (fabs(dx) < xacc || f == 0.0) return rts;
   }
   nrerror("Maximum number of iterations exceeded in rtsec");
   return 0.0;
}

// Newton-Raphson
float rtnewt(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float df,dx,f,rtn;

   rtn=0.5*(x1+x2);
   for (j=1;j<=MAX_IT_NT;j++) {
      *iter = j;
	  (*funcd)(rtn,&f,&df);
      dx=f/df;
      rtn -= dx;
      if ((x1-rtn)*(rtn-x2) < 0.0)
         nrerror("Jumped out of brackets in rtnewt");
      if (fabs(dx) < xacc) return rtn;
   }
   nrerror("Maximum number of iterations exceeded in rtnewt");
   return 0.0;
}

// Newton with bracketing
float rtsafe(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float df,dx,dxold,f,fh,fl;
   float temp,xh,xl,rts;

   (*funcd)(x1,&fl,&df);
   (*funcd)(x2,&fh,&df);
   if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
      nrerror("Root must be bracketed in rtsafe");
   if (fl == 0.0) return x1;
   if (fh == 0.0) return x2;
   if (fl < 0.0) {
      xl=x1;
      xh=x2;
   } else {
      xh=x1;
      xl=x2;
   }
   rts=0.5*(x1+x2);
   dxold=fabs(x2-x1);
   dx=dxold;
   (*funcd)(rts,&f,&df);
   for (j=1;j<=MAX_IT_NT_BR;j++) {
      *iter = j;
	  if ((((rts-xh)*df-f)*((rts-xl)*df-f) > 0.0)
         || (fabs(2.0*f) > fabs(dxold*df))) {
         dxold=dx;
         dx=0.5*(xh-xl);
         rts=xl+dx;
         if (xl == rts) return rts;
      } else {
         dxold=dx;
         dx=f/df;
         temp=rts;
         rts -= dx;
         if (temp == rts) return rts;
      }
      if (fabs(dx) < xacc) return rts;
      (*funcd)(rts,&f,&df);
      if (f < 0.0)
         xl=rts;
      else
         xh=rts;
   }
   nrerror("Maximum number of iterations exceeded in rtsafe");
   return 0.0;
}

