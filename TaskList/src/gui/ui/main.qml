import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.11
import QtQml.StateMachine 1.0 as FSM

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    property int currentTask: -1
    property int curState: 0
    TaskDialog {
        id: taskDialog
        onFinished: {
            if (currentTask === -1)
                tasks.append(title, priority)
            else
                tasks.set(currentTask, title, priority)
        }
    }

    FSM.StateMachine
    {
        id: stateMachine
        initialState: picking
        running: true
        FSM.State {
            id: picking
            FSM.SignalTransition {
                targetState: editing
                signal: editBtn.clicked
            }
            FSM.SignalTransition {
                targetState: deleting
                signal: deleteBtn.clicked
            }
            onEntered: {
                curState = 0
                pickBtn.checked = true
                pickBtn.checkable = false
            }
            onExited: {
                pickBtn.checked = false;
            }
        }
        FSM.State {
            id: editing
            FSM.SignalTransition {
                targetState: picking
                signal: pickBtn.clicked
            }
            FSM.SignalTransition {
                targetState: deleting
                signal: deleteBtn.clicked
            }
            onEntered: {
                curState = 1
                editBtn.checked = true;
                editBtn.checkable = false
            }
            onExited: {
                editBtn.checked = false;
            }

        }
        FSM.State {
            id: deleting
            FSM.SignalTransition {
                targetState: picking
                signal: pickBtn.clicked
            }
            FSM.SignalTransition {
                targetState: editing
                signal: editBtn.clicked
            }
            onEntered: {
                curState = 2
                deleteBtn.checked = true;
                deleteBtn.checkable = false
            }
            onExited: {
                deleteBtn.checked = false;
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            Layout.fillWidth: true
            ScrollView {
                Layout.fillWidth: true
                Layout.preferredWidth: 5
                TaskList
                {
                    anchors.fill: parent
                    id: list
                    model: tasks
                    onItemClicked:
                    {
                        console.log("pressed: " + row + " state: " + curState)
                        switch(curState)
                        {
                        case 1:
                            currentTask = currentIndex
                            taskDialog.editTask(tasks.get(currentIndex))
                            break
                        case 2:
                            tasks.remove(row)
                        }
                    }
                }
            }



            ColumnLayout{
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                ToolButton{
                    id: pickBtn
                    Layout.fillWidth: true
                    text: "Pick"
                }
                ToolButton{
                    id: editBtn
                    Layout.fillWidth: true
                    text: "Edit"
                }
                ToolButton{
                    id: deleteBtn
                    Layout.fillWidth: true
                    text: "Delete"
                }
            }
        }

        Button
        {
            text: "Add task"
            Layout.fillWidth: true
            height: 100
            onClicked:
            {
                currentTask = -1
                taskDialog.createTask()
            }
        }
    }
}
