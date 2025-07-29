#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ObjectNotation.h"

#define SHAPE_NONE      0
#define SHAPE_RECT      1
#define SHAPE_LINE      2
#define SHAPE_CIRCLE    3
#define SHAPE_SPLINE    4

typedef struct Shape {
    int Type;
    void* Memory;
    size_t ElementSize;
    void (*Update)(void*);
    void (*Render)(void*);
} Shape;

Shape Shape_New(int Type,void* Memory,size_t ElementSize,void (*Update)(void*),void (*Render)(void*)){
    Shape s;
    s.Type = Type;
    s.Memory = malloc(ElementSize);
    memcpy(s.Memory,Memory,ElementSize);
    s.Update = Update;
    s.Render = Render;
    return s;
}
void Shape_Free(Shape* s){
    if(s->Memory) free(s->Memory);
    s->Memory = NULL;
}

Vec2* Focused = NULL;
Rect ScreenRect;
TransformedView tv;
Vector Objects;

typedef struct SRect {
    Vec2 p;
    Vec2 e;
} SRect;

void SRect_Update(SRect* r){
    Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
    Vec2 l = (Vec2){ 20.0f,20.0f };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p;
	}
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->e;
	}
}
void SRect_Render(SRect* r){
    if(Overlap_Rect_Rect(ScreenRect,(Rect){ r->p,Vec2_Sub(r->e,r->p) })){
		Vec2 p = TransformedView_WorldScreenPos(&tv,r->p);
		Vec2 e = TransformedView_WorldScreenPos(&tv,r->e);
        Vec2 l = TransformedView_WorldScreenLength(&tv,(Vec2){ 20.0f,20.0f });
		RenderRectWire(p.x,p.y,e.x-p.x,e.y-p.y,RED,1.0f);
        p = Vec2_Sub(p,Vec2_Mulf(l,0.5f));
        e = Vec2_Sub(e,Vec2_Mulf(l,0.5f));
        RenderRect(p.x,p.y,l.x,l.y,GREEN);
        RenderRect(e.x,e.y,l.x,l.y,GREEN);
	}
}

typedef struct SLine {
    Vec2 p;
    Vec2 e;
} SLine;

void SLine_Update(SLine* r){
    Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
    Vec2 l = (Vec2){ 20.0f,20.0f };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p;
	}
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->e;
	}
}
void SLine_Render(SLine* r){
    if(Overlap_Rect_Rect(ScreenRect,(Rect){ r->p,Vec2_Sub(r->e,r->p) })){
		Vec2 p = TransformedView_WorldScreenPos(&tv,r->p);
		Vec2 e = TransformedView_WorldScreenPos(&tv,r->e);
        Vec2 l = TransformedView_WorldScreenLength(&tv,(Vec2){ 20.0f,20.0f });
		RenderLine(p,e,RED,1.0f);
        p = Vec2_Sub(p,Vec2_Mulf(l,0.5f));
        e = Vec2_Sub(e,Vec2_Mulf(l,0.5f));
        RenderRect(p.x,p.y,l.x,l.y,GREEN);
        RenderRect(e.x,e.y,l.x,l.y,GREEN);
	}
}

typedef struct SCircle {
    Vec2 p;
    Vec2 e;
} SCircle;

void SCircle_Update(SCircle* r){
    Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
    Vec2 l = (Vec2){ 20.0f,20.0f };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p;
	}
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->e;
	}
}
void SCircle_Render(SCircle* r){
    if(Overlap_Rect_Rect(ScreenRect,(Rect){ r->p,Vec2_Sub(r->e,r->p) })){
		Vec2 p = TransformedView_WorldScreenPos(&tv,r->p);
		Vec2 e = TransformedView_WorldScreenPos(&tv,r->e);
        Vec2 l = TransformedView_WorldScreenLength(&tv,(Vec2){ 20.0f,20.0f });
		
        float radius = Vec2_Mag(Vec2_Sub(e,p));
        RenderCircleWire(p,radius,RED,1.0f);
        p = Vec2_Sub(p,Vec2_Mulf(l,0.5f));
        e = Vec2_Sub(e,Vec2_Mulf(l,0.5f));
        RenderRect(p.x,p.y,l.x,l.y,GREEN);
        RenderRect(e.x,e.y,l.x,l.y,GREEN);
	}
}

typedef BCurve SSpline;

