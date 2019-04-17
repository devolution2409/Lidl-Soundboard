#include "SaveController.h"


namespace LIDL {

namespace Controller {

SaveController* SaveController::self = nullptr;

SaveController::SaveController()
{

}

SaveController *SaveController::GetInstance()
{
    if (self == nullptr)
        self = new SaveController();
    return self;

}

void SaveController::Save()
{

    // if file doesn't exist we throw the save as prompt
    if (this->_saveName.isEmpty())
    {
        this->SaveAs();
        return;
    }
    else
    {
        this->SaveAs(this->_saveName);
    }
}

void SaveController::SetStopKeyName(QString name)
{
    this->_stopKeyName = name;
}

void SaveController::SetStopVirtualKey(unsigned int vk)
{
    this->_stopVirtualKey = vk;
}

void SaveController::OpenSaveFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,QObject::tr("Open file"),
                                                    LIDL::Controller::SettingsController::GetInstance()->GetDefaultSoundboardFolder() ,
                                                    QObject::tr("LIDL JSON file(*.lidljson)"));

    if ((fileName).isEmpty())
            return;

    QFile file(fileName);
    QJsonObject json;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)   )
    {

        QString jsonAsString = file.readAll();

        //QJsonParseError error;
        QJsonDocument cdOMEGALUL = QJsonDocument::fromJson(jsonAsString.toUtf8());
        if (cdOMEGALUL.isNull())
        {
            file.close();
            QString errorMsg( "\""  +  fileName +  "\" isn't a valid .lidljson file.");
            return;
        }

        json = cdOMEGALUL.object();
        file.close();
    }
    // searching for version to know how to read

    if (!(json.contains("Settings")))
    {
        QMessageBox::warning(nullptr,QObject::tr("Error"),
                             QObject::tr("File \"%1\" is not a valid lidljson file.").arg(fileName));
        return;
    }

    QString version;

    if (json.contains("Version"))
        version = json.value("Version").toString();
    else
        version = "1.x.x";

        qDebug() << "gneee" << version;

    QRegExp regexp("^(\\d+)\\.(\\d+).+");
    //execute the regex, i think
    regexp.indexIn(version);

    if (regexp.capturedTexts().size() < 3){
        // show error about invalid string version in json
        return;
    }

    //save 1.9.x introduced profiles
    bool saveFormat190 = false;
    if ( regexp.cap(1).toInt() > 1){
        saveFormat190 = true;
    } else if (regexp.cap(1).toInt() == 1 && regexp.cap(2).toInt() >= 9){
       saveFormat190 = true;
    }
    // we clear the soundboard and the profile
    // here forsenD

    QString mainOutputDevice, vacOutputDevice;
    QString pttName;
    int pttScanCode=-1, pttVirtualKey =-1;
    QString stopName; int stopVirtualKey =-1;

    this->_saveName = fileName;
    if (saveFormat190)
    {

        //parse the "new" json syntax

        // parsing the settings
        QJsonObject settings = json.value("Settings").toObject();
        if (settings.contains("Main Output Device"))
            mainOutputDevice = settings.value("Main Output Device").toString();
        if (settings.contains("VAC Output Device"))
            vacOutputDevice = settings.value("VAC Output Device").toString();
        if (settings.contains("Stop Sound Key"))
        {
            QJsonObject settingsStop = settings.value("Stop Sound Key").toObject();
            if (settingsStop.contains("Key Name"))
                stopName = settingsStop.value("Key Name").toString();
            if (settingsStop.contains("VirtualKey"))
                stopVirtualKey = settingsStop.value("VirtualKey").toInt();
        }

        //parsing the profiles

        if (json.contains("Profiles")){
            QJsonArray profiles = json.value("Profiles").toArray();
            foreach (const QJsonValue & value, profiles) {
                QVector <std::shared_ptr<SoundWrapper>> wrappers;
                QJsonObject profile = value.toObject();
                QString profileName;
                QSet<QString> exes;
                if (profile.contains("Profile Name")){
                  profileName = profile.value("Profile Name").toString();
                }
                if (profile.contains("Executables")){
                    QJsonArray execs = profile.value("Executables").toArray();
                    foreach(const QJsonValue & value, execs )
                    {
                        exes.insert(value.toString());
                    }

                }

                // sounds
                if (profile.contains("Sounds")){
                    QJsonArray wrappersArray = profile.value("Sounds").toArray();
                    for (auto i:wrappersArray)
                    {
                        QJsonObject item = i.toObject();
                        LIDL::Playback playbackmode;
                        QString shortcutString;
                        int shortcutVirtualKey;
                        QVector<LIDL::SoundFile*> fileArray;
                        // Playback
                        // don't mind me just avoiding the may be unitialized warnings ppHop
                        playbackmode = LIDL::Playback::Singleton;
                        if (item.contains("Playback Mode"))
                        {
                            playbackmode = static_cast<LIDL::Playback>(item.value("Playback Mode").toInt());
                            //accounting for the removval of singleton
                            if (playbackmode == LIDL::Playback::Singleton)
                                playbackmode = LIDL::Playback::Sequential;

                        }
                        // Shortcut info
                        // don't mind me just avoiding the may be unitialized warnings ppHop
                        shortcutVirtualKey = -1;
                        if (item.contains("Shortcut"))
                        {
                            QJsonObject shortcut = item.value("Shortcut").toObject();
                            if (shortcut.contains("Key"))
                                shortcutString = shortcut.value("Key").toString();
                            if (shortcut.contains("VirtualKey"))
                                shortcutVirtualKey = shortcut.value("VirtualKey").toInt();
                        }

                        // Sound collection
                        if (item.contains("Sound Collection"))
                        {
                            qDebug() << "uuuuh? DuckerZ";
                            if (item.value("Sound Collection").isObject())
                            {
                                qDebug() << "this is an object forsenE, so new file format forsenE";
                                QJsonObject soundCollection = item.value("Sound Collection").toObject();


                                // Have to use traditional iterators because auto doesn't allow to use key DansGame
                                for (QJsonObject::iterator it = soundCollection.begin(); it!= soundCollection.end(); it++)
                                {
                                    QJsonObject settings;
                                    QString fileName;
                               //     if (ok170) // if ver > 1.7.0 we need to go down the arborescence (forsen2)
                                  //  {
                                        QJsonObject subObject = it.value().toObject();
                                        fileName = subObject.keys().at(0);
                                        settings = subObject.value(fileName).toObject();
                                    //}
//                                    else // < 1.7.0
//                                    {
//                                        fileName = it.key();
//                                        settings = it.value().toObject();
//                                    }



                                    float mainVolume = 1.0;
                                    float vacVolume = 1.0;


                                    if (settings.contains("Main Volume"))
                                        mainVolume = static_cast<float>(settings.value("Main Volume").toInt()/100.0);
                                    if (settings.contains("VAC Volume"))
                                        vacVolume  = static_cast<float>(settings.value("VAC Volume").toInt()/100.0);
                                    // SFX
                                    LIDL::SFX sfx;
                                    if (settings.contains("SFX Flags"))
                                        sfx.flags = static_cast<LIDL::SFX_TYPE>(settings.value("SFX Flags").toInt());

                                    if (settings.contains("SFX"))
                                    {
                                        QJsonObject sfx_obj =  settings.value("SFX").toObject();
                                        if (sfx_obj.contains("Distortion"))
                                        {
                                            QJsonObject distObj = sfx_obj.value("Distortion").toObject();
                                            for (QJsonObject::iterator l = distObj.begin(); l!= distObj.end();l++)
                                            {
                                                if (l.key() == "Cutoff")
                                                    sfx.distortion.fPreLowpassCutoff = static_cast<float>(l.value().toInt());
                                                if (l.key() =="EQBandwidth")
                                                    sfx.distortion.fPostEQBandwidth = static_cast<float>(l.value().toInt());
                                                if (l.key() =="EQCenterFrequency")
                                                    sfx.distortion.fPostEQCenterFrequency = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Edge")
                                                    sfx.distortion.fEdge = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Gain")
                                                    sfx.distortion.fGain=  static_cast<float>(l.value().toInt());
                                            }
                                        }
                                        if (sfx_obj.contains("Chorus"))
                                        {
                                            QJsonObject chorusObj = sfx_obj.value("Chorus").toObject();
                                            for (QJsonObject::iterator l = chorusObj.begin(); l!= chorusObj.end();l++)
                                            {
                                                if (l.key() == "Delay")
                                                    sfx.chorus.fDelay = static_cast<float>(l.value().toInt());
                                                if (l.key() == "Depth")
                                                    sfx.chorus.fDepth = static_cast<float>(l.value().toInt());
                                                if (l.key() == "Feedback")
                                                    sfx.chorus.fFeedback = static_cast<float>(l.value().toInt());
                                                if (l.key() == "Frequency")
                                                    sfx.chorus.fFrequency = static_cast<float>(l.value().toInt());
                                                if (l.key() == "WetDryMix"  || l.key() == "Wet Dry Mix")
                                                    sfx.chorus.fWetDryMix = static_cast<float>(l.value().toInt());
                                                if (l.key() == "Phase")
                                                    sfx.chorus.lPhase = static_cast<int>(l.value().toInt());
                                                if (l.key() == "Waveform")
                                                    sfx.chorus.lWaveform =  static_cast<int>(l.value().toInt());
                                            }

                                        }
                                        if (sfx_obj.contains("Echo"))
                                        {
                                            QJsonObject obj = sfx_obj.value("Echo").toObject();
                                            for (QJsonObject::iterator l = obj.begin(); l!= obj.end();l++)
                                            {
                                                if (l.key() =="Feedback" )
                                                    sfx.echo.fFeedback = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Left Delay" )
                                                    sfx.echo.fLeftDelay = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Right Delay" )
                                                    sfx.echo.fRightDelay = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Wet Dry Mix" )
                                                    sfx.echo.fWetDryMix = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Swap" )
                                                    sfx.echo.lPanDelay = static_cast<bool>(l.value().toInt());
                                            }

                                        }

                                        if (sfx_obj.contains("Flanger"))
                                        {
                                            QJsonObject obj = sfx_obj.value("Flanger").toObject();
                                            for (QJsonObject::iterator l = obj.begin(); l!= obj.end();l++)
                                            {
                                                if (l.key() =="Delay")
                                                    sfx.flanger.fDelay = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Depth")
                                                    sfx.flanger.fDepth = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Feedback" )
                                                    sfx.flanger.fFeedback = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Frequency" )
                                                    sfx.flanger.fFrequency = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Phase" )
                                                    sfx.flanger.lPhase = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Waveform" )
                                                    sfx.flanger.lWaveform = static_cast<bool>(l.value().toInt());
                                            }
                                        }
                                        if (sfx_obj.contains("Compressor"))
                                        {
                                            QJsonObject obj = sfx_obj.value("Compressor").toObject();
                                            for (QJsonObject::iterator l = obj.begin(); l!= obj.end();l++)
                                            {
                                                if (l.key() =="Attack")
                                                    sfx.compressor.fAttack = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Gain")
                                                    sfx.compressor.fGain = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Predelay")
                                                    sfx.compressor.fPredelay = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Ratio")
                                                    sfx.compressor.fRatio = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Release")
                                                    sfx.compressor.fRelease = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Threshold")
                                                    sfx.compressor.fThreshold = static_cast<float>(l.value().toInt());
                                            }
                                        }

                                        if (sfx_obj.contains("Gargle"))
                                        {
                                            QJsonObject obj = sfx_obj.value("Gargle").toObject();
                                            for (QJsonObject::iterator l = obj.begin(); l!= obj.end();l++)
                                            {
                                                if (l.key() =="Rate")
                                                    sfx.gargle.dwRateHz = static_cast<float>(l.value().toInt());
                                                if (l.key() =="Waveform")
                                                    sfx.gargle.dwWaveShape = static_cast<bool>(l.value().toBool());
                                            }
                                        }
                                    }
                                     qDebug() << "[SaveController::OpenSaveFile] Profile:" << profileName;
                                      qDebug() << "[SaveController::OpenSaveFile] File:" << fileName;
                                    fileArray.append(new LIDL::SoundFile(fileName,
                                                                         mainVolume,
                                                                         vacVolume,sfx));
                                }
                            }

                            //Else If this is an array than we are on the old save system without the volumes
                            else if (item.value("Sound Collection").isArray())
                            {
                                //qDebug() << "Old file detected forsenBee";
                                QJsonArray soundArray = item.value("Sound Collection").toArray();
                                // We iterate over the sound files and add them to the array
                                // (default volume is 100%).


                                for (auto j: soundArray)
                                {
                                    fileArray.append(new LIDL::SoundFile(j.toString(),LIDL::Controller::SettingsController::GetInstance()->GetDefaultMainVolume(),LIDL::Controller::SettingsController::GetInstance()->GetDefaultVacVolume())  );
                                }
                            } // else if sound collection is array( rly fucking old format)


                        }


                        /***************************************************
                                                   CREATING THE WRAPPERS
                            ****************************************************/

                        wrappers.append( std::make_shared<SoundWrapper>(fileArray,
                                                        playbackmode,
                                                        QKeySequence(shortcutString),
                                                        shortcutVirtualKey,
                                                        -1,
                                                        -1,
                                                        nullptr));



                    } // end for auto wrapper
                    // Once the wrappers are created we need to create the profile i guess PepeS

                } // end if profile contains sounds
                // Now we can add the profile i guess PepeS
                Profile::Builder temp;
                temp.setName(profileName);
                for ( QString i: exes)
                {
                    temp.addExe(i);
                }
                temp.setSounds(wrappers);
                LIDL::Controller::ProfileController::GetInstance()->AddProfile(temp.Build());

            } // end for each profile


        } // end if contain profiles

    }
    else
    {
        // parse the old one (1.8.0) era
    }

    // once parse, we set the device on the soundboard
    // and we swap to default profile, it should re-add the sound with the correct device

}

