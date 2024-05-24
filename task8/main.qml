import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 200
    title: "Timer App"

    property int milliseconds: 0
    property int seconds: 0
    property int minutes: 0
    property int hours: 0

    Timer {
        id: timer
        interval: 1
        repeat: true
        running: false
        onTriggered: {
            milliseconds++
            if (milliseconds === 1000) {
                milliseconds = 0
                seconds++
                if (seconds === 60) {
                    seconds = 0
                    minutes++
                    if (minutes === 60) {
                        minutes = 0
                        hours++
                    }
                }
            }
        }
    }

    function formatTime(value, length) {
        var result = value.toString()
        while (result.length < length) {
            result = "0" + result
        }
        return result
    }

    Column {
        spacing: 10
        anchors.centerIn: parent

        Text {
            text: formatTime(hours, 2) + ":" + formatTime(minutes, 2) + ":" + formatTime(seconds, 2) + ":" + formatTime(milliseconds, 3)
            font.bold: true
            font.pointSize: 20
        }

        Button {
            text: timer.running ? "Stop Timer" : "Start Timer"
            onClicked: {
                timer.running = !timer.running
            }
        }

    }
}
