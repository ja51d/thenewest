import QtQuick 2.15
import QtQuick.Window 2.14
import QtQuick3D 1.15
import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Controls 2.15






    View3D {
        property double roll: 0
        property double yaw: 0
        property double pitch: 0



        id: view
        anchors.fill: parent
        camera: camera
        renderMode: View3D.Overlay



        PerspectiveCamera {


            id: camera



            position: Qt.vector3d(0, 0, 350)



            eulerRotation.x: 0


        }

        DirectionalLight {



            eulerRotation.x: 10



            }



        Model {

            id: cylinder
            visible: true
            position: Qt.vector3d(0,0,0)

            source: "#Cylinder"

            materials: DefaultMaterial {



          specularAmount: 4
          specularRoughness: 4

          diffuseMap: Texture { source: "qrc:/a4e8ec629a520b2ef8c8865a1b075bbf.jpg"}



            }




          scale.x: 1
          scale.y: 2
          scale.z: 1

          eulerRotation.x: pitch
          eulerRotation.y: roll
          eulerRotation.z: yaw



                     }




Timer {
                                 id: updateTimer
                                 interval: 1000// Update every 1000 milliseconds (1 second)
                                 repeat: true
                                 running: true

                                 onTriggered: {


                                     eulerFunction(roll, yaw, pitch)
                                                                       console.log(roll, yaw, pitch)

}





}

function eulerFunction(rolll, yaww, pitchh){
cylinder.eulerRotationChanged(roll - rolll, yaw - yaww, pitch - pitchh)
          roll = rolll
          yaw = yaww
          pitch = pitchh


            }









        Rectangle {

anchors.fill:parent

color: "transparent"



            Image {
               anchors.fill: parent

                source: "qrc:/oie_1SZ5qFRcqEV6.jpg"
                anchors.left:parent.left
                anchors.top: parent.top
            }
            Image {
                width:170
                height:170
                source: "itutay01.png"
                anchors.top: parent.top
                anchors.right: parent.right
            }
            Text {
                id: text
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                color: "#052b47"
                font.pixelSize: 25
                text: qsTr("")
                font.family: "Gill Sans"
                font.bold: false
                style: Text.Outline
            }
            Text {
                    id: textt

                    anchors.centerIn: parent.TopLeft
                    color: "#052b47"
                    font.pixelSize: 25
                    text: qsTr("")
                    font.family: "Gill Sans"
                    font.bold: false
                    style: Text.Outline
            }
}
        }
