//
//  ArtnetControl.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "ArtnetControl.hpp"

ArtnetControl::ArtnetControl(MidiControl *mc):_MC(mc)
{
    _GUI = new AnimatorGUI(ofRectangle(200,200,500,500));
    _GUI->createAnimationGUI(LedAnimator::ANIMATION_COUNT,LedAnimator::ENABLE_COUNT);
    ofAddListener(_GUI->animationPressed, this, &ArtnetControl::guiAnimationPressed);
    ofAddListener(_GUI->enablePressed, this, &ArtnetControl::guiEnablePressed);
    _preAnimator = new LedAnimator(_MC);
    _liveAnimator = new LedAnimator(_MC);
    loadNodes();
    //init the gui
    _GUI->init();
    _test = false;
    
    ofAddListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
}

ArtnetControl::~ArtnetControl()
{
    // blackout all
    ofRemoveListener(_GUI->animationPressed, this, &ArtnetControl::guiAnimationPressed);
    ofRemoveListener(_GUI->enablePressed, this, &ArtnetControl::guiEnablePressed);
    ofRemoveListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
    clearNodes();
    delete _preAnimator;
    delete _liveAnimator;
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        delete _preSegments[i];
        delete _liveSegments[i];
    }
}

void ArtnetControl::clearNodes()
{
    for(int i = 0;i < _nodes.size();i++)
    {
        delete _nodes[i];
    }
    _nodes.clear();
}

void ArtnetControl::loadNodes()
{
    clearNodes();
    //important for the mapping a segment cant go from one univcerse to another,
    //otherwise its is getting complicated!!!!
    //load nodes from xml
    ofxXmlSettings nodes("nodes.xml");
    int count = nodes.getNumTags("node");
    for (int i = 0; i < count; i++)
    {
        nodes.pushTag("node",i);
        Node *n = new Node();
        n->ip = nodes.getValue("ip", "127.0.0.1");
        n->name = nodes.getValue("name", "A1");
        cout << n->ip << " " << n->name << endl;
        //string ip = "192.168.12."+ ofToString(200+i);
        n->artnet.begin(n->ip.c_str());
        //fill universes black
        for (int u = 0; u < 8; u++)
        {
            for (int l = 0; l < 512; l++)
            {
                n->universes[u][l] = 0;
            }
        }
        _nodes.push_back(n);
        nodes.popTag();
    }
    // now load the segments
    _preSegments.clear();
    _liveSegments.clear();
    ofxXmlSettings segments("segments.xml");
    int numSegments = segments.getNumTags("segment");
    /*	<id>0</id>
     <node>0</node>
     <universe>0</universe>
     <beginPixel>0</beginPixel>
     <length>150</length>
     */
    for (int i = 0; i < numSegments; i++)
    {
        segments.pushTag("segment",i);
        
        //int universe,int begin,int end, Node * node
        //or settings based on an xml from the settings
        int node = segments.getValue("node", 0);
        int universe = segments.getValue("universe", 0);
        //int nodeID,int universe,int beginLed,int endLed,int segmentID
        int begin = segments.getValue("beginPixel", 0);
        int length = segments.getValue("length", 150);//ofRandom(150); // should be max 150
        Segment *newSegP = new Segment(node,universe,begin,length,i);
        Segment *newSegL = new Segment(node,universe,begin,length,i);
        _preSegments.push_back(newSegP);
        _liveSegments.push_back(newSegL);
        segments.popTag();
    }
    
    // update the ledanimator size
    //whiteout
    //also create the preview images
    _preIMG.allocate(150, _preSegments.size(),OF_IMAGE_COLOR); // max of 510 channels
    _liveIMG.allocate(150, _preSegments.size(),OF_IMAGE_COLOR);
    _preIMG.setColor(ofColor(0,0,0));
    _liveIMG.setColor(ofColor(0,0,0));
    // fill the images black!!!
    _preAnimator->setAnimation(2);
    _preAnimator->setSegmentSize(_preSegments.size());
    _liveAnimator->setAnimation(2);
    _liveAnimator->setSegmentSize(_liveSegments.size());
    //setup the mesh
    _GUI->getMesh().clear();
    _GUI->getMesh().setMode(OF_PRIMITIVE_LINES);
    for (int i = 0; i < 16; i++)
    {
        float x = 0.25 + (i * (0.5/16.));
        float y = 0.10;
        _GUI->getMesh().addVertex(ofVec2f(x,y));
        _GUI->getMesh().addTexCoord(ofVec2f(0,i));
        y = 1-y;
        _GUI->getMesh().addVertex(ofVec2f(x,y));
        _GUI->getMesh().addTexCoord(ofVec2f(150,i));
    }
    
    // now the triangles left and right, as losse lines
    //left and right
    for (int s = 0; s < 2; s++)
    {
        ofVec2f center = ofVec2f(0.125 + (s * 0.75),0.5);
        for (int d = 0; d < 3; d++)//every driehoek
        {
            float rad = 0.66 - (d * (0.66/3));//inverse great to small
            
            _GUI->getMesh().addVertex(ofVec2f(center.x,center.y - center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,0 + (d*3)));
            _GUI->getMesh().addVertex(ofVec2f(center.x + 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,0 + (d*3)));
            
            _GUI->getMesh().addVertex(ofVec2f(center.x + 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,1 + (d*3)));
            _GUI->getMesh().addVertex(ofVec2f(center.x - 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,1 + (d*3)));
            
            _GUI->getMesh().addVertex(ofVec2f(center.x - 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,2 + (d*3)));
            _GUI->getMesh().addVertex(ofVec2f(center.x,center.y - center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,2));
        }
    }
    
}

