# LIP
Paper about Learned Index,LIP for short
## classical
- B Tree/B+ Tree
- R Tree family:
- Quad Tree/OCT Tree
- KDTree
## Overview paper
- [学习索引 现状与研究展望](./学习索引%20现状与研究展望.pdf):中文综述，可以作为了解学习型索引的入门文章

- [A Tutorial on Learned Multi-dimensional Indexes](./overview/A%20Tutorial%20on%20Learned%20Multi-dimensional%20Indexes.pdf):多维学习型索引综述姊妹篇上，介绍了2020年之前多维学习型索引发展初期的情况，可以作为初步了解文章

- [A Survey of Learned Indexes for the Multi-dimensional Space](./overview/A%20Survey%20of%20Learned%20Indexes%20for%20the%20Multi-dimensional%20Space.pdf):多维学习型索引综述姊妹篇下，详细介绍了24年之前的多维及一维学习型索引研究情况，内容很多

- [A Survey of Multi-dimensional Indexes: Past and Future Trends](./overview/A_Survey_of_Multi-Dimensional_Indexes_Past_and_Future_Trends.pdf):另一个角度的多维学习型索引情况的介绍，同时介绍了传统多维索引的发展

## single dimension
- [The Case for Learned Index Structures](./single%20dimension/The%20Case%20for%20Learned%20Index%20Structures.pdf):学习型索引的开山之作，将索引看作模型，查询操作基于预测，并提出递归查找的学习型索引框架RMI

- [FITing-Tree: A Data-aware Index Structure](./single%20dimension/FITing%20Tree.pdf)
## multi dimension
- [SageDB/SageDB: A learned database system](./multi%20dimension/CIDR2019_SageDB.pdf):

- [ZMI/Learned Index for Spatial Queries](./multi%20dimension/ZM%20index.pdf):projection based space filling curve z-order curve + single dimension model

- [MLI/The ML-Index: A Multidimensional, Learned Index for Point,Range, and Nearest-Neighbor Queries](./multi%20dimension/ML-index.pdf):projection based i-Distance + single dimension model

- [RSMI/Effectively Learning Spatial Indices](./multi%20dimension/RSMI.pdf):feedforward neural network(FNN)

- [LISA/LISA：A Learned Index Structure for Spatial Data](./multi%20dimension/LISA：A%20Learned%20Index%20Structure%20for%20Spatial%20Data.pdf):

- [Flood/Learning Multi-dimensional Indexes](./multi%20dimension/Flood.pdf)

- [Tsunami/Tsunami A Learned Multi-dimensional index](./multi%20dimension/Tsunami%20A%20Learned%20Multi-dimensional%20index.pdf)
## Disk
Description:Learned Index + Disk(SSD/HDD)

- [Updatable Learned Indexes Meet Disk-Resident DBMS](./Disk/Updatable%20Learned%20Indexes%20Meet%20Disk-Resident%20DBMS.pdf):
    - overview:
    - code:
- [Making In-Memory Learned Indexes Efficient on Disk](./Disk/Making%20In-Memory%20Learned%20Indexes%20Efficient%20on%20Disk.pdf)：
    - overview:
    - code:[JiaoyiZhang/Efficient-Disk-Learned-Index](https://github.com/JiaoyiZhang/Efficient-Disk-Learned-Index)
