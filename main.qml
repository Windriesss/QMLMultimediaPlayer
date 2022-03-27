import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 2.15


Window {
    width: 1280
    height: 960
    visible: true
    title: qsTr("Hello World")

    RowLayout{
        anchors.fill: parent
        spacing:0

        VideoPlayback{
            id:video
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width*0.8
            Layout.fillHeight: true
        }

        Gallery{
            id:tools
            Layout.alignment: QT.AlignRight
            Layout.preferredWidth: parent.width*0.2
            Layout.fillWidth:true
            Layout.fillHeight: true
        }

        Component.onCompleted: {
            tools.selectedVideo.connect(video.playVideo)
        }
    }


}
