import unreal


PROJECT_ROOT = "/Game"
UI_DIR = f"{PROJECT_ROOT}/UI"
WIDGET_PATH = f"{UI_DIR}/WBP_LobbyTest"
LOBBY_MAP = "/Game/Maps/Lobby"
MISSION_MAP = "/Game/Maps/MissionPrototype"

# UE 5.7 Python Commandlet does not expose stable WidgetBlueprint designer tree editing APIs.
# This script creates the Widget Blueprint asset and configures the Lobby map.
# Add or edit the widget hierarchy in the editor, using names that match UOverdriveLobbyWidget BindWidgetOptional fields.


def get_class(path, fallback_name):
    loaded_class = unreal.load_class(None, path)
    if loaded_class:
        return loaded_class
    return getattr(unreal, fallback_name)


def ensure_directory(path):
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_or_update_lobby_widget():
    ensure_directory(UI_DIR)

    lobby_widget_class = get_class("/Script/OVERDRIVE.OverdriveLobbyWidget", "OverdriveLobbyWidget")
    widget_blueprint = unreal.EditorAssetLibrary.load_asset(WIDGET_PATH) if unreal.EditorAssetLibrary.does_asset_exist(WIDGET_PATH) else None

    if not widget_blueprint:
        factory = unreal.WidgetBlueprintFactory()
        factory.set_editor_property("parent_class", lobby_widget_class)
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        widget_blueprint = asset_tools.create_asset("WBP_LobbyTest", UI_DIR, unreal.WidgetBlueprint, factory)

    unreal.EditorAssetLibrary.save_loaded_asset(widget_blueprint)


def configure_map(map_path, game_mode_class_path, fallback_name):
    game_mode_class = get_class(game_mode_class_path, fallback_name)
    level_editor_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    unreal_editor_subsystem = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
    level_editor_subsystem.load_level(map_path)
    world = unreal_editor_subsystem.get_editor_world()
    world_settings = world.get_world_settings()
    world_settings.set_editor_property("default_game_mode", game_mode_class)
    level_editor_subsystem.save_current_level()


create_or_update_lobby_widget()
configure_map(LOBBY_MAP, "/Script/OVERDRIVE.OverdriveLobbyGameMode", "OverdriveLobbyGameMode")
configure_map(MISSION_MAP, "/Script/OVERDRIVE.OverdriveMissionGameMode", "OverdriveMissionGameMode")
unreal.log("Created WBP_LobbyTest and configured Lobby/Mission maps.")
