#include <stdio.h>

int main()
{
    double x;
    int i;

    freopen("fNN-30-1-Weight100.txt","r",stdin);
    freopen("out.txt","w",stdout);

    printf("float coef[] = {");

    scanf("%lf%lf%lf",&x,&x,&x);

    for(i = 0; i < 30; i++)
    {
        scanf("%lf",&x);

        if(i != 0)
        {
            printf(",");
        }

        printf("%lf",x);
    }

    printf("};\n");
}
