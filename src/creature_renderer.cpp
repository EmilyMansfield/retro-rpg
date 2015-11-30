#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>

#include "creature_renderer.hpp"
#include "tile_set.hpp"

void CreatureRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void CreatureRenderer::setPosition(const sf::Vector2f& pos)
{
	sprite.setPosition(static_cast<float>(ts) * pos);
}

void CreatureRenderer::setFrame(const std::string& animStr, float interp)
{
	// Calculate the animation frame based on the interpolation value
	// then set the texture rectangle accordingly
	Animation& a = tiles->animations[animStr];
	sprite.setTextureRect(sf::IntRect(floor(a.x + interp * a.len) * ts, a.y * ts, ts, ts));
}