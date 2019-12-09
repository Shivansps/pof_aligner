#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "viewers.h"


int view_pof(ubyte *pof_bytes, int total_size)
{
    unsigned int chunk_size;
    unsigned long offset=0;
    char chunk_typechar[5];
    int version,alignment=0;
    read_typechar_and_size(pof_bytes, chunk_typechar,&chunk_size);
    printf("|%s | %d|->%ld\n",chunk_typechar,chunk_size,offset);
    memcpy(&version,pof_bytes+4,4);
    pof_bytes+=8;
    offset+=8;

    while(offset+8<total_size)
    {
        read_typechar_and_size(pof_bytes, chunk_typechar,&chunk_size);
        printf("|%s | %d|->%ld ",chunk_typechar,chunk_size,offset);

        if ((chunk_size % 4) != 0)
        {
            int newsize = chunk_size + 4 - (chunk_size % 4);
            printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
            alignment+=1;
        }
        else
        {
            printf("(Aligned)\n");
        }

        if (strcmp(chunk_typechar, "HDR2") == 0 || strcmp(chunk_typechar, "OHDR") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "TXTR") == 0)
        {
            alignment+=view_txtr(pof_bytes+8);
        }
        else if (strcmp(chunk_typechar, "PINF") == 0)
        {
            alignment+=view_pinf(pof_bytes+8,chunk_size);
        }
        else if (strcmp(chunk_typechar, "PATH") == 0)
        {
            alignment+=view_path(pof_bytes+8);
        }
        else if (strcmp(chunk_typechar, "SPCL") == 0)
        {
            alignment+=view_spcl(pof_bytes+8);
        }
        else if (strcmp(chunk_typechar, "SHLD") == 0)
        {
        }
        else if (strcmp(chunk_typechar, " EYE") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "GPNT") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "MPNT") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "TGUN") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "TMIS") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "DOCK") == 0)
        {
            alignment+=view_dock(pof_bytes+8,1);
        }
        else if (strcmp(chunk_typechar, "FUEL") == 0)
        {
            alignment+=view_fuel(pof_bytes+8,version);
        }
        else if (strcmp(chunk_typechar, "OBJ2") == 0 || strcmp(chunk_typechar, "SOBJ") == 0)
        {
            alignment+=view_bsp_data(pof_bytes+8,chunk_size,version);
        }
        else if (strcmp(chunk_typechar, "ACEN") == 0)
        {
        }
        else if (strcmp(chunk_typechar, "GLOW") == 0)
        {
            alignment+=view_glow(pof_bytes+8);
        }
        else if (strcmp(chunk_typechar, "SLDC") == 0)
        {
        }
        else
        {
            /*Bad Chunk*/
        }
        printf("\n");
        offset+=chunk_size+8;
        pof_bytes+=chunk_size+8;
    }
    return alignment;
}

int view_fuel(ubyte *bytes, int version)
{
    int i, a=0;
    memcpy(&i,bytes,4);
    printf("********* FUEL NUM THRUSTERS : %d \n",i);
    if(version>=2117)
    {
        bytes+=4;
        for(int z=0;z<i;z++)
        {
            int glows=0;
            int x;
            char name[260];
            memcpy(&x,bytes,4);
            glows+=x;
            bytes+=4;
            memcpy(&x,bytes,4);
            bytes+=4;
            memcpy(name,bytes,x);
            *(name+x)='\0';
            bytes+=x;
            printf("********* FUEL PROPERTIES=> LENGTH: %d | PROPERTIES : %s | GLOWS: %d ",x,name,glows);
            if ((x % 4) != 0)
            {
                printf("(Warning: Unaligned)\n");
                a++;
            }
            else
            {
                printf("(Aligned)\n");
            }
            bytes+=28*glows;
        }
    }
    return a;
}

