WORK_PATH=`dirname $(readlink -f $0)`

# install android sdk
apt-get update
apt-get install -y openjdk-8-jdk-headless unzip wget gettext ccache
wget https://dl.google.com/android/repository/sdk-tools-linux-4333796.zip
unzip sdk-tools-linux-4333796.zip -d ${WORK_PATH}/android-sdk
rm sdk-tools-linux-4333796.zip
${WORK_PATH}/android-sdk/tools/bin/sdkmanager --update
${WORK_PATH}/android-sdk/tools/bin/sdkmanager "tools" "platform-tools" "ndk-bundle"
yes | ${WORK_PATH}/android-sdk/tools/bin/sdkmanager --licenses

export USE_CCACHE=1
export ANDROID_SDK_ROOT=${WORK_PATH}/android-sdk
export ANDROID_HOME=${WORK_PATH}/android-sdk
export ANDROID_NDK_ROOT=${WORK_PATH}/android-sdk/ndk-bundle
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools:$ANDROID_SDK_ROOT/tools:$ANDROID_NDK_ROOT
