#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "vp.h"

void write_vp_header(FILE *vp,unsigned int index_offset, unsigned int num_files)
{
    char header[4]="VPVP";
    int version = 2;

    if(vp!=NULL)
    {
        fseek(vp, 0, SEEK_SET);
        fwrite(header,1,4,vp);
        fwrite(&version,4,1,vp);
        fwrite(&index_offset,4,1,vp);
        fwrite(&num_files,4,1,vp);
    }
}

void write_vp_index_entry(FILE *vp, vp_index_entry index, int gotoend)
{
    if(vp!=NULL)
    {
        if(gotoend)
            fseek(vp, 0, SEEK_END);

        fwrite(&index.offset,4,1,vp);
        fwrite(&index.filesize,4,1,vp);
        fwrite(index.name,1,32,vp);
        fwrite(&index.timestamp,4,1,vp);
    }
}

void write_vp_file(FILE *vp, ubyte *file, char *name, unsigned int filesize, unsigned int timestamp)
{
    /*This dosent work*/
    long vpsize;
    unsigned int index_offset=16,numfiles=1;

    if(vp!=NULL)
    {
        fseek(vp, 0, SEEK_END);
        vpsize = ftell(vp);
        fseek(vp, 0, SEEK_SET);

        if(vpsize==0)
        {
            /*FIRST IS ALWAYS A FOLDER*/
            write_vp_header(vp, index_offset, numfiles);
            vp_index_entry ie;
            ie.filesize=0;
            memcpy(ie.name,name,32);
            ie.offset=0;
            ie.timestamp=0;
            write_vp_index_entry(vp,ie,1);
        }
        else
        {
            /*ADD TO FILE*/
            if(filesize!=0&&timestamp!=0)
            {
                /*THIS IS A FILE*/
                unsigned int version, file_offset;
                char header[5];
                read_vp_header(vp,header,&version,&index_offset,&numfiles);
                vp_index_entry index[numfiles];
                load_vp_index(vp,index,index_offset,numfiles);

                fseek(vp, index_offset, SEEK_SET);

                fwrite(file,1,filesize,vp);

                file_offset=index_offset;

                index_offset+=filesize;

                numfiles++;

                write_vp_header(vp, index_offset, numfiles);

                fseek(vp, index_offset, SEEK_SET);

                for(int x=0;x<numfiles-1;x++)
                {
                    write_vp_index_entry(vp,index[x],0);
                }

                vp_index_entry ie;
                ie.filesize=filesize;
                memcpy(ie.name,name,32);
                ie.offset=file_offset;
                ie.timestamp=timestamp;
                write_vp_index_entry(vp,ie,0);
            }
            else
            {
                /*THIS IS A FOLDER OR BACKSLASH*/

                unsigned int version;
                char header[5];
                read_vp_header(vp,header,&version,&index_offset,&numfiles);
                numfiles+=1;
                write_vp_header(vp, index_offset, numfiles);

                vp_index_entry ie;
                ie.filesize=0;
                memcpy(ie.name,name,32);
                ie.offset=0;
                ie.timestamp=0;
                write_vp_index_entry(vp,ie,1);
            }
        }
    }

}

ubyte* load_vp_file(FILE *vp, vp_index_entry vp_index_entry)
{
    if(vp!=NULL)
    {
        ubyte *file;
        file=(ubyte*)malloc(vp_index_entry.filesize);
        fseek(vp, 0, SEEK_SET);
        fseek(vp, vp_index_entry.offset, SEEK_SET);
        fread(file,vp_index_entry.filesize,1,vp);
        return file;
    }
    return NULL;
}

void load_vp_index(FILE *vp, vp_index_entry *vp_index_entry, unsigned int index_offset, unsigned int num_files)
{
    if(vp!=NULL)
    {
        char bytes[44];
        fseek(vp, 0, SEEK_SET);
        fseek(vp, index_offset, SEEK_SET);
        for(int x=0;x<num_files;x++)
        {
            fread(bytes,44,1,vp);
            memcpy(&vp_index_entry[x].offset,bytes,4);
            memcpy(&vp_index_entry[x].filesize,bytes+4,4);
            memset(&vp_index_entry[x].name,'\0',36);
            memcpy(&vp_index_entry[x].name,bytes+8,32);
            memcpy(&vp_index_entry[x].timestamp,bytes+40,4);
        }
    }
}

int read_vp_header(FILE *vp, char *header,unsigned int *version, unsigned int *index_offset, unsigned int *num_files)
{
    if(vp!=NULL)
    {
        fseek(vp, 0, SEEK_SET);
        fread(header,1,4,vp);
        fread(version,4,1,vp);
        fread(index_offset,1,4,vp);
        fread(num_files,1,4,vp);
        header[4]='\0';
        if(strcmp("VPVP\0",header)==0)
            return 1;
        else
            return 0;
    }
    return 0;
}
