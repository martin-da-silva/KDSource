#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<math.h>

#include "kdsource.h"


void display_usage() {
	printf("Usage: kdtool resample sourcefile [options]\n\n");
	printf("Resample particles from source defined in XML file sourcefile, and save them in\n");
	printf("a MCPL file.\n\n");
	printf("Options:\n");
	printf("\t-o outfile: Name of MCPL file with new samples\n");
	printf("\t            (default: \"resampled.mcpl\").\n");
	printf("\t-n N:       Number of new samples (default: 1E5).\n");
	printf("\t-h, --help: Display usage instructions.\n");
}

int resample_parse_args(int argc, char **argv, const char ***xml_files, const char **outfilename, long int *N) {
	*outfilename = 0;
	*N = 1E5;
	int xml_count = 0;
	int xml_capacity = 4; // Initial capacity for XML files array
    *xml_files = (const char **)malloc(xml_capacity * sizeof(char *));
    if (*xml_files == NULL){
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
				*xml_files = (const char **)realloc(*xml_files, xml_capacity * sizeof(char *));
				if (*xml_files == NULL) {
					perror("Failed to reallocate memory for XML files");
					exit(1);
				}
			}
			(*xml_files)[xml_count++] = argv[i];
			continue;
		}
		printf("Error: Invalid argument: %s.\nUse -h or --help for help.\n",argv[i]);
		exit(1);
	}
	
	if(xml_count == 0) {
		printf("No XML source files. Use -h or --help for help.\n");
		exit(1);
	}

    *xml_files = (const char **)realloc(*xml_files, xml_count * sizeof(char *));
    if (*xml_files == NULL) {
        perror("Failed to reallocate memory for XML files");
        exit(1);
    }

	if(!*outfilename) *outfilename = "resampled.mcpl";
	return xml_count;
}

int main(int argc, char *argv[]) {
	const char **xml_files;
    const char *outfilename;
    long int N;
    int xml_count = resample_parse_args(argc, argv, &xml_files, &outfilename, &N);

    // Example usage of xml_files
    for (int i = 0; i < xml_count; i++) {
        printf("XML file: %s\n", xml_files[i]);
    }

    printf("Output file: %s\n", outfilename);
    printf("Number of samples: %ld\n", N);

    
    free(xml_files);

	return 0;
}