void ArtnetControl::update()
{
    // update artnet based on the selected sce or use the hottbutton function
    // which is overriding the actual state
    // also use the delta time and next beat from the mmidi controler
    // to run through the step sequences from the on off function

    if(_MC->getBeat() == true)
    {
        _preAnimator->addStep();
        _liveAnimator->addStep();
    }
    //fill all nodes by segment
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        //here i can read the old and do a fading or particles and decrreasing the rray before writing a new
         ///do something
        //_preSegments[i]->getArray() decrease value of 5 and let stay at zero or decrease it and sinewave or beat conrtroled shift of a static sinfunction array (pixel shifting )
        if(_test == 0) // crazy blink blik
        {
            //holly shit function
            
            for (int e=0; e < _preSegments[i]->getLength() - 3; e++)
            {
                //wegfaden
                float curve = 0.96 * (ofNoise(ofGetElapsedTimef() + i*3 + floor(e/3))) * 2; // giv it a offset
                //oder funky
                _preSegments[i]->getArray()[e] *= curve;//maybe another curve
                _liveSegments[i]->getArray()[e] *= curve;//maybe another curve
                
                //hier kann zufaellig noch nen neur dot entstehen wie ein stern,bling bling
                int isPixelID = e/3;
                // and shift them by pixel
                _preSegments[i]->getArray()[e + 3] = _preSegments[i]->getArray()[e];
            }
            if (_MC->getBeat())
            {
                //zum schluss fuege ein pixel random zu
                int selectPixel = ofRandom(_preSegments[i]->getLength()/3);
                _preSegments[i]->setPixel(selectPixel,255,255,255);
//                _preSegments[i]->setPixel(selectPixel+1,255,255,255);
//                _preSegments[i]->setPixel(selectPixel+2,255,255,255);
//                _preSegments[i]->setPixel(selectPixel+3,255,255,255);
//                _preSegments[i]->setPixel(selectPixel+4,255,255,255);
            }
        }
        else if(_test == 1) /// another function
        {
            
            for (int e=0; e < _preSegments[i]->getLength(); e++)
            {
                //wegfaden
                float curve = 0.96; // giv it a offset
                _preSegments[i]->getArray()[e] *= curve;//maybe another curve
                _liveSegments[i]->getArray()[e] *= curve;//maybe another curve
                //hier kann zufaellig noch nen neur dot entstehen wie ein stern,bling bling
                //int isPixelID = e/3;
            }
            if (_MC->getBeat())
            {
                //zum schluss fuege ein pixel random zu
                int selectPixel = ofRandom(_preSegments[i]->getLength()/3);
                _preSegments[i]->setPixel(selectPixel,255,255,255);
                _preSegments[i]->setPixel(selectPixel+1,255,255,255);
                _preSegments[i]->setPixel(selectPixel+2,255,255,255);
                _preSegments[i]->setPixel(selectPixel+3,255,255,255);
                _preSegments[i]->setPixel(selectPixel+4,255,255,255);
            }
        }
        else if(_test == 2) /// inverse by beat
        {
            if (_MC->getBeat())
            {
                for (int e=0; e < _preSegments[i]->getPixelLength(); e++)
                {

                    
                    float curve = 0.96; // giv it a offset
                    _preSegments[i]->getPixel(e)[0] = 255 -_preSegments[i]->getPixel(e)[0];//maybe another curve
                    _preSegments[i]->getPixel(e)[1] = 255 -_preSegments[i]->getPixel(e)[1];//maybe another curve
                    _preSegments[i]->getPixel(e)[2] = 255 -_preSegments[i]->getPixel(e)[2];//maybe another curve
                    _liveSegments[i]->getPixel(e)[0] = 255 -_liveSegments[i]->getPixel(e)[0];//maybe another curve
                    _liveSegments[i]->getPixel(e)[1] = 255 -_liveSegments[i]->getPixel(e)[1];//maybe another curve
                    _liveSegments[i]->getPixel(e)[2] = 255 -_liveSegments[i]->getPixel(e)[2];//maybe another curve
                    //hier kann zufaellig noch nen neur dot entstehen wie ein stern,bling bling
                    //int isPixelID = e/3;
                }
            }
        }
        else if(_test == 3) /// the original function
        {
            _preAnimator->animationToArray(i, _preSegments[i]->getArray(), _preSegments[i]->getLength(), 0);
            _liveAnimator->animationToArray(i, _liveSegments[i]->getArray(), _liveSegments[i]->getLength(), 0);
        }
    }
    // now updaste the arrays to the visualizer
    writeSegmentsToImage(_preSegments, _preIMG);
    writeSegmentsToImage(_liveSegments, _liveIMG);
    
    sendToNodes();
}

