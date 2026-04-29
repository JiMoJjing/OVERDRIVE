# Work Log - 2026-04-29 - OverdriveCharacter 기본 스폰 및 입력 골격

## Branch

`feature/mission-character-spawn`

## Task Summary

- 미션 맵에서 사용할 기본 플레이어 Pawn 클래스 `AOverdriveCharacter`를 추가했습니다.
- `AOverdriveMissionGameMode`의 기본 Pawn을 `AOverdriveCharacter`로 연결했습니다.
- 테스트를 위해 Character 내부에 Enhanced Input 기반 최소 이동/시점 입력 골격을 추가했습니다.
- 에디터에서 UE Manny 기반 테스트 Character Blueprint와 Enhanced Input 에셋을 구성하고, MissionPrototype 스폰/입력/동기화를 검증했습니다.

## Files Changed

- `Source/OVERDRIVE/Character/OverdriveCharacter.h`: Character 클래스, 카메라 컴포넌트, 입력 에셋 슬롯, 입력 함수 선언 추가
- `Source/OVERDRIVE/Character/OverdriveCharacter.cpp`: 이동/카메라 기본값, Enhanced Input 바인딩, Move/Look 처리, MappingContext 등록 구현
- `Source/OVERDRIVE/Mission/OverdriveMissionGameMode.cpp`: Mission GameMode의 `DefaultPawnClass`를 `AOverdriveCharacter`로 지정

## Implementation Summary

- Character는 Replication과 movement replication을 명시적으로 켰습니다.
- TPS 테스트에 필요한 `CameraBoom`과 `FollowCamera`를 C++ 기본 컴포넌트로 추가했습니다.
- `PawnClientRestart()`에서 로컬 플레이어의 `UEnhancedInputLocalPlayerSubsystem`에 기본 MappingContext를 등록하도록 했습니다.
- `SetupPlayerInputComponent()`에서 `MoveAction`, `LookAction`을 Enhanced Input에 바인딩했습니다.
- `Move()`는 컨트롤러 Yaw 기준 전후좌우 이동을 처리합니다.
- `Look()`은 마우스/스틱 축으로 Yaw/Pitch 입력을 처리합니다.
- 에디터에서 `IA_Move`, `IA_Look`, IMC 에셋을 생성하고 `BP_OverdriveCharacter`에 할당했습니다.
- `BP_OverdriveMissionGameMode`를 생성해 기본 Pawn을 `BP_OverdriveCharacter`로 설정하고, MissionPrototype 레벨 GameMode로 지정했습니다.
- 테스트용 SkeletalMesh로 UE Manny를 사용했습니다.

## Networking Impact

- Character는 서버에서 스폰되고 PlayerController가 Possess하는 기본 Unreal 흐름을 따릅니다.
- 이동 입력은 owning client에서 발생하고 CharacterMovement replication 흐름을 사용합니다.
- 새 RPC는 추가하지 않았습니다.
- 테스트 결과 각 클라이언트가 자신의 캐릭터를 정상 소유했고, 다른 클라이언트에서도 movement replication이 정상 동기화되는 것을 확인했습니다.

## GAS Impact

- ASC, Ability, GameplayEffect, GameplayTag, Attribute 변경은 없습니다.
- `PossessedBy()`와 `OnRep_PlayerState()`에 추후 GAS Owner/Avatar 초기화 지점만 마련했습니다.

## Problems

- 없음.

## Decisions / Solutions

- Lyra식 입력 ActorComponent 분리는 아직 하지 않고, 테스트 우선으로 Character 내부에 최소 입력만 추가했습니다.
- 무기 발사, 조준, 재장전, GAS Ability 입력이 들어오는 시점에 입력 계층 분리를 다시 검토합니다.

## TODO

- Character 입력 구조는 현재 Character 내부 최소 구현으로 유지합니다.
- 추후 무기 발사/조준/재장전/GAS Ability 입력이 추가될 때 입력 ActorComponent 또는 GameplayTag 기반 입력 매핑 분리를 재검토합니다.
- 다음 단계로 기본 총기 장착 위치, 카메라 기준 조준, 발사 Ability 골격을 검토합니다.

## Portfolio Summary

- 미션 진입 후 서버 권한 기반 플레이어 스폰을 검증하기 위한 `AOverdriveCharacter` 기본 골격과 최소 Enhanced Input 이동/시점 입력을 구축했습니다.