int view_path(ubyte *bytes)
{
    int i,a=0;
    memcpy(&i,bytes,4);
    printf("********* PATH NUM PATHS : %d \n",i);
    bytes+=4;
    for(int z=0;z<i;z++)
    {
        int verts;
        int x;
        char name[260];
        memcpy(&x,bytes,4);
        bytes+=4;
        memcpy(name,bytes,x);
        *(name+x)='\0';
        bytes+=x;
        printf("*********** PATH NAME => LENGTH: %d | NAME : %s ",x,name);
        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        memcpy(&x,bytes,4);
        bytes+=4;
        memcpy(name,bytes,x);
        *(name+x)='\0';
        bytes+=x;
        printf("*********** PATH PROPERTIES => LENGTH: %d | PROPERTIES : %s ",x,name);
        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        memcpy(&verts,bytes,4);
        bytes+=4;
        printf("*********** PATH NUM VERTS : %d \n",verts);
        for(int v=0;v<verts;v++)
        {
            float x;
            float y;
            float z;
            float rad;
            int turrets;
            memcpy(&x,bytes,4);
            memcpy(&y,bytes+4,4);
            memcpy(&z,bytes+8,4);
            bytes+=12;
            memcpy(&rad,bytes,4);
            bytes+=4;
            memcpy(&turrets,bytes,4);
            bytes+=4;
            printf("*********** PATH VERT: %d | POSITION: ( %.2f ; %.2f ; %.2f ) | RADIUS: %.2f | NUM TURRETS: %d\n",v,x,y,z,rad,turrets);
            bytes+=turrets*4;
        }
    }
    return a;
}

