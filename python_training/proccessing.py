import numpy as np
import librosa
import soundfile as sf
from pedalboard import Pedalboard, Distortion, Gain

def process_audio(input_file, dry_output_file, wet_output_file):
    # File path
    input_file = "13_ElecGtr03DI.wav"

    # Load the audio (set sr=44100 to have a constant sample rate)
    audio, sr = librosa.load(input_file, sr=44100, mono=True)

    # Calculate duration in seconds
    duration = len(audio) / sr
    print(f"Duration of the sound file: {duration:.2f} seconds")

    # Find the segments with audio (returns start-end intervals)
    non_silent_intervals = librosa.effects.split(audio, top_db=40)

    # Concatenate these segments into a new, condensed signal
    audio_condensed = np.concatenate([audio[start:end] for start, end in non_silent_intervals])

    # Calculate the new duration
    new_duration = len(audio_condensed) / sr
    print(f"Duration after removing silence: {new_duration:.2f} seconds")

    # Create a pedalboard with distortion effect
    board = Pedalboard([Distortion(drive_db=30)])

    # Process the audio through the pedalboard
    wet_audio = board(audio_condensed, sample_rate=sr)

    # Save the processed audio to a new file
    dry_output_file = "dataset_dry.wav"
    sf.write(dry_output_file, audio_condensed, sr)
    print(f"Dry audio saved to: {dry_output_file}")

    wet_output_file = "dataset_wet.wav"
    sf.write(wet_output_file, wet_audio, sr)
    print(f"Processed audio saved to: {wet_output_file}")
    
    return dry_output_file, wet_output_file