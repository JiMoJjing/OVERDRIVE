# Work Log - 2026-05-01 - Weapon Fire GameplayAbility 전환

## Branch

`feature/gas-playerstate-asc`

## Task Summary

- 최신 `main`의 무기 장착 / ADS / 로컬 발사 프로토타입을 기준으로 발사 Trace 계산을 `GA_WeaponFire`로 이전했습니다.
- `UOverdriveGameplayAbility_WeaponFire` C++ 클래스를 추가했습니다.
- Character의 Fire 입력은 더 이상 직접 Trace를 수행하지 않고 ASC Ability activation만 요청합니다.
- Character에는 임시 `TSubclassOf<UGameplayAbility>` 슬롯으로 기본 Fire Ability를 관리하도록 추가했습니다.
- 서버 Possess 후 PlayerState ASC에 기본 Fire Ability를 한 번 부여하도록 연결했습니다.
- 클라이언트 AimStart / AimDirection을 ASC replicated TargetData로 서버에 전달하고, 서버 GA가 해당 값을 검증한 뒤 서버 ViewPoint 기준 Trace를 재실행하도록 변경했습니다.
- 서버 검증 Trace가 Hit Actor를 확정하면, Ability에 지정된 Blueprint Damage GameplayEffect를 대상 ASC에 적용하도록 연결했습니다.
- TargetDummy가 Health 변경을 감지하고, 서버에서 Health가 0 이하가 되면 사망 상태로 전환되도록 추가했습니다.
- 서버 검증 Hit 후 `GameplayCue.Weapon.Impact`를 실행하고, 에셋 없이도 로그와 DebugSphere로 확인할 수 있도록 추가했습니다.
- `GA_WeaponFire` 종료를 TargetData 콜백이 아니라 FireRate 타이머가 전담하도록 변경했습니다.
- Fire 입력은 Press 유지 중 반복 activation 시도를 위해 `Triggered` 이벤트에 바인딩했습니다.

## Files Changed

- `Source/OVERDRIVE/GAS/Abilities/OverdriveGameplayAbility_WeaponFire.h`: Local Predicted 발사 Ability 클래스 선언 추가
- `Source/OVERDRIVE/GAS/Abilities/OverdriveGameplayAbility_WeaponFire.cpp`: Ability 활성화, 클라이언트 예측 Trace, ASC TargetData 전송, 서버 TargetData 수신, 서버 검증 Trace 구현
- `Source/OVERDRIVE/GAS/Abilities/OverdriveGameplayAbility_WeaponFire.cpp`: FireRate 타이머 기반 Ability 종료, TargetData delegate 정리 추가
- `Config/DefaultGameplayTags.ini`: `GameplayCue.Weapon.Impact` 태그 추가
- `Source/OVERDRIVE/Character/OverdriveCharacter.h`: 기본 Fire Ability 슬롯, Ability 부여/활성화 함수 선언 추가
- `Source/OVERDRIVE/Character/OverdriveCharacter.cpp`: 기존 Character 직접 Trace 제거, Fire 입력을 ASC Ability activation으로 변경, 서버 기본 Ability Grant 구현
- `Source/OVERDRIVE/Character/OverdriveCharacter.cpp`: Fire 입력을 `Triggered`로 바인딩해 FireRate 타이머 종료 이후 재활성화될 수 있도록 변경
- `Source/OVERDRIVE/GAS/OverdriveAttributeSet.h`: Damage Meta Attribute와 GE 후처리 override 추가
- `Source/OVERDRIVE/GAS/OverdriveAttributeSet.cpp`: Damage를 Health에 반영하는 `PostGameplayEffectExecute()` 구현
- `Source/OVERDRIVE/Combat/OverdriveTargetDummy.h`: TargetDummy에 `IAbilitySystemInterface`, ASC, AttributeSet 추가
- `Source/OVERDRIVE/Combat/OverdriveTargetDummy.cpp`: TargetDummy ASC 생성/초기화, Health 변경 델리게이트 바인딩, 사망 상태 Replication 및 비활성 처리 추가

