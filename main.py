import torch
import json
import soundfile as sf
from train_model import predict_full_audio, prepare_and_train, chunk_size
from proccessing import process_audio

# Device configuration
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# Process the audio and get the paths to the dry and wet files
dry_file, wet_file = process_audio("13_ElecGtr03DI.wav", "dataset_dry.wav", "dataset_wet.wav")

# Train the model and get the trained model and the dry tensor for inference
model, dry_tensor = prepare_and_train(dry_file, wet_file, device=device)

# Predict the full audio using the trained model
predicted_audio = predict_full_audio(model, dry_tensor, chunk_size, device)

# Save the predicted audio to a file
output_path = 'predicted_full_audio.wav'
sf.write(output_path, predicted_audio, 44100)
print(f"The file is succesfully saved at: {output_path}")

# Save dictionary
weights = model.state_dict()
weights_dict = {k: v.cpu().numpy().tolist() for k, v in weights.items()}
with open('model_weights.json', 'w') as f:
    json.dump(weights_dict, f)