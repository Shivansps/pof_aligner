#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <dirent.h>
#include "aligner.h"
#include "viewers.h"
#include "vp.h"
#include "common.h"

void align_file_pof(char *in, char *out, int verbose)
{
    system("@cls||clear");
    ubyte *pof_bytes;
    ubyte *fixed_pof;
    unsigned long size;
    int a=0;
    FILE *file_in,*file_out;
    file_in=fopen(in,"rb");
    if(file_in!=NULL)
    {
        fseek(file_in, 0, SEEK_END);
        size = ftell(file_in);
        fseek(file_in, 0, SEEK_SET);
        pof_bytes = (ubyte*)malloc(size);
        memset(pof_bytes,0,size);
        fread(pof_bytes,sizeof(ubyte),size,file_in);
        fclose(file_in);

        if(verbose==1||verbose==3)
        {
            a = view_pof(pof_bytes,size);

            if(a)
            {
                printf("FILE is UNALIGNED\n");
            }
            else
            {
                printf("FILE is ALIGNED\n");
            }
        }

        fixed_pof = (ubyte*)malloc(size*2);
        memset(fixed_pof,'\0',size*2);
        int newsize = align_pof(pof_bytes, size, fixed_pof);
        if(verbose==2||verbose==3)
        {
            a = view_pof(fixed_pof,newsize);

            if(a)
            {
                printf("FILE is UNALIGNED\n");
            }
            else
            {
                printf("FILE is ALIGNED\n");
            }
        }
        file_out=fopen(out,"wb");
        fwrite(fixed_pof,sizeof(ubyte),newsize,file_out);
        fclose(file_out);
        free(fixed_pof);
        free(pof_bytes);
    }
    else
    {
        printf("CANT OPEN FILE.");
    }
}

void align_folder_pofs(char *folder_in,char *folder_out,int verbose, int pause)
{
    //system("@cls||clear");
    ubyte *pof_bytes;
    ubyte *fixed_pof;
    unsigned long size;
    FILE *pof_file;
    DIR *d;
    struct dirent *dir;
    d = opendir(folder_in);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".pof") != NULL)
            {
                char din[500];
                char don[500];

                strcpy(din,folder_in);
                strcpy(don,folder_out);

                strcat(din,"\\");
                strcat(din,dir->d_name);
                printf("IN: %s\n",din);

                strcat(don,"\\");
                strcat(don,dir->d_name);
                printf("OUT: %s\n",don);

                pof_file=fopen(din,"rb");
                if(pof_file)
                {
                    int a=0;
                    fseek(pof_file, 0, SEEK_END);
                    size = ftell(pof_file);
                    fseek(pof_file, 0, SEEK_SET);
                    pof_bytes = (ubyte*)malloc(size);
                    memset(pof_bytes,0,size);
                    fread(pof_bytes,sizeof(ubyte),size,pof_file);
                    fclose(pof_file);

                    if(verbose==1||verbose==3)
                    {
                        a = view_pof(pof_bytes,size);

                        if(a)
                        {
                            printf("FILE %s is UNALIGNED\n",dir->d_name);
                        }
                        else
                        {
                            printf("FILE %s is ALIGNED\n",dir->d_name);
                        }
                    }

                    fixed_pof = (ubyte*)malloc(size*2);
                    memset(fixed_pof,'\0',size*2);
                    int newsize = align_pof(pof_bytes, size, fixed_pof);
                    if(verbose==2||verbose==3)
                    {
                        a = view_pof(fixed_pof,newsize);

                        if(a)
                        {
                            printf("FILE %s is UNALIGNED\n",dir->d_name);
                        }
                        else
                        {
                            printf("FILE %s is ALIGNED\n",dir->d_name);
                        }
                    }
                    pof_file=fopen(don,"wb");
                    fwrite(fixed_pof,sizeof(ubyte),newsize,pof_file);
                    fclose(pof_file);
                    if(pause)
                    {
                        printf("Press any key to view next file...");
                        getch();
                        system("@cls||clear");
                    }
                    free(fixed_pof);
                    free(pof_bytes);
                }
                else
                {
                    printf("CANT OPEN FILE: %s\n",dir->d_name);
                }
            }
        }
        closedir(d);
    }
}

void show_folder_pofs(char *folder_in, int pause)
{
    //system("@cls||clear");
    ubyte *pof_bytes;
    unsigned long size;
    FILE *pof_file;
    DIR *d;
    struct dirent *dir;
    d = opendir(folder_in);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".pof") != NULL)
            {
                char din[500];
                strcpy(din,folder_in);
                strcat(din,"\\");
                strcat(din,dir->d_name);
                printf("%s\n",din);
                pof_file=fopen(din,"rb");
                if(pof_file)
                {
                    int a=0;
                    fseek(pof_file, 0, SEEK_END);
                    size = ftell(pof_file);
                    fseek(pof_file, 0, SEEK_SET);
                    pof_bytes = (ubyte*)malloc(size);
                    memset(pof_bytes,0,size);
                    fread(pof_bytes,sizeof(ubyte),size,pof_file);
                    fclose(pof_file);

                    a = view_pof(pof_bytes,size);

                    if(a)
                    {
                        printf("FILE %s is UNALIGNED\n",dir->d_name);
                    }
                    else
                    {
                        printf("FILE %s is ALIGNED\n",dir->d_name);
                    }

                    if(pause)
                    {
                        printf("Press any key to view next file...");
                        getch();
                        system("@cls||clear");
                    }
                    free(pof_bytes);
                }
                else
                {
                    printf("CANT OPEN FILE: %s\n",dir->d_name);
                }
            }
        }
        closedir(d);
    }
    else
    {
        printf("CANT OPEN DIRECTORY: %s\n",dir->d_name);
    }
}

