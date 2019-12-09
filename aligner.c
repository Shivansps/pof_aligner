#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aligner.h"
#include "misc.h"

unsigned long align_pof(ubyte *pof_bytes, unsigned long total_size, ubyte *aligned_pof)
 {
    unsigned int chunk_size;
    unsigned long copied=0;
    char chunk_typechar[5];
    int unaligned=0;
    int version;
    ubyte *init_pos, *init_aligned;
    init_pos=pof_bytes;
    init_aligned=aligned_pof;
    memcpy(&version,pof_bytes+4,4);
    printf("POF VERSION: %d | SIZE: %ld | STARTING...\n",version,total_size);
    memcpy(aligned_pof,pof_bytes,8);
    pof_bytes+=8;
    aligned_pof+=8;
    copied+=8;

    while(pof_bytes<init_pos+total_size)
    {
        read_typechar_and_size(pof_bytes, chunk_typechar,&chunk_size);
        //printf("|%s | %d|\n",chunk_typechar,chunk_size);
        unaligned=0;
        if ((chunk_size % 4) != 0)
        {
            unaligned = chunk_size + 4 - (chunk_size % 4);
        }

        if (strcmp(chunk_typechar, "FUEL") == 0 && version >= 2117)  /*************/
        {
            int num_thr,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            //Copy chunk basic data
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            //Copy num thrusters
            memcpy(&num_thr,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            //for each thruster
            for(int z=0;z<num_thr;z++)
            {
                int glows;
                //Copy glow number
                memcpy(&glows,pof_bytes,4);
                memcpy(aligned_pof,pof_bytes,4);
                new_sizes+=4;
                pof_bytes+=4;
                copied+=4;
                aligned_pof+=4;

                //Copy properties, enforce alignment
                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("FUEL DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }
                //Copy glow data
                memcpy(aligned_pof,pof_bytes,28*glows);
                new_sizes+=28*glows;
                pof_bytes+=28*glows;
                copied+=28*glows;
                aligned_pof+=28*glows;
            }
            //Update chunk size
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "GLOW") == 0)
        {
            int num_gb,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            memcpy(&num_gb,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            for(int z=0;z<num_gb;z++)
            {
                int num_glows;
                memcpy(aligned_pof,pof_bytes,28);
                new_sizes+=28;
                pof_bytes+=28;
                copied+=28;
                aligned_pof+=28;
                memcpy(&num_glows,pof_bytes-4,4);

                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("GLOW DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }

                memcpy(aligned_pof,pof_bytes,28*num_glows);
                new_sizes+=28*num_glows;
                pof_bytes+=28*num_glows;
                copied+=28*num_glows;
                aligned_pof+=28*num_glows;
            }
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "SPCL") == 0)
        {
            int num_points,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            memcpy(&num_points,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            for(int z=0;z<num_points;z++)
            {
                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("SPCL DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }


                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("SPCL DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }
                memcpy(aligned_pof,pof_bytes,16);
                new_sizes+=16;
                pof_bytes+=16;
                copied+=16;
                aligned_pof+=16;
            }
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "TXTR") == 0)
        {
            int num_textures,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            memcpy(&num_textures,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            for(int z=0;z<num_textures;z++)
            {
                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("TXRT DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }
            }
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "DOCK") == 0)  /*************/
        {
            int num_docks,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            //Copy chunk basic data
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            //Copy num docks
            memcpy(&num_docks,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            //For each dock
            for(int z=0;z<num_docks;z++)
            {
                int spline;
                //Copy Properties, enforce alignment
                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("DOCK DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }

                //Copy spline paths
                memcpy(&spline,pof_bytes,4);
                memcpy(aligned_pof,pof_bytes,4);
                new_sizes+=4;
                pof_bytes+=4;
                copied+=4;
                aligned_pof+=4;


                memcpy(aligned_pof,pof_bytes,4*spline);
                new_sizes+=4*spline;
                pof_bytes+=4*spline;
                copied+=4*spline;
                aligned_pof+=4*spline;

                //Now copy dock points and vectors
                memcpy(&spline,pof_bytes,4);
                memcpy(aligned_pof,pof_bytes,4);
                new_sizes+=4;
                pof_bytes+=4;
                copied+=4;
                aligned_pof+=4;

                memcpy(aligned_pof,pof_bytes,24*spline);
                new_sizes+=24*spline;
                pof_bytes+=24*spline;
                copied+=24*spline;
                aligned_pof+=24*spline;
            }
            //Update chunk size
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "PATH") == 0)  /*************/
        {
            int num_paths,i,new_sizes=0;
            char name[260];
            ubyte *size_offset;
            memcpy(aligned_pof,pof_bytes,12);
            pof_bytes+=8;
            size_offset=aligned_pof+4;
            aligned_pof+=12;

            memcpy(&num_paths,pof_bytes,4);
            pof_bytes+=4;
            new_sizes+=4;

            for(int z=0;z<num_paths;z++)
            {
                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("PATH DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }

                memcpy(&i,pof_bytes,4);
                pof_bytes+=4;
                memcpy(name,pof_bytes,i);
                if ((i % 4) != 0)
                {
                    printf("PATH DATA ALIGNED.\n");
                    int newsize = i + 4 - (i % 4);
                    memcpy(aligned_pof,&newsize,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    memset(aligned_pof+i,'\0',newsize-i);
                    new_sizes+=newsize+4;
                    aligned_pof+=newsize;
                    copied+=newsize+4;
                    pof_bytes+=i;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,pof_bytes,i);
                    aligned_pof+=i;
                    new_sizes+=i+4;
                    pof_bytes+=i;
                    copied+=i+4;
                }

                int verts;
                memcpy(&verts,pof_bytes,4);
                memcpy(aligned_pof,pof_bytes,4);
                new_sizes+=4;
                pof_bytes+=4;
                copied+=4;
                aligned_pof+=4;

                for(int v=0;v<verts;v++)
                {
                    memcpy(aligned_pof,pof_bytes,20);
                    new_sizes+=20;
                    pof_bytes+=20;
                    copied+=20;
                    aligned_pof+=20;
                    int turrets;
                    memcpy(&turrets,pof_bytes-4,4);
                    if(turrets!=0)
                    {
                        memcpy(aligned_pof,pof_bytes,turrets*4);
                        new_sizes+=turrets*4;
                        pof_bytes+=turrets*4;
                        copied+=turrets*4;
                        aligned_pof+=turrets*4;
                    }
                }
            }
            memcpy(size_offset,&new_sizes,4);
        }
        else if (strcmp(chunk_typechar, "SLDC") == 0)  /*************/
        {   /**PROBLEMATIC, THIS IS UNALIGNED BY DESIGN**/
            memcpy(aligned_pof,pof_bytes,chunk_size+8);

            if(unaligned)
            {
                printf("SLDC DATA ALIGNED.\n");
                memcpy(aligned_pof+4,&unaligned,4);
                aligned_pof+=unaligned+8;
                copied+=unaligned+8;
            }
            else
            {
                aligned_pof+=chunk_size+8;
                copied+=chunk_size+8;
            }
            pof_bytes+=chunk_size+8;
        }
        else if (strcmp(chunk_typechar, "OBJ2") == 0 || strcmp(chunk_typechar, "SOBJ") == 0) /*************/
        {
            int i, ini_cp=copied;
            char name[260];
            ubyte *this_chunk_size,*end, *bsp_data;
            bsp_data=(ubyte*)malloc(chunk_size+8);
            memcpy(bsp_data,pof_bytes,chunk_size+8);
            memcpy(aligned_pof,bsp_data,68);
            this_chunk_size=aligned_pof+4;
            end=bsp_data+chunk_size+8;
            aligned_pof+=68;
            bsp_data+=68;
            copied+=68;

            memcpy(&i,bsp_data,4);
            bsp_data+=4;
            memcpy(name,bsp_data,i);
            if ((i % 4) != 0)
            {
                printf("OBJ DATA ALIGNED.\n");
                int newsize = i + 4 - (i % 4);
                memcpy(aligned_pof,&newsize,4);
                aligned_pof+=4;
                memcpy(aligned_pof,bsp_data,i);
                memset(aligned_pof+i,'\0',newsize-i);
                aligned_pof+=newsize;
                bsp_data+=i;
                copied+=newsize+4;
            }
            else
            {
                memcpy(aligned_pof,&i,4);
                aligned_pof+=4;
                memcpy(aligned_pof,bsp_data,i);
                aligned_pof+=i;
                bsp_data+=i;
                copied+=i+4;
            }


            memcpy(&i,bsp_data,4);
            bsp_data+=4;
            memcpy(name,bsp_data,i);
            if ((i % 4) != 0)
            {
                printf("OBJ DATA ALIGNED.\n");
                int newsize = i + 4 - (i % 4);
                memcpy(aligned_pof,&newsize,4);
                aligned_pof+=4;
                memcpy(aligned_pof,bsp_data,i);
                memset(aligned_pof+i,'\0',newsize-i);
                aligned_pof+=newsize;
                bsp_data+=i;
                copied+=newsize+4;
            }
            else
            {
                if(i==0)
                {
                   printf("OBJ DATA ALIGNED.\n");
                   int z=4;
                   memcpy(aligned_pof,&z,4);
                   memset(aligned_pof+4,'\0',4);
                   copied+=8;
                   aligned_pof+=8;
                }
                else
                {
                    memcpy(aligned_pof,&i,4);
                    aligned_pof+=4;
                    memcpy(aligned_pof,bsp_data,i);
                    aligned_pof+=i;
                    bsp_data+=i;
                    copied+=i+4;
                }
            }
            memcpy(aligned_pof,bsp_data,16);
            bsp_data+=16;
            aligned_pof+=16;
            copied+=16;

            unsigned int bsp_chunk_type,bsp_chunk_size;
            do{
                read_typeint_and_size(bsp_data,&bsp_chunk_type,&bsp_chunk_size);
                if(bsp_chunk_type==0)
                    bsp_chunk_size=4;

                //printf("|%d | %d|\n",bsp_chunk_type,bsp_chunk_size);

                /*DEFPOINTS is the only bsp data chunk that could be unaligned*/
                if(bsp_chunk_type==1)
                {
                    if ((bsp_chunk_size % 4) != 0)
                    {
                        printf("BSP DEFPOINTS DATA ALIGNED.\n");
                        int newsize = bsp_chunk_size + 4 - (bsp_chunk_size % 4);
                        memcpy(bsp_data+4,&newsize,4);
                        memcpy(aligned_pof,bsp_data,bsp_chunk_size);
                        int vertex_offset;
                        memcpy(&vertex_offset,bsp_data+16,4);
                        memmove(aligned_pof+vertex_offset+(newsize-bsp_chunk_size),aligned_pof+vertex_offset,bsp_chunk_size-vertex_offset);
                        vertex_offset+=(newsize-bsp_chunk_size);
                        memcpy(aligned_pof+16,&vertex_offset,4);

                        bsp_data+=bsp_chunk_size;
                        aligned_pof+=newsize;
                        copied+=newsize;
                    }
                    else
                    {
                        memcpy(aligned_pof,bsp_data,bsp_chunk_size);
                        bsp_data+=bsp_chunk_size;
                        aligned_pof+=bsp_chunk_size;
                        copied+=bsp_chunk_size;
                    }
                }
                else
                {
                    memcpy(aligned_pof,bsp_data,bsp_chunk_size);
                    bsp_data+=bsp_chunk_size;
                    aligned_pof+=bsp_chunk_size;
                    copied+=bsp_chunk_size;
                }
            }while(bsp_data<end);

            ini_cp=copied-ini_cp-8;

            if ((ini_cp % 4) != 0)
            {
                int newsize = ini_cp + 4 - (ini_cp % 4);
                aligned_pof+=newsize-ini_cp;
                ini_cp=newsize;
            }
            memcpy(this_chunk_size,&ini_cp,4);
            pof_bytes+=chunk_size+8;
        }
        else if (strcmp(chunk_typechar, "PINF") == 0)
        {
            memcpy(aligned_pof,pof_bytes,chunk_size+8);

            if(unaligned)
            {
                printf("PINF DATA ALIGNED.\n");
            }
            else
            {
                unaligned=chunk_size;
            }

            unaligned+=32;
            memcpy(aligned_pof+4,&unaligned,4);
            memset(aligned_pof+8+chunk_size,'\0',unaligned-chunk_size);


            strcat(aligned_pof+8+chunk_size-2,"\nAligned with POF_Aligner v ");
            strcat(aligned_pof+28+chunk_size+6,VERSION);

            aligned_pof+=unaligned+8;
            copied+=unaligned+8;

            pof_bytes+=chunk_size+8;
        }
        else    /*************/
        {
            /*Default Basic Chunk Alignment*/
            memcpy(aligned_pof,pof_bytes,chunk_size+8);

            if(unaligned)
            {
                printf("%s CHUNK DATA ALIGNED. BASIC CHUNK ALIGNMENT WAS USED.\n",chunk_typechar);
                memcpy(aligned_pof+4,&unaligned,4);
                aligned_pof+=unaligned+8;
                copied+=unaligned+8;
            }
            else
            {
                aligned_pof+=chunk_size+8;
                copied+=chunk_size+8;
            }
            pof_bytes+=chunk_size+8;
        }
    }
    aligned_pof=init_aligned;
    printf("FINISHED. NEW SIZE IS: %ld (+%ld)\n\n",copied,copied-total_size);
    return copied;
}
