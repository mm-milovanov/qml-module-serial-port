import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Pane{
    id: sPortSelectorPage

    property QtObject cppObject

    Pane {
        anchors.centerIn: parent

        Loader {
            anchors.fill: parent
            Component {
                id: sPortSelectorInvalid
                Label {
                    text: "SERIAL DEVICES ARE NOT PRESENTED ON THIS MACHINE"
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            Component {
                id: sPortSelector

                Column{
                    width: 256
                    spacing: 8

                    ComboBox {
                        id: comboBox
                        enabled: !cppObject.connected
                        width: parent.width
                        textRole: "display"
                        model: cppObject
                        onCountChanged: {
                            //console.log(currentIndex, count)
                            if(comboBox.currentIndex > count-1){
                                comboBox.currentIndex -= 1
                            }
                        }

                        Component.onCompleted: {
                            //console.log("Completed Running!", comboBox.currentIndex)
                            comboBox.currentIndex = cppObject.index
                        }

                        Binding {
                            target: cppObject
                            property: "index"
                            value: comboBox.currentIndex
                        }
                    }

                    TextField {
                        id: baudSelector
                        enabled: !cppObject.connected
                        width: parent.width
                        text: cppObject.baud
                        placeholderText: qsTr("Baudrate")
                        validator: IntValidator {bottom: 0}
                        horizontalAlignment: Text.AlignHCenter

                        Binding {
                            target: cppObject
                            property: "baud"
                            value: parseInt(baudSelector.text, 10)
                        }
                    }

                    ComboBox {
                        id: dataBitsSelector
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
                        enabled: !cppObject.connected
                        text: "System location: " + cppObject.location
                    }

                    Label {
                        enabled: !cppObject.connected
                        text: "Description: " + cppObject.description
                    }

                    Button {
                        Material.background: {
                            cppObject.connected ?
                                        Material.Red :
                                        Material.Indigo
                        }
                        Material.foreground: "white"
                        width: parent.width
                        text: cppObject.connected ? "DISCONNECT" : "CONNECT"
                        onClicked: cppObject.onConnectionButtonClicked()
                    }
                }
            }
            sourceComponent: {
                cppObject.count > 0 ? sPortSelector : sPortSelectorInvalid
            }
        }
    }
}
