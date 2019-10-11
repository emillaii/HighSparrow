import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


RowLayout {

    Label { text: "hello" }
    Button{
        text: aaCoreParams.MinArea

        onClicked: {
            visionModule.testVision()
            aaCoreParams.setMinArea(999)
        }
    }

}
