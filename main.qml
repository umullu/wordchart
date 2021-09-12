import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
//import Qt.labs.platform 1.0

Window {
    id: window
    title: "Folder dialog test"
    visible: true
    width: 600
    height: 400

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        RowLayout {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            TextField {
                id: fileName
                enabled: false
                text: fileDialog.fileUrl
                Layout.fillWidth: true
            }
            Button {
                id: selectButton
                text: "Select"
                onClicked: fileDialog.open();

                FileDialog {
                    id: fileDialog
                    title: "Please choose a file"
                    //folder: shortcuts.home
                    selectExisting: true
                    selectMultiple: false
                    onAccepted: {
                        console.log("You chose: " + fileDialog.fileUrl);
                        thread.path = fileDialog.fileUrl;
                        thread.processText();
                        //Qt.quit()
                    }
                    onRejected: {
                        console.log("Canceled")
                        //Qt.quit()
                    }
                }
            }
        }
        RowLayout {
            id: progressLayout
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            ProgressBar {
                id: progressBar
                Layout.fillWidth: true
                from: 0
                to: 100

                Connections {
                    target: thread
                    onDataUpdated: {
                        progressBar.value = thread.progress;
                        console.log(thread.progress)
                    }
                }
            }
            Button {
                id: cancelButton
                text: "X"
                onClicked: thread.cancelProcessing();
            }
            Connections {
                target: thread
                onTextProcessed: {
                    progressLayout.visible = false
                }
            }
        }
        GridView {
            id: chart
        }
    }
}
