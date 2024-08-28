import QtQuick 2.12
import QtQuick.Window 2.12

Rectangle
{
    anchors.fill: parent
    color: "grey"

Rectangle
{
    id: root
    width: 500
    height: 500
    x: 500
    y: 500
    radius: 75
    color: "green"
    anchors.centerIn: parent

    antialiasing: false

    Text {
        id: someText
        text: qsTr("Some Text With Curvessss!!!");
        anchors.centerIn: parent
        /*x: 100
        y: 100*/
        font.pixelSize: 50
        antialiasing: true
    }
}

}
