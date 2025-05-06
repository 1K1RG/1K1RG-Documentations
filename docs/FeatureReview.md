# 🌾 Feature Review: SNP-Seek Genotype Search

[![Status](https://img.shields.io/badge/status-active-brightgreen.svg)](#)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](#license)
[![Platform](https://img.shields.io/badge/platform-web%20%7C%20API-blueviolet.svg)](#)

---

## 🌾 Overview

The **SNP-Seek Genotype Search** is a core genomics tool supporting the **1K Rice Genome Project – Philippines**, which focuses on sequencing, analyzing, and utilizing the genomes of 1,001 traditional Philippine rice varieties. It facilitates the rapid identification of genetic markers associated with:

- Yield  
- Stress tolerance  
- Disease resistance  
- Nutritional quality  

---

## 🔬 Purpose

SNP-Seek enables researchers and breeders to **query**, **analyze**, and **visualize** SNPs and genotype data across thousands of rice accessions. Its primary applications include:

- Genomic characterization  
- Varietal comparison  
- Marker-assisted selection (MAS)  
- Haplotype mapping  
- Interactive genome browsing via **JBrowse**  

---

## 🚀 Key Features

### 🔍 Search by Genomic Position or Gene
- Input **chromosome coordinates** or **gene IDs**  
- Retrieve SNPs located within the selected region  
- View associated genotypes across all accessions  

### 🎯 Filter by Alleles
- Specify **reference** and/or **alternate alleles**  
- Track specific variants across different rice lines  

### 🧬 Genotype Matrix
- Results displayed as a matrix:  
  - **Rows**: SNPs  
  - **Columns**: Rice accessions  
- Genotype formats: `AA`, `TT`, `CC`, `GG`, or missing (`.`)  
- Interactive sorting and filtering supported  

### 📁 Export Options
- Download results in multiple formats:  
  - `VCF`  
  - Tab-delimited text  
  - `FASTA` sequences  

### 🧩 Haplotype View
- Visualize haplotypes in selected genomic regions  
- Compare variation across subpopulations or groups  

### 🧬 Allele Frequency Viewer
- View **allele frequency** across subpopulations  
- Helps identify rare or major alleles in the dataset  
- Useful for trait association and diversity studies  

### 🌐 JBrowse Integration
- Interactive genome browser for detailed visualization  
- View SNPs, gene structures, QTLs, and annotations  
- Zoom in to see SNP positions in relation to coding regions  

---

## 📌 Notes
- Integration with **JBrowse** allows users to view SNPs in their genomic context  
- SNP-Seek is especially valuable in **MAS workflows**, population genetics, and rice breeding  

---

## 📚 References

1. **Alexandrov, N. et al.** (2015). *SNP-Seek database of SNPs derived from 3000 rice genomes*. **Nucleic Acids Research**, 43(D1), D1023–D1027. https://doi.org/10.1093/nar/gku1039  
2. **3K RGP (3,000 Rice Genomes Project Consortium)**. (2014). *The 3,000 rice genomes project*. **GigaScience**, 3(1), 7. https://doi.org/10.1186/2047-217X-3-7  
3. **IRRI SNP-Seek Database**. International Rice Research Institute. [https://snp-seek.irri.org](https://snp-seek.irri.org)  
4. **JBrowse Genome Browser**. Buels, R. et al. (2016). *JBrowse: a dynamic web platform for genome visualization and analysis*. **Genome Biology**, 17(1), 66. https://doi.org/10.1186/s13059-016-0924-1  
5. **IRRI Tools and Data Portals**. [https://www.irri.org/tools](https://www.irri.org/tools)

