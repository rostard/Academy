import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11


ListView {
    function formatTime(time) {
        var hours = Math.floor(time / 3600);
        var minutes = Math.floor((time % 3600) / 60);
        var seconds = time % 60;
        return "" + hours + ":" + minutes + ":" + seconds
    }

    spacing: 2
    signal itemClicked(int row)

    delegate: ItemDelegate {
        width: parent.width
        font.pointSize: 16
        font.family: "Helvetica"
        height: 30

        onClicked:
        {
            list.currentIndex = index
            itemClicked(index)
        }
        Rectangle{
            opacity: timerState == 1 ? 1.0: 0.5
            anchors.fill: parent
            color: priority == 1 ? "#98fb98" : priority == 2 ? "#effd5f" : "#ed2939"
        }

        RowLayout{

            anchors.fill: parent
            Label{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 3
                Layout.fillWidth: true
                text: title
            }
            Label{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 1
                Layout.fillWidth: true
                text: formatTime(timer)
            }
            Label{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                text: timerState == 1 ? "Running" : timerState == 0 ? "New" : timerState == 2 ? "Paused" : "Finished"

            }
            Button{
                text: timerState == 1 ? "P" : "S"
                Layout.preferredWidth: 30
                Layout.fillHeight: true
                onClicked: {
                    tasks.run(index)
                }
            }
            Button{
                text: "F"
                Layout.preferredWidth: 30
                Layout.fillHeight: true
                onClicked: {
                    tasks.finish(index)
                }
            }
            Button{
                text: "R"
                Layout.preferredWidth: 30
                Layout.fillHeight: true
                onClicked: {
                    tasks.reset(index)
                }
            }

        }


    }

}

