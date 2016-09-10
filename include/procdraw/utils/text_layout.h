#pragma once

#include <vector>

namespace procdraw {

struct GlyphCoords
{
  float Left;
  float Right;
  float Top;
  float Bottom;
  float TextureLeft;
  float TextureRight;
  float TextureTop;
  float TextureBottom;
};

template <typename T>
class TextLayout
{
public:
  typedef typename std::vector<std::vector<T>>::size_type size_type;
  int FixedGlyphWidth = 0;
  int GlyphHeight = 0;
  int LinespacePixels = 0;
  int MaxLineWidthPixels = 0;
  void OpenNewLine();
  size_type NumLines() const;
  void AddGlyph(const GlyphCoords& glyphCoords, int cursorX, int cursorY);
  const std::vector<T>& GetVerticesForLine(size_type lineNum) const;
  void SetNumCharsInLine(int n);
  int GetNumCharsInLine(size_type lineNum) const;
  void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int& out_x,
                                         int& out_y, int& out_width,
                                         int& out_height) const;

private:
  std::vector<std::vector<T>> lines_;
  std::vector<int> numChars_;
};

template <typename T>
void
TextLayout<T>::OpenNewLine()
{
  lines_.resize(lines_.size() + 1);
  numChars_.push_back(0);
}

template <typename T>
typename TextLayout<T>::size_type
TextLayout<T>::NumLines() const
{
  return lines_.size();
}

template <typename T>
void
TextLayout<T>::AddGlyph(const GlyphCoords& glyphCoords, int cursorX,
                        int cursorY)
{
  if (!lines_.empty()) {
    std::vector<T>& vertices = lines_.back();

    // Left top
    vertices.push_back(cursorX + glyphCoords.Left);
    vertices.push_back(cursorY + glyphCoords.Top);
    vertices.push_back(glyphCoords.TextureLeft);
    vertices.push_back(glyphCoords.TextureTop);
    // Left bottom
    vertices.push_back(cursorX + glyphCoords.Left);
    vertices.push_back(cursorY + glyphCoords.Bottom);
    vertices.push_back(glyphCoords.TextureLeft);
    vertices.push_back(glyphCoords.TextureBottom);
    // Right top
    vertices.push_back(cursorX + glyphCoords.Right);
    vertices.push_back(cursorY + glyphCoords.Top);
    vertices.push_back(glyphCoords.TextureRight);
    vertices.push_back(glyphCoords.TextureTop);

    // Left bottom
    vertices.push_back(cursorX + glyphCoords.Left);
    vertices.push_back(cursorY + glyphCoords.Bottom);
    vertices.push_back(glyphCoords.TextureLeft);
    vertices.push_back(glyphCoords.TextureBottom);
    // Right bottom
    vertices.push_back(cursorX + glyphCoords.Right);
    vertices.push_back(cursorY + glyphCoords.Bottom);
    vertices.push_back(glyphCoords.TextureRight);
    vertices.push_back(glyphCoords.TextureBottom);
    // Right top
    vertices.push_back(cursorX + glyphCoords.Right);
    vertices.push_back(cursorY + glyphCoords.Top);
    vertices.push_back(glyphCoords.TextureRight);
    vertices.push_back(glyphCoords.TextureTop);
  }
}

template <typename T>
const std::vector<T>&
TextLayout<T>::GetVerticesForLine(size_type lineNum) const
{
  return lines_.at(lineNum);
}

template <typename T>
void
TextLayout<T>::SetNumCharsInLine(int n)
{
  if (!numChars_.empty()) {
    numChars_.back() = n;
  }
}

template <typename T>
int
TextLayout<T>::GetNumCharsInLine(size_type lineNum) const
{
  return numChars_.at(lineNum);
}

template <typename T>
void
TextLayout<T>::CalculateFixedWidthBlockCursorPos(int cursorTextPosition,
                                                 int& out_x, int& out_y,
                                                 int& out_width,
                                                 int& out_height) const
{
  int physicalCursorTextPosition = cursorTextPosition;
  int physicalLineNum = 0;
  for (size_type i = 0; i < this->NumLines(); i++) {
    int numCharsThisLine = this->GetNumCharsInLine(i);
    if (physicalCursorTextPosition <= numCharsThisLine) {
      break;
    }
    physicalCursorTextPosition -= numCharsThisLine;
    ++physicalLineNum;
  }

  int x = physicalCursorTextPosition * this->FixedGlyphWidth;
  if (x + this->FixedGlyphWidth > this->MaxLineWidthPixels) {
    // If we can't fit the cursor within MaxLineWidthPixels,
    // move it to the start of the next line
    x = 0;
    ++physicalLineNum;
  }

  out_x = x;
  out_y = physicalLineNum * this->LinespacePixels;
  out_width = this->FixedGlyphWidth;
  out_height = this->GlyphHeight;
}
}
