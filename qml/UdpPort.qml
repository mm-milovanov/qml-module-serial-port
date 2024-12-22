import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Pane{
    id: sPortSelectorPage

    property QtObject cppObject
    property int innerWidth: 256

    Pane {
        anchors.centerIn: parent
        Column {
            width: innerWidth
            spacing: 8

            Label {
                width: innerWidth
                text: "UDP PORT"
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                text: "Source address:"
            }

            Row {
                spacing: 10
                TextField {
                    id: srcAdrsTextField
                    enabled: !cppObject.connected && !cppObject.lock
                    width: innerWidth/2
                    text: cppObject.srcAdrs
                    validator: RegularExpressionValidator {
                        regularExpression: /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/
                    }
                    horizontalAlignment: Text.AlignHCenter
                    onEditingFinished: cppObject.srcAdrs = text
                }

                TextField {
                    id: srcPortTextField
                    enabled: !cppObject.connected && !cppObject.lock
                    width: parent.width/3
                    text: cppObject.srcPort
                    validator: IntValidator {
                        bottom: 0
                    }
                    horizontalAlignment: Text.AlignHCenter
                    onEditingFinished: cppObject.srcPort = text
                }
            }

            Label {
                text: "Destination address:"
            }

            Row {
                spacing: 10
                TextField {
                    id: dstAdrsTextField
                    enabled: !cppObject.connected && !cppObject.lock
                    width: innerWidth/2
                    text: cppObject.dstAdrs
                    validator: RegularExpressionValidator {
                        regularExpression: /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/
                    }
                    horizontalAlignment: Text.AlignHCenter
                    onEditingFinished: cppObject.dstAdrs = text
                }

                TextField {
                    id: dstPortTextField
                    enabled: !cppObject.connected && !cppObject.lock
                    width: parent.width/3
                    text: cppObject.dstPort
                    validator: IntValidator {
                        bottom: 0
                    }
                    horizontalAlignment: Text.AlignHCenter
                    onEditingFinished: cppObject.dstPort = text
                }
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
}
