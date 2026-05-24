## 任务 1：跑通 Nerfstudio 示例数据
1. 开始时间：timestamp: 2026-05-23_110801
2. 结束时间：timestamp: 2026-05-23_111444
3. GPU 型号：GeoForce RTX 4070ti super
4. 显存占用：1148MiB
5. 训练步数：max_num_iterations: 30000
6. Viewer 地址：http://127.0.0.1:7007/
7. 输出目录：/home/sunlight/下载/outputs/

## 任务 2：导出 3DGS 模型
1. 导出文件路径：/home/sunlight/下载/exports/stage1_poster_splat/splat.ply
2. 导出文件大小：104.9 MB
3. 是否能打开：能
4. 打开工具：SuperSplat（网页在线端）

## 任务 3：使用 SuperSplat 查看结果
1. 打开导出的 Gaussian Splat 文件
2. 截取至少 3 张不同视角截图
3. 记录是否存在漂浮点、空洞、破碎、模糊等问题
4. 说明这些问题可能来自数据、位姿、训练参数还是场景本身

## 任务 4：完成一组小型对比实验
### 方案 C：分辨率对比
实验编号 	分辨率设置 	观察内容
E3-A 	原始分辨率 	质量、耗时、显存
E3-B 	1/2 分辨率 	质量变化、速度变化
E3-C 	1/4 分辨率 	是否出现细节丢失

# ==================== E3-A：原始分辨率 ====================
rm -rf data/processed/aukerman_A
ns-process-data images \
    --data data/public/webodm/aukerman/odm_data_aukerman-master/images \
    --output-dir data/processed/aukerman_A \
    --num-downscales 2

# ==================== E3-B：1/2 分辨率 ====================
rm -rf data/processed/aukerman_B
ns-process-data images \
    --data data/public/webodm/aukerman/odm_data_aukerman-master/images \
    --output-dir data/processed/aukerman_B \
    --num-downscales 4

# ==================== E3-C：1/4 分辨率 ====================
rm -rf data/processed/aukerman_C
ns-process-data images \
    --data data/public/webodm/aukerman/odm_data_aukerman-master/images \
    --output-dir data/processed/aukerman_C \
    --num-downscales 6

E3-A（原始分辨率）：
    ns-train splatfacto \
    --data data/processed/aukerman_A \
    --output-dir outputs/aukerman_resolution \
    --experiment-name E3-A_original \
    --max_num_iterations 8000 \
    --pipeline.model.resolution_schedule 2000

E3-B（1/2 分辨率）：
    ns-train splatfacto \
    --data data/processed/aukerman_B \
    --output-dir outputs/aukerman_resolution \
    --experiment-name E3-B_half \
    --max_num_iterations 8000 \
    --pipeline.model.resolution_schedule 1500

E3-C（1/4 分辨率）：
    ns-train splatfacto \
    --data data/processed/aukerman_C \
    --output-dir outputs/aukerman_resolution \
    --experiment-name E3-C_quarter \
    --max_num_iterations 8000 \
    --pipeline.model.resolution_schedule 1000
