# Modular Mech Equipment Rules

## 기본 원칙

이 프로젝트의 메카 장비 시스템은 단순 외형 변경 시스템이 아닙니다.

장착된 파츠는 외형과 게임플레이 능력 모두에 영향을 줄 수 있습니다.

---

## 장착 파츠가 영향을 줄 수 있는 요소

- SkeletalMesh 외형
- 부여되는 GameplayAbility
- 적용되는 GameplayEffect
- GameplayTag
- Attribute
- 무기 소켓 또는 발사 위치
- 이동 방식
- 방어 성능
- 에너지 관련 성능

---

## 예상 슬롯

초기 예상 슬롯은 다음과 같습니다.

- Torso
- Legs
- Arms
- WeaponRight
- WeaponLeft
- Accessories

초기 구현에서는 모든 슬롯을 완성하려 하지 않습니다.  
먼저 소수 슬롯과 테스트 파츠로 구조를 검증합니다.

---

## 장비 변경 흐름

장비 변경은 서버 권한으로 처리합니다.

```text
클라이언트가 장비 변경 요청
→ 서버가 요청 검증
→ 서버가 장비 상태 변경
→ SkeletalMesh 파츠 교체
→ Ability Grant / Remove
→ GameplayEffect 적용 / 제거
→ 장비 상태 Replication
→ 클라이언트 UI / 외형 갱신
```

---

## 데이터 구조 방향

파츠 데이터는 가능하면 데이터 에셋 기반으로 관리합니다.

예상 데이터:

- 파츠 ID
- 슬롯 타입
- 표시 이름
- SkeletalMesh
- 부여할 Ability 목록
- 적용할 GameplayEffect 목록
- 부여할 GameplayTag 목록
- 장착 조건
- 설명 텍스트

---

## 구현 주의점

- 처음부터 대량의 파츠를 만들지 않습니다.
- 파츠 시스템이 검증되기 전에는 콘텐츠 양을 늘리지 않습니다.
- 장비 변경 로직과 GAS Grant 로직을 분리하되, 흐름은 추적 가능해야 합니다.
- 클라이언트 로컬에서만 장비가 바뀌는 구조를 피합니다.
- Late Join 시에도 장착 상태가 올바르게 보여야 합니다.
