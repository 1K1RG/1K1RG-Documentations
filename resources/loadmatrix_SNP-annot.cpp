/*Roven Rommel Fuentes
g++ -o loadmatrix loadmatrix.cpp -lhdf5
Edited: Feb. 14, 2018
*
*REMINDER: Correctly identify the delimeters of the input data
*COMPILING: g++ -o loadmatrix_geno loadmatrix_SNP.cpp -lhdf5
*RUNNING: ./loadmatrix_geno
*/

#include <stdlib.h>
#include "hdf5.h"
#include <fstream>
#include <string.h>
#include <map>
#include <iostream>
#include <getopt.h>

// Cache size for HDF5 dataset chunks
#define CACHESIZE 153600000 //104857600 //52428800 //2147483648 //52428800//250MB - 2147483648

using namespace std;

static const char *options="m:n:r:to:";

static int CHUNK_Y=0;

static int CHUNK_X=0;

static int SAMCOUNT = 0;

static bool is_transposed = false;

static char output_file[1000];

void parseArgs(int argc, char **argv){
    extern char *optarg;
    int i;
    while((i=getopt(argc, argv, options)) != -1){
        switch(i){
            case 'm': CHUNK_Y = atoi(optarg); break;
            case 'n': CHUNK_X = atoi(optarg); break;
	    	case 'r': SAMCOUNT = atoi(optarg); break; // This should give the exact number of rows
	    	case 't': is_transposed = true; break;
  	    	case 'o': strcpy(output_file, optarg); break;
            default: break;
        }
    }
}


/* Making an IUPAC codes table */

void setAlleleStates(map<string,char> &Calls){
    Calls["AA"]='A';
    Calls["AT"]='W';
    Calls["AC"]='M';
    Calls["AG"]='R';
    Calls["TA"]='W';
    Calls["TT"]='T';
    Calls["TC"]='Y';
    Calls["TG"]='K';
    Calls["CA"]='M';
    Calls["CT"]='Y';
    Calls["CC"]='C';
    Calls["CG"]='S';
    Calls["GA"]='R';
    Calls["GT"]='K';
    Calls["GC"]='S';
    Calls["GG"]='G';
    Calls["00"]='?';
    Calls[".."]='?';
		/* Spanning deletions calls: currently not encodable in SNP dataset, so replaced by missing calls. */
	Calls["**"]='?';
	Calls["A*"]='?';
	Calls["G*"]='?';
	Calls["C*"]='?';
	Calls["T*"]='?';
  Calls["*A"]='?';
	Calls["*C"]='?';
	Calls["*G"]='?';
	Calls["*T"]='?';

}

