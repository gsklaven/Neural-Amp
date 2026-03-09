import torch
import numpy as np
import torch.nn as nn
import soundfile as sf
from torch.utils.data import TensorDataset, DataLoader, random_split


# Parameters
epochs = 50
batch_size = 64
chunk_size = 4096

class GuitarAmpModel(nn.Module):
    def __init__(self):
        super().__init__() 
        
        # Create the layers of our model
        
        # 1. Add an LSTM layer that takes 1 input and outputs 32 hidden units
        self.lstm = nn.LSTM(input_size=1, hidden_size=32, batch_first=True)
        
        # 2. Add a Linear layer that takes 32 inputs and outputs 1
        self.linear = nn.Linear(in_features=32, out_features=1)

    def forward(self, x):
        # Here we will pass the data through the layers of our model
        
        x = self.lstm(x)[0]  # Pass through LSTM and take the output
        x = self.linear(x)    # Pass through Linear layer
        return x
    
# Transfer the model to GPU if available, otherwise to CPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = GuitarAmpModel().to(device)
print(model)


def train(model, dataloader, optimizer, criterion, device):
    # Set the model to training mode
    model.train()

    total_loss = 0.0

    # Iterate over training data
    for batch, (inputs, targets) in enumerate(dataloader):
        inputs = inputs.to(device)
        targets = targets.to(device)

        # Forward pass: prediction
        outputs = model(inputs) 
        
        # Calculate the loss between the predicted outputs and the actual targets
        loss = criterion(outputs, targets) 
        total_loss += loss.item()

        # Backpropagation & optimization
        optimizer.zero_grad() 
        loss.backward() 
        optimizer.step() 

        # Print progress
        if batch % 100 == 0:
            current = batch * dataloader.batch_size + len(inputs)
            print(f"loss: {loss.item():>7f}  [{current:>5d}/{len(dataloader.dataset):>5d}]")

    # Return average loss for this epoch
    return total_loss / len(dataloader)


def test(model, dataloader, criterion, device):
    model.eval()
    total_loss = 0.0
    
    all_preds = []
    all_targets = []

    with torch.no_grad():
        for inputs, targets in dataloader:
            inputs = inputs.to(device)
            targets = targets.to(device)

            outputs = model(inputs)
            loss = criterion(outputs, targets)
            total_loss += loss.item()

            all_preds.extend(outputs.detach().cpu().numpy())
            all_targets.extend(targets.detach().cpu().numpy())

    return total_loss / len(dataloader), np.array(all_preds), np.array(all_targets)


def run_training(model, train_loader, val_loader, optimizer, criterion, device, epochs):
    history = {'train_loss': [], 'val_loss': []}
    print(f"Starting training on {device} for {epochs} epochs...")

    for epoch in range(epochs):
        t_loss = train(model, train_loader, optimizer, criterion, device) 
        v_loss, _, _ = test(model, val_loader, criterion, device)

        history['train_loss'].append(t_loss)
        history['val_loss'].append(v_loss)

        print(f"Epoch {epoch+1}/{epochs} | Train Loss: {t_loss:.4f} | Val Loss: {v_loss:.4f}")

    return history


def prepare_and_train(dry_file, wet_file, device, epochs=epochs, batch_size=batch_size, chunk_size=chunk_size):
    # Read the audio files
    dry_audio, sr = sf.read(dry_file)
    wet_audio, _ = sf.read(wet_file)

    # Convert to PyTorch tensors and add channel dimension
    dry_tensor = torch.tensor(dry_audio, dtype=torch.float32).unsqueeze(1)
    wet_tensor = torch.tensor(wet_audio, dtype=torch.float32).unsqueeze(1)

    # Create chunks of the audio for training
    num_chunks = len(dry_tensor) // chunk_size
    dry_chunks = dry_tensor[:num_chunks * chunk_size].view(num_chunks, chunk_size, 1)
    wet_chunks = wet_tensor[:num_chunks * chunk_size].view(num_chunks, chunk_size, 1)

    # Dataset and DataLoader
    full_dataset = TensorDataset(dry_chunks, wet_chunks)
    train_size = int(0.8 * len(full_dataset))
    val_size = len(full_dataset) - train_size
    train_dataset, val_dataset = random_split(full_dataset, [train_size, val_size])

    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False)

    # Initialize the model, Loss, and Optimizer
    model = GuitarAmpModel().to(device)
    criterion = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

    # Run the training
    history = run_training(model, train_loader, val_loader, optimizer, criterion, device, epochs)

    # Return the trained model (and the dry_tensor for inference)
    return model, dry_tensor


def predict_full_audio(model, dry_tensor, chunk_size, device):
    model.eval()
    output_chunks = []
    
    # Split the audio into chunks
    num_chunks = len(dry_tensor) // chunk_size
    
    with torch.no_grad():
        for i in range(num_chunks):
            # Take the current chunk
            start = i * chunk_size
            end = start + chunk_size
            chunk = dry_tensor[start:end].view(1, chunk_size, 1).to(device)
            
            # Predict from the model
            prediction = model(chunk)
            
            # Store the output (move to CPU and convert to numpy)
            output_chunks.append(prediction.cpu().numpy().flatten())
            
    # Concatenate all chunks into a single array
    return np.concatenate(output_chunks)