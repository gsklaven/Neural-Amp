import torch
import json
import soundfile as sf
from pathlib import Path
from train_model import predict_full_audio, prepare_and_train, chunk_size
from proccessing import process_audio

# Base directory = python_training/
base_dir = Path(__file__).parent

# Device configuration
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# Process the audio
dry_file, wet_file = process_audio(
    "13_ElecGtr03DI.wav",
    "dataset_dry.wav",
    "dataset_wet.wav"
)

# Train the model
model, dry_tensor = prepare_and_train(dry_file, wet_file, device=device)

# Predict
predicted_audio = predict_full_audio(model, dry_tensor, chunk_size, device)

# Save predicted audio inside python_training/
output_path = base_dir / "predicted_full_audio.wav"
sf.write(str(output_path), predicted_audio, 44100)
print(f"Saved at: {output_path}")

# Save weights in correct RTNeural format
weights = model.state_dict()

rtneural_weights = {
    "in_shape": [None, None, 1],
    "layers": [
        {
            "type": "lstm",
            "activation": "tanh",
            "recurrent_activation": "sigmoid",
            "shape": [None, None, 32],
            "weights": [
                weights["lstm.weight_ih_l0"].cpu().numpy().T.tolist(),
                weights["lstm.weight_hh_l0"].cpu().numpy().T.tolist(),
                (weights["lstm.bias_ih_l0"] + weights["lstm.bias_hh_l0"]).cpu().numpy().tolist()
            ]
        },
        {
            "type": "dense",
            "activation": "",
            "shape": [None, None, 1],
            "weights": [
                weights["linear.weight"].cpu().numpy().T.tolist(),
                weights["linear.bias"].cpu().numpy().tolist()
            ]
        }
    ]
}

weights_path = base_dir / "model_weights.json"
with open(str(weights_path), 'w') as f:
    json.dump(rtneural_weights, f)
print(f"Weights saved at: {weights_path}")