void SSpline_Update(SSpline* r){
    Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
    Vec2 l = (Vec2){ 20.0f,20.0f };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p1,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p1;
	}
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p2,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p2;
	}
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p3,Vec2_Mulf(l,0.5f)),l },p)){
	    Focused = &r->p3;
	}
}
void SSpline_Render(SSpline* r){
    Vec2 p1 = TransformedView_WorldScreenPos(&tv,r->p1);
	Vec2 p2 = TransformedView_WorldScreenPos(&tv,r->p2);
    Vec2 p3 = TransformedView_WorldScreenPos(&tv,r->p3);
    Vec2 l = TransformedView_WorldScreenLength(&tv,(Vec2){ 20.0f,20.0f });
	
    BCurve_Render((BCurve){ p1,p2,p3 },RED);
    RenderRect(p1.x-l.x*0.5f,p1.y-l.y*0.5f,l.x,l.y,GREEN);
    RenderRect(p2.x-l.x*0.5f,p2.y-l.y*0.5f,l.x,l.y,GREEN);
    RenderRect(p3.x-l.x*0.5f,p3.y-l.y*0.5f,l.x,l.y,GREEN);
}


void Setup(AlxWindow* w){
    tv = TransformedView_New((Vec2){ 1.8f,1.8f });
    TransformedView_Offset(&tv,(Vec2){ -100.0f,20.0f });

	//TransformedView_Focus(&tv,&Me.p);
    Objects = Vector_New(sizeof(Shape));

    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_RECT,(Rect[]){ {100.0f,100.0f},{200.0f,200.0f} },sizeof(Rect),(void(*)(void*))SRect_Update,(void(*)(void*))SRect_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_LINE,(SLine[]){ {300.0f,300.0f},{400.0f,400.0f} },sizeof(SLine),(void(*)(void*))SLine_Update,(void(*)(void*))SLine_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_CIRCLE,(SCircle[]){ {500.0f,500.0f},{600.0f,400.0f} },sizeof(SCircle),(void(*)(void*))SCircle_Update,(void(*)(void*))SCircle_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_SPLINE,(SSpline[]){ {500.0f,500.0f},{600.0f,400.0f},{600.0f,400.0f} },sizeof(SSpline),(void(*)(void*))SSpline_Update,(void(*)(void*))SSpline_Render) });
}

