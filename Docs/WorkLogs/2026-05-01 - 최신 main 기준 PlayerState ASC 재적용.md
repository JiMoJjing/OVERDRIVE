# Work Log - 2026-05-01 - 최신 main 기준 PlayerState ASC 재적용

## Branch

`feature/gas-playerstate-asc`

## Task Summary

- `main` 최신화 이후 최신 Character / Weapon / Camera 구조를 기준으로 PlayerState 기반 GAS 초기화 작업을 다시 적용했습니다.
- `AOverdrivePlayerState`에 AbilitySystemComponent를 추가했습니다.
- 기본 체력 Attribute를 가진 `UOverdriveAttributeSet`을 추가했습니다.
- `AOverdriveCharacter`가 서버 Possess 및 클라이언트 PlayerState Replication 시점에 ASC ActorInfo를 초기화하도록 연결했습니다.
- 최신 `main`에 이미 들어온 무기 장착, ADS, 로컬 발사 프로토타입 흐름은 변경하지 않았습니다.

## Files Changed

- `Source/OVERDRIVE/GAS/OverdriveAttributeSet.h`: Health / MaxHealth Attribute와 Replication Notify 선언 추가
- `Source/OVERDRIVE/GAS/OverdriveAttributeSet.cpp`: 기본 체력값 초기화, Attribute Replication, RepNotify 구현
- `Source/OVERDRIVE/Player/OverdrivePlayerState.h`: `IAbilitySystemInterface` 구현, ASC / AttributeSet 멤버와 getter 추가
- `Source/OVERDRIVE/Player/OverdrivePlayerState.cpp`: ASC 생성, Mixed Replication Mode 설정, AttributeSet 생성, GAS getter 구현
- `Source/OVERDRIVE/Character/OverdriveCharacter.h`: ASC ActorInfo 초기화 함수 선언 추가
- `Source/OVERDRIVE/Character/OverdriveCharacter.cpp`: `PossessedBy()`와 `OnRep_PlayerState()`에서 PlayerState ASC의 Owner/Avatar 초기화 연결

## Implementation Summary

- ASC Ownership은 PlayerState 기반으로 유지했습니다.
- `AOverdrivePlayerState`가 `IAbilitySystemInterface`를 구현하고 `GetAbilitySystemComponent()`로 ASC를 반환합니다.
- PlayerState 생성자에서 ASC를 생성하고 Replication을 활성화했습니다.
- 플레이어 캐릭터용 GAS 상태 동기화를 고려해 ASC Replication Mode는 `Mixed`로 설정했습니다.
- `UOverdriveAttributeSet`은 초기 검증용으로 `Health`, `MaxHealth`만 포함합니다.
- Attribute 기본값은 둘 다 `100.0f`로 초기화했습니다.
- Attribute는 `DOREPLIFETIME_CONDITION_NOTIFY`와 `GAMEPLAYATTRIBUTE_REPNOTIFY`를 사용해 Replication Notify 흐름을 갖추었습니다.
- Character는 ASC를 직접 소유하지 않고 PlayerState ASC의 AvatarActor 역할을 맡습니다.
- 서버에서는 `PossessedBy()`, 클라이언트에서는 `OnRep_PlayerState()`에서 `InitAbilityActorInfo(PlayerState, Character)`를 호출합니다.
- 최신 Character의 `SpawnDefaultWeapon()` 호출은 유지했고, 그 전에 ASC ActorInfo를 초기화하도록 배치했습니다.

## Networking Impact

- ASC는 PlayerState의 Replicated Component로 관리됩니다.
- 플레이어 단위 GAS 상태가 Character 생명주기와 분리되어, 추후 리스폰이나 Seamless Travel 이후 상태 유지 구조를 설명하기 쉬워졌습니다.
- Character는 서버 스폰/소유 흐름을 유지하며 ASC Avatar만 담당합니다.
- 새 RPC는 추가하지 않았습니다.
- Attribute Replication은 클라이언트 UI 또는 추후 전투 상태 표시의 기반이 됩니다.

## GAS Impact

- Ability 변경: 없음
- GameplayEffect 변경: 없음
- GameplayTag 변경: 없음
- AttributeSet 변경: `UOverdriveAttributeSet` 추가
- Attribute 변경: `Health`, `MaxHealth` 추가
- Ability Grant / Remove 변경: 없음
- ASC Ownership: PlayerState 기반 ASC
- ASC 초기화: Character Possess / OnRep_PlayerState 시점에 Owner=PlayerState, Avatar=Character로 초기화
- Replication / Prediction 영향: ASC Mixed Replication 기반 준비
- Weapon Parts Integration 영향: 추후 파츠 장착 시 PlayerState ASC에 Ability/Effect를 부여할 기반 마련

## Problems

- 기존 작업 브랜치를 만들기 전에 `main` 최신화가 누락되어, 최신 `main` 기준으로 작업을 다시 적용했습니다.
- 최신 `main`에는 무기 장착, ADS, 로컬 발사 프로토타입이 이미 들어와 있어 Character 변경 범위를 ASC ActorInfo 초기화로 제한했습니다.

## Decisions / Solutions

- 발사 Ability와 Damage GE는 이번 재적용 범위에서 제외했습니다.
- 이유는 최신 `main`의 무기 프로토타입과 충돌하지 않고, GAS Ownership / Attribute 기반만 먼저 안정화하기 위함입니다.
- Attribute는 Health / MaxHealth만 추가해 범위를 제한했습니다.
- 무기 관련 Attribute와 GameplayEffect는 다음 GAS 작업에서 추가합니다.

## TODO

- 최신 무기 발사 프로토타입을 GAS Ability activation 구조로 이전
- 서버 검증 Trace 결과 기준 Damage GameplayEffect 적용
- Placeholder TargetDummy에 ASC 또는 GE 수신 구조 연결
- 파츠 장착 시 Ability / GameplayEffect Grant 구조 설계

## Validation

- `OVERDRIVEEditor Win64 Development` 빌드 성공
- UHT 헤더 파싱 성공
- ASC / AttributeSet 추가 후 컴파일 성공
- 최신 Character / Weapon / Camera 코드와 함께 링크 성공

## Portfolio Summary

- 최신 무기 프로토타입 구조를 보존하면서 Dedicated Server 기반 멀티플레이 전투 구조를 위한 PlayerState 기반 GAS Ownership과 기본 Attribute Replication 기반을 구현했습니다.
