use core::slice;

use crate::config::LlamaConfigJson;
use crate::tensor::Tensor;
use safetensors::{SafeTensors, View};
pub struct LLamaParams<T> {

    pub embedding_table: Tensor<T>, 

    pub rms_att_w: Vec<Tensor<T>>, 
    pub wq: Vec<Tensor<T>>, 
    pub wk: Vec<Tensor<T>>,        
    pub wv: Vec<Tensor<T>>,  
    pub wo: Vec<Tensor<T>>,       

    pub rms_ffn_w: Vec<Tensor<T>>, 
    pub w_up: Vec<Tensor<T>>,   
    pub w_gate: Vec<Tensor<T>>,    
    pub w_down: Vec<Tensor<T>>,   

    pub rms_out_w: Tensor<T>,
    pub lm_head: Tensor<T>,
}

impl LLamaParams<f32> {
    pub fn from_safetensors(safetensor: &SafeTensors, config: &LlamaConfigJson) -> Self {
        let layers = config.num_hidden_layers;

        let get_tensor = |name: &str| -> Tensor<f32> {
            match safetensor.tensor(name) {
                Ok(data) => {
                    let p: usize = data.shape().iter().product();
                    let new_data =
                        unsafe { slice::from_raw_parts(data.data().as_ptr() as *const f32, p) };
                    Tensor::new(Vec::from(new_data), &data.shape().to_vec())
                }
                Err(_) => {
                    eprintln!("Warning: Failed to load tensor: {}", name);
                    Tensor::new(vec![0.0], &vec![1]) 
                }
            }
        };

        let get_layer_tensors = |prefix: &str, suffix: &str| -> Vec<Tensor<f32>> {
            (0..layers)
                .map(|i| get_tensor(&format!("{}.{}.{}", prefix, i, suffix)))
                .collect()
        };

        Self {
            embedding_table: get_tensor("lm_head.weight"), 
            rms_att_w: get_layer_tensors("model.layers", "input_layernorm.weight"),
            wq: get_layer_tensors("model.layers", "self_attn.q_proj.weight"),
            wk: get_layer_tensors("model.layers", "self_attn.k_proj.weight"),
            wv: get_layer_tensors("model.layers", "self_attn.v_proj.weight"),
            wo: get_layer_tensors("model.layers", "self_attn.o_proj.weight"),
            rms_ffn_w: get_layer_tensors("model.layers", "post_attention_layernorm.weight"),
            w_up: get_layer_tensors("model.layers", "mlp.up_proj.weight"),
            w_gate: get_layer_tensors("model.layers", "mlp.gate_proj.weight"),
            w_down: get_layer_tensors("model.layers", "mlp.down_proj.weight"),
            rms_out_w: get_tensor("model.norm.weight"),
            lm_head: get_tensor("lm_head.weight"),
        }
    }
}
