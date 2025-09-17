#ifndef GRIDLOCK_DEFENDERS_ASSETMANAGER_H
#define GRIDLOCK_DEFENDERS_ASSETMANAGER_H

#include <Texture.h>
#include <Sprite.h>
#include <functional>
#include <variant>
#include <optional>
#include <filesystem>
#include <unordered_map>

namespace gld {

    enum class Asset {
        Battlefield,
        Cursor,
        AnimationSpriteAtlas,
        TextureSpriteAtlas,
        SpriteBlobStanding,
        SpriteBlobMoving,
        SpriteCursorAnimated,
        SpriteCursor2Animated
    };

    using AssetVariant = std::variant<gld::Texture, gld::Sprite>;

    template<typename AssetT>
    using load_asset_fn_t = std::function<std::optional<AssetT>(const std::filesystem::path&)>;

    class AssetManager {
    public:
        AssetManager(std::filesystem::path asset_dir, load_asset_fn_t<gld::Texture> load_texture_fn);
        AssetManager(std::filesystem::path asset_dir, const std::filesystem::path &catalog, load_asset_fn_t<gld::Texture> load_texture_fn);

        void load_catalog_from_file(const std::filesystem::path &catalog_path);

        std::optional<gld::Texture> load_texture(const std::filesystem::path &asset_filename) const;

        template<typename T>
        std::optional<T> get(Asset asset) const {
            if (m_assets.contains(asset) && std::holds_alternative<T>(m_assets.at(asset))) {
                return std::get<T>(m_assets.at(asset));
            } else {
                return std::nullopt;
            }
        }

    private:
        std::filesystem::path m_asset_dir;
        load_asset_fn_t<gld::Texture> m_load_texture_fn;
        std::unordered_map<gld::Asset, gld::AssetVariant> m_assets;

        void load_sprite_atlas_from_file(const gld::Texture &texture, const std::filesystem::path &atlas_path);
        std::filesystem::path effective_path(const std::filesystem::path &path) const;
    };
}

#endif //GRIDLOCK_DEFENDERS_ASSETMANAGER_H