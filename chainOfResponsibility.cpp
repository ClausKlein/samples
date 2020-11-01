//
// Chain of Responsibility
//

#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Photo
{
public:
    Photo(string s) : mTitle(std::move(s))
    {
        cout << "Processing " << mTitle << " ...\n";
    }

private:
    string mTitle;
};

class PhotoProcessor
{
public:
    PhotoProcessor() = default;

public:
    void process(Photo &p)
    {
        processImplementation(p);
        if (mNextProcessor) {
            mNextProcessor->process(p);
        }
    }

    virtual ~PhotoProcessor() = default;

    void setNextProcessor(PhotoProcessor *p) { mNextProcessor = p; }

protected:
    virtual void processImplementation(Photo &a) = 0;

private:
    PhotoProcessor *mNextProcessor{nullptr};
};

class Scale : public PhotoProcessor
{
public:
    enum SCALE
    {
        S50,
        S100,
        S200,
        S300,
        S500
    };
    Scale(SCALE s) : mSCALE(s) {}

private:
    void processImplementation(Photo &a) override
    {
        cout << "Scaling photo X " << mSCALE << endl;
    }

    SCALE mSCALE;
};

class RedEye : public PhotoProcessor
{
private:
    void processImplementation(Photo &a) override
    {
        cout << "Removing red eye\n";
    }
};

class Filter : public PhotoProcessor
{
private:
    void processImplementation(Photo &a) override
    {
        cout << "Applying filters\n";
    }
};

class ColorMatch : public PhotoProcessor
{
private:
    void processImplementation(Photo &a) override
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

auto main() -> int
{
    Photo p("Y2013 Photo");

    processPhoto(p);

    return 0;
}