## Implementation Summary

- `UOverdriveGameplayAbility_WeaponFire`는 `InstancedPerActor`로 설정했습니다.
- `NetExecutionPolicy`는 `LocalPredicted`로 설정했습니다.
- Ability 내부에서 Avatar Actor를 `AOverdriveCharacter`로 조회합니다.
- 클라이언트 예측 경로에서는 Character의 `GetEquippedWeapon()`으로 현재 무기를 조회합니다.
- 클라이언트는 Controller ViewPoint 기준 Aim Trace를 수행하고, Aim Target Point를 계산합니다.
- 클라이언트 시각 피드백은 장착 무기의 `GetMuzzleTransform()` 기준 Fire Trace로 표시합니다.
- 클라이언트는 AimStart와 AimDirection을 `FGameplayAbilityTargetData_SingleTargetHit`의 TraceStart / TraceEnd에 담아 ASC TargetData 경로로 전달합니다.
- 서버 GA는 `AbilityTargetDataSetDelegate`로 TargetData를 수신합니다.
- 서버는 전달받은 AimStart가 서버 ViewPoint와 너무 멀지 않은지 확인합니다.
- 서버는 전달받은 AimDirection이 서버 Control/View 방향과 허용 각도 안에 있는지 확인합니다.
- 서버 검증 Trace는 서버 ViewPoint에서 시작하며, 서버 애니메이션 총구 소켓에 의존하지 않습니다.
- 서버 검증 Trace Hit Actor가 ASC를 가지고 있고 `DamageEffectClass`가 지정되어 있으면 서버에서 Damage GE를 적용합니다.
- 서버 검증 Trace Hit 시 `ImpactGameplayCueTag`로 GameplayCue를 실행합니다.
- 현재 별도 GameplayCue Notify 에셋은 없으므로, Cue 실행 확인용 로그와 주황색 DebugSphere를 함께 출력합니다.
- Ability 활성화 시 클라이언트와 서버 모두 FireRate 타이머를 시작합니다.
- TargetData 콜백은 서버 검증 / GE / GC만 처리하고 즉시 `EndAbility()`를 호출하지 않습니다.
- FireRate 타이머 만료 시 `EndAbility()`를 호출해 같은 Ability 재활성화 간격을 제어합니다.
- `UOverdriveAttributeSet`의 `Damage` Meta Attribute는 `PostGameplayEffectExecute()`에서 `Health` 감소로 반영됩니다.
- TargetDummy는 Health 변경 로그를 출력하고, 서버에서 Health가 0 이하가 되면 `bIsDead`를 Replicate합니다.
- `bIsDead`가 적용되면 TargetDummy Mesh의 충돌과 표시가 비활성화됩니다.
- 로컬 예측 Trace와 서버 검증 Trace를 디버그 색상으로 구분합니다.
  - 로컬 Aim Trace: 파란색
  - 로컬 Fire Trace: 초록색
  - 서버 검증 Trace: 빨간색
- 기존 Character의 `HandleFire()` 직접 Trace 흐름은 제거했습니다.
- `StartFire()`는 회전 모드 갱신 후 `ActivateWeaponFireAbility()`만 호출합니다.
- 서버 `PossessedBy()` 이후 `GrantDefaultAbilities()`를 호출해 PlayerState ASC에 기본 Fire Ability를 부여합니다.

## Networking Impact

- 클라이언트는 Fire 입력 시 ASC를 통해 Ability activation을 요청합니다.
- Ability는 Local Predicted로 실행되어 클라이언트 즉시 Trace 피드백을 줄 수 있습니다.
- 서버는 클라이언트가 보낸 AimStart / AimDirection을 검증한 뒤 서버 ViewPoint 기준으로 권한 Trace를 수행합니다.
- Character에 Fire 요청 전달용 Server RPC를 두지 않고 ASC replicated TargetData 경로를 사용합니다.
- 기존 TargetDummy HitReact 직접 호출은 이번 작업에서 제거했습니다.
- 실제 데미지/피격 확정은 서버 Trace 결과 기준 GameplayEffect 적용으로 처리합니다.

