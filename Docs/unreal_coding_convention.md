# Unreal Coding Convention

## 기본 원칙

Unreal Engine의 일반적인 C++ 스타일을 따릅니다.

코드는 다음을 우선합니다.

1. 명확한 역할
2. Unreal 관례 준수
3. 읽기 쉬운 이름
4. 작은 책임 단위
5. Blueprint와 C++ 책임 분리

---

## 접두사 규칙

- Actor 클래스: `A`
- UObject / ActorComponent 클래스: `U`
- Struct: `F`
- Enum: `E`
- Interface: `I`
- bool 변수: `b`

예시:

```cpp
AMissionGameMode
AOverdriveCharacter
UWeaponComponent
UWeaponPartsComponent
UWeaponPartDefinition
FWeaponPartRuntimeData
EWeaponPartSlot
IWeaponPartInteractableInterface
bool bIsReady
```

---

## 이름 규칙

클래스와 변수 이름은 역할이 드러나야 합니다.

좋은 예시:

```cpp
UWeaponPartsComponent
UWeaponPartDefinition
UGameplayAbility_WeaponFire
UGameplayAbility_WeaponReload
AMissionGameMode
AMissionPlayerState
```

피해야 할 예시:

```cpp
Manager
System
Data
Temp
NewActor
```

단, 문맥상 의미가 명확한 경우에는 예외적으로 사용할 수 있습니다.

---

## C++ / Blueprint 책임 분리

C++에서 담당할 것:

- 핵심 게임플레이 로직
- 서버 권한 판단
- Replication 구조
- GAS Ability / Effect / Attribute 연결
- 총기 파츠 시스템의 핵심 구조
- 발사 / 피해 / 사망 / 보상 처리

Blueprint에서 담당할 것:

- 시각적 조립
- 에셋 할당
- Animation Blueprint
- Hand IK 시각 조정
- UI 배치
- 데이터 튜닝
- 간단한 연결 로직

핵심 게임플레이 권한 로직이 Blueprint에만 존재해서는 안 됩니다.

---

## 코드 작성 원칙

- 불필요하게 복잡한 추상화를 피합니다.
- 기능이 검증되기 전에는 과도하게 일반화하지 않습니다.
- `Tick` 사용은 최소화합니다.
- Replicated 변수는 목적이 명확해야 합니다.
- RPC는 필요한 경우에만 사용합니다.
- 임시 구현은 TODO로 남깁니다.

---

## 클래스 선언 순서

클래스 내부에서는 접근 지정자별로 함수 선언을 먼저 모으고, 그 아래에 변수 선언을 배치합니다.

권장 순서:

```cpp
public:
	// Constructors / overrides / public functions

protected:
	// Protected functions

private:
	// Private functions

public:
	// Public variables, delegates, BlueprintAssignable events

protected:
	// Protected variables

private:
	// Private variables
```

- 함수와 변수가 섞이지 않게 합니다.
- `OnRep`, RPC 구현 함수도 함수 선언 영역에 둡니다.
- `UPROPERTY` 변수와 Delegate 변수는 함수 선언 뒤에 둡니다.

---

## 줄바꿈 기준

함수 선언, 함수 호출, Delegate 선언, 조건식은 한 줄로 읽을 수 있으면 임의로 줄바꿈하지 않습니다.

좋은 예시:

```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLobbySummaryChanged, int32, PlayerCount, int32, ReadyPlayerCount, EOverdriveLobbyState, LobbyState);

void SetLobbySummary(int32 NewPlayerCount, int32 NewReadyPlayerCount, EOverdriveLobbyState NewLobbyState);

if (LobbySummary.PlayerCount == NewPlayerCount && LobbySummary.ReadyPlayerCount == NewReadyPlayerCount && LobbySummary.LobbyState == NewLobbyState)
{
	return;
}

OnLobbySummaryChanged.Broadcast(LobbySummary.PlayerCount, LobbySummary.ReadyPlayerCount, LobbySummary.LobbyState);
```

- 단순히 길다는 이유만으로 인자 목록이나 조건식을 여러 줄로 나누지 않습니다.
- 줄바꿈은 중첩 람다, 복잡한 체이닝, 여러 단계의 계산처럼 한 줄이 오히려 의미를 흐릴 때만 사용합니다.