void SaveController::SaveAs(QString fileName)
{
    // if filename is empty we show the prompt
    if (fileName.isEmpty())
    {
        fileName  = QFileDialog::getSaveFileName(nullptr,
                                                     QObject::tr("Save Soundboard As.."),
                                                     LIDL::Controller::SettingsController::GetInstance()->GetDefaultSoundboardFolder() ,
                                                     QObject::tr("LIDL JSON file(*.lidljson)"));
    }
    //  fileName.append(".lidljson");
    QJsonObject save = GenerateSaveFile();
    QJsonDocument *doc = new QJsonDocument(save);
    QString jsonString = doc->toJson(QJsonDocument::Indented);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {

        QMessageBox::information(nullptr, QObject::tr("Unable to open file"), file.errorString());
        return;
    }
    else
    {
        this->_saveName = fileName;


        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << jsonString.toUtf8();
        // lidljson detected is used to add the file in the recent file thing
     //   emit lidlJsonDetected(QFileInfo(file));
        file.close();
        // Saving Soundboard state in the SettingsController object
       // emit SaveSoundboardState();
       // this->SetStatusTextEditText("Succesfully saved file: " + fileName);
    }
}


QJsonObject SaveController::GenerateSaveFile() const
{
    QJsonObject obj;

    // inserting version
    // defined in qmake (.pro) file
    obj.insert("Version", VER_STRING);

    QJsonObject settings;
    settings.insert("Main Output Device", this->_mainOutputDevice);
    settings.insert("VAC Output Device",  this->_vacOutputDevice);
    settings.insert("Show Flags", static_cast<int>(LIDL::Controller::SettingsController::GetInstance()->getShowFlags()));

    QJsonObject stopSoundKey;
    stopSoundKey.insert("Key Name",this->_stopKeyName);
    stopSoundKey.insert("VirtualKey" ,static_cast<int>(this->_stopVirtualKey));

    settings.insert("Stop Sound Key",stopSoundKey);

    obj.insert("Settings", settings);

    QJsonArray profiles;

    for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
        profiles.append(i->GetProfileAsObject());

    }

    obj.insert("Profiles",profiles);
    return obj;
   // qDebug() << QJsonDocument(obj).toJson();
}

void SaveController::SetMainOutputDevice(QString device)
{
    this->_mainOutputDevice = device;
}

void SaveController::SetVacOutputDevice(QString device)
{
    this->_vacOutputDevice = device;
}

QString SaveController::GetMainOutputDevice() const
{
    return this->_mainOutputDevice;
}

QString SaveController::GetVacOutputDevice() const
{
    return this->_vacOutputDevice;
}


} // end namespace controller

} // end namespace LIDL
