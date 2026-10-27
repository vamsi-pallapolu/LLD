#include <iostream>
#include <string>

class AudioOutput{
public:
    // Asking compiler to use default implementation of destructor
    virtual ~AudioOutput() = default; 

    virtual void play(const std::string & song)= 0; // pure virtual function
        
};

// Each device decide how for itself
class PhoneSpeaker: public AudioOutput{
    public:
        void play(const std::string & song) override{
            std::cout << "Playing " << song << " through Phone Speaker" << std::endl;
        }
};

class WiredHeadset: public AudioOutput{
public:
    void play(const std::string &song) override{
        std::cout << "Playing " << song << " through Wired Headset" << std::endl;
    }
};

class BluetoothHeadset: public AudioOutput{

    public:
        void play(const std::string& song) override{
            std::cout << "Playing " << song << " through Bluetooth Headset" << std::endl;
        }
};

class MusicPlayer{
private:
 AudioOutput* _output;
public:
    explicit MusicPlayer(AudioOutput& output):_output{&output}{

    }

    void connect(AudioOutput &output){
        _output = &output;
    }

    void play(const std::string & song){
        _output->play(song);
    }
};

int main(){

    WiredHeadset wiredhs;
    PhoneSpeaker ps;
    MusicPlayer player(ps);
    player.play("Thelusa Manasa Song");

    player.connect(wiredhs);
    player.play("Ranu Bombay ki ranu");


    return 0;
}