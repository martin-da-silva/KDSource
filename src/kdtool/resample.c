#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<math.h>

#include "kdsource.h"


void display_usage() {
	printf("Usage: kdtool resample sourcefile1 [sourcefile2 ...] [options]\n\n");
    printf("Resample particles from sources defined in XML files, and save them\n");
	printf("in a MCPL file.\n\n");
	printf("Options:\n");
	printf("\t-o outfile: Name of MCPL file with new samples\n");
	printf("\t            (default: \"resampled.mcpl\").\n");
	printf("\t-n N:       Number of new samples (default: 1E5).\n");
	printf("\t-h, --help: Display usage instructions.\n");
}

int resample_parse_args(int argc, char **argv, const char ***xmlfilenames, const char **outfilename, long int *N) {
	*outfilename = 0;
	*N = 1E5;
	int xml_count = 0;
	int xml_capacity = 4; // Initial capacity for XML files array
    *xmlfilenames = (const char **)malloc(xml_capacity * sizeof(char *));
    if (*xmlfilenames == NULL){
        perror("Failed to allocate memory for XML files");
        exit(1);
    }

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '\0')
			continue;
		if(strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0) {
			display_usage();
			exit(0);
		}
		if(strcmp(argv[i],"-o") == 0) {
			*outfilename = argv[++i];
			continue;
		}
		if(strcmp(argv[i],"-n") == 0) {
			*N = atof(argv[++i]);
			continue;
		}
		const char *firstdot = strchr(argv[i], '.');
		if(firstdot && strcmp(firstdot, ".xml") == 0) {
			
			if (xml_count >= xml_capacity) {
				xml_capacity *= 2;
				*xmlfilenames = (const char **)realloc(*xmlfilenames, xml_capacity * sizeof(char *));
				if (*xmlfilenames == NULL) {
					perror("Failed to reallocate memory for XML files");
					exit(1);
				}
			}
			(*xmlfilenames)[xml_count++] = argv[i];
			continue;
		}
		printf("Error: Invalid argument: %s.\nUse -h or --help for help.\n",argv[i]);
		exit(1);
	}
	
	if(xml_count == 0) {
		printf("No XML source files. Use -h or --help for help.\n");
		exit(1);
	}

    *xmlfilenames = (const char **)realloc(*xmlfilenames, xml_count * sizeof(char *));
    if (*xmlfilenames == NULL) {
        perror("Failed to reallocate memory for XML files");
        exit(1);
    }

	if(!*outfilename) *outfilename = "resampled.mcpl";
	return xml_count;
}

int main(int argc, char *argv[]) {
	const char **xmlfilenames;
    const char *outfilename;
    long int N;
    int xml_count = resample_parse_args(argc, argv, &xmlfilenames, &outfilename, &N);
	
	mcpl_particle_t part;
	mcpl_outfile_t file = mcpl_create_outfile(outfilename);
	mcpl_hdr_set_srcname(file, "KDSource resample");

	if (xml_count == 1) {
		KDSource* kds = KDS_open(xmlfilenames[0]);
		double w_crit = KDS_w_mean(kds, 1000, NULL);

		printf("Resampling...\n");
		long int i;
		for(i=0; i<N; i++){
			KDS_sample2(kds, &part, 1, w_crit, NULL, 1);
			mcpl_add_particle(file, &part);
		}
	}
	else if (xml_count > 1)	{
		MultiSource* ms = MS_open(xml_count, xmlfilenames, NULL);
		double w_crit = MS_w_mean(ms, 1000, NULL);

		printf("Resampling...\n");
		for(long int i = 0; i < N; i++){
			MS_sample2(ms, &part, 1, w_crit, NULL, 1);
			mcpl_add_particle(file, &part);
		}
	}
	
	mcpl_closeandgzip_outfile(file);
	printf("Successfully sampled %ld particles.\n", N);
    free(xmlfilenames);

	return 0;
}
