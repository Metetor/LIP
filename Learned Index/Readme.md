
- 开源测试库:https://github.com/qyliu-hkust/learnedbench

B/B+ Tree (we can consider the B-Tree as regressionn tree) -> ML Module to describe the data distribution 

      |5 |50 |500
     /  \  \   \      ---->   Model
    |..| |..| |..| |..|
        \
        pos

the case(开山之作):提出了用神经网络或简单的机器学习模型来代替B+树给出key和pos的映射关系
FITing tree:试图解决the case中无法insert数据更新的缺陷，将数据空间分段训练，采用缓冲区插入策略




多维范围查询: