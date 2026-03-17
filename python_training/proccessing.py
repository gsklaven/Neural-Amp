import numpy as np
import librosa
import soundfile as sf
from pedalboard import Pedalboard, Distortion
from pathlib import Path

def process_audio(input_file, dry_output_file, wet_output_file):
    # Resolve path relative to the script's directory
    script_dir = Path(__file__).parent
    input_path = script_dir / input_file

    if not input_path.exists():
        raise FileNotFoundError(
            f"Audio file not found: {input_path}\n"
            f"Make sure '{input_file}' is in: {script_dir}"
        )

    # Load the audio
    audio, sr = librosa.load(str(input_path), sr=44100, mono=True)

    duration = len(audio) / sr
    print(f"Duration of the sound file: {duration:.2f} seconds")

    # Remove silence
    non_silent_intervals = librosa.effects.split(audio, top_db=40)
    audio_condensed = np.concatenate([audio[start:end] for start, end in non_silent_intervals])

    new_duration = len(audio_condensed) / sr
    print(f"Duration after removing silence: {new_duration:.2f} seconds")

    # Apply distortion
    board = Pedalboard([Distortion(drive_db=30)])
    wet_audio = board(audio_condensed, sample_rate=sr)

    # Save outputs next to the script
    dry_path  = script_dir / dry_output_file
    wet_path  = script_dir / wet_output_file

    sf.write(str(dry_path), audio_condensed, sr)
    print(f"Dry audio saved to: {dry_path}")

    sf.write(str(wet_path), wet_audio, sr)
    print(f"Wet audio saved to: {wet_path}")

    return str(dry_path), str(wet_path)