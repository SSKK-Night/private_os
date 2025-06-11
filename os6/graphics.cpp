void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                    const Vector2D<int>& size, const PixelColor& c) {
    for (int dy = 0; dy < size.y; ++dy) {
        for (int dx = 0; dx < size.x; ++dx) {
            writer.Write(pos.x + dx, pos.y + dy, c);
        }
    }
}

void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                    const Vector2D<int>& size, const PixelColor& c) {
    for (int dx = 0; dx < size.x; ++dx) {
        writer.Write(pos.x + dx, pos.y, c);
        writer.Write(pos.x + dx, pos.y + size.y - 1, c);
    }
    for (int dy = 1; dy < size.y -1; ++dy) {
        writer.Write(pos.x, pos.y + dy, c)
        writer.Write(pos.x + size.x - 1, pos.y + dy, c);
    }
}