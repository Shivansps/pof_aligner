#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
/*Verbose 0=Minimal, 1=Show Input, 2=Show Output, 3=Show Both*/
void align_vp_pofs(FILE *vp_in,FILE *vp_out,int verbose, int pause);
void align_folder_pofs(char *folder_in,char *folder_out,int verbose, int pause);
void align_file_pof(char *in, char *out, int verbose);

void show_folder_pofs(char *folder_in, int pause);
void show_vp_pofs(FILE *vp, int pause);
void view_vp_files(FILE *vp);

#endif // COMMON_H_INCLUDED
