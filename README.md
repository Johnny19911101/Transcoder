# Transcoder
Check with https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu
It is for TS file.
First. \
  1.Build by ffmpeg and c++ \
  2.Need ffmpeg source lib and put libavcodec.a libx264.a libfdk_aac.a in lib folder \
  3.Use CMake for makefile. \
  4.This Transcoder is for dvbt ts to other codec. \
  5.Use x264 encoder and fdk_aac encdoer. \
  6.For DVbt Spec.\
  7.Finite State Machine.
  8.Compile configure ad below
  Put the libraries in lib folder\
  /configure \
  --prefix="$HOME/ffmpeg_build" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" \
  --extra-libs="-lpthread -lm" \
  --bindir="$HOME/bin" \
  --enable-gpl \
  --enable-libfdk-aac \
  --enable-libx264 \
  --enable-nonfree