void align_vp_pofs(FILE *vp_file,FILE *vp_out,int verbose, int pause)
{
    char header[5];
    unsigned int version, index_offset,numfiles,totalpofs=0, files_offset=16;


    system("@cls||clear");

    if(read_vp_header(vp_file, header,&version, &index_offset, &numfiles)==1)
    {

    }
    else
    {
        printf("Not a VP file.\n");
        return;
    }
    vp_index_entry index[numfiles];
    load_vp_index(vp_file,index,index_offset,numfiles);

    for(int x=0;x<numfiles;x++)
    {
        if(index[x].offset!=0&&index[x].filesize!=0&&index[x].timestamp!=0&&strstr(tolower(index[x].name), ".pof") != NULL)
        {
            totalpofs++;
        }
    }

    int count=1;
    unsigned int wvp_num_files=0, wvp_index_offset=16;
    vp_index_entry index_out[numfiles];

    for(int x=0;x<numfiles;x++)
    {
        if(index[x].offset!=0&&index[x].filesize!=0&&index[x].timestamp!=0&&strstr(tolower(index[x].name), ".pof") != NULL)
        {
            printf("\nFILE %d/%d | NAME: %s\n",count,totalpofs,index[x].name);
            ubyte *file;
            file=load_vp_file(vp_file, index[x]);
            int a =0;
            ubyte *fixed_pof;

            if(verbose==1||verbose==3)
            {
                a = view_pof(file,index[x].filesize);

                if(a)
                {
                    printf("FILE %s is UNALIGNED\n",index[x].name);
                }
                else
                {
                    printf("FILE %s is ALIGNED\n",index[x].name);
                }
            }
            fixed_pof = (ubyte*)malloc(index[x].filesize*2);
            memset(fixed_pof,'\0',index[x].filesize*2);
            unsigned int newsize = align_pof(file, index[x].filesize, fixed_pof);

            if(verbose==2||verbose==3)
            {
                a = view_pof(fixed_pof,newsize);

                if(a)
                {
                    printf("FILE %s is UNALIGNED\n",index[x].name);
                }
                else
                {
                    printf("FILE %s is ALIGNED\n",index[x].name);
                }
            }

            write_vp_file(vp_out,fixed_pof,index[x].name,newsize,getUnixTime(),index_out,&wvp_num_files,&wvp_index_offset);

            free(file);

            if(pause)
            {
                printf("Press any key to view next file...");
                getch();
                system("@cls||clear");
            }
            count++;
        }
        else
        {

            ubyte *file=NULL;
            if(index[x].offset!=0&&index[x].filesize!=0&&index[x].timestamp!=0)
            {
                file=load_vp_file(vp_file, index[x]);
            }
            write_vp_file(vp_out,file,index[x].name,index[x].filesize,index[x].timestamp,index_out,&wvp_num_files,&wvp_index_offset);
            free(file);

        }
    }
}

void show_vp_pofs(FILE *vp_file, int pause)
{
    char header[5];
    unsigned int version, index_offset,numfiles,totalpofs=0;

    system("@cls||clear");

    if(read_vp_header(vp_file, header,&version, &index_offset, &numfiles)==1)
    {

    }
    else
    {
        printf("Not a VP file.\n");
        return;
    }

    vp_index_entry index[numfiles];
    load_vp_index(vp_file,index,index_offset,numfiles);

    for(int x=0;x<numfiles;x++)
    {
        if(index[x].offset!=0&&index[x].filesize!=0&&index[x].timestamp!=0&&strstr(tolower(index[x].name), ".pof") != NULL)
        {
            totalpofs++;
        }
    }

    int count=1;
    for(int x=0;x<numfiles;x++)
    {
        if(index[x].offset!=0&&index[x].filesize!=0&&index[x].timestamp!=0&&strstr(tolower(index[x].name), ".pof") != NULL)
        {
            printf("\nFILE %d/%d | NAME: %s\n",count,totalpofs,index[x].name);
            ubyte *file;
            file=load_vp_file(vp_file, index[x]);
            int a = view_pof(file,index[x].filesize);
            if(a)
            {
                printf("FILE %s is UNALIGNED\n",index[x].name);
            }
            else
            {
                printf("FILE %s is ALIGNED\n",index[x].name);
            }
            free(file);
            if(pause)
            {
                printf("Press any key to view next file...");
                getch();
                system("@cls||clear");
            }
            count++;
        }
    }
}

void view_vp_files(FILE *vp)
{
    if(vp!=NULL)
    {
        char header[5];
        unsigned int version, index_offset,numfiles;
        if(read_vp_header(vp, header,&version, &index_offset, &numfiles)==1)
        {
            printf("%s|v:%d|index:%d|files:%d\n",header,version,index_offset,numfiles);
        }
        else
        {
            printf("Not a VP file.\n");
            return;
        }

        vp_index_entry index[numfiles];
        load_vp_index(vp,index,index_offset,numfiles);

        for(int z=0;z<numfiles;z++)
        {
            printf("File: %d/%d | Offset: %d | Size: %d | Name: %s | Time: %d\n",z+1,numfiles,index[z].offset,index[z].filesize,index[z].name,index[z].timestamp);
        }
    }
}

