#ifndef TILED_TILE_LAYER_DRAWABLE_HPP
#define TILED_TILE_LAYER_DRAWABLE_HPP

#include <SFML/Graphics.hpp>
#include <tiled/TiledTileLayer.hpp>

class TiledTileLayerDrawable : public sf::Drawable, public sf::Transformable
{
public:
  TiledTileLayerDrawable(sf::Texture& p_texture, TiledTileLayer& p_layer, TiledTileset& p_tileset);

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  sf::VertexArray m_vertices;
  sf::Texture& m_texture;
};

#endif
