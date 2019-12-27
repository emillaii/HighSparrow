import QtQuick 2.0
import QtQuick.Controls 2.0
import LanguageConfig 1.0

Popup{
    id: root
    Column{
        Repeater{
            model: ListModel{
                ListElement{
                    role: "简体中文"
                    language: LanguageConfig.Chinese
                }
                ListElement{
                    role: "English"
                    language: LanguageConfig.English
                }
            }
            delegate: Button{
                text: role
                onClicked: {
                    languageConfig.setLanguage(language)
                    root.close()
                }
            }

        }
    }
}
