# gas_rules.md

## 목적

이 문서는 Codex가 Unreal Engine GAS 관련 코드를 작성하거나 수정할 때 따라야 할 규칙을 정의한다.

이 프로젝트에서 GAS는 단순 스킬 시스템이 아니라, 다음 요소를 연결하는 핵심 게임플레이 시스템이다.

* Ability
* GameplayEffect
* AttributeSet
* GameplayTag
* AbilitySystemComponent, ASC
* Modular Weapon Parts System
* Dedicated Server 기반 멀티플레이 구조

---

## Core Principles

GAS 관련 기능은 다음 요소를 중심으로 설계한다.

* GameplayAbility
* GameplayEffect
* AttributeSet
* GameplayTag
* AbilitySystemComponent

구현 전에는 다음을 먼저 판단한다.

* 일반 변수로 처리할 값인가?
* Attribute로 관리할 값인가?
* GameplayEffect로 변경해야 하는 값인가?
* GameplayTag로 상태를 표현해야 하는가?
* Ability로 분리할 행동인가?
* 총기 파츠와 연결되어야 하는 기능인가?

기본 원칙은 다음과 같다.

* 데미지, 회복, 버프, 디버프, 코스트, 쿨다운은 가능하면 GameplayEffect로 처리한다.
* Ability 내부에 복잡한 수치 계산을 직접 넣지 않는다.
* 반복 사용되는 계산은 ExecutionCalculation 또는 별도 C++ 로직으로 분리한다.
* 상태 표현은 bool 변수나 enum보다 GameplayTag를 우선 검토한다.
* 핵심 게임플레이 판단은 서버 권한을 기준으로 처리한다.
* Cosmetic 처리는 GameplayCue 사용을 우선 검토한다.

---

## ASC Ownership

ASC를 PlayerState에 둘지 Character에 둘지는 기능 특성에 따라 명확히 결정한다.

### PlayerState 기반 ASC를 우선 검토할 경우

다음 값이나 기능은 PlayerState 기반 ASC를 우선 검토한다.

* 리스폰 후에도 유지되어야 하는 능력
* 플레이어의 장기 상태
* 플레이어 전적 또는 누적 스탯과 연관된 값
* 미션 중 유지되어야 하는 무기 파츠 기반 Ability

장점:

* 리스폰 후에도 ASC 유지 가능
* PlayerState의 Replication 흐름과 잘 맞음
* 플레이어 단위 상태 관리에 적합

주의점:

* Character Possess 시점에 AvatarActor 갱신이 필요하다.
* InitAbilityActorInfo 호출 흐름을 명확히 해야 한다.
* Character가 바뀌었을 때 Ability의 AvatarActor 참조가 올바른지 확인해야 한다.

### Character 기반 ASC를 우선 검토할 경우

다음 값이나 기능은 Character 기반 ASC를 검토한다.

* Character 생명주기와 함께 사라져도 되는 능력
* Pawn 전용 임시 상태
* AI 적 또는 보스처럼 Pawn 자체가 주체인 경우
* 리스폰 시 새로 초기화해도 되는 전투 상태

장점:

* 구조가 단순하다.
* Character와 ASC의 생명주기가 일치한다.

주의점:

* 리스폰 시 Ability, Attribute, GameplayEffect 재초기화가 필요하다.
* PlayerState에 남아야 하는 정보와 분리해야 한다.

---

## ASC Initialization

ASC 초기화 흐름은 명확해야 한다.

기본 규칙은 다음과 같다.

* ASC의 OwnerActor와 AvatarActor를 구분한다.
* PlayerState 기반 ASC라면 OwnerActor는 보통 PlayerState가 된다.
* Character 기반 ASC라면 OwnerActor와 AvatarActor가 모두 Character일 수 있다.
* Character Possess 또는 Replication 완료 시점에 InitAbilityActorInfo 호출이 필요한지 확인한다.
* 서버와 클라이언트 양쪽에서 ASC ActorInfo가 올바르게 초기화되는지 확인한다.

PlayerState 기반 ASC 사용 시 확인할 것:

* Server Possess 시 InitAbilityActorInfo 호출 여부
* Client OnRep_PlayerState 시 InitAbilityActorInfo 호출 여부
* Character 재생성 후 AvatarActor 갱신 여부
* 입력 바인딩 시점과 ASC 초기화 시점 충돌 여부

Character 기반 ASC 사용 시 확인할 것:

* BeginPlay 또는 PossessedBy 시점 초기화 여부
* Client 측 초기화 여부
* 리스폰 시 Ability / Attribute / Effect 초기화 여부

---

## ASC Replication

멀티플레이 환경에서는 ASC Replication Mode를 명확히 선택한다.

기본 검토 기준은 다음과 같다.

* 플레이어 캐릭터는 Mixed Mode를 우선 검토한다.
* AI, 일반 적, 보스 등 비플레이어 대상은 Minimal Mode를 우선 검토한다.
* 모든 GameplayEffect 정보를 클라이언트가 알아야 하는 경우에만 Full Mode를 검토한다.

확인해야 할 항목:

* 필요한 GameplayTag가 클라이언트에 Replication 되는가?
* 필요한 GameplayEffect 상태가 클라이언트에서 확인 가능한가?
* UI 갱신에 필요한 Attribute가 클라이언트에 전달되는가?
* 총기 파츠 변경으로 부여된 Ability가 서버와 클라이언트에서 일관되게 보이는가?
* Late Join 시 무기 파츠 상태와 GAS 상태가 올바르게 복원되는가?

---

## Attribute

HP, Shield, Energy, MoveSpeed, 무기 관련 수치는 AttributeSet으로 관리한다.

예상 Attribute 예시:

* Health
* MaxHealth
* Shield
* MaxShield
* Energy
* MaxEnergy
* MoveSpeed
* WeaponDamageMultiplier
* WeaponFireRateMultiplier
* WeaponReloadSpeedMultiplier
* WeaponRecoilMultiplier

기본 규칙은 다음과 같다.

* Attribute 변경은 가능하면 GameplayEffect를 통해 처리한다.
* UI 갱신은 Attribute Change Delegate 기반으로 처리한다.
* Clamp, Death Check 등 후처리는 AttributeSet 또는 관련 Component에서 일관되게 처리한다.
* Attribute 변경 위치가 여러 곳으로 흩어지지 않게 한다.

후처리 규칙:

* MaxHealth 변경 시 Health Clamp를 고려한다.
* MaxEnergy 변경 시 Energy Clamp를 고려한다.
* 데미지와 회복의 최종 반영은 PostGameplayEffectExecute 사용을 우선 검토한다.
* MoveSpeed처럼 CurrentValue Clamp가 필요한 값은 PreAttributeChange 사용을 검토한다.
* Death Check는 중복 호출되지 않도록 일관된 위치에서 처리한다.
* Shield, Armor, Damage Reduction 같은 방어 계산은 처리 순서를 명확히 문서화한다.

---

## GameplayAbility

GameplayAbility는 플레이어 또는 적이 수행하는 행동과 게임플레이 기능을 표현한다.

예시:

* 주 무기 발사
* 재장전
* 조준
* 파츠 특수 발사
* 대시
* 보스 패턴

기본 규칙은 다음과 같다.

* 입력, 비용, 쿨다운, 실행 조건을 Ability 단위에서 명확히 관리한다.
* Ability는 시작, 성공, 실패, 취소, 종료 흐름을 명확히 가진다.
* Montage, WaitTargetData, Delay 등 비동기 흐름은 AbilityTask 사용을 우선 검토한다.
* Ability 내부에 과도한 수치 계산을 넣지 않는다.
* 재사용 가능한 계산은 ExecutionCalculation 또는 별도 C++ 로직으로 분리한다.
* Ability가 끝날 때 필요한 상태 정리를 누락하지 않는다.

Ability 구현 시 확인할 것:

* CanActivateAbility 조건
* Cost 적용 방식
* Cooldown 적용 방식
* Cancel 조건
* EndAbility 호출 보장
* 서버 권한 필요 여부
* Prediction 필요 여부
* GameplayTag 차단 조건
* 총기 파츠와 연결 여부