void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	ScreenRect = TransformedView_Rect(&tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });
    
    if(Stroke(ALX_MOUSE_L).PRESSED){
        Focused = NULL;
        for(int i = 0;i<Objects.size;i++){
            Shape* s = (Shape*)Vector_Get(&Objects,i);
            s->Update(s->Memory);
        }
    }
    if(Stroke(ALX_MOUSE_L).RELEASED){
        Focused = NULL;
    }

    if(Stroke(ALX_KEY_1).PRESSED){
        Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_RECT,(Rect[]){ TransformedView_ScreenWorldPos(&tv,GetMouse()),TransformedView_ScreenWorldPos(&tv,GetMouse()) },sizeof(Rect),(void(*)(void*))SRect_Update,(void(*)(void*))SRect_Render) });
    }
    if(Stroke(ALX_KEY_2).PRESSED){
        Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_LINE,(SLine[]){ TransformedView_ScreenWorldPos(&tv,GetMouse()),TransformedView_ScreenWorldPos(&tv,GetMouse()) },sizeof(SLine),(void(*)(void*))SLine_Update,(void(*)(void*))SLine_Render) });
    }
    if(Stroke(ALX_KEY_3).PRESSED){
        Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_CIRCLE,(SCircle[]){ TransformedView_ScreenWorldPos(&tv,GetMouse()),TransformedView_ScreenWorldPos(&tv,GetMouse()) },sizeof(SCircle),(void(*)(void*))SCircle_Update,(void(*)(void*))SCircle_Render) });
    }
    if(Stroke(ALX_KEY_4).PRESSED){
        Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_SPLINE,(SSpline[]){ TransformedView_ScreenWorldPos(&tv,GetMouse()),TransformedView_ScreenWorldPos(&tv,GetMouse()),TransformedView_ScreenWorldPos(&tv,GetMouse()) },sizeof(SSpline),(void(*)(void*))SSpline_Update,(void(*)(void*))SSpline_Render) });
    }
    if(Stroke(ALX_KEY_5).PRESSED){
        Table t = Table_New();
        Table_Parse(&t,"./data/Objects.alxon");
        //Table_Print(&t);
        for(int i = 0;i<t.Root->Childs.size;i++){
            Entry* e = (*(Branch**)Vector_Get(&t.Root->Childs,i))->Memory;
            Vector out = CStr_ChopDown(e->Value,' ');
            if(CStr_Cmp(e->Name,"RECT")){
                if(out.size==4){
                    Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_RECT,(SRect[]){ 
                        Double_Parse(*(char**)Vector_Get(&out,0),1),
                        Double_Parse(*(char**)Vector_Get(&out,1),1),
                        Double_Parse(*(char**)Vector_Get(&out,2),1),
                        Double_Parse(*(char**)Vector_Get(&out,3),1),
                    },sizeof(SRect),(void(*)(void*))SRect_Update,(void(*)(void*))SRect_Render) });
                }
            }else if(CStr_Cmp(e->Name,"LINE")){
                if(out.size==4){
                    Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_LINE,(SLine[]){ 
                        Double_Parse(*(char**)Vector_Get(&out,0),1),
                        Double_Parse(*(char**)Vector_Get(&out,1),1),
                        Double_Parse(*(char**)Vector_Get(&out,2),1),
                        Double_Parse(*(char**)Vector_Get(&out,3),1),
                    },sizeof(SLine),(void(*)(void*))SLine_Update,(void(*)(void*))SLine_Render) });
                }
            }else if(CStr_Cmp(e->Name,"CIRCLE")){
                if(out.size==4){
                    Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_CIRCLE,(SCircle[]){ 
                        Double_Parse(*(char**)Vector_Get(&out,0),1),
                        Double_Parse(*(char**)Vector_Get(&out,1),1),
                        Double_Parse(*(char**)Vector_Get(&out,2),1),
                        Double_Parse(*(char**)Vector_Get(&out,3),1),
                    },sizeof(SCircle),(void(*)(void*))SCircle_Update,(void(*)(void*))SCircle_Render) });
                }
            }else if(CStr_Cmp(e->Name,"SPLINE")){
                if(out.size==6){
                    Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_SPLINE,(SSpline[]){ 
                        Double_Parse(*(char**)Vector_Get(&out,0),1),
                        Double_Parse(*(char**)Vector_Get(&out,1),1),
                        Double_Parse(*(char**)Vector_Get(&out,2),1),
                        Double_Parse(*(char**)Vector_Get(&out,3),1),
                        Double_Parse(*(char**)Vector_Get(&out,4),1),
                        Double_Parse(*(char**)Vector_Get(&out,5),1),
                    },sizeof(SSpline),(void(*)(void*))SSpline_Update,(void(*)(void*))SSpline_Render) });
                }
            }
            for(int j = 0;j<out.size;j++){
                char* cstr = *(char**)Vector_Get(&out,j);
                free(cstr);
            }
            Vector_Free(&out);
        }
        //Table_Write(&t,"C:/Wichtig/Hecke/C/Win_Polymorphism/Objects.alxon");
        Table_Free(&t);
    }
    if(Stroke(ALX_KEY_6).PRESSED){
        Table t = Table_New();
        Branch_Set(t.Root,(Entry[]){ Entry_New("Objects",NULL,0) },sizeof(Entry));
        for(int i = 0;i<Objects.size;i++){
            Shape* s = (Shape*)Vector_Get(&Objects,i);
            if(s->Type==SHAPE_RECT){
                SRect* o = (SRect*)s->Memory;
                String str = String_Format("%f %f %f %f",o->p.x,o->p.y,o->e.x,o->e.y);
                char* cstr = String_CStr(&str);
                Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("RECT",cstr,0) },sizeof(Entry)));
                free(cstr);
                String_Free(&str);
            }else if(s->Type==SHAPE_LINE){
                SLine* o = (SLine*)s->Memory;
                String str = String_Format("%f %f %f %f",o->p.x,o->p.y,o->e.x,o->e.y);
                char* cstr = String_CStr(&str);
                Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("LINE",cstr,0) },sizeof(Entry)));
                free(cstr);
                String_Free(&str);
            }else if(s->Type==SHAPE_CIRCLE){
                SCircle* o = (SCircle*)s->Memory;
                String str = String_Format("%f %f %f %f",o->p.x,o->p.y,o->e.x,o->e.y);
                char* cstr = String_CStr(&str);
                Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("CIRCLE",cstr,0) },sizeof(Entry)));
                free(cstr);
                String_Free(&str);
            }else if(s->Type==SHAPE_SPLINE){
                SSpline* o = (SSpline*)s->Memory;
                String str = String_Format("%f %f %f %f %f %f",o->p1.x,o->p1.y,o->p2.x,o->p2.y,o->p3.x,o->p3.y);
                char* cstr = String_CStr(&str);
                Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("SPLINE",cstr,0) },sizeof(Entry)));
                free(cstr);
                String_Free(&str);
            }
        }
        //Table_Print(&t);
        Table_Write(&t,"./data/Objects.alxon");
        Table_Free(&t);
    }

    if(Focused){
        *Focused = TransformedView_ScreenWorldPos(&tv,GetMouse());
    }

    Clear(DARK_BLUE);

    for(int i = 0;i<Objects.size;i++){
        Shape* s = (Shape*)Vector_Get(&Objects,i);
        s->Render(s->Memory);
    }

    String str = String_Format("| Offset: %f,%f - Zoom: %f,%f |",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	RenderCStrSize(str.Memory,str.size,0,0,WHITE);
	String_Free(&str);
}

void Delete(AlxWindow* w){
    Vector_Free(&Objects);
}

int main(){
    if(Create("Polymorphism1",1800,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}