int view_dock(ubyte *bytes, int show)
{
    int i,a=0;
    memcpy(&i,bytes,4);
    printf("********* DOCK NUM DOCKS : %d \n",i);
    bytes+=4;

    for(int z=1;z<=i;z++)
    {
        int m;
        int n;
        char name[260];

        memcpy(&m,bytes,4);
        bytes+=4;
        memcpy(name,bytes,m);
        *(name+m)='\0';
        bytes+=m;

        memcpy(&n,bytes,4);
        bytes+=4;
        printf("********* DOCK %d PROPERTIES LENGTH: %d | PROPERTIES : %s | SPLINE PATHS: %d ",z,m,name,n);
        if ((m % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        bytes+=n*4;

        int f;
        memcpy(&f,bytes,4);
        printf("*********** DOCK NUM POINTS : %d \n",f);
        bytes+=4;

        for(int v=1;v<=f;v++)
        {
            float x;
            float y;
            float z;
            memcpy(&x,bytes,4);
            memcpy(&y,bytes+4,4);
            memcpy(&z,bytes+8,4);
            bytes+=12;
            if(show!=0)
                printf("*********** DOCK POINT: %d | POSITION: ( %.2f ; %.2f ; %.2f )\n",v,x,y,z);
            memcpy(&x,bytes,4);
            memcpy(&y,bytes+4,4);
            memcpy(&z,bytes+8,4);
            bytes+=12;
            if(show!=0)
                printf("*********** DOCK POINT: %d | NORMAL: ( %.2f ; %.2f ; %.2f )\n",v,x,y,z);
        }
    }
    return a;
}

int view_glow(ubyte *bytes)
{
    int i, a=0;
    memcpy(&i,bytes,4);
    printf("********* GLOW NUM GLOWBANCKS : %d \n",i);
    bytes+=4;
    for(int z=0;z<i;z++)
    {
        int disp_time;
        int on_time;
        int off_time;
        int obj_parent;
        int LOD;
        int type;
        int num_glowpoints;
        int x;
        char name[260];

        memcpy(&disp_time,bytes,4);
        memcpy(&on_time,bytes+4,4);
        memcpy(&off_time,bytes+8,4);
        memcpy(&obj_parent,bytes+12,4);
        memcpy(&LOD,bytes+16,4);
        memcpy(&type,bytes+20,4);
        memcpy(&num_glowpoints,bytes+24,4);

        bytes+=28;

        memcpy(&x,bytes,4);
        bytes+=4;

        memcpy(name,bytes,x);
        *(name+x)='\0';
        bytes+=x;
        printf("\n");
        printf("*********** GLOW DIS_TIME: %d | ON_TIME: %d | OFF_TIME: %d | PARENT: %d | LOD: %d | TYPE: %d | NUM_GLOWPOINTS: %d\n",disp_time,off_time,on_time,obj_parent,LOD,type,num_glowpoints);
        printf("*********** GLOW PROPERTIES=> LENGTH: %d | PROPERTIES : %s ",x,name);

        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        bytes+=28*num_glowpoints;
    }
    return a;
}

int view_pinf(ubyte *bytes, unsigned int chunk_size)
{
    int a=0;
    char s[chunk_size+1];
    memcpy(&s,bytes,chunk_size);
    s[chunk_size+1]='\0';
    printf("%s \n",s);

    if ((chunk_size % 4) != 0)
    {
        a++;
    }
    return a;
}


int view_txtr(ubyte *bytes)
{
    int i,a=0;
    memcpy(&i,bytes,4);
    printf("********* TXTR TEXTURE NUMBER : %d \n",i);
    bytes+=4;

    for(int z=0;z<i;z++)
    {
        int x;
        char name[260];
        memcpy(&x,bytes,4);
        bytes+=4;
        memcpy(name,bytes,x);
        *(name+x)='\0';
        printf("********* TXTR TEXTURE LENGTH: %d | NAME : %s ",x,name);
        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        bytes+=x;
    }
    return a;
}


int view_spcl(ubyte *bytes)
{
    int i,a=0;
    memcpy(&i,bytes,4);
    printf("********** SPCL NUM SPECIAL POINTS : %d \n",i);
    bytes+=4;
    for(int z=0;z<i;z++)
    {
        printf("**********\n");
        int x;
        char name[260];
        memcpy(&x,bytes,4);
        bytes+=4;
        memcpy(name,bytes,x);
        *(name+x)='\0';
        printf("********** SPCL LENGTH: %d -> NAME : %s ",x,name);
        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        bytes+=x;
        memcpy(&x,bytes,4);
        bytes+=4;
        memcpy(name,bytes,x);
        *(name+x)='\0';
        printf("********** SPCL LENGTH %d -> PROPS: %s ",x,name);
        if ((x % 4) != 0)
        {
            printf("(Warning: Unaligned)\n");
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }
        bytes+=x;
        float f;
        memcpy(&f,bytes,4);
        printf("********** SPCL VECTOR POINT: ( %.2f;",f);
        bytes+=4;
        memcpy(&f,bytes,4);
        printf("%.2f;",f);
        bytes+=4;
        memcpy(&f,bytes,4);
        printf("%.2f )",f);
        bytes+=4;
        memcpy(&f,bytes,4);
        printf(" | RADIUS: %f\n",f);
        bytes+=4;
    }
    return a;
}

int view_bsp_data(ubyte *bytes, unsigned int size_int, int version)
{
    unsigned int chunk_type,chunk_size,offset=0, a=0;

    ubyte *bsp_data;
    bsp_data = (ubyte*)malloc(size_int);
    memcpy(bsp_data,bytes,size_int);

    int i;
    memcpy(&i,bsp_data,4);
    printf("********* OBJ SUBMODEL NUMBER : %d \n",i);
    //
    float radius;
    if(version>=2116)
    {
        /*************************OBJ2*****************************/
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("********* OBJ SUBOBJECT RADIUS : %.2f \n",radius);
        //
        bsp_data+=4;
        memcpy(&i,bsp_data,4);
        printf("********* OBJ SUBOBJECT PARENT : %d \n",i);
        //
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("********* OBJ VEC3D OFFSET : ( %.2f;",radius);
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("%.2f;",radius);
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("%.2f ) \n",radius);
        //
        /*********************************************************/
    }
    else
    {
        /************************SOBJ*****************************/
        bsp_data+=4;
        memcpy(&i,bsp_data,4);
        printf("********* OBJ SUBOBJECT PARENT : %d \n",i);
        //
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("********* OBJ VEC3D OFFSET : ( %.2f;",radius);
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("%.2f;",radius);
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("%.2f ) \n",radius);
        //
        bsp_data+=4;
        memcpy(&radius,bsp_data,4);
        printf("********* OBJ SUBOBJECT RADIUS : %.2f \n",radius);
        //
        /********************************************************/
    }
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("********* OBJ GEOMETRIC CENTER : ( %.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f ) \n",radius);
    //
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("********* OBJ BOUNDING BOX MIN : ( %.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f ) \n",radius);
    //
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("********* OBJ BOUNDING BOX MAX : ( %.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f;",radius);
    bsp_data+=4;
    memcpy(&radius,bsp_data,4);
    printf("%.2f ) \n",radius);
    //
    bsp_data+=4;
    char name[36];
    memcpy(&i,bsp_data,4);
    bsp_data+=4;
    memcpy(name,bsp_data,i);
    printf("********* OBJ SUBMODEL NAME : %s | LENGHT: %d ",name,i);
    if ((i % 4) != 0)
    {
        int newsize = i + 4 - (i % 4);
        printf("(Warning: Unaligned, string size should be %d)\n",newsize);
        a++;
    }
    else
    {
        printf("(Aligned)\n");
    }
    //
    bsp_data+=i;
    char props[260];
    memcpy(&i,bsp_data,4);
    bsp_data+=4;
    memcpy(props,bsp_data,i);
    printf("********* OBJ SUBMODEL PROPS : %s | LENGHT: %d ",props,i);
    if ((i % 4) != 0)
    {
        int newsize = i + 4 - (i % 4);
        printf("(Warning: Unaligned, string size should be %d)\n",newsize);
        a++;
    }
    else
    {
        printf("(Aligned)\n");
    }
    //
    bsp_data+=i;
    memcpy(&i,bsp_data,4);
    printf("********* OBJ MOVEMENT TYPE : %d\n",i);
    //kb_orbital_station
    bsp_data+=4;
    memcpy(&i,bsp_data,4);
    printf("********* OBJ MOVEMENT AXIS : %d\n",i);
    //
    bsp_data+=4;
    memcpy(&i,bsp_data,4);
    printf("********* OBJ RESERVED : %d\n",i);
    //
    bsp_data+=4;
    memcpy(&i,bsp_data,4);
    printf("********* OBJ BSP DATA SIZE : %d\n",i);
    //
    bsp_data+=4;
    offset=0;
    do{
        read_typeint_and_size(bsp_data, &chunk_type,&chunk_size);
        printf("*********|%d | %d|->%d ",chunk_type,chunk_size,offset);

        if ((chunk_size % 4) != 0)
        {
            int newsize = chunk_size + 4 - (chunk_size % 4);
            printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
            a++;
        }
        else
        {
            printf("(Aligned)\n");
        }

        if(chunk_type==1) //DEFPOINTS
        {
            a+=view_bsp_defpoints(bsp_data);
        }

        if(chunk_type==4) //SORTNORM
        {
            a+=view_bsp_sortnorm(bsp_data,0,0);
        }

        bsp_data+=chunk_size;
        offset+=chunk_size;
    }while(chunk_type!=0);
    printf("\n");
    return a;
}

int view_bsp_defpoints(ubyte *bytes)
{
    int n_verts,n_norms,vertex_offset,a=0;
    memcpy(&n_verts,bytes+8,4);
    memcpy(&n_norms,bytes+12,4);
    memcpy(&vertex_offset,bytes+16,4);
    printf("*************BSP DEFPOINTS: N VERTS: %d | N NORMS: %d| VERTEX OFFSET: %d ",n_verts,n_norms,vertex_offset);
    if ((vertex_offset % 4) != 0)
    {
            printf("(Warning: Vertex offset is unaligned)\n");
            a++;
    }
    else
    {
            printf("(Aligned)\n");
    }
    return a;
}

int view_bsp_sortnorm(ubyte *bytes, int count,int show)
{
    int a=0;
    int front;
    int back;
    int pre;
    int post;
    int on;
    unsigned long offset;
    ubyte *p;
    unsigned int chunk_type,chunk_size;

    memcpy(&front,bytes+36,4);
    memcpy(&back,bytes+40,4);
    memcpy(&pre,bytes+44,4);
    memcpy(&post,bytes+48,4);
    memcpy(&on,bytes+52,4);

    if(show!=0)
    {
        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* SORTNORM\n");
        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* FRONT: %d \n",front);

        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* BACK: %d \n",back);

        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* PRE: %d \n",pre);

        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* POST: %d \n",post);

        for(int z=0;z<count;z++)
        {
            printf("**");
        }
        printf("******************* ON: %d \n",on);
    }

    if(front!=0)
    {
        p=bytes+front;
        offset=0;
        do{
            read_typeint_and_size(p, &chunk_type,&chunk_size);
            if(show!=0||(chunk_size % 4) != 0)
            {
                for(int z=0;z<count;z++)
                {
                    printf("**");
                }
                printf("*******************|%d | %d|->%ld ",chunk_type,chunk_size,offset);
            }

            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);

                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }

            if(chunk_type==4) //SORTNORM
            {
                a+=view_bsp_sortnorm(p,count+1,show);
            }

            offset+=chunk_size;
            p+=chunk_size;
            offset+=chunk_size;
        }while(chunk_type!=0);
    }
    if(back!=0)
    {
        p=bytes+back;
        offset=0;
        do{
            read_typeint_and_size(p, &chunk_type,&chunk_size);
            if(show!=0||(chunk_size % 4) != 0)
            {
                for(int z=0;z<count;z++)
                {
                    printf("**");
                }
                printf("*******************|%d | %d|->%ld ",chunk_type,chunk_size,offset);
            }

            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }

            if(chunk_type==4) //SORTNORM
            {
                a+=view_bsp_sortnorm(p,count+1,show);
            }

            offset+=chunk_size;
            p+=chunk_size;
            offset+=chunk_size;
        }while(chunk_type!=0);
    }
    if(pre!=0)
    {
        p=bytes+pre;
        offset=0;
        do{
            read_typeint_and_size(p, &chunk_type,&chunk_size);
            if(show!=0||(chunk_size % 4) != 0)
            {
                for(int z=0;z<count;z++)
                {
                    printf("**");
                }
                printf("*******************|%d | %d|->%ld ",chunk_type,chunk_size,offset);
            }

            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);

                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }

            if(chunk_type==4) //SORTNORM
            {
                a+=view_bsp_sortnorm(p,count+1,show);
            }

            offset+=chunk_size;
            p+=chunk_size;
            offset+=chunk_size;
        }while(chunk_type!=0);
    }
    if(post!=0)
    {
        p=bytes+post;
        offset=0;
        do{
            read_typeint_and_size(p, &chunk_type,&chunk_size);
            if(show!=0||(chunk_size % 4) != 0)
            {            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }
                for(int z=0;z<count;z++)
                {
                    printf("**");
                }
                printf("*******************|%d | %d|->%ld ",chunk_type,chunk_size,offset);
            }

            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                if(show!=0)
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }

            if(chunk_type==4) //SORTNORM
            {
                a+=view_bsp_sortnorm(p,count+1,show);
            }

            offset+=chunk_size;
            p+=chunk_size;
            offset+=chunk_size;
        }while(chunk_type!=0);
    }
    if(on!=0)
    {
        p=bytes+on;
        offset=0;
        do{
            read_typeint_and_size(p, &chunk_type,&chunk_size);
            if(show!=0||(chunk_size % 4) != 0)
            {
                for(int z=0;z<count;z++)
                {
                    printf("**");
                }
                printf("*******************|%d | %d|->%ld ",chunk_type,chunk_size,offset);
            }

            if ((chunk_size % 4) != 0)
            {
                int newsize = chunk_size + 4 - (chunk_size % 4);
                    printf("(Warning: Unaligned, chunk size should be %d)\n",newsize);
                a++;
            }
            else
            {
                if(show!=0)
                    printf("(Aligned)\n");
            }

            if(chunk_type==4) //SORTNORM
            {
                a+=view_bsp_sortnorm(p,count+1,show);
            }

            offset+=chunk_size;
            p+=chunk_size;
            offset+=chunk_size;
        }while(chunk_type!=0);
    }
    return a;
}
