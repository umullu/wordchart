import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtCharts 2.3

Window {
    id: window
    title: "Word bar chart"
    visible: true
    width: 1280
    height: 720

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id: fileLayout
            Layout.margins: 30
            Layout.fillWidth: true
            TextField {
                id: fileName
                enabled: false
                text: fileDialog.fileUrl
                Layout.fillWidth: true
            }
            Button {
                id: chooseButton
                text: "..."
                onClicked: fileDialog.open();

                FileDialog {
                    id: fileDialog
                    title: "Please choose a file"
                    //folder: shortcuts.home
                    selectExisting: true
                    selectMultiple: false
                    onAccepted: {
                        console.log("File name: " + fileDialog.fileUrl);
                        thread.path = fileDialog.fileUrl;
                        thread.processText();
                        progressLayout.visible = true
                    }
                }
            }
        }
        RowLayout {
            id: progressLayout
            Layout.leftMargin: 30
            Layout.rightMargin: 30
            Layout.topMargin: -30
            Layout.bottomMargin: -30
            Layout.fillWidth: true
            visible: false
            ProgressBar {
                id: progressBar
                Layout.fillWidth: true
                from: 0
                to: 100

                Connections {
                    target: thread
                    function onDataUpdated() {
                        progressBar.value = thread.progress;
                        //console.log("Progress: " + thread.progress + "%")
                    }
                }
            }
            Button {
                id: cancelButton
                text: "X"
                onClicked: {
                    thread.cancelProcessing();
                    barCategoryAxis.categories = [];
                    barSet.values = [];
                    barSeries.axisY.min = 0;
                    barSeries.axisY.max = 4;
                    //chart.removeAllSeries();
                }
            }
            Connections {
                target: thread
                function onTextProcessed() {
                    progressLayout.visible = false;
                }
            }
        }
        ChartView {
            id: chart
            Layout.fillWidth: true
            Layout.fillHeight: true
            antialiasing: true
            legend.visible: false

            BarSeries {
                id: barSeries
                axisX: BarCategoryAxis { id: barCategoryAxis; gridVisible: false; categories: [] }
                BarSet { id: barSet; values: [] }

                Connections {
                    target: thread
                    function onDataUpdated() {
                        barCategoryAxis.categories = thread.words
                        barSet.values = thread.values
                        barSeries.axisY.min = 0
                        barSeries.axisY.max = thread.maximum
                        //barSeries.axisY.applyNiceNumbers()
                    }
                }
            }
            Component.onCompleted : {
                barSeries.axisY.min = 0
                barSeries.axisY.max = 4
                barSeries.axisY.labelFormat = "%d"
            }
        }
    }
}
