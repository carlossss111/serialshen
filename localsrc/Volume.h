
class Volume{
public:
    //set the volume as a percentage
    void setVolume(int applicationNum, int percentVolume);

    //get the application identifier
    int getApplicationNum(const char * applicationName) const;
};