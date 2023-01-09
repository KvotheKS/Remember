#include "FpsPrinter.h"
#include "Text.h"

FpsPrinter::FpsPrinter(GameObject& associated)
    : GameObject(associated), count(0), dt(0.0f)
{}

void FpsPrinter::Update(float dt)
{
    this->count++;
    this->dt += dt;
    if(this->dt >= 1.0f)
    {
        Text* txt = (Text*)this->associated.GetComponent("Text");
        txt->SetText(std::to_string(this->count));
        this->count = 0;
        this->dt -= 1.0f;
    }
}

void FpsPrinter::Render()
{}

bool FpsPrinter::Is(std::string type)
{ return type == "FpsPrinter"; }

bool FpsPrinter::Is(C_ID type)
{ return type == C_ID::FpsPrinter; }