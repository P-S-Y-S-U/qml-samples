import QtQuick 2.12
import QtQuick.Window 2.12

Rectangle {
    color: "green"
    width: parent.width
    height: parent.height


    ColorAnimation on color {
        to: "red"
        duration: 5000
    }

    onColorChanged: {
        if( Qt.colorEqual( color, '#ff0000' ) )
        {
            surfacedumper.acquireStates();
            console.log("Hello Nice");
        }
    }
}
