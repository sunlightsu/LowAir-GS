#include "FusionConfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

FusionConfig FusionConfig::defaultConfig() {
    FusionConfig cfg;
    cfg.sceneName       = "demo06_default";
    cfg.coordinateFrame = "LOCAL_SCENE";
    cfg.unit            = "meter";
    return cfg;
}
