# AGENTS.md

## 역할

이 파일은 Codex가 프로젝트 작업을 시작할 때 가장 먼저 읽는 최상위 지침입니다.

세부 규칙은 `Docs/` 폴더의 문서를 참조합니다.  
작업 성격에 따라 필요한 문서를 먼저 확인한 뒤 작업합니다.

---

## 새 채팅 시작 시 컨텍스트 복구 규칙

Codex는 새 채팅에서 작업을 시작할 때 다음 순서로 컨텍스트를 복구합니다.

1. 이 `AGENTS.md`를 기준 지침으로 삼습니다.
2. 작업 요청의 성격에 따라 `Docs/`의 관련 문서를 선별해 확인합니다.
3. 이어서 진행하는 작업이거나 최근 변경 맥락이 필요하면 `Docs/WorkLogs/`의 최근 작업 로그를 확인합니다.
4. 모든 문서를 무조건 읽기보다, 현재 작업 판단에 필요한 문서를 우선합니다.

---

## 프로젝트 이름

OVERDRIVE

---

## 프로젝트 요약

이 프로젝트는 Unreal Engine 5.7 기반 C++ 포트폴리오 프로젝트입니다.

목표는 상업 출시 수준의 콘텐츠 제작이 아니라, 게임 클라이언트 프로그래머로서 다음 역량을 증명하는 것입니다.

- Unreal Engine C++ 개발 역량
- GAS 활용 능력
- Dedicated Server 기반 멀티플레이 구조 이해
- 서버 권한 기반 게임플레이 구현 능력
- Modular Weapon Parts System 설계/구현 능력
- AI 활용 개발 파이프라인 구축 능력

게임 방향은 다음과 같습니다.

- 장르: Co-op PvE Mission TPS
- 흐름: 로비 → Ready → 미션 시작 → 선형 PvE 전투 → 총기 파츠 드랍/장착 → 거대 보스전 → 결과 화면
- 초기 목표: Dedicated Server 로비와 기본 인간형 플레이어, 총기 파츠 드랍/장착 프로토타입

자세한 프로젝트 내용은 다음 문서를 참조합니다.

- `Docs/project_overview.md`

---

## 최우선 판단 기준

구현 판단이 필요할 때는 다음 우선순위를 따릅니다.

1. 서버 권한 구조의 정확성
2. 포트폴리오에서 설명 가능한 구조
3. 단순하고 유지보수 가능한 구조
4. Unreal Engine 권장 방식 준수
5. GAS 구조의 일관성
6. Replication 구조의 정확성
7. 시각적 완성도
8. 콘텐츠 양

명시적인 요청이 없다면 프로젝트 범위를 임의로 확장하지 않습니다.

---

## 상황별 참조 문서

### 프로젝트 방향 확인이 필요할 때

- `Docs/project_overview.md`
- `Docs/scope_control_rules.md`

### Codex의 작업 방식, 응답 형식, 작업 단위를 정할 때

- `Docs/codex_workflow_rules.md`
- `Docs/definition_of_done.md`

### Unreal C++ 코드를 작성하거나 수정할 때

- `Docs/unreal_coding_convention.md`
- `Docs/unreal_development_rules.md`

### GAS 관련 작업을 할 때

- `Docs/gas_rules.md`
- `Docs/weapon_parts_rules.md`

### Dedicated Server, Replication, RPC, GameMode/GameState/PlayerState 관련 작업을 할 때

- `Docs/dedicated_server_networking_rules.md`
- `Docs/unreal_development_rules.md`

### 총기 파츠, 무기 장착, 런타임 커스터마이징 관련 작업을 할 때

- `Docs/weapon_parts_rules.md`
- `Docs/gas_rules.md`
- `Docs/dedicated_server_networking_rules.md`

### 브랜치 생성, 커밋, GitHub 작업을 할 때

- `Docs/github_workflow_rules.md`

### 작업이 끝났거나 기록이 필요할 때

- `Docs/work_log_rules.md`
- `Docs/notion_summary_rules.md`
- `Docs/definition_of_done.md`

---

## 기본 작업 규칙

작업 전에는 다음을 확인합니다.

- 작업 목표
- 수정 예상 파일
- 변경 범위
- 서버/클라이언트 책임
- GAS 영향 여부
- Replication 영향 여부

작업 중에는 다음을 지킵니다.

- 요청된 작업에 집중합니다.
- 관련 없는 리팩토링을 하지 않습니다.
- 새 플러그인, 외부 패키지, 큰 의존성을 임의로 추가하지 않습니다.
- 아키텍처를 조용히 변경하지 않습니다.
- 한 번에 너무 많은 기능을 구현하지 않습니다.
- 영리한 코드보다 명확한 코드를 우선합니다.
- 캐릭터 커스터마이징보다 총기 파츠 시스템과 전투 구조를 우선합니다.

작업 후에는 다음을 정리합니다.

- 변경 파일
- 구현 요약
- 게임플레이 영향
- 서버/클라이언트 구조 영향
- Replication 영향
- GAS 영향
- 필요한 에디터 설정 또는 에셋 설정
- 검증 내용
- 남은 TODO
- 다음 작업

---

## 금지 또는 주의할 행동

명시적인 승인 없이 다음 행동을 하지 않습니다.

- 큰 폴더 삭제
- 주요 아키텍처 재작성
- 관련 없는 플러그인 추가
- Marketplace 의존성 추가
- 엔진 버전 변경
- 프로젝트 장르 또는 핵심 루프 변경
- 핵심 C++ 시스템을 Blueprint 전용 구조로 변경
- 현재 마일스톤 밖의 대형 시스템 추가
- Force Push 또는 Git History 재작성
- 불필요한 Unreal Generated File 수정
- 복잡한 인벤토리, 파츠 등급, 랜덤 옵션, 시너지 빌드 시스템 추가

---

## Codex 응답 형식

작업 결과를 보고할 때는 다음 형식을 사용합니다.

```text
Summary
- 변경 요약

Files Changed
- 파일 경로: 변경 이유

Technical Notes
- Networking 영향
- GAS 영향
- Unreal Editor / Asset 설정 영향

Validation
- 확인한 내용
- 아직 확인이 필요한 내용

Next Step
- 권장 다음 작업
```

응답은 간결하게 작성하되, 포트폴리오 정리와 디버깅에 필요한 정보는 포함합니다.
