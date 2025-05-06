[`1K1 Rice Genome Project – Philippines (Overview)`](https://github.com/1K1RG) 

# 📊 **1kRG Activity Plan and Deliverables - IRRI**

## Table of Contents
1. [🎯 Objectives](#-objectives)
2. [📅 Year 1 Targets](#-year-1-targets)
   - [📅 Quarter 1](#-quarter-1)
     - [📋 Activities](#-activities)
     - [📅 Sub-Activities](#-sub-activities)
     - [📜 Deliverables](#-deliverables)
     - [🎯 Tangible Results](#-tangible-results-q1)

---

### 🎯 **OBJECTIVES**

1. **🔧 Implement SNP-Seek Database Components** of PH Rice GDB Portal at UPLB (including server configuration)  
2. **🌾 Populate Databases** with information relevant to the Philippine TRVs from the public domain  
3. **🧬 Implement Bioinformatic Tools** that will allow use of the new reference genomes and the re-sequencing data from the Philippine TRVs  
4. **📚 Training on Genome Assembly and Annotation**  
5. **🖥️ Training on Database Development and Curation**  
6. **🔍 Training on the Use of the PH Rice GDB Database** and other associated tools  

---

### 📅 **Year 1 Targets**

- **💻 Software Development and Server Setup** on Y1 Q1–Q3  
- **🚀 Pilot SNP-Seek Component** of PH Rice GDB **alpha version** released by Y1 Q4  
- **📊 Collated and Curated Passport** and existing **agro-morpho data** for TRVs by Y1 Q3–Q4  
- **🎓 First Training** conducted by Y1 Q3  
- **📚 Continuous Training** of project staff by the IRRI team from Y1 Q1–Q3  
- **👩‍🎓 1st Intake of Students** as a special project in UPLB by **Q2**  

---

### 📅 **Quarter 1**

#### 📋 **Activities**
- **Software Development and Server Setup**

#### 📅 **Sub-Activities**

1. **Evaluation and gap analysis of current SNP-Seek database (v3)** software for viability for PH Rice GDB  
2. **Genotyping Data Preparation and Transformation**  
3. **Variety and phenotyping data preparation and transformation**

### 📜 **Deliverables**

#### **Deliverable 1**: Current SNP-Seek DB Evaluation, Gap Analysis, and Development Roadmap

**1. Review SNP-Seek v3 Features**  
- Identify key features and functionalities required by PH Rice GDB  
- Confirm feature alignment with user needs (from project team discussions)

**2. Set Up Software Development Version Control**  
- Create a project directory structure  
- Organize configurations, components, utilities, and assets  
- Identify and design initial API structure, use cases, and endpoints required by PH Rice GDB  
- Identify additional use cases not in the current design but important to PH Rice GDB end users

🎯 **Tangible Results** 
1. [Feature review document](https://1k1rg.github.io/deliverablesAndDocs/docs/FeatureReview) (includes API/Use Case Design Document)  
2. [Code Repository](https://github.com/1K1RG/1k1RG-SNPseek) (Bitbucket or GitHub) with the initial project structure  

---

#### **Deliverable 2**: Data Preparation and Transformation  
*Genotyping data for subset 3KRG accessions from PH*

1. Clean, normalize, and validate data (VCFs)  
2. Transform data into a suitable intermediate format (prior to HDF5 creation)  
3. Generate HDF5 files for efficient storage and retrieval of structured data  
4. Prepare and curate passport and agro-morphological data for 1k1 TRVs  
5. Transform variety data for application loading

🎯 **Tangible Results**  
1. [Scripts](https://github.com/1K1RG/1k1RG-Scripts) for data validation and transformation, uploaded to repository  
2. [Transformed data (genotype)](https://github.com/1K1RG/1kRG-sample) in required formats  
3. [HDF5 files](https://github.com/1K1RG/1k1RG-sample) with structured data
