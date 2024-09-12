sudo  ./gradlew assembleDebug
adb install /home/tom/Creative/cpp/Bouldrovka/app/build/outputs/apk/debug/app-debug.apk
adb shell monkey -p com.bouldrovka.app -c android.intent.category.LAUNCHER 1

