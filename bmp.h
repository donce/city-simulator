
#ifndef INC_BMP_H
#define INC_BMP_H

//--------------------------------------------------------------------------- 

class AUX_RGBImageRec {
   void convertBGRtoRGB();
 public:
   byte *data;
   DWORD sizeX;
   DWORD sizeY;
   bool NoErrors;
   AUX_RGBImageRec(): NoErrors(false), data(0) {};
   AUX_RGBImageRec(const char *FileName);
   ~AUX_RGBImageRec();
   bool loadFile(const char *FileName);
   friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};



#endif
