//
//  AnimatorGUI.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#ifndef AnimatorGUI_hpp
#define AnimatorGUI_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ColorSelector.hpp"
#include "Button.hpp"
#include "RotarySlider.hpp"
#include "segmentSelector.hpp"
#include "LightControl.hpp"


#define SLIDERS 4

//#define COLLUMNEDIT1 0
//#define COLLUMNEDIT2 300
#define COLLUMNLIVE 700

class AnimatorGUI
{
public:
    /*
    struct Button
    {
        int id;
        string name;
        ofRectangle area;
        ofColor color;
        bool toggle;
        bool isPressed;
    };
    */
    AnimatorGUI(ofRectangle area);
    ~AnimatorGUI();
    
    void init();
    void createAnimationGUI(int animationCount);
    void draw(ofImage &pre, ofImage &live);
    
    ofMesh &getMesh(){return _realStructure;}; //here we can work on the mesh
    
    void mousePressed(ofMouseEventArgs &args);
    ofEvent<int>    curveAPressed;
    ofEvent<int>    curveBPressed;
    ofEvent<int>    patronPLAY;
    ofEvent<int>    patronEDIT;
    ofEvent<int>    patronPLAYSTEPPED;
    ofEvent<bool>   colorSelectPressed;
    
//    void setColor(string &c);
    
    ColorSelector colorselectorA;
    ColorSelector colorselectorB;

    ColorSelector &getColorselectorA(){return colorselectorA;};
    ColorSelector &getColorselectorB(){return colorselectorB;};

    float getSlidersAMapped(int id){return _slidersA[id]->getValueMapped();};
    float getSlidersBMapped(int id){return _slidersB[id]->getValueMapped();};

    RotarySlider *getCurveSlidersA(){return _slidersA[0];};
    RotarySlider *getCurveSlidersB(){return _slidersB[0];};
    RotarySlider *getTimeSliderA(){return _slidersA[1];}
    RotarySlider *getTimeSliderB(){return _slidersB[1];}
    RotarySlider *getFreqSliderA(){return _slidersA[2];}
    RotarySlider *getFreqSliderB(){return _slidersB[2];}
    RotarySlider *getDirSliderA(){return _slidersA[3];}
    RotarySlider *getDirSliderB(){return _slidersB[3];}
    
    segmentSelector segmenselectorA;
    segmentSelector segmenselectorB;
    
    segmentSelector &getSegmenselectorA(){return segmenselectorA;};
    segmentSelector &getSegmenselectorB(){return segmenselectorB;};
    
    LightControl lightcontrol;
    
    
    void editButtonPressed(int &id);
    void liveButtonPressed(int &id);
    
    //Positioning GUI
    int CollumnEdit1;
    int CollumnEdit2;
    int Collumnwidth;
    int Padding;
    int RowsEdit[5] = {50,200,280,380,520};
    int RowsLive[5] = {50,150,280,380,520};
    
private:

    ofRectangle         _drawArea;
    ofMesh              _realStructure;// the mesh to paint on
    vector<Button*>     _patLiveButtons;//the buttons to select a patron for playing or edit selectie
    vector<Button*>     _patEditButtons;//the buttons to select a patron for playing or edit selectie
    int                 _isPlay;
    int                 _isEdit;
    int                 _isPlayStepped;
    ofTrueTypeFont      _verdana;
    
    RotarySlider*   _slidersA[SLIDERS];
    RotarySlider*   _slidersB[SLIDERS];
};

#endif /* AnimatorGUI_hpp */
