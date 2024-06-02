# short for essay
## SageDB: A Learned Database System(2019)
学习型数据库的设想，实际上啥也没实现
## A Tutorial on Learned Multi-dimensional indexes(2020)
学术会议总结，没有太多干货，只是对一维学习型索引做了简单概念上的介绍，以及对提出的索引提了个名字，多维学习型索引的内容少的可怜，完全不符合名字.
##Learned Index for Spatial Queries(ZM-index,2019)
zm-index应该是第一个学习型多维索引，key idea是通过z-order空间填充曲线将多维数据映射进一维，并在一维空间构建多阶段学习型索引MMI，论文中说它使用的是ANN，但我很怀疑使用ANN的话build time应该会很大，而且估计不适用于DBMS
- implementation
source:https://github.com/Liuguanli/ELSI/ZM

