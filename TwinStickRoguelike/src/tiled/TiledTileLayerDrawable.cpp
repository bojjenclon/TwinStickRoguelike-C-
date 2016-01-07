#include <tiled/TiledTileLayerDrawable.hpp>

TiledTileLayerDrawable::TiledTileLayerDrawable(sf::Texture& p_texture, const TiledTileLayer& p_layer, const TiledTileset& p_tileset) : m_texture(p_texture)
{
  auto width = p_layer.getWidth();
  auto height = p_layer.getHeight();

  auto tileWidth = p_tileset.getTileWidth();
  auto tileHeight = p_tileset.getTileHeight();

  m_vertices.setPrimitiveType(sf::Quads);
  m_vertices.resize(width * height * 4);

  // populate the vertex array, with one quad per tile
  for (auto x = 0; x < width; ++x)
  {
    for (auto y = 0; y < height; ++y)
    {
      // get the current tile number
      auto tileNumber = p_layer.getTileId(x, y);
      
      // find its position in the tileset texture
      auto tu = tileNumber % (p_tileset.getImageWidth() / tileWidth);
      auto tv = tileNumber / (p_tileset.getImageWidth() / tileWidth);

      // get a pointer to the current tile's quad
      auto quad = &m_vertices[(x + y * width) * 4];

      // define its 4 corners
      quad[0].position = sf::Vector2f(
        static_cast<float>(x * tileWidth), 
        static_cast<float>(y * tileHeight));
      quad[1].position = sf::Vector2f(
        static_cast<float>((x + 1) * tileWidth), 
        static_cast<float>(y * tileHeight));
      quad[2].position = sf::Vector2f(
        static_cast<float>((x + 1) * tileWidth), 
        static_cast<float>((y + 1) * tileHeight));
      quad[3].position = sf::Vector2f(
        static_cast<float>(x * tileWidth), 
        static_cast<float>((y + 1) * tileHeight));

      // define its 4 texture coordinates
      quad[0].texCoords = sf::Vector2f(
        static_cast<float>(tu * tileWidth), 
        static_cast<float>(tv * tileHeight));
      quad[1].texCoords = sf::Vector2f(
        static_cast<float>((tu + 1) * tileWidth), 
        static_cast<float>(tv * tileHeight));
      quad[2].texCoords = sf::Vector2f(
        static_cast<float>((tu + 1) * tileWidth), 
        static_cast<float>((tv + 1) * tileHeight));
      quad[3].texCoords = sf::Vector2f(
        static_cast<float>(tu * tileWidth), 
        static_cast<float>((tv + 1) * tileHeight));
    }
  }
}

void TiledTileLayerDrawable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // apply the transform
  states.transform *= getTransform();

  // apply the tileset texture
  states.texture = &m_texture;

  // draw the vertex array
  target.draw(m_vertices, states);
}
