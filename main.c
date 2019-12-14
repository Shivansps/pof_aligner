#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "aligner.h"
#include "viewers.h"
#include "vp.h"
#include "common.h"
/**
    Alignment:
    -align -fol_in XXXX -fol_out XXX
    -align -fil_in XXXX -fil_out XXX
    -align -vp_in XXXX


    View:
    -view -pof XXXX
    -view -fol_in XXXX -pause
    -view -vp_in XXXX -pause

    ListVP:
    -list -vp XXXX
**/
int main(int argc, char *argv[])
{
    /**For Debug**/

    char filename[60]="D:\\FreeSpace2\\models_in\\\0";
    char filename2[60]="D:\\FreeSpace2\\models_out\\\0";
    align_folder_pofs(filename,filename2,0,0);

    /*
    char filename[60]="D:\\FreeSpace2\\models_out\\\0";
    show_folder_pofs(filename,1);
    */
/*
    char filename[60]="D:\\kif_darket.pof\0";
    char filename2[60]="D:\\kif_darket-a.pof\0";
    align_file_pof(filename,filename2,2);
*/
    /**/

    if(argc>2)
    {
        if(strcmp(argv[1],"-view")==0)
        {

            if(strcmp(argv[2],"-pof")==0)
            {
                int a=0; long size; ubyte *pof_bytes;
                FILE *file_in=fopen(argv[3],"rb");
                if(file_in!=NULL)
                {
                    fseek(file_in, 0, SEEK_END);
                    size = ftell(file_in);
                    fseek(file_in, 0, SEEK_SET);
                    pof_bytes = (ubyte*)malloc(size);
                    memset(pof_bytes,0,size);
                    fread(pof_bytes,sizeof(ubyte),size,file_in);
                    fclose(file_in);
                }

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
            else if(strcmp(argv[2],"-fol_in")==0)
            {
                if(argc==5)
                {
                    if(strcmp(argv[4],"-pause")==0)
                        show_folder_pofs(argv[3],1);
                }
                else
                {
                    show_folder_pofs(argv[3],0);
                }
            }
            else if(strcmp(argv[2],"-vp_in")==0)
            {
                FILE *vp=fopen(argv[3],"rb");
                if(vp)
                {
                    if(argc==5)
                    {
                        if(strcmp(argv[4],"-pause")==0)
                            show_vp_pofs(vp,1);
                    }
                    else
                    {
                        show_vp_pofs(vp,0);
                    }
                }
                else
                {
                    printf("Unable to open VP.");
                }
            }
        }
        else if(strcmp(argv[1],"-align")==0)
        {
            if(strcmp(argv[2],"-fol_in")==0)
            {
                if(strcmp(argv[4],"-fol_out")==0)
                {
                    align_folder_pofs(argv[3],argv[5],0,0);
                }
            }
            if(strcmp(argv[2],"-fil_in")==0)
            {
                if(strcmp(argv[4],"-fil_out")==0)
                {
                    align_file_pof(argv[3],argv[5],0);
                }
            }
            if(strcmp(argv[2],"-vp_in")==0)
            {
                FILE *vp=fopen(argv[3],"rb");
                if(vp)
                {
                    align_vp_pofs(vp,NULL,0,0);
                }
                else
                {
                    printf("Unable to open VP.");
                }
            }
        }
        else if(strcmp(argv[1],"-list")==0)
        {
            if(strcmp(argv[2],"-vp")==0)
            {
                FILE *vp=fopen(argv[3],"rb");
                if(vp)
                {
                    view_vp_files(vp);
                }
                else
                {
                    printf("Unable to open VP.");
                }
            }
        }
    }
}
