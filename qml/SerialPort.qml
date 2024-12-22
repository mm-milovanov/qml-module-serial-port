import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

ColumnLayout {
    required property QtObject cppObject

    spacing: 10

    ComboBox {
        id: comboBox

        Layout.fillWidth: true

        enabled: !cppObject.connected
        width: parent.width
        textRole: "display"
        model: cppObject
        onCountChanged: {
            if (comboBox.currentIndex > count-1)
                comboBox.currentIndex -= 1;
        }

        Component.onCompleted:
            comboBox.currentIndex = cppObject.index;

        Binding {
            target: cppObject
            property: "index"
            value: comboBox.currentIndex
        }
    }

    TextField {
        id: baudSelector

        Layout.fillWidth: true

        enabled: !cppObject.connected
        width: parent.width
        text: cppObject.baud
        placeholderText: qsTr("Baudrate")
        validator: IntValidator {
            bottom: 0
        }
        horizontalAlignment: Text.AlignHCenter

        Binding {
            target: cppObject
            property: "baud"
            value: parseInt(baudSelector.text, 10)
        }
    }

    ComboBox {
        id: dataBitsSelector

        Layout.fillWidth: true

        enabled: !cppObject.connected
        width: parent.width
        currentIndex: 3
        model : [
            "DataBits 5",
            "DataBits 6",
            "DataBits 7",
            "DataBits 8"
        ]

        Binding {
            target: cppObject
            property: "data"
            value: dataBitsSelector.currentIndex
        }
    }

    ComboBox {
        id: paritySelector

        Layout.fillWidth: true

        enabled: !cppObject.connected
        width: parent.width
        model : [
            "NoParity",
            "EvenParity",
            "OddParity",
            "SpaceParity",
            "MarkParity"
        ]

        Binding {
            target: cppObject
            property: "parity"
            value: paritySelector.currentIndex
        }
    }

    ComboBox {
        id: stopBitsSelector

        Layout.fillWidth: true

        enabled: !cppObject.connected
        width: parent.width
        model : [
            "OneStop",
            "OneAndHalfStop",
            "TwoStop"
        ]

        Binding {
            target: cppObject
            property: "stop"
            value: stopBitsSelector.currentIndex
        }
    }

    Label {
        Layout.fillWidth: true

        enabled: !cppObject.connected
        text: "System location: " + cppObject.location
    }

    Label {
        Layout.fillWidth: true

        enabled: !cppObject.connected
        text: "Description: " + cppObject.description
    }

    Button {
        Layout.fillWidth: true

        highlighted: cppObject.connected
        width: parent.width
        text:
            cppObject.connected ? qsTr("DISCONNECT") : qsTr("CONNECT");
        onClicked: cppObject.onConnectionButtonClicked()
    }
}
