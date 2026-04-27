# Modular Weapon Parts Rules

## 기본 원칙

이 프로젝트의 총기 파츠 시스템은 단순 외형 변경 시스템이 아닙니다.

장착된 총기 파츠는 무기 성능, GameplayAbility, GameplayEffect, GameplayTag, 전투 흐름에 영향을 줄 수 있습니다.

플레이어 캐릭터는 단순 인간형 캐릭터로 고정하고, 핵심 커스터마이징과 성장 요소는 총기 파츠 시스템에 집중합니다.

---

## 총기 파츠가 영향을 줄 수 있는 요소

- 무기 외형
- 부여되는 GameplayAbility
- 적용되는 GameplayEffect
- GameplayTag
- Attribute
- 총구 위치 / 발사 위치
- 데미지
- 사거리
- 탄속
- 탄창 크기
- 재장전 속도
- 반동
- 탄 퍼짐
- 특수 탄환 속성

---

## 예상 슬롯

초기 예상 슬롯은 다음과 같습니다.

- Barrel
- Magazine
- Stock
- Scope / Sight
- Muzzle
- Core Module / Element Module
- Optional Attachment

초기 구현에서는 모든 슬롯을 완성하려 하지 않습니다.  
먼저 소수 슬롯과 테스트 파츠로 구조를 검증합니다.

추천 초기 슬롯:

- Barrel
- Magazine
- Core Module

---

## 파츠 획득 / 장착 흐름

파츠 획득과 장착은 서버 권한으로 처리합니다.

```text
적 처치
→ 서버가 파츠 드랍 생성
→ 클라이언트가 파츠 획득 요청
→ 서버가 획득 가능 여부 검증
→ 서버가 현재 무기의 해당 슬롯 파츠 교체
→ GameplayAbility Grant / Remove
→ GameplayEffect 적용 / 제거
→ GameplayTag 갱신
→ 무기 파츠 상태 Replication
→ 클라이언트 UI / 무기 외형 갱신
```

초기 버전에서는 복잡한 인벤토리보다 즉시 교체형 파츠 시스템을 우선합니다.

```text
드랍된 파츠와 상호작용
→ 현재 무기의 같은 슬롯 파츠와 교체
→ 기존 파츠는 제거하거나 월드에 드랍
```

---

## 데이터 구조 방향

총기 파츠 데이터는 가능하면 데이터 에셋 기반으로 관리합니다.

예상 데이터:

- 파츠 ID
- 슬롯 타입
- 표시 이름
- StaticMesh 또는 SkeletalMesh
- 부여할 Ability 목록
- 적용할 GameplayEffect 목록
- 부여할 GameplayTag 목록
- 변경할 무기 스탯
- 총구 / 소켓 보정 정보
- 장착 조건
- 설명 텍스트

무기 런타임 상태는 파츠 데이터와 분리합니다.

예상 런타임 상태:

- 현재 장착된 파츠 ID 목록
- 서버가 부여한 AbilitySpecHandle 목록
- 서버가 적용한 GameplayEffect Handle 목록
- Replication에 필요한 최소 상태

---

## 구현 주의점

- 처음부터 대량의 파츠를 만들지 않습니다.
- 파츠 시스템이 검증되기 전에는 콘텐츠 양을 늘리지 않습니다.
- 파츠 등급, 랜덤 옵션, 복잡한 시너지 시스템은 후순위입니다.
- 캐릭터 외형 커스터마이징을 핵심 기능으로 확장하지 않습니다.
- 파츠 장착 로직과 GAS Grant 로직을 분리하되, 흐름은 추적 가능해야 합니다.
- 클라이언트 로컬에서만 파츠가 바뀌는 구조를 피합니다.
- Late Join 시에도 무기 파츠 상태가 올바르게 보여야 합니다.
- 발사, 피격, 데미지 판정은 서버 권한 기준으로 처리합니다.
