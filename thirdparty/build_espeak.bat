@echo on
setlocal

set EDIR=espeak-ng\src\
set E_IN=%EDIR%espeak-ng.c %EDIR%compat\getopt.c %EDIR%libespeak-ng\common.c %EDIR%libespeak-ng\soundicon.c %EDIR%libespeak-ng\translateword.c %EDIR%ucd-tools\src\case.c %EDIR%ucd-tools\src\categories.c %EDIR%ucd-tools\src\ctype.c %EDIR%ucd-tools\src\proplist.c %EDIR%ucd-tools\src\scripts.c %EDIR%ucd-tools\src\tostring.c %EDIR%libespeak-ng\compiledata.c %EDIR%libespeak-ng\compiledict.c %EDIR%libespeak-ng\compilembrola.c %EDIR%libespeak-ng\dictionary.c %EDIR%libespeak-ng\encoding.c %EDIR%libespeak-ng\error.c %EDIR%libespeak-ng\espeak_api.c %EDIR%libespeak-ng\ieee80.c %EDIR%libespeak-ng\intonation.c %EDIR%libespeak-ng\klatt.c %EDIR%libespeak-ng\langopts.c %EDIR%libespeak-ng\mbrowrap.c %EDIR%libespeak-ng\mnemonics.c %EDIR%libespeak-ng\numbers.c %EDIR%libespeak-ng\phoneme.c %EDIR%libespeak-ng\phonemelist.c %EDIR%libespeak-ng\readclause.c %EDIR%libespeak-ng\setlengths.c %EDIR%libespeak-ng\spect.c %EDIR%libespeak-ng\speech.c %EDIR%libespeak-ng\sPlayer.c %EDIR%libespeak-ng\ssml.c %EDIR%libespeak-ng\synthdata.c %EDIR%libespeak-ng\synthesize.c %EDIR%libespeak-ng\synth_mbrola.c %EDIR%libespeak-ng\translate.c %EDIR%libespeak-ng\tr_languages.c %EDIR%libespeak-ng\voices.c %EDIR%libespeak-ng\wavegen.c %EDIR%pcaudiolib\src\audio.c %EDIR%pcaudiolib\src\windows.c %EDIR%pcaudiolib\src\xaudio2.cpp %EDIR%speechPlayer\src\frame.cpp %EDIR%speechPlayer\src\speechPlayer.cpp %EDIR%speechPlayer\src\speechWaveGenerator.cpp %EDIR%windows\com\comentrypoints.c %EDIR%windows\com\ttsengine.cpp
set E_INC=/I %EDIR%include\ /I %EDIR%include\compat\ /I %EDIR%libespeak-ng\ /I %EDIR%pcaudiolib\src\ /I %EDIR%pcaudiolib\src\include\ /I %EDIR%ucd-tools\src\include\ /I %EDIR%windows\
  
cl /MD /c %E_IN% %E_INC% /DESPEAK_NG_EXPORTS

endlocal