void ArtnetControl::writeSegmentsToImage(vector<Segment*> seg, ofImage& img)
{
    /// every segments get his own line
    for (int i = 0; i < seg.size(); i++)
    {
        int start = seg[i]->getBegin(); // is not in led is in array
        for (int l = 0; l < seg[i]->getLength()/3; l++)
        {
            int x = (start / 3) + l;
            int r = seg[i]->getArray()[start + (l*3) + 0];
            int g = seg[i]->getArray()[start + (l*3) + 1];
            int b = seg[i]->getArray()[start + (l*3) + 2];
            ofColor c = ofColor(r,g,b);//seg[i]->getArray()
            img.setColor(x, i, c);
        }
    }
    img.update();
}


void ArtnetControl::drawGui()
{
    // draw som buttons from a controler gui class
    _GUI->draw(_preIMG,_liveIMG);
}


void ArtnetControl::sendToNodes()
{
    //copy segment arrays to nodes
    int nodeID,universe,first,last = 0;
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        //each segment is drawn by the Segment abstraction
        
        nodeID = _liveSegments[i]->getNodeID();
        universe = _liveSegments[i]->getUniverse();
        first = _liveSegments[i]->getBegin();
        last = _liveSegments[i]->getEnd();
        for (int cell = first; cell < last; cell++)
        {
            //get the array where to write which function
            //and write it to the nodes
            u_int64_t data = _preSegments[i]->getArray()[first+cell];
//            u_int64_t data = _liveSegments[i]->getArray()[first+cell];
            // mach was damit
            _nodes[nodeID]->universes[universe][cell] = data;//_liveSegments[i]->getArray()[first+cell];
        }
    }
    //then send the nodes
    //basic sending
    //int universe = 0;
    //int chnCount = 450;//33 leds * 3
    //artnet.send(universe1,universe,chnCount);
    for (int n = 0; n < _nodes.size(); n++)
    {
        for (int u = 0; u < 8; u++)
        {
            // we set the length to 3x170 = 510 has to been tested
            _nodes[n]->artnet.send(_nodes[n]->universes[u],u,510);
        }
    }
}

void ArtnetControl::guiAnimationPressed(int &buttonid)
{
    setAnimationPreview(buttonid);
    //only by now is later automated and hidden
    setAnimationLive(buttonid);
}

void ArtnetControl::guiEnablePressed(int &buttonid)
{
    setEnablePreview(buttonid);
    //only by now is later automated and hidden
    setEnableLive(buttonid);
}


void ArtnetControl::specialFunction(int id)
{
    if(id == 0)
    {
        //flash everything in white
    }
    else if(id  == 1)
    {
        //invert the output colors
    }
}

void ArtnetControl::keyPressed(ofKeyEventArgs &key)
{
    _test++;
    _test = _test%4;
    cout << key.keycode << endl;
    
}
