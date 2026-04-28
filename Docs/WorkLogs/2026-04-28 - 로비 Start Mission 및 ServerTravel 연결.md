# Work Log - 2026-04-28 - 로비 Start Mission 및 ServerTravel 연결

## Branch

`feature/lobby-system`

## Task Summary

- 로비 맵 경로를 `Content/Maps/` 구조로 맞췄습니다.
- 로비 방장 상태를 서버 권한 PlayerState Replication으로 추가했습니다.
- 방장만 Start Mission을 요청할 수 있도록 서버 검증 흐름을 추가했습니다.
- 모든 플레이어가 Ready 상태일 때 `MissionPrototype` 맵으로 ServerTravel하는 흐름을 연결했습니다.
- MissionPrototype용 GameMode/GameState 골격을 추가하고 seamless travel 설정을 적용했습니다.
- Seamless Travel 이후 서버 PlayerController에서 owning client로 Client RPC를 보내 기존 로비 UI를 제거하도록 정리했습니다.

## Files Changed

- `Config/DefaultEngine.ini`: 기본 게임/에디터 시작 맵을 `/Game/Maps/Lobby`로 변경
- `Tools/CreateLobbyTestAssets.py`: 로비 자동화 대상 맵 경로를 `/Game/Maps/Lobby`로 변경
- `Source/OVERDRIVE/Player/OverdrivePlayerState.h`: Lobby Leader 상태, getter, setter, delegate 선언
- `Source/OVERDRIVE/Player/OverdrivePlayerState.cpp`: Lobby Leader Replication 및 OnRep 구현
- `Source/OVERDRIVE/Player/OverdrivePlayerController.h`: Start Mission 요청 함수, Server RPC, Seamless Travel 이후 UI 정리용 Client RPC 선언
- `Source/OVERDRIVE/Player/OverdrivePlayerController.cpp`: Start Mission Server RPC를 Lobby GameMode로 전달하고 Seamless Travel 이후 Client RPC로 로비 UI 제거
- `Source/OVERDRIVE/Lobby/OverdriveLobbyGameMode.h`: Start Mission 요청, 방장 지정, ServerTravel 설정 추가
- `Source/OVERDRIVE/Lobby/OverdriveLobbyGameMode.cpp`: 방장 지정/재지정, Start Mission 서버 검증, ServerTravel 구현
- `Source/OVERDRIVE/UI/OverdriveLobbyWidget.h`: Start Mission 버튼과 방장 표시용 optional widget 슬롯 추가
- `Source/OVERDRIVE/UI/OverdriveLobbyWidget.cpp`: Start Mission 버튼 클릭, 방장 상태 표시, 버튼 활성화 조건 구현
- `Source/OVERDRIVE/Mission/OverdriveMissionGameMode.h`: 미션 GameMode 골격 추가
- `Source/OVERDRIVE/Mission/OverdriveMissionGameMode.cpp`: 미션용 기본 PlayerController/PlayerState/GameState 지정 및 seamless travel 설정
- `Source/OVERDRIVE/Mission/OverdriveMissionGameState.h`: 미션 GameState 골격 추가
- `Source/OVERDRIVE/Mission/OverdriveMissionGameState.cpp`: 미션 GameState 구현 파일 추가

## Implementation Summary

- 첫 접속자 또는 방장이 없는 상태에서 새 후보를 Lobby Leader로 지정합니다.
- 방장이 나가면 남은 PlayerState 중 하나를 새 Lobby Leader로 지정합니다.
- Start Mission 요청은 PlayerController Server RPC를 통해 GameMode로 전달됩니다.
- GameMode는 요청자가 Lobby Leader인지, 모든 플레이어가 Ready인지, 이미 Starting 상태가 아닌지 검증합니다.
- 검증 통과 시 GameState를 `Starting`으로 갱신하고 `/Game/Maps/MissionPrototype`으로 `ServerTravel`합니다.
- Lobby/Mission GameMode 모두 `bUseSeamlessTravel`을 활성화했습니다.
- MissionPrototype 맵은 `AOverdriveMissionGameMode`를 사용하도록 자동화 스크립트로 설정합니다.
- Seamless Travel 이후 서버 `AOverdrivePlayerController::PostSeamlessTravel()`에서 owning client에 UI 제거 RPC를 보냅니다.
- `BeginPlay()`와 `ShowLobbyWidget()`은 Lobby 맵에서만 로비 UI를 생성하도록 제한합니다.

## Networking Impact

- `bIsLobbyLeader`는 PlayerState Replicated State로 관리합니다.
- 클라이언트 UI는 방장 여부를 표시할 수 있지만, Start Mission 권한 판단은 서버 GameMode가 다시 검증합니다.
- ServerTravel은 서버 권한에서만 실행됩니다.
- Seamless Travel을 사용해 PlayerController/PlayerState 유지 여부를 확인할 수 있게 했습니다.
- 로비 UI 제거는 클라이언트 로컬 Viewport 작업이므로 Client RPC를 통해 owning client에서 수행합니다.

## GAS Impact

- 없음.

## Problems

- 없음.

## Decisions / Solutions

- LocalPlayer index나 PlayerId에 의존하지 않고, 서버가 명시적으로 Lobby Leader를 지정하는 방식으로 구현했습니다.
- Widget Blueprint 계층은 에디터에서 수동 구성하고, C++은 `BindWidgetOptional` 슬롯과 동작만 제공합니다.
- Start Mission 목적지는 초기 테스트용으로 `/Game/Maps/MissionPrototype`을 사용합니다.

## TODO

- Blueprint에서 `StartMissionButton`, `StartMissionButtonText`, `LobbyLeaderText`를 필요에 따라 추가/연결
- PIE 또는 Dedicated Server 환경에서 방장 Start Mission과 ServerTravel 검증
- Seamless Travel 이후 PlayerController/PlayerState 유지 여부 검증
- MissionPrototype 진입 후 미션용 UI/Spawn 흐름 정리

## Portfolio Summary

- 서버 권한 기반 Lobby Leader와 Start Mission 검증을 추가하고, Ready 완료 시 Dedicated Server에서 미션 맵으로 전환하는 흐름을 구현했습니다.
