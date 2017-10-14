//
// Chain of Responsibility
//

#include <iostream>
#include <string>
using namespace std;


class Photo
{
public:
    Photo(string s) : mTitle(s)
    {
        cout << "Processing " << mTitle << " ...\n";
    }

private:
    string mTitle;
};


class PhotoProcessor
{
public:
    PhotoProcessor() : mNextProcessor(0) { }

public:
    void process(Photo &p)
    {
        processImplementation(p);
        if (mNextProcessor)
        {
            mNextProcessor->process(p);
        }
    }

    virtual ~PhotoProcessor() { }

    void setNextProcessor(PhotoProcessor *p)
    {
        mNextProcessor = p;
    }

protected:
    virtual void processImplementation(Photo &a) = 0;

private:
    PhotoProcessor *mNextProcessor;
};


class Scale : public PhotoProcessor
{
public:
    enum SCALE { S50, S100, S200, S300, S500 };
    Scale(SCALE s) : mSCALE(s) { }

private:
    void processImplementation(Photo &a)
    {
        cout << "Scaling photo X " << mSCALE << endl;
    }

    SCALE mSCALE;
};


class RedEye : public PhotoProcessor
{
private:
    void processImplementation(Photo &a)
    {
        cout << "Removing red eye\n";
    }
};


class Filter : public PhotoProcessor
{
private:
    void processImplementation(Photo &a)
    {
        cout << "Applying filters\n";
    }
};


class ColorMatch : public PhotoProcessor
{
private:
    void processImplementation(Photo &a)
    {
        cout << "Matching colors\n";
    }
};


void processPhoto(Photo &photo)
{
    ColorMatch match;
    RedEye eye;
    Filter filter;
    Scale scale(Scale::S200);
    scale.setNextProcessor(&eye);
    eye.setNextProcessor(&match);
    match.setNextProcessor(&filter);
    scale.process(photo);
}


int main()
{
    Photo p("Y2013 Photo");

    processPhoto(p);

    return 0;
}

