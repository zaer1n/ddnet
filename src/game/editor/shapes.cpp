#include "editor.h"

void CEditor::DrawLine(ivec2 P1, ivec2 P2, int Stroke, CTile Tile)
{
    std::shared_ptr<CLayerTiles> pTileLayer = std::static_pointer_cast<CLayerTiles>(GetSelectedLayerType(0, LAYERTYPE_TILES));
    if (pTileLayer)
    {
        int x1 = P1.x, y1 = P1.y;
        int x2 = P2.x, y2 = P2.y;

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        auto PlotWithWidth = [&](int x, int y) {
            for (int offset = -Stroke / 2; offset <= Stroke / 2; ++offset) {
                if (std::abs(dx) > std::abs(dy)) {
                    pTileLayer->SetTile(x, y + offset, CTile{TILE_SOLID});
                } else {
                    pTileLayer->SetTile(x + offset, y, CTile{TILE_SOLID});
                }
            }
        };

        while (true) {
            PlotWithWidth(x1, y1);
            if (x1 == x2 && y1 == y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
}

void CEditor::DrawCircle(ivec2 o, int r, int Stroke, CTile Tile)
{
    std::shared_ptr<CLayerTiles> pTileLayer = std::static_pointer_cast<CLayerTiles>(GetSelectedLayerType(0, LAYERTYPE_TILES));
    
    if (pTileLayer)
    {
        int f = 1 - r;
        ivec2 Ddf(0, -2 * r);
        int x = 0;
        int y = r;

        DrawLine(ivec2(o.x, o.y - r), ivec2(o.x, o.y - r), 1, Tile);
        DrawLine(ivec2(o.x - r, o.y), ivec2(o.x + r, o.y), 1, Tile);

        while(x < y) 
        {
            if(f >= 0) 
            {
                y--;
                Ddf.y += 2;
                f += Ddf.y;
            }
            x++;
            Ddf.x += 2;
            f += Ddf.x + 1;    
            DrawLine(ivec2(o.x - x, o.y + y), ivec2(o.x + x, o.y + y), 1, Tile);
            DrawLine(ivec2(o.x - x, o.y - y), ivec2(o.x + x, o.y - y), 1, Tile);
            DrawLine(ivec2(o.x - y, o.y + x), ivec2(o.x + y, o.y + x), 1, Tile);
            DrawLine(ivec2(o.x - y, o.y - x), ivec2(o.x + y, o.y - x), 1, Tile);
        }
        if (Stroke > 0)
            DrawCircle(o, r - Stroke, 0, CTile{TILE_AIR});
    }
}

void CEditor::DrawPolygon(ivec2 o, int r, int Sides, int Stroke, CTile Tile) 
{
    double AngleStep = 2 * pi / Sides;
    ivec2 Pp(0, 0), Fp(0, 0);

    for (int i = 0; i < Sides; ++i) {
        double Angle = i * AngleStep; 
        int x = static_cast<int>(o.x + r * std::cos(Angle));
        int y = static_cast<int>(o.y + r * std::sin(Angle));
        ivec2 p(x, y);

        if (i == 0) {
            Fp = p;
        } else {
            DrawLine(Pp, p, Stroke, Tile);
        }

        Pp = p;
    }

    DrawLine(Pp, Fp, Stroke, Tile);
}