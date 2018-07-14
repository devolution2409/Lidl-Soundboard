//#ifndef PRESETCONTROLLER_H
//#define PRESETCONTROLLER_H

//#include <QObject>
//#include <QWidget>
//#include <QWizard>
//#include <map>
//#include <vector>
//#include <QString>
//#include "lib/bass.h"
//namespace LIDL {


///*!
//  * \brief The PresetController singleton.
//  *
//  * Deals with storing the presets and acessing them.
//  * \author Devolution
//  * \version 1.7.0
//  *
//  * \since 1.7.0
//  */
//class PresetController : public QObject
//{
//    Q_OBJECT
//public:
//    /*!
//     * \brief GetInstance, returns the instance of the controller, creating one if it doesn't exist.
//     */
//    static GetInstance();

//private:
//    /*!
//     * \brief PresetController default constructor.
//     * \param parent
//     */
//    explicit PresetController(QObject *parent = nullptr);

//    static PresetController * self; /*!< A pointer to the only existing PresetController.*/


//    std::vector<std::map<QString, BASS_DX8_DISTORTION>> _distortionPreset;



//signals:

//public slots:



//};
//}
//#endif // PRESETCONTROLLER_H
