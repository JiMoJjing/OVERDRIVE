# GitHub Workflow Rules

## 기본 원칙

GitHub는 단순 백업이 아니라 개발 과정과 작업 단위를 보여주는 포트폴리오 자료로 사용합니다.

커밋과 브랜치는 작고 명확하게 관리합니다.

---

## 브랜치 규칙

브랜치 이름은 다음 형식을 따릅니다.

```text
feature/<feature-name>
fix/<bug-name>
docs/<document-name>
refactor/<target-name>
```

예시:

```text
feature/lobby-ready-state
feature/mech-equipment-component
feature/gas-ability-grant
fix/equipment-replication-desync
docs/update-gas-rules
refactor/split-equipment-runtime-data
```

---

## 브랜치 사용 기준

### feature

새 기능 추가 시 사용합니다.

예시:

- 로비 Ready 상태 추가
- 메카 파츠 장착 기능 추가
- GAS Ability Grant 구조 추가

### fix

버그 수정 시 사용합니다.

예시:

- 클라이언트 장비 외형 동기화 오류 수정
- Ready 상태 Replication 누락 수정
- Dedicated Server 실행 오류 수정

### docs

문서 수정 시 사용합니다.

예시:

- AGENTS.md 수정
- GAS 규칙 문서 추가
- 작업 로그 템플릿 수정

### refactor

동작 변화보다 구조 개선이 중심일 때 사용합니다.

예시:

- 장비 데이터와 런타임 상태 분리
- 컴포넌트 책임 분리
- 중복 코드 제거

---

## 커밋 메시지 규칙

커밋 메시지는 다음 형식을 따릅니다.

```text
type: summary
```

사용할 type:

- `feat`: 새 기능
- `fix`: 버그 수정
- `docs`: 문서 수정
- `refactor`: 리팩토링
- `test`: 테스트 추가 / 수정
- `chore`: 빌드, 설정, 기타 작업

예시:

```text
feat: add lobby ready state replication
fix: resolve equipment desync on clients
docs: add GAS equipment rules
refactor: split mech equipment data from runtime state
```

---

## 커밋 단위

가능하면 하나의 커밋에는 하나의 논리적 변경만 포함합니다.

피해야 할 예시:

```text
로비 구현 + GAS 수정 + 문서 수정 + UI 수정
```

좋은 예시:

```text
feat: add replicated lobby ready state
docs: add lobby ready state work log
fix: handle ready state reset on disconnect
```

---

## PR / 기록 방향

혼자 개발하더라도 PR 단위 또는 브랜치 단위 기록을 남기면 포트폴리오 설명에 도움이 됩니다.

PR 설명에는 다음을 포함합니다.

- 구현한 기능
- 서버 / 클라이언트 구조
- Replication 영향
- GAS 영향
- 테스트 내용
- 남은 TODO
