## AquaGWAS
### Usage for GUI
**There are five modules in AquaGWAS:** 
>1. PROJECT, 2. ASSOC, 3. MAN/QQ, 4. PCA/LD, 5. ANNOTATION
+ **PROJECT (Project setting)**   
>Select an appropriate output directory and set an easily distinguishable project name to which all AquaGWAS final output will be based.   
![PROJECT](https://note.youdao.com/favicon.ico)
+ **ASSOC (Association analysis)**   
>1. Open input file  
Click on the left side of the interface to open the required input file.  
**Phenotype** file and genotype file is necessary. The phenotype file can be multiple phenotype data file with header in the first line. Phenotype file also can be single phenotype data file with suffix "phe", contains three columns, respectively are "FID IID PHE" and without header in the first line.   
**Genotype** file should be a VCF, PED, TPED or BED file.   
**Map** file should be TFAM corresponds to TPED or MAP corresponds to PED, with no input allowed, AquaGWAS will automatically look for map file with the same filename (corresponding suffix) in the same path as the genotype file.  
**Covariate** file is an optional file，the first three columns should be "FID IID 1". Note that the intercept has to be included, meaning that the third column is recommended to be 1 always, and the covariates needs to be included from the fourth column. The order of the individual IDs should conform to the .tfam files, similar to the phenotype files.   
>2. Set tool and model  
Select the appropriate analysis tool and model (plink: liear, logistic; gemma: LMM BSLMM; emmax: EMMA), gemma and emmax can click "**>>**" to set more tool-specific parameters.  
>3. Set quality control parameters 
Three basic parameters respectively are MAF (minor allele frequency)， MIND(SNPs with low genotype calls are removed) and GENO(individual with low genotype calls are removed). "**>>**" can be clicked to set parameters of LD fitering(window size, step length and r2 threshold).
>4. Select phenotype  
When the input phenotype data file contains multiple phenotype data, you can select the phenotype to be analyzed in the lower right corner of the interface. AquaGWAS will analyze each phenotype separately.   
![ASSOC](https://note.youdao.com/favicon.ico)
+ **MAN/QQ (Manhattan plot and QQ plot)**
>After association analysis, the p-value file will automatically open, click the "plot" respectively, obtain Manhattan and Quantile-Quantile plot.   
**Notice:** Make sure R is installed before plotting. If it is not installed, R can be installed using the following commands in Ubuntu:  
> ```   
> sudo apt install r-base   
> ```  
> and commands in CentOS:
> ```  
> yum install epel-release   
> yum install R    
> ```  
>![MAN/QQ](https://note.youdao.com/favicon.ico)
+ **PCA/LD (Principal component analysis and linkage disequilibrium)** 
> Both PCA and LD need to the genotype file, you can open genotype in the left side of interface.  
> **For PCA**, you can set the number of PCs and how many threads will run by GCTA.(Need R to get a plot,  installation method refer to #4)  
> ![PCA](https://note.youdao.com/favicon.ico)   
**For LD**, you can choose whether to select "By family" or not. When you select "By family", LD will be analyzed per family according to FID, and each family will get a result respectively.   
![LD](https://note.youdao.com/favicon.ico)
+ **ANNOTATION (Structural annotation and functional annotation)**
> 1. annotaion  
The module annotation contains structural annotation and fucntional annotation. The input files for  structual anntation are avinput, reference gene and reference sequence. The avinupt is a input file for annovar, the format of avinput:  
![AVINPUT](https://note.youdao.com/favicon.ico)  
The reference gene could be a gtf file or gff file, and reference sequence should be a .fasta file. In addition, you can offer a refGene.txt as reference gene and a refMrna.fa as reference sequence.   
The input files for functional anntation are snp position and functional annotation reference database. The data of snp position file should be "snp_ID p-val CHR BP" without header. The functional annotation reference database file can be three formats: 1. The first column is "Gene_ID" and the others(can be multiple columns) are description, the suggested file suffix is "funcanno". 2. A database download from NCBI(with suffix "*ncbi.csv"). 3. A database download from ensemble. 
(We offered "*_refGene.txt", "*refGeneMrna.fa" and funcation annotation databse file of some species)  
**Notice:** 1. When no input of functional annotation, the functional annotaion won't be executed.  
![ANNOTATION](https://note.youdao.com/favicon.ico)  
> 2. step  
the "avinput" and "snp_pos" file can be generated from a ".vcf"(open in Genotype) and a p-value file of association analysis(open automatically after association analysis). The threshold is used for filtering SNP and getting significant SNP.  
![STEP](https://note.youdao.com/favicon.ico)
