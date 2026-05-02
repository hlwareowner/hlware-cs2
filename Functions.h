//#pragma once
//
//#include "Includes.h"
//#include "LoadedVariables.h"
//#include "imgui/imgui.h"
//#include "imgui/imgui_internal.h"
//#include "Classes.h"
//
//#include <iostream>
//
//namespace Ragebot {
//
//}
//
//namespace Legitbot {
//	void Legitbot();
//}
//
//namespace ESP {
//	void ESP();
//}
//
//namespace Miscellaneous {
//
//}
//
//namespace Calculations {
//    inline bool WorldToScreen(Vector3Struct WorldPosition, Vector2Struct& ScreenPosition) {
//        // Cálculo do clip W (profundidade)
//        const ViewMatrixStruct ViewMatrix = *(ViewMatrixStruct*)(LoadedVariables::ViewMatrix);
//        float w = ViewMatrix.Matrix[3][0] * WorldPosition.X + ViewMatrix.Matrix[3][1] * WorldPosition.Y + ViewMatrix.Matrix[3][2] * WorldPosition.Z + ViewMatrix.Matrix[3][3];
//
//        // Se W < 0.01, o jogador está atrás da câmera
//        if (w < 0.01f) return false;
//
//        float invw = 1.0f / w;
//        float x = (ViewMatrix.Matrix[0][0] * WorldPosition.X + ViewMatrix.Matrix[0][1] * WorldPosition.Y + ViewMatrix.Matrix[0][2] * WorldPosition.Z + ViewMatrix.Matrix[0][3]) * invw;
//        float y = (ViewMatrix.Matrix[1][0] * WorldPosition.X + ViewMatrix.Matrix[1][1] * WorldPosition.Y + ViewMatrix.Matrix[1][2] * WorldPosition.Z + ViewMatrix.Matrix[1][3]) * invw;
//
//        ImGuiIO IO = ImGui::GetIO();
//        float ScreenSizeX = IO.DisplaySize.x;
//        float ScreenSizeY = IO.DisplaySize.y;
//
//        ScreenPosition.X = (ScreenSizeX * 0.5f) + (x * ScreenSizeX * 0.5f);
//        ScreenPosition.Y = (ScreenSizeY * 0.5f) - (y * ScreenSizeY * 0.5f);
//
//        return true;
//    }
//}
//
//namespace ImGuiFunctions {
//    inline void DrawBox(const float X, const float Y, const float W, const float H, const ColorRGBAStruct& Color) {
//        // 1. Guardamos o DrawList em uma variável local (Cache)
//        auto* drawList = ImGui::GetForegroundDrawList();
//
//        // 2. Preparamos os ImVec2 uma única vez para evitar reconstrução na stack
//        const ImVec2 min = { X, Y };
//        const ImVec2 max = { X + W, Y + H };
//
//        // 3. Desenhar o Outline (Contorno) primeiro
//        // Usamos um offset sutil e uma espessura fina para o visual minimalista
//        // Nota: ImColor(0,0,0) convertido para ImU32 é mais rápido
//        drawList->AddRect(
//            { min.x - 1.0f, min.y - 1.0f },
//            { max.x + 1.0f, max.y + 1.0f },
//            IM_COL32(0, 0, 0, 255), // Macro para cor preta (U32)
//            0.0f,
//            0,
//            1.0f
//        );
//
//        // 4. Desenhar a Caixa Principal por cima
//        drawList->AddRect(
//            min,
//            max,
//            Includes::ConvertRGBAToImColor(Color),
//            0.0f,
//            0,
//            1.5f
//        );
//    };
//    inline void DrawLine(const ImVec2& X, const ImVec2& Y, const ColorRGBAStruct& Color) {
//        ImGui::GetForegroundDrawList()->AddLine(
//            X, Y, Includes::ConvertRGBAToImColor(Color), 2.0f
//        );
//    }
//}