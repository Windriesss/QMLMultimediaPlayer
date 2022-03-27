import QtQuick 2.0
import com.ghj.asset

Rectangle{
    id:root
    color:"transparent"
    //visible: false

    property VideoAssetModel videoModel: VideoAssetModel{}

    signal selectedVideo(string url)

    GridView{
        id:grid
        anchors.fill: parent
        cellWidth: 120;cellHeight: 100
        focus:true//接受鼠标键盘操作
        model:videoModel
        currentIndex: -1

        highlight: Rectangle {
            width: 200; height: 200
            color: "lightsteelblue"
        }
        delegate: Item{
            clip: true
            width:grid.cellWidth
            height: grid.cellHeight
            Image{
                id:coverImage
                width:grid.cellWidth*0.9
                height:grid.cellHeight*0.6
                source:asset.coverImage
                anchors.horizontalCenter: parent.horizontalCenter
            }
            property alias nameLength: name.paintedWidth
            property alias nameTextWidth:name.width
            property alias animation: animation
            Flickable{
                id:flickable
                width:grid.cellWidth*0.9
                y:parent.height*0.8
                Text{
                    id:name
                    width:parent.width
                    text:asset.name
                    color:"black"
                    elide:Text.ElideNone
                    //anchors{top: coverImage.bottom;horizontalCenter:parent.horizontalCenter}
                    horizontalAlignment:Text.AlignHCenter | Text.AlignVCenter
                }

                SequentialAnimation on contentX {
                    id:animation
                    //loops: Animation.Infinite
                    loops: 1
                    running:name.paintedWidth>name.width
                    PropertyAnimation {
                        from:-flickable.width
                        to: name.paintedWidth-flickable.width
                        duration: name.paintedWidth*12
                    }
                    PropertyAnimation {
                        from: name.paintedWidth-flickable.width
                        to:0
                        duration: name.paintedWidth*7
                    }
                }

            }


            MouseArea{
                anchors.fill: parent
                acceptedButtons:Qt.LeftButton|Qt.RightButton
                onClicked:(event)=>{
                              if(event.button === Qt.LeftButton){
                                  grid.currentIndex=model.index
                                  root.selectedVideo(asset.url)
                              }
                              else if(event.button === Qt.RightButton){
                                  grid.model.removeVideo(model.index)
                              }
                          }
                onReleased: {
                    animation.start()
                }

            }
        }
        onCurrentItemChanged:{
            if(currentItem.nameLength>currentItem.nameTextWidth){
                currentItem.animation.start();
            }
        }
        Component.onCompleted:{
            model.loadAssets()
        }
    }

    DropArea{
        anchors.fill: parent
        onDropped: (drop)=>{
                       dropFile(drop)
                       drop.accept()
                   }
    }


    function dropFile(file){
        console.log(file.urls)
        for(var i=0;i<file.urls.length;++i){
            videoModel.addVideo(file.urls[i]);
        }
    }

}