## GAS Impact

- Ability 변경: `UOverdriveGameplayAbility_WeaponFire` 추가
- GameplayEffect 변경: Blueprint Damage GE를 지정할 수 있는 `DamageEffectClass` 슬롯 추가
- GameplayTag 변경: 없음
- GameplayCue 변경: `GameplayCue.Weapon.Impact` 실행 지점 추가
- AttributeSet / Attribute 변경: `Damage` Meta Attribute 추가, Damage 후처리로 Health 감소 구현
- Ability Grant / Remove 변화: 서버에서 PlayerState ASC에 기본 Fire Ability 부여
- ASC Ownership 변화: 없음, PlayerState ASC 유지
- Replication / Prediction 영향: Fire Ability를 `LocalPredicted`로 설정
- Weapon Parts Integration 영향: 현재는 Character의 임시 Ability 슬롯을 사용합니다. 추후 무기 또는 파츠 데이터 기반 Grant 구조로 이전 예정입니다.

## Problems

- 없음.

## Decisions / Solutions

- Character는 전투 판정을 직접 갖지 않고, 입력 수신과 Avatar / 장착 무기 상태 제공 역할에 가깝게 유지하는 방향으로 정리했습니다.
- Trace 계산은 Ability 내부로 이동했습니다.
- 이유는 이후 Damage GE, Cost, Cooldown, 파츠 효과, 서버 검증을 GAS 중심으로 설명하고 확장하기 위함입니다.
- 서버 검증 Trace는 총구 기준이 아니라 서버 ViewPoint 기준으로 수행합니다.
- 이유는 Dedicated Server에서 애니메이션 총구 소켓이 클라이언트 시각 상태와 다를 수 있기 때문입니다.
- 카메라 모드 전체 이전은 이번 작업에 포함하지 않았습니다.
- 이유는 발사 판정 이전을 먼저 검증하고, ADS / Camera Mode 이전은 별도 작업으로 나누는 편이 안전하기 때문입니다.

## TODO

- `BP_OverdriveCharacter`에 `DefaultWeaponFireAbility` 지정
- TargetDummy 사망 후 리스폰/리셋 테스트 흐름 추가
- 기존 ADS / Camera Mode 흐름을 GameplayAbility 또는 GAS 상태 기반으로 이전 검토
- Fire Ability에 Cost / Ammo / Reload 정책 추가
- 무기 / 파츠 데이터 기반 Ability Grant 구조로 이전
- 서버 TargetData 대기 중 데이터가 오지 않는 경우의 취소/타임아웃 처리 검토
- FireRate 값 튜닝 및 무기 데이터 기반 이전
- 추후 `GameplayCue.Weapon.Impact`용 GameplayCue Notify Blueprint 생성

## Validation

- `OVERDRIVEEditor Win64 Development` 빌드 성공
- UHT 헤더 파싱 성공
- Character / Weapon / PlayerState ASC 구조와 함께 컴파일 및 링크 성공
- TargetDummy ASC / AttributeSet 추가 후 컴파일 및 링크 성공
- TargetDummy Health 변경 감지 / 사망 비활성 처리 추가 후 컴파일 및 링크 성공
- Impact GameplayCue 실행 로그 / DebugSphere 추가 후 컴파일 및 링크 성공
- FireRate 타이머 기반 종료 흐름 추가 후 컴파일 및 링크 성공
- PIE 테스트에서 Fire 입력 `Triggered` + FireRate 활성화 제한 흐름 정상 동작 확인
- PIE 테스트에서 클라이언트 예측 Trace, 서버 검증 Trace, Damage GE, TargetDummy 사망, Impact GameplayCue 로그 확인

## Portfolio Summary

- Character에 있던 발사 Trace 계산을 GAS Ability로 이동하고, Dedicated Server 슈터 구조에서 요구되는 클라이언트 예측, 서버 검증, Damage GE, 사망 처리, GameplayCue 실행까지 이어지는 WeaponFire 1차 Vertical Slice를 구현했습니다.
