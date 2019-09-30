#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_IT_BIS 4000 			// rtbis 	Bisection
#define MAX_IT_LI 3000 				// rtflsp 	Linear Interpolation
#define MAX_IT_SEC 3000 			// rtsec 	Secant
#define MAX_IT_NT 2000 				// rtnewt	Newton-Raphson
#define MAX_IT_NT_BR 10000 			// rtsafe	Newton with bracketing
#define ABS(X) (X < 0 ? -X : X)

void nrerror(char error_text[]) {
    printf("[calculating error] %s\n", error_text);
    //exit(1);
}

void zbrak(void (*funcd)(float, float *, float *), float x1, float x2, int n, float xb1[], float xb2[], int *nb);
float rtbis(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float rtflsp(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float rtsec(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float rtnewt(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float rtsafe(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);
float muller(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter);

void zbrak(void (*funcd)(float, float*, float*), float x1, float x2, int n, float xb1[], float xb2[], int *nb)
{
   int nbb,i;
   float x,fp,fc,dx;

   /*
   // funcd wrapper
   float (*fx)(float x){
	   float y;
	   float dy;
	   funcd(x,&y, &dy);
	   return y;
   };
   */

   nbb=0;
   dx=(x2-x1)/n;
   
   //fp=(*fx)(x=x1);
   float _;
   funcd(x=x1,&fp,&_);

   for (i=1;i<=n;i++) {
	   
	   //fc=(*fx)(x += dx);
	   funcd(x+=dx,&fc,&_);
	   
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
float rtbis(void (*funcd)(float, float*, float*), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float dx,f,fmid,xmid,rtb;

   //f=(*func)(x1);
   //fmid=(*func)(x2);
   float _;
   funcd(x1,&f,&_);
   funcd(x2,&fmid,&_);

   if (f*fmid >= 0.0) nrerror("Root must be bracketed for bisection in rtbis");
   rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
   for (j=1;j<=MAX_IT_BIS;j++) {
      *iter = j;
	  
	  //fmid=(*func)(xmid=rtb+(dx *= 0.5));
      funcd(xmid=rtb+(dx *= 0.5), &fmid, &_);
	  
	  if (fmid <= 0.0) rtb=xmid;
      if (fabs(dx) < xacc || fmid == 0.0) {
            return rtb;
        }
   }
   nrerror("Too many bisections in rtbis");
   *iter=-2;
   return 0.0;
}

// Linear interpolation
float rtflsp(void (*funcd)(float, float*, float*), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float fl,fh,xl,xh,swap,dx,del,f,rtf;

   //fl=(*func)(x1);
   //fh=(*func)(x2);
   float _;
   funcd(x1, &fl, &_);
   funcd(x2, &fh, &_);

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
      
	  //f=(*func)(rtf);
	  funcd(rtf, &f, &_);
      
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
   *iter=-2;
   return 0.0;
}

// Secant
float rtsec(void (*funcd)(float, float*, float*), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   int j;
   float fl,f,dx,swap,xl,rts;


   //fl=(*func)(x1);
   //f=(*func)(x2);
   float _;
   funcd(x1, &fl, &_);
   funcd(x2, &f, &_);

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
      
	  //f=(*func)(rts);
	  funcd(rts, &f, &_);

      if (fabs(dx) < xacc || f == 0.0) return rts;
   }
   nrerror("Maximum number of iterations exceeded in rtsec");
   *iter=-2;
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
      if ((x1-rtn)*(rtn-x2) < 0.0){
         nrerror("Jumped out of brackets in rtnewt");
		 *iter=-1;
		 return 0.0;
	  }
      if (fabs(dx) < xacc) return rtn;
   }
   nrerror("Maximum number of iterations exceeded in rtnewt");
   *iter=-2;
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
   *iter=-2;
   return 0.0;
}

// Muller
float muller(void (*funcd)(float, float *, float *), float x1, float x2, float xacc, int* iter)
{
   void nrerror(char error_text[]);
   float a,b,c,p0,p1,p2,p3,y0,y1,y2,_;
   int j;

   p0=x1;
   p1=x2;
   p2=(x1+x2)/2;

   for (j=1;j<=MAX_IT_NT_BR;j++) {
	  *iter = j;
	  
	  funcd(p0,&y0,&_);
	  funcd(p1,&y1,&_);
	  funcd(p2,&y2,&_);
	  
	  a=((p1-p2)*floor(y0-y2)-(p0-p2)*floor(y1-y2))/((p0-p2)*(p1-p2)*(p0-p1));
	  b=((p0-p2)*(p0-p2)*floor(y1-y2)-(p1-p2)*(p1-p2)*floor(y0-y2))/((p0-p2)*(p1-p2)*(p0-p1));
	  c=y2;
	  float sgn=b/ABS(b);
	  p3=p2-(2*c)/(b+sgn*sqrt(b*b-4*a*c));

	  if(ABS(p3-p2)<xacc) return p3;

	  p0=p1;
	  p1=p2;
	  p2=p3;
   }
   nrerror("Maximum number of iterations exceeded in rtsafe");
   *iter=-2;
   return 0.0;
}

