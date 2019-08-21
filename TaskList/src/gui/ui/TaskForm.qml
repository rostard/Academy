import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

GridLayout {
    id: grid
    property alias title: title
    property alias priority: priority

    property int minimumInputSize: 120
    property string placeholderText: qsTr("<enter>")

    rows: 2
    columns: 2

    Label {
        text: qsTr("Task title")
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
    }

    TextField {
        id: title
        focus: true
        Layout.fillWidth: true
        Layout.minimumWidth: grid.minimumInputSize
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        placeholderText: grid.placeholderText
    }

    Label {
        text: qsTr("Priority")
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
    }

    ComboBox {
        id: priority
        model: ListModel {
            ListElement { text: "Low"; color: "#98fb98" }
            ListElement { text: "Medium"; color: "#effd5f" }
            ListElement { text: "High"; color: "#ed2939" }
        }
        textRole: "text"
        Layout.fillWidth: true
        Layout.minimumWidth: grid.minimumInputSize
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
    }
}
