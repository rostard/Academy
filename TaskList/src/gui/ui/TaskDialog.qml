import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: dialog

    signal finished(string title, int priority)

    function createTask() {
        form.title.clear();
        form.priority.currentIndex=1;
        console.log("here")
        dialog.title = qsTr("Add Task");
        dialog.open();
    }

    function editTask(contact) {
        form.title.text = contact.title;
        form.priority.currentIndex = contact.priority - 1;
//        form.city.text = contact.city;
//        form.number.text = contact.number;

        dialog.title = qsTr("Edit Task");
        dialog.open();
    }

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    focus: true
    modal: true
    title: qsTr("Add Contact")
    standardButtons: Dialog.Ok | Dialog.Cancel

    contentItem: TaskForm {
        id: form
    }

    onAccepted: finished(form.title.text, form.priority.currentIndex+1)
}
