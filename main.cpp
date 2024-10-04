#include <iostream>
#include <map>

/**
1.可解释聚类
an exemplars-based approach for explainable clustering: complexity and efficient approximation algorithms
通过示例进行解释的聚类算法，结合聚类质量和解释质量为1个参数便于操作，开发嵌入生成代表性实例最小化聚类直径的多项式时间近似算法
interpretable clustering: an optimization approach
基于混合整数优化(MIO)的无监督学习问题公式，开发ICOT算法实现全局最优聚类树
加权距离度量方法 -> 有效处理混合数值和分类数据 -> 防止单一变量类型主导距离计算
无需考虑调节树的复杂度计算，损失函数考虑了类内密度和类间分隔
explainable k-means and k-medians clustering
迭代错误最小化算法(IMM):使用小型决策树划分数据集 -> 每个族的分配用少量特征解释
构建带有k个阈值树来近似k-均值或k-中值聚类，通过动态规划有效寻找分割点，最小化每个节点的错误数
explaination of clustering result based on multi-objective optimization
多目标优化的超立方体叠加模型(HcubeOM) -> 通过优化叠加方案生成聚类结果(可解释性)
1个目标函数是确定超立方体的最佳数量，另一个目标函数是优化超立方体的紧凑性和重叠最小化
设计1种简洁性指标 -> 验证每个聚类生成的解释性能

 2.小波变换
wavelet convolutions for large receptive fields
waveDH(wavelet sub-bands guided convnet for efficient image dehazing)
SFFNet(a wavelet-based spatial and frequency domain fusion network for sensing segmentation)

3.多模态
github.com/baaivision/Emu3
emu.baai.ac.cn
arxiv.org/pdf/2409.17692
openai.com/index/hello-gpt-4o
arxiv.org/pdf/2409.18964
steven/sw.github.io/physgen
arxiv.org/pdf/2409.17146
molmo.allenai.org
llama.meta.com
huggingface.co/meta-llama

4.物理信息机器学习(physics-informed machine learning(piml))
https://www.osti.gov/pages/servlets/purl/2282016  science正刊
可视化电池随使用时间的退化模式解析，预测电池在使用的生命周期中寿命轨迹的温度适应性
https://arxiv.org/pdf/2406.00276
https://github.com/terencetaothucb/TBSI-Sunwoda-Battery-Dataset
使用预训练神经网络模型(GlassNet)预测玻璃稳定性(glass stability)，得到的参数来估计玻璃形成能力(glass forming ability)的可行性
https://arxiv.org/pdf/2403.10682
https://github.com/citrineinformatics-erd-public/piml_glass_forming_ability
用衡量函数近似效果的方法(kolmogorov n-widths)来分析和比较多任务piml在解决工程领域中偏微分方程问题的效果
https://arxiv.org/pdf/2402.11126v2(ok)
https://github.com/mpenwarden/knw-piml
将偏微分方程作为正则项加入到机器学习的目标函数，证明了在特定条件下比传统方法更快的收敛速度(物理信息整合到机器学习中)
https://arxiv.org/pdf/2402.07514
https://github.com/nathandoumeche/piml_as_a_kernel_method
利用流体力学的伯努利方程将泄漏检测(leakage detection)问题线性化，通过神经网络估计不规则水需求来提高泄漏检测的准确性
https://arxiv.org/pdf/2309.02935
https://github.com/swn-group-at-tu-berlin/lila-pinn
结合神经震子提高piml的泛化能力
https://arxiv.org/pdf/2308.08989
https://github.com/taniyakapoor/AAAI24_Generalization_PIML
与DeepONet比显著减少计算时间的同时具有相当或更好的准确性
https://arxiv.org/pdf/2407.15887
使用高斯函数近似狄拉克detha函数模拟移动荷载的正向和逆向问题(moving load problems)，在不同荷载大小下预测梁的绕曲精度小于1%的相对误差
https://iopscience.iop.org/article/10.1088/1742-6596/2647/15/152003/pdf
近似奇异摄动系统的慢不变流形(slow invariant manifolds(SIMs))方法以一种显式形式构建降阶模型(reduced order models(ROMs))
https://arxiv.org/pdf/2309.07946

5.强化学习
1.信息处理
1.1.提取有效表征
recore(regularized contrastive representation learning of world model)
https://arxiv.org/pdf/2312.09056
efficient policy adaptation with contrastive prompt ensemble for embodied agents
papers.nips.cc/paper_files/paper/2023/file/ad72633e034990a97e878fc2fc100afb-Paper-Conference.pdf
representation learning with contrastive predictive coding
https://arxiv.org/pdf/1807.03748
github.com/pat-coady/contrast-pred-code
learning transferable visual models from natural language supervision
https://arxiv.org/pdf/2103.00020
github.com/OpenAI/CLIP
semantic HELM(a human-readable memory for reinforcement learning)
https://arxiv.org/pdf/2306.09312
github.com/ml-jku/helm
history compression via language models in reinforcement learning
https://arxiv.org/pdf/2205.12258
github.com/ml-jku/helm
1.2.翻译(自然语言和环境(视觉)翻译为规范的任务语言)
natural language conditioned reinforcement learning with inside-out task language development and translation
https://arxiv.org/pdf/2302.09368
github.com/ppq12138/TALAR
self-supervised training of text-based reinforcement learning agent with large language models
https://arxiv.org/pdf/2406.05872
github.com/IBM/starling-agent
grounding language to non markovian tasks with no supervision of task specifications
h2r.cs.brown.edu/wp-content/uploads/patel20.pdf
learning rewards from linguistic feedback
https://arxiv.org/pdf/2009.14715
github.com/tsumers/rewards
language model programs for embodied control
https://arxiv.org/pdf/2209.07753
github.com/google-research/google-research/tree/master/code_as_policies
few-shot grounded planning for embodied agents with large language models
https://arxiv.org/pdf/2212.04088
github.com/OSU-NLP-Group/LLM-Planner
informing reinforcement learning agents by grounding natural language to markov decision processes
openreview.net/pdf?id=P4op21eju0
2.评价和奖励
2.1.隐式奖励函数设计
LLM输出任务prompt和文本-图像对齐的方式生成奖励
https://arxiv.org/pdf/2303.00001
github.com/minaek/reward_design_with_llms
read and rewards(learning to play Atari with the help of instruction manuals)
QA模块(从说明书中提取关键信息) -> 推理模块(评估+奖励)
https://arxiv.org/pdf/2302.04449
github.com/Holmeswww/RnR
grounding large language models in interactive environments with online reinforcement learning
https://arxiv.org/pdf/2302.02662
github.com/flowersteam/Grounding_LLMs_with_online_RL
vision-language models are zero-shot reward models for reinforcement learning
https://arxiv.org/pdf/2310.12921
github.com/AlignmentResearch/vlmrm
accelerating reinforcement learning of robotic manipulations via feedback from large language models
https://arxiv.org/pdf/2311.02379
guide your agent with adaptive multimodal rewards
https://arxiv.org/pdf/2309.10790
github.com/csmile-1006/ARP
language reward modulation for pretraining reinforcement learning
https://arxiv.org/pdf/2308.12270
github.com/ademiadeniji/lamp
2.2.显式奖励函数设计(通过输入的环境规范信息LLM生成执行奖励函数的代码(python)显式地计算且根据评估自主修正程序)
language to rewards for robotic skill synthesis
https://arxiv.org/pdf/2306.08647
github.com/google-deepmind/language_to_reward_2023
self-refine(iterative refinement with self feedback)
https://arxiv.org/pdf/2303.17651
github.com/madaan/self-refine
self-refined large language model as automated reward function designer for deep reinforcement learning in robotics
https://arxiv.org/pdf/2309.06687
github.com/zhehuazhou/LLM_Reward_Design
human-level reward design via coding large language models
https://arxiv.org/pdf/2310.12931
github.com/eureka-research/Eurka
text2reward(reward shaping with language models for reinforcement learning)
https://arxiv.org/pdf/2309.11489
github.com/xlang-ai/text2reward
3.推理决策
3.1.直接决策(离线强化学习的决策问题(trainsformer))
unleashing the power of pretrained language models for offline reinforcement learning
https://arxiv.org/pdf/2310.20587
github.com/srzer/LaMo-2023
can wikipedia help offline reinforcement learning
https://arxiv.org/pdf/2201.12122
github.com/machelreid/can-wikipedia-help-offline-rl
pretrained language models for interactive decision making
https://arxiv.org/pdf/2202.01771
github.com/ShuangLI59/Pre-Trained-Language-Models-for-Interactive-Decision-Making
think before you act(unified policy for interleaving language reasoning with actions)
https://arxiv.org/pdf/2304.11063
scalable in-context reinforcement learning for adaptive agent
https://arxiv.org/pdf/2310.09971
github.com/UT-Austin-RPL/amago
vision language action models transfer web knowledge to robotic control
https://arxiv.org/pdf/2307.15818
github.com/kyegomez/rt-2
3.2.间接决策(动作候选/缩小动作范围/生成参考策略/指导rl更新策略)
language models for action generation in text-based games
https://arxiv.org/pdf/2010.02903
github.com/princeton-nlp/calm-textgame
do as can, not as say(grounding language in robotic affordances)
https://arxiv.org/pdf/2204.01691
github.com/google-research/google-research/tree/master/saycan
language instructed reinforcement learning for human-ai coordination
https://arxiv.org/pdf/2304.07297
large language model is a good policy teacher for training reinforcement learning agents
https://arxiv.org/pdf/2311.13373
github.com/ZJLAB-AMMI/LLM4Teach
4.生成
4.1.多模态的世界模型(答案)
decision transformer(reinforcement learning via sequence modeling)
https://arxiv.org/pdf/2106.01345
github.com/kzl/decision-transformer
transformers are efficient world models
https://arxiv.org/pdf/2209.00588
github.com/eloialonso/iris
transformer based world models are happy with 100k interactions
https://arxiv.org/pdf/2303.07109
github.com/jrobine/twm
reinforcement learning with transformer world models
https://arxiv.org/pdf/2202.09481
github.com/changchencc/TransDreamer
reinforcement learning with action-free pretraining from videos
https://arxiv.org/pdf/2203.13880
github.com/younggyoseo/apv
4.2.对策略的解释(在调优或调用rl模型时可参考LLM(prompt)提供的策略解释)
a survey of explainable reinforcement learning
https://arxiv.org/pdf/2202.08434
concept-based explanations to benefit agent learning and user understanding
https://arxiv.org/pdf/2309.12482
understanding language in the world by predicting the future
openreview.net/pdf?id=eWLOoaShEH
 */
struct word {
    std::string type;
    std::map<std::vector<std::string>, std::deque<char16_t[]>> transfer;
    std::string desc;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