---

## GameplayEffect

GameplayEffect는 Attribute 변화와 상태 효과를 표현한다.

기본 규칙은 다음과 같다.

* Instant / Duration / Infinite 용도를 명확히 구분한다.
* 데미지, 회복, 코스트 차감은 Instant GE를 우선 검토한다.
* 버프, 디버프, 지속 효과는 Duration 또는 Infinite GE를 사용한다.
* 총기 파츠로 인한 지속 스탯 변화는 Infinite GE를 우선 검토한다.
* 쿨다운은 Cooldown GE로 관리하는 것을 우선 검토한다.

용도 예시:

### Instant GE

* 데미지 적용
* 회복 적용
* Energy 또는 Ammo 소모
* Shield 즉시 회복

### Duration GE

* 일정 시간 공격력 증가
* 일정 시간 이동속도 감소
* 상태이상

### Infinite GE

* 파츠 장착 중 유지되는 데미지 배율 증가
* 파츠 장착 중 유지되는 재장전 속도 변경
* 파츠 장착 중 유지되는 반동 보정
* 패시브 효과

---

## GameplayTag

GameplayTag는 상태, 조건, 차단 조건, 입력, 무기 파츠 상태를 표현한다.

기본 규칙은 다음과 같다.

* Ability 상태, 차단 조건, 입력, 상태이상은 GameplayTag로 표현한다.
* 문자열 비교나 enum 남발 대신 계층형 Tag를 우선 검토한다.
* 태그 이름은 프로젝트 네이밍 규칙을 따른다.
* GameplayTag는 중앙에서 관리한다.
* 하드코딩된 문자열 태그 사용을 피한다.

예상 Tag 예시:

```text
Ability.Weapon.Primary
Ability.Weapon.Reload
Ability.Weapon.Aim
Ability.Weapon.Special
Ability.Movement.Dash

State.Dead
State.Stunned
State.Dashing
State.UsingAbility
State.Reloading
State.Aiming

Weapon.Part.Barrel
Weapon.Part.Magazine
Weapon.Part.Stock
Weapon.Part.Scope
Weapon.Part.Muzzle
Weapon.Part.Core
Weapon.Part.Optional
Weapon.Status.Equipped
Weapon.Status.Reloading

Damage.Fire
Damage.Electric
Damage.Explosive
Damage.Ice

Cooldown.Weapon.Primary
Cooldown.Weapon.Reload
Cooldown.Weapon.Special

Input.Weapon.Primary
Input.Weapon.Reload
Input.Weapon.Aim
Input.Movement.Dash
```

---

## Weapon Parts Integration

이 프로젝트에서 총기 파츠 장착은 GAS와 직접 연결된다.

파츠 장착은 단순 외형 변경이 아니라 다음 요소를 변경할 수 있다.

* 무기 메시 또는 부착 메시
* GameplayAbility
* GameplayEffect
* GameplayTag
* Attribute
* 무기 발사 위치
* 탄환 속성
* 반동 / 탄 퍼짐 / 재장전 속도

파츠 획득과 장착 흐름은 다음을 따른다.

```text
적 처치
→ 서버가 파츠 드랍 생성
→ 클라이언트가 획득 / 장착 요청
→ 서버가 요청 검증
→ 서버가 무기 파츠 상태 변경
→ GameplayAbility 부여 / 제거
→ GameplayEffect 적용 / 제거
→ GameplayTag 갱신
→ 무기 상태 Replication
→ 클라이언트 UI / 무기 외형 갱신
```

기본 규칙:

* Ability Grant / Remove는 서버 권한에서 수행한다.
* 장착 해제 시 해당 파츠가 부여한 Ability, GameplayEffect, GameplayTag를 정리한다.
* Grant된 AbilitySpecHandle은 추적 가능해야 한다.
* Applied GameplayEffect Handle도 필요한 경우 추적한다.
* 파츠 장착 상태와 GAS 상태가 불일치하지 않게 한다.
* 클라이언트 로컬에서만 Ability가 생기거나 사라지는 구조를 피한다.
* Late Join 시 무기 파츠 상태와 부여된 Ability/Effect 상태가 올바르게 복원되어야 한다.

