import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle{
    color:"black"

    function playVideo(source){
        video.source=source;
        video.play();
    }

    function getTimeString(time){
        var sec=(time / 1000).toFixed(0);
        let min=(sec / 60).toFixed(0);
        sec=sec % 60;
        let formattedMin = min > 9 ? min : "0" + min
        let formattedSec = sec > 9 ? sec : "0" + sec
        return formattedMin + ":" + formattedSec
    }

    Video{
        id:video
        anchors.fill: parent

        property string currentTime: "00:00"
        property string totalTime:"00:00"
        focus: true


        onPlaybackStateChanged: {
            console.log("onPlaybackStateChanged!",video.playbackState,MediaPlayer.PlayingState)
            play.icon.source = (video.playbackState === MediaPlayer.PlayingState) ? "./player-icons/player-pause.png" : "./player-icons/player-play.png"
        }

        onPositionChanged: {
            progress.value = video.position/video.duration
            currentTime=getTimeString(video.position)
            playbackTimeLabel.text=currentTime + "/" + totalTime
        }

        onDurationChanged: {
            totalTime=getTimeString(video.duration)
            playbackTimeLabel.text=currentTime + "/" + totalTime

        }


        MouseArea{
            anchors.fill:parent
            hoverEnabled:true
            onClicked: {
                video.playbackState === MediaPlayer.PlayingState ? video.pause() : video.play()
            }
            onEntered:{
                control.visible=true
            }
            onExited: {
                if(!(play.hovered|prev.hovered|next.hovered))
                    control.visible=false
            }

            Rectangle{
                id:control
                visible: hovered | parent.hovered | play.hovered
                width:parent.width * 0.7
                height:parent.height * 0.1
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom:parent.bottom
                    bottomMargin: height * 0.5
                }
                radius:height*0.1
                color:Qt.rgba(0.1,0.1,0.1,0.9)


                RowLayout{
                    id:playerControl
                    width:parent.width * 0.4;
                    y:parent.height * 0.05;
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button{
                        id:prev
                        hoverEnabled: true
                        icon.source: "./player-icons/player-track-prev.png"
                        Layout.alignment: Qt.AlignLeft
                        onClicked: {
                            video.position -=2000
                        }

                    }

                    Button{
                        id:play
                        icon.source:"./player-icons/player-play.png"
                        hoverEnabled: true
                        onClicked: {
                            console.log("play button clicked!")
                            video.playbackState=== MediaPlayer.PlayingState ? video.pause() : video.play() ;
                        }
                    }
                    Button{
                        id:next
                        icon.source: "./player-icons/player-track-next.png"
                        hoverEnabled: true
                        onClicked:{
                            video.position +=2000
                        }
                    }

                }
                RowLayout{
                    width:control.width*0.8
                    anchors{
                        top:playerControl.bottom
                        horizontalCenter: playerControl.horizontalCenter
                    }

                    Layout.alignment: Qt.AlignVCenter
                    spacing: 5
                    ProgressBar{
                        id:progress
                        value:0
                        Layout.fillWidth: true
                    }

                    Label{
                        id:playbackTimeLabel
                        text:"00:00/00:00"
                        color:"white"
                    }

                }

            }
        }

    }

}
