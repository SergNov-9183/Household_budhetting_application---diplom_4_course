import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

 Popup {
    visible: false
       width: 400
       height: 300
       background: FrameItem {}

       ColumnLayout {
           HFLabel {
               text: qsTr("Все время")
           }
           HFLabel {
               text: qsTr("Период")
           }
       }


}
