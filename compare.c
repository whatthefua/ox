#include <stdio.h>

int main()
{
    int n,i,a,b,s = 0;
    double x;

    FILE *fp1 = fopen("rawtest_exp.txt","r");
    FILE *fp2 = fopen("testout.txt","r");

    fscanf(fp1,"%d",&n);
    fscanf(fp2,"%d",&n);

    for(i = 0; i < n; i++)
    {
        fscanf(fp1,"%d",&a);
        fscanf(fp2,"%lf",&x);

        if(x <= (double)0.5)
        {
            b = 0;
        } else {
            b = 1;
        }

        if(a == b)
        {
            s++;
        }
    }

    printf("%d %d %lf",s,n,(double)s / (double)n * (double)100);
}
