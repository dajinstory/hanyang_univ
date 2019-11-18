# Numerical Analysis

## hw8
- Load image file and Resize using bilinearinterpolation method


### Quick Start
```
jupyter notebook
```
opencv 모듈 설치에 에러가 많이 발생하여 파이썬, 쥬피터노트북을 활용하여 작성했습니다.
Can check result with running interpolation.ipynb 

### Report


##### 코드설명
```
typedef double(*funcPtr)(double, double, int*);
typedef struct distributor{
    funcPtr func;
    char* name;
    double arg1;
    double arg2; 
} distributor;

double uniform_random(double l, double r, int* idum);
double gaussian_random(double  m, double s, int* idum);

distributor distributors[2] = {
    { uniform_random, "Uniform Distribution", -3.0, 2.0 },
    { gaussian_random, "Gaussian Distribution", -0.5, 1.5 }
}; 
```
function ptr 사용하여 uniform random, gaussian random 함수 모두 돌아가게끔 설계했습니다.   
다만 과제명세서에 나온 random function은 gauusian random뿐이기에, 해당 구조체를 사용하지는 않았습니다.

```
double uniform_random(double l, double r, int* idum){
    double p = ran1(idum);
    return l + (r-l)*p;
}
double gaussian_random(double  m, double s, int* idum){
    double n = gasdev(idum);
    return n*s + m;
}
```
gasdev.c 파일의 내용을 참고하여 작성했습니다. 0과 1 사이의 값이 나오는 ran1 함수를 사용하여 원하는 영역 내에서만 값이 나오도록 했습니다.

```
    for(int i=1;i<=n;i++){
        for(int j=i;j<=n;j++){
            matrix[i][j]=matrix[j][i]=gaussian_random(0,1.0,&idum);
        }
    }

    jacobi(matrix, n, eigenvalue, eigenvector, &nrot);
    eigsrt(eigenvalue, eigenvector, n);
```
jacobi function은 symmetric한 행렬에 대해서 유요하기 때문에, symmetric한 행렬을 만들어 적용해보았습니다.

##### Result
![result](./example_resized_1000x1000.jpeg)
![result](./masking1_resized_1000x1000.jpg)
