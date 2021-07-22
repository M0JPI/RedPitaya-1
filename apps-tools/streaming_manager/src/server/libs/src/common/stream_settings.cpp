#include <iostream>
#include <fstream>
#include "stream_settings.h"
#include "json/json.h"

CStreamSettings::CStreamSettings(){
    m_host = "";
    m_port = "";
    m_protocol = TCP;
    m_samples = -1;
    m_format = WAV;
    m_type = RAW;
    m_channels = CH1;
    m_res = BIT_8;
    m_decimation = 1;

#ifndef Z20
    m_attenuator = A_1_1;
    m_calib = false;
#endif

#ifdef Z20_250_12
    m_ac_dc = AC;
#endif
    reset();
}

void CStreamSettings::reset(){
    m_Bhost =
    m_Bport =
    m_Bprotocol =
    m_Bsamples =
    m_Bformat =
    m_Btype =
    m_Bchannels =
    m_Bres =
    m_Bdecimation = false;

#ifndef Z20
    m_Battenuator = false;
    m_Bcalib = false;
#endif

#ifdef Z20_250_12
    m_Bac_dc = false;
#endif
}

bool CStreamSettings::isSetted(){
    bool res = true;
    res =   m_Bhost &&
            m_Bport &&
            m_Bprotocol &&
            m_Bsamples &&
            m_Bformat &&
            m_Btype &&
            m_Bchannels &&
            m_Bres &&
            m_Bdecimation;

#ifndef Z20
    res = res && m_Battenuator && m_Bcalib;
#endif

#ifdef Z20_250_12
    res = res && m_Bac_dc;
#endif

    return res;
}

bool CStreamSettings::writeToFile(string _filename){
    if (isSetted()){
        Json::Value root;
        root["host"] = getHost();
        root["port"] = getPort();
        root["protocol"] = getProtocol();
        root["samples"] = getSamples();
        root["format"] = getFormat();
        root["type"] = getType();
        root["channels"] = getChannels();
        root["resolution"] = getResolution();
        root["decimation"] = getDecimation();
#ifndef Z20
        root["attenuator"] = getAttenuator();
        root["calibration"] = getCalibration();
#endif

#ifdef Z20_250_12
        root["coupling"] = getAC_DC();
#endif
        Json::StreamWriterBuilder builder;
        const std::string json_file = Json::writeString(builder, root);
        ofstream file(_filename , 	ios::out | ios::trunc);
        if (!file) {
            std::cerr << "file open failed: " << std::strerror(errno) << "\n";
            return false;
        }
        file << json_file;
        return true;
    }
    return false;
}

auto CStreamSettings::readFromFile(string _filename) -> bool {

    Json::Value root;
    std::ifstream file(_filename , 	ios::in);
    if (!file) {
        std::cerr << "file open failed: " << std::strerror(errno) << "\n";
        return false;
    }

    reset();
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, file, &root, &errs)) {
        std::cerr << "[CStreamSettings] Error parse json" << errs << std::endl;
        return false;
    }
    if (root.isMember("host"))
        setHost(root["host"].asString());
    if (root.isMember("port"))
        setPort(root["port"].asString());
    if (root.isMember("protocol"))
        setProtocol(static_cast<Protocol>(root["protocol"].asInt()));
    if (root.isMember("samples"))
        setSamples(root["samples"].asInt());
    if (root.isMember("format"))
        setFormat(static_cast<DataFormat>(root["format"].asInt()));
    if (root.isMember("type"))
        setType(static_cast<DataType>(root["type"].asInt()));
    if (root.isMember("channels"))
        setChannels(static_cast<Channel>(root["channels"].asInt()));
    if (root.isMember("resolution"))
        setResolution(static_cast<Resolution>(root["resolution"].asInt()));
    if (root.isMember("decimation"))
        setDecimation(root["decimation"].asUInt());
#ifndef Z20
    if (root.isMember("attenuator"))
        setAttenuator(static_cast<Attenuator>(root["attenuator"].asInt()));
    if (root.isMember("calibration"))
        setCalibration(root["calibration"].asBool());
#endif

#ifdef Z20_250_12
    if (root.isMember("coupling"))
        setAC_DC(static_cast<AC_DC>(root["coupling"].asInt()));
#endif
    return isSetted();

}



void CStreamSettings::setHost(string _host){
    m_host  = _host;
    m_Bhost = true;
}

string CStreamSettings::getHost(){
    return m_host;
}

void CStreamSettings::setPort(string _port){
    m_port  = _port;
    m_Bport = true;
}

string CStreamSettings::getPort(){
    return m_port;
}

void  CStreamSettings::setProtocol(CStreamSettings::Protocol _protocol){
    m_protocol  = _protocol;
    m_Bprotocol = true;
}

CStreamSettings::Protocol CStreamSettings::getProtocol(){
    return m_protocol;
}

void CStreamSettings::setSamples(int32_t _samples){
    m_samples  = _samples;
    m_Bsamples = true;
}

int32_t CStreamSettings::getSamples(){
    return  m_samples;
}

void CStreamSettings::setFormat(CStreamSettings::DataFormat _format){
    m_format = _format;
    m_Bformat = true;
}

CStreamSettings::DataFormat CStreamSettings::getFormat(){
    return m_format;
}

void CStreamSettings::setType(DataType _type){
    m_type = _type;
    m_Btype = true;
}

CStreamSettings::DataType CStreamSettings::getType(){
    return m_type;
}

void CStreamSettings::setChannels(CStreamSettings::Channel _channels){
    m_channels = _channels;
    m_Bchannels = true;
}

CStreamSettings::Channel CStreamSettings::getChannels(){
    return m_channels;
}

void CStreamSettings::setResolution(Resolution _resolution){
    m_res = _resolution;
    m_Bres = true;
}

CStreamSettings::Resolution CStreamSettings::getResolution(){
    return m_res;
}

void CStreamSettings::setDecimation(uint32_t _decimation){
    m_decimation = _decimation;
    m_Bdecimation = true;
}

uint32_t CStreamSettings::getDecimation(){
    return m_decimation;
}

#ifndef Z20
void CStreamSettings::setAttenuator(CStreamSettings::Attenuator _attenuator){
    m_attenuator = _attenuator;
    m_Battenuator = true;
}

CStreamSettings::Attenuator CStreamSettings::getAttenuator(){
    return m_attenuator;
}

void CStreamSettings::setCalibration(bool _calibration){
    m_calib = _calibration;
    m_Bcalib = true;
}

bool CStreamSettings::getCalibration(){
    return m_calib;
}
#endif

#ifdef Z20_250_12
void CStreamSettings::setAC_DC(CStreamSettings::AC_DC _value){
    m_ac_dc = _value;
    m_Bac_dc = true;
}

CStreamSettings::AC_DC CStreamSettings::getAC_DC(){
    return m_ac_dc;
}
#endif
