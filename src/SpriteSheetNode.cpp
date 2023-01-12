#include "SpriteSheetNode.h"

/*
    detalhe extra: clip_line é o filete onde a animação está na texture, ou seja, 
    SSNode serve para usarmos arquivos de animação como os do mk3
*/
SSNode::SSNode(std::string file, SDL_Rect clip_line, int frameCount, float frameTime, Vec2 scale,bool reverse, bool flipped)
    : AnimNode(file, frameCount,frameTime, scale, reverse, flipped)
{ this->clip_line = clip_line; SetFrame(0); }

void SSNode::SetFrame(int id)
{
    currentFrame = id;
    clipRect = clip_line;
    clipRect.w /= frameCount;
    clipRect.x += clipRect.w * id;
}

int SSNode::GetWidth()
{ return (clip_line.w / frameCount) * scale.x; }

int SSNode::GetHeight()
{ return clip_line.h * scale.y; }