---

## Multiplayer

GAS 관련 기능은 Dedicated Server 기반 멀티플레이 구조를 전제로 작성한다.

기본 규칙:

* Ability 실행 가능 여부는 서버 권한 기준으로 판단한다.
* 클라이언트는 입력과 요청을 보낼 수 있다.
* 서버는 요청을 검증하고 최종 결과를 확정한다.
* 클라이언트 즉시 반응이 필요한 Ability는 Prediction 적용 여부를 검토한다.
* Cosmetic 처리는 GameplayCue 사용을 우선 검토한다.
* GameplayCue는 게임플레이 판정의 근거로 사용하지 않는다.

검토해야 할 상황:

* Dedicated Server에서 Ability가 정상 실행되는가?
* 클라이언트에서 입력 후 반응성이 충분한가?
* Prediction이 필요한 Ability인가?
* Prediction 실패 시 롤백 또는 보정이 가능한가?
* Ability Cancel이 서버와 클라이언트에서 일관되는가?
* GameplayCue가 모든 클라이언트에서 적절히 재생되는가?

---

## Damage / Healing Rules

데미지와 회복은 가능하면 GameplayEffect 기반으로 처리한다.

기본 규칙:

* 데미지는 Instant GE 또는 ExecutionCalculation을 우선 검토한다.
* 복잡한 데미지 공식은 ExecutionCalculation 분리를 검토한다.
* 회복도 GameplayEffect 기반으로 처리한다.
* Shield, Defense, Damage Reduction 계산 순서를 명확히 한다.
* Death Check는 중복 호출되지 않게 한다.
* 서버 권한에서 최종 피해 결과를 확정한다.

데미지 처리 시 확인할 것:

* 공격자 ASC
* 대상 ASC
* Source Object
* Damage Type 또는 GameplayTag
* 총기 파츠로 인한 데미지 변형 여부
* 방어 스탯 반영 여부
* Shield 우선 차감 여부
* 사망 처리 위치

---

## Cost / Cooldown Rules

Ability의 비용과 쿨다운은 명확히 관리한다.

기본 규칙:

* Energy, Ammo, Heat 같은 비용은 Cost GE 사용을 우선 검토한다.
* Cooldown은 Cooldown GE와 GameplayTag를 사용해 관리하는 것을 우선 검토한다.
* Ability 내부에서 임의로 쿨다운 시간을 직접 관리하지 않는다.
* UI는 Cooldown Tag 또는 GE 상태를 기반으로 갱신한다.

확인할 것:

* 비용이 부족할 때 Ability가 실패하는가?
* 비용 차감이 서버 권한으로 처리되는가?
* 쿨다운이 클라이언트 UI에 표시되는가?
* 총기 파츠로 인해 쿨다운이 변할 수 있는가?

---

## Reporting Rules

Codex가 GAS 관련 코드를 변경했다면 다음 내용을 보고한다.

```text
GAS Summary
- 변경된 Ability
- 변경된 GameplayEffect
- 변경된 AttributeSet / Attribute
- 변경된 GameplayTag
- Ability Grant / Remove 변화
- ASC Ownership 또는 초기화 변화
- Replication / Prediction 영향
- Weapon Parts Integration 영향
```

보고 시 실제로 변경한 것과 검토만 한 것을 구분한다.

---

## 금지 또는 주의할 행동

명시적인 승인 없이 다음을 하지 않는다.

* ASC Ownership 구조 변경
* 기존 AttributeSet 대규모 재작성
* Ability Grant / Remove 구조 변경
* Replication Mode 변경
* GameplayTag 네이밍 체계 변경
* 총기 파츠 시스템과 GAS 연결 구조 변경
* 핵심 전투 계산을 Blueprint 전용으로 이동
* Ability 내부에 대량의 하드코딩 수치 추가
* 서버 검증 없이 클라이언트에서 게임플레이 결과 확정
