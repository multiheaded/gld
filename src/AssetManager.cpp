#include "AssetManager.h"

#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace gld {

    static std::optional<gld::Asset> name_to_asset(const std::string& name) {
        static const std::unordered_map<std::string, gld::Asset> lut {
            {"battlefield", gld::Asset::Battlefield},
            {"cursor", gld::Asset::Cursor},
            {"spriteatlastexture", gld::Asset::TextureSpriteAtlas},
            {"animations", gld::Asset::AnimationSpriteAtlas},
            {"blob/standing", gld::Asset::SpriteBlobStanding},
            {"blob/moving", gld::Asset::SpriteBlobMoving},
            {"cursor/animated", gld::Asset::SpriteCursorAnimated},
            {"cursor2/animated", gld::Asset::SpriteCursor2Animated}
        };

        auto it = lut.find(name);
        if (it != lut.end()) {
            return it->second;
        } else return {};
    }

    AssetManager::AssetManager(std::filesystem::path asset_dir, load_asset_fn_t<gld::Texture> load_texture_fn)
            : m_asset_dir(std::move(asset_dir))
            , m_load_texture_fn(std::move(load_texture_fn))
    {
    }

    AssetManager::AssetManager(std::filesystem::path asset_dir, const std::filesystem::path &catalog, load_asset_fn_t<gld::Texture> load_texture_fn)
            : m_asset_dir(std::move(asset_dir))
            , m_load_texture_fn(std::move(load_texture_fn))
    {
        load_catalog_from_file(catalog);
    }

    std::optional<gld::Texture> AssetManager::load_texture(const std::filesystem::path &asset_filename) const {
        return m_load_texture_fn(effective_path(asset_filename));
    }

    std::filesystem::path AssetManager::effective_path(const std::filesystem::path& path) const {
        return m_asset_dir/path;
    }

    void AssetManager::load_catalog_from_file(const std::filesystem::path &catalog_path) {
        std::ifstream f(effective_path(catalog_path));
        json data = json::parse(f);
        for (json &texture: data["textures"]) {
            std::string name(texture["name"].get<std::string>());
            std::filesystem::path path(texture["path"].get<std::string>());

            if ( auto tex = load_texture(path) ) {
                if ( auto asset = name_to_asset(name) ) {
                    m_assets.emplace(asset.value(), gld::AssetVariant(tex.value()));
                } else {
                    // TODO: introduce logging and log that the asset could not be identified
                    std::cerr << "AssetManager::load_catalog_from_file(): Could not load texture " << path << "\n";
                }
            }
        }

        for (json &sprite_atlas: data["atlases"]) {
            std::string name(sprite_atlas["name"].get<std::string>());
            std::filesystem::path path(sprite_atlas["path"].get<std::string>());
            std::string tex_name = sprite_atlas["texture"].get<std::string>();
            if ( auto texture_asset = name_to_asset(tex_name) ) {
                if ( auto atlas_asset = name_to_asset(name) ) {
                    if ( !m_assets.contains(atlas_asset.value()) ) {
                        gld::Texture texture = get<gld::Texture>(texture_asset.value()).value();
                        load_sprite_atlas_from_file(texture, path);
                    }
                }
            } else {
                // TODO: introduce logging and log that the asset could not be identified
                std::cerr << "AssetManager::load_catalog_from_file(): Could not resolve asset name " << name << "\n";
            }
        }
    }

    void AssetManager::load_sprite_atlas_from_file(const gld::Texture &texture, const std::filesystem::path &atlas_path) {
        std::ifstream f(effective_path(atlas_path));
        json data = json::parse(f);
        for (json &sprite: data["sprites"]) {
            std::string name = sprite["name"].get<std::string>();
            gld::Vector2u size{
                sprite["size"]["width"].get<std::size_t>(),
                sprite["size"]["height"].get<std::size_t>()
            };
            gld::Vector2u center;
            if (sprite.contains("center")) {
                center = {
                    sprite["center"]["x"].get<std::size_t>(),
                    sprite["center"]["y"].get<std::size_t>()
                };
            } else {
                center = size/2;
            }
            for (json &variant: sprite["variants"]) {
                std::string variantName = variant["name"].get<std::string>();
                gld::Vector2u origin{
                    variant["origin"]["x"].get<std::size_t>(),
                    variant["origin"]["y"].get<std::size_t>()
                };
                std::size_t framecount = variant["frame count"].get<std::size_t>();
                float duration = variant["duration"].get<float>();

                std::string lookup_direction = variant["direction"].get<std::string>();
                gld::Vector2u ldir;
                if (lookup_direction == "horizontal") {
                    ldir = {1, 0};
                } else if (lookup_direction == "vertical") {
                    ldir = {0, 1};
                }

                // compute the time increment to pass before the frame needs to be advanced next (time increement = duration/framecount)
                const std::chrono::duration<float> time_increment(duration / static_cast<float>(framecount));

                gld::Sprite s{
                    .id = texture.id,
                    .rect{origin, size},
                    .center{center},
                    .animation{
                        .origin{origin},
                        .stride{ldir.cwiseProduct(size)},
                        .frame_duration{std::chrono::duration_cast<std::chrono::milliseconds>(time_increment)},
                        .frame_count = framecount
                    }
                };

                if ( auto asset = name_to_asset(name+"/"+variantName) ) {
                    if (!m_assets.contains(asset.value()) ) {
                        m_assets.emplace(asset.value(), gld::AssetVariant(s));
                    }
                }
            }
        }
    }
}
