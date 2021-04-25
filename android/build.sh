#!/bin/bash
export USE_CCACHE=1
export NDK_CCACHE="$(which ccache)"
export CCACHE_COMPILERCHECK=content
WORK_PATH=`dirname $(readlink -f $0)`
VERSION=$1
ccache -M 2G
ccache --show-stats
./gradlew assembleExperimentalRelease -Pj=`nproc` -Pabi_arm_32=false -Pabi_arm_64=true -Poverride_version=${VERSION}
mv ${WORK_PATH}/app/build/outputs/apk/experimental/release/cataclysmdda-${VERSION}-experimental-arm64-v8a-release-unsigned.apk ${WORK_PATH}/cdda-${VERSION}-arm64.apk
jarsigner -sigalg SHA256withRSA -digestalg SHA-256 -keystore cdda.keystore -storepass CataclysmDDA ${WORK_PATH}/cdda-${VERSION}-arm64.apk cdda
ccache --show-stats
