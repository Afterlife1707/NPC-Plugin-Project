import sys
from pydub import AudioSegment
import wave
import azure.cognitiveservices.speech as speechsdk

# Azure subscription details
subscription_key = ""
region = "uksouth"

def convert_audio_to_mono_and_16k(file_path, output_path):
    audio = AudioSegment.from_file(file_path)
    audio = audio.set_channels(1).set_frame_rate(16000)
    audio.export(output_path, format="wav")
    return output_path

def log_wav_file_details(file_path):
    with wave.open(file_path, 'rb') as wav_file:
        n_channels = wav_file.getnchannels()
        sample_width = wav_file.getsampwidth()
        frame_rate = wav_file.getframerate()
        n_frames = wav_file.getnframes()
        duration = n_frames / frame_rate
       

def transcribe_audio(file_path):
    speech_config = speechsdk.SpeechConfig(subscription=subscription_key, region=region)
    audio_config = speechsdk.audio.AudioConfig(filename=file_path)
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_config)

    result = speech_recognizer.recognize_once_async().get()

    if result.reason == speechsdk.ResultReason.RecognizedSpeech:
        recognized_text = result.text
    elif result.reason == speechsdk.ResultReason.NoMatch:
        recognized_text = "No speech could be recognized."
    elif result.reason == speechsdk.ResultReason.Canceled:
        cancellation_details = result.cancellation_details
        recognized_text = f"Speech Recognition canceled: {cancellation_details.reason}"
        if cancellation_details.reason == speechsdk.CancellationReason.Error:
            recognized_text += f" Error details: {cancellation_details.error_details}"

    return recognized_text

if __name__ == "__main__":
    # Get input and output file paths from command-line arguments
    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]

    # Process audio file
    converted_file_path = convert_audio_to_mono_and_16k(input_file_path, output_file_path)
    log_wav_file_details(output_file_path)
    
    # Perform transcription and only output the recognized text
    transcription_result = transcribe_audio(converted_file_path)
    sys.stdout.write(transcription_result + "\n")  