int main(int argc, char **argv){

	//  outfile
    hid_t outfile,fapl,dset,space,cparms,dataprop,chunkspace;
    herr_t status;
    int col,divi1; //div
    string linestream1,geno;
    map<string,char> states; //allele states

    parseArgs(argc, argv);

    if(CHUNK_Y == 0 || CHUNK_X == 0 ){
                std::cerr << "Usage:\n" << *argv
                  << " -r  Number of rows in the input genotype matrix \n"					
                  << " -m chunk dimension (height)\n"
                  << " -n chunk dimernsion (width)\n"
                  << " -t transposed orientation\n"
                  << " -o output-file"
                  << std::endl;
    }

    char **data=NULL;
    ifstream fp1("Matrix.txt");
    if(!fp1.is_open()){ 
	printf("ERROR: Cannot open file.");
	return 1;
    }
 
    setAlleleStates(states);

    int idx=0,
	    i=0,
		tmp=0;  

    /*
	//remove header -this only applies to _ transposed input datasets_ where SNP information takes up the first 4 lines
    
	if(!is_transposed){
     	getline(fp1,linestream1);
    	getline(fp1,linestream1);
    	getline(fp1,linestream1);
    	getline(fp1,linestream1);
    }
	*/
   
    getline(fp1,linestream1); //use first line to compute SNP count // or sample count in general...
    
    if(is_transposed){
    	tmp = linestream1.find_first_of("\t"); //chromosome
    	tmp = linestream1.find_first_of("\t",++tmp); //start position
    	tmp = linestream1.find_first_of("\t",++tmp); //reference allele
    	tmp = linestream1.find_first_of("\t",++tmp)+1; //alternate allele
    	col = (linestream1.size()-(tmp-1))/3; //number of columns
    } else {
    	col = (linestream1.size()+1)/3; //number of columns assuming all entries take two bytes with one space between entries
	    tmp = 0;
    }
    //printf("%d %d\n",col,linestream1.size()); 
    
    divi1 = SAMCOUNT / CHUNK_Y; //number of slabs of size CHUNK_Y rows (possibly not counting the last incomplete one) // in input params $r/$n
	// The data is processed in parts of CHUNK_Y rows, called slabs in HDF5 jargon
	//  Each slab is further divided into chunks of size (CHUNK_Y * CHUNK_X)
	// These parameters are set during dataset creation

	// dim1 is the dimensions for the whole matrix - used to create the output data space ("space" variable)
	// dim2 is the dimensions for one slab
    hsize_t dim1[2] = {SAMCOUNT,col},
	        dim2[2] = {CHUNK_Y,col};
			
    hsize_t chkdim[2]={CHUNK_Y,CHUNK_X}; //{CHUNK_Y,CHUNK_X} - _chunk_ size
    hsize_t offset[2]={0,0},
	        count[2]={CHUNK_Y,col};  // how much to allocate at each step  (slab size)
    
	
			/*  H5Pcreate() creates a new property list as an instance of some property list class. 
			The new property list is initialized with default values for the specified class. 
			*/
	// Create HDF5 output file
    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_libver_bounds(fapl, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST); //use latest format
    outfile = H5Fcreate(output_file,H5F_ACC_TRUNC,H5P_DEFAULT,fapl);  // H5F_ACC_TRUNC : Overwrite existing files 
    
	space = H5Screate_simple(2,dim1,NULL); //create data space for the full dataset
    
	cparms = H5Pcreate(H5P_DATASET_CREATE); //chunk property list  - needed for creating a chunked dataset. The commands below specify these properties further
    
	status = H5Pset_chunk(cparms,2,chkdim); //set chunk dim 
	// H5Pset_chunk() sets the size of the chunks used to store a chunked layout dataset. This function is only valid for dataset creation property lists.
    status = H5Pset_deflate(cparms,8); // set compression level
	
    dataprop = H5Pcreate(H5P_DATASET_ACCESS); // Property list for dataset access
	/* Chunk cache
	 Sets the raw data chunk cache parameters
	  herr_t H5Pset_chunk_cache	(	hid_t 	dapl_id, // Dataset access property list identifier
						size_t 	rdcc_nslots,  // The number of chunk slots in the raw data chunk cache for this dataset. 
						size_t 	rdcc_nbytes,  // The total size of the raw data chunk cache for this dataset.
						double 	rdcc_w0   // The chunk preemption policy for this dataset
						)	
	   // Regarding rdcc_w0: 
	  -  If your application only reads or writes data once, this can be safely set to 1
	  -  If the value passed is H5D_CHUNK_CACHE_W0_DEFAULT, then the property will not be set on dapl_id and the parameter will come from the file access property list.
	*/
	size_t 	nslots = 9973 ; 
    status = H5Pset_chunk_cache(dataprop,
								nslots,
								CACHESIZE,
								H5D_CHUNK_CACHE_W0_DEFAULT);
	
	/*. Finally, creating the HDF5 dataset associated with the output file
	// note the definition of the function as #define H5Dcreate  H5Dcreate2
	

		hid_t H5Dcreate2 ( hid_t 	loc_id,
						const char * 	name, // Name of the dataset to create
						hid_t 	type_id,   //Datatype identifier
						hid_t 	space_id,  // Dataspace identifier
						hid_t 	lcpl_id,   // Link creation property list identifier
						hid_t 	dcpl_id,   // Dataset creation property list identifier
						hid_t 	dapl_id    // Dataset access property list identifier
						)	
	 Creates a new dataset and links it into the file.
	
	*/
    dset = H5Dcreate(outfile,
					"data",
					H5T_NATIVE_CHAR,
					space,
					H5P_DEFAULT,
					cparms,
					dataprop);
    status = H5Sclose(space);

	// Allocate memory for the slab buffer - the data[] array
	// The array holds CHUNK_Y * number_of_columns values
	// (Again, the input is processed in chunks of CHUNK_Y rows)
    data = (char**)malloc(CHUNK_Y*sizeof(char*));
    data[0] = (char*)malloc(CHUNK_Y*col*sizeof(char));
	// setting up pointers
    for(int j=1;j<CHUNK_Y;j++) data[j]=data[0]+j*col;
    
	// Create HDF5 space for the slab 
    chunkspace = H5Screate_simple(2,dim2,NULL);  // note: dim2 = {CHUNK_Y,col};
	

	/*  Next we use the function
	herr_t H5Sselect_hyperslab (	hid_t 	space_id,  // Dataspace identifier
									H5S_seloper_t 	op,  // Operation to perform on current selection
									const hsize_t 	start[], // Offset of start of hyperslab
									const hsize_t 	stride[],
									const hsize_t 	count[], // Number of blocks included in hyperslab
									const hsize_t 	block[] 
	Selects a hyperslab region to add to the current selected region.
	
	The selection operator op determines how the new selection is to be combined with the already existing selection for the dataspace. 
	H5S_SELECT_SET   Replaces the existing selection with the parameters from this call. Overlapping blocks are not supported with this operator.
	*/
    status = H5Sselect_hyperslab(chunkspace,H5S_SELECT_SET,offset,NULL,count,NULL);
	
    // Main loop
	i=0;
    do{
       
        if(is_transposed){
			// skip the first 4 elements
            tmp = linestream1.find_first_of("\t"); //chromosome
            tmp = linestream1.find_first_of("\t",++tmp); //start position
            tmp = linestream1.find_first_of("\t",++tmp); //reference allele
            tmp = linestream1.find_first_of("\t",++tmp)+1; //alternate allele
        }  
		// Read the current row of input genotype matrix (the ith row in the input, becoming idx-th row in the data[] array)
		for(int j=0;j<col;j++){ 
		    geno = linestream1.substr(tmp+j*3,2);
		    data[idx][j] = states.find(geno)->second;  
		} 
		
		if( idx+1 == CHUNK_Y){ // if the full slab (CHUNK_Y rows) is loaded
		    printf("%d %d\n",i,idx);
			//
		    space = H5Dget_space(dset);
			// H5Dget_space() makes a copy of the dataspace of the dataset specified by dset_id. 
			// The function returns an identifier for the new copy of the dataspace.
		    offset[0]=i-idx; //printf("%d\n",i-idx);
	            status = H5Sselect_hyperslab(space, H5S_SELECT_SET, offset,NULL,count,NULL);
				
			
			/* Writing to the file
			herr_t    H5Dwrite	(	hid_t 	dset_id,
							hid_t 	mem_type_id,
							hid_t 	mem_space_id,  // Identifier of the memory dataspace
							hid_t 	file_space_id,
							hid_t 	dxpl_id,       // Dataset transfer property list identifier
							const void * 	buf 
							)	
				
				*/
		    status = H5Dwrite(dset,
						H5T_NATIVE_CHAR,
						chunkspace, 
						space, 
						H5P_DEFAULT,
						&data[0][0]); 
						
		    status = H5Sclose(space);
			
		    idx = -1;  // Reset the buffer
		    
			if(i+1 == CHUNK_Y*divi1){ //printf("Change var\n"); // The next chunk will be the last,  smaller one.
				// Here we re-allocate the buffer for the smaller last piece. Not sure why 
				int newsize = SAMCOUNT % CHUNK_Y; // remainder
				free(data[0]);
				free(data);
				data = (char**)malloc(newsize*sizeof(char*));
	    		data[0] = (char*)malloc(newsize*col*sizeof(char));
	    		for(int j=1;j<newsize;j++) data[j]=data[0]+j*col;
	 	    }
		} else if(i>CHUNK_Y*divi1 && idx + 1 == SAMCOUNT % CHUNK_Y){ //add this part to enable adding of the last chunk which has smaller size
			// i.e. the condition means we reached the last line
		    //factor of GOPAL92 1, 83, 218419, 18128777
		    printf("%d  %d\n",i,idx);
		    space = H5Dget_space(dset);
	            offset[0]=i-idx; //printf("%d\n",i-idx);
		    count[0] = SAMCOUNT % CHUNK_Y;
		    dim2[0] = SAMCOUNT % CHUNK_Y;
	            status = H5Sselect_hyperslab(space, H5S_SELECT_SET, offset,NULL,count,NULL);
		    H5Sclose(chunkspace);
		    chunkspace = H5Screate_simple(2,dim2,NULL);
		    status=H5Dwrite(dset,H5T_NATIVE_CHAR,chunkspace, space, H5P_DEFAULT,&data[0][0]);
	            status = H5Sclose(space);
		    idx=-1;
		}
        i++; idx++; 
    } while(getline(fp1,linestream1)); // && getline(fp2,linestream2));
    H5Sclose(chunkspace);
    H5Dclose(dset);
    H5Fclose(outfile);
    free(data[0]);
    free(data);

}
