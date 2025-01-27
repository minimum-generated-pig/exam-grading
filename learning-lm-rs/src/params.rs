use core::slice; // 引入 Rust 标准库中的 slice 模块，用于处理切片

use crate::config::LlamaConfigJson; // 引入自定义的 LlamaConfigJson 配置结构体
use crate::tensor::Tensor; // 引入自定义的 Tensor 类型
use safetensors::{SafeTensors, View}; // 引入 safetensors 库中的 SafeTensors 和 View 类型

// 定义 LLamaParams 结构体，泛型 T 代表元素的数据类型
pub struct LLamaParams<T> {

    pub embedding_table: Tensor<T>, // 嵌入层的权重张量

    pub rms_att_w: Vec<Tensor<T>>, // 自注意力层的 RMS 权重张量
    pub wq: Vec<Tensor<T>>, // 自注意力层的查询（Q）权重张量
    pub wk: Vec<Tensor<T>>, // 自注意力层的键（K）权重张量
    pub wv: Vec<Tensor<T>>, // 自注意力层的值（V）权重张量
    pub wo: Vec<Tensor<T>>, // 自注意力层的输出（O）权重张量

    pub rms_ffn_w: Vec<Tensor<T>>, // 前馈神经网络（FFN）的 RMS 权重张量
    pub w_up: Vec<Tensor<T>>, // 前馈神经网络的上升（up）权重张量
    pub w_gate: Vec<Tensor<T>>, // 前馈神经网络的门控（gate）权重张量
    pub w_down: Vec<Tensor<T>>, // 前馈神经网络的下降（down）权重张量

    pub rms_out_w: Tensor<T>, // 输出层的 RMS 权重张量
    pub lm_head: Tensor<T>, // 语言模型头的权重张量
}

// 为 LLamaParams<f32> 类型实现 from_safetensors 函数，解析 SafeTensors 对象并加载权重
impl LLamaParams<f32> {
    // 从 SafeTensors 加载权重的函数
    pub fn from_safetensors(safetensor: &SafeTensors, config: &LlamaConfigJson) -> Self {
        let layers = config.num_hidden_layers; // 从配置中获取隐藏层的数量

        // 定义一个闭包函数，用于根据张量的名称从 safetensor 中加载 Tensor<f32> 对象
        let get_tensor = |name: &str| -> Tensor<f32> {
            match safetensor.tensor(name) { // 尝试获取指定名称的张量
                Ok(data) => { 
                    let p: usize = data.shape().iter().product(); // 获取张量的元素个数
                    let new_data =
                        unsafe { slice::from_raw_parts(data.data().as_ptr() as *const f32, p) }; // 安全地从原始数据中创建一个指向 f32 类型的切片
                    Tensor::new(Vec::from(new_data), &data.shape().to_vec()) // 使用数据和形状创建一个新的 Tensor<f32> 对象
                }
                Err(_) => { 
                    eprintln!("Warning: Failed to load tensor: {}", name); // 如果加载失败，打印警告信息
                    Tensor::new(vec![0.0], &vec![1]) // 返回一个默认的 Tensor<f32>（一个只有一个元素 0 的 Tensor）
                }
            }
        };

        // 定义一个闭包函数，用于根据给定的前缀和后缀加载多个层的张量
        let get_layer_tensors = |prefix: &str, suffix: &str| -> Vec<Tensor<f32>> {
            (0..layers) // 遍历所有隐藏层
                .map(|i| get_tensor(&format!("{}.{}.{}", prefix, i, suffix))) // 获取每一层的张量
                .collect() // 将它们收集到一个 Vec 中
        };

        // 创建 LLamaParams 实例，加载所有所需的张量
        Self {
            embedding_table: get_tensor("lm_head.weight"), // 加载嵌入层的权重
            rms_att_w: get_layer_tensors("model.layers", "input_layernorm.weight"), // 加载自注意力层的 RMS 权重
            wq: get_layer_tensors("model.layers", "self_attn.q_proj.weight"), // 加载查询（Q）权重
            wk: get_layer_tensors("model.layers", "self_attn.k_proj.weight"), // 加载键（K）权重
            wv: get_layer_tensors("model.layers", "self_attn.v_proj.weight"), // 加载值（V）权重
            wo: get_layer_tensors("model.layers", "self_attn.o_proj.weight"), // 加载输出（O）权重
            rms_ffn_w: get_layer_tensors("model.layers", "post_attention_layernorm.weight"), // 加载前馈神经网络的 RMS 权重
            w_up: get_layer_tensors("model.layers", "mlp.up_proj.weight"), // 加载前馈神经网络的上升权重
            w_gate: get_layer_tensors("model.layers", "mlp.gate_proj.weight"), // 加载前馈神经网络的门控权重
            w_down: get_layer_tensors("model.layers", "mlp.down_proj.weight"), // 加载前馈神经网络的下降权重
            rms_out_w: get_tensor("model.norm.weight"), // 加载输出层的 RMS 权重
            lm_head: get_tensor("lm_head.weight"), // 加载语言模型头的权重
        }
    }
}
