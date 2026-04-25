# Work Log Rules

## 목적

작업 로그는 다음 목적으로 작성합니다.

- 오늘 무엇을 했는지 기록
- 다음 작업을 빠르게 파악
- 포트폴리오 작성 자료 확보
- AI를 어떻게 활용했는지 추적
- 기술적 의사결정 근거 보존

---

## 작성 기준

의미 있는 작업을 수행했다면 작업 로그를 남깁니다.

의미 있는 작업의 예시:

- 새 기능 구현
- 버그 수정
- 구조 변경
- GAS / Replication 관련 변경
- 에셋 구조 결정
- 중요한 기술적 의사결정
- 성능 문제 분석
- 빌드 / 서버 실행 문제 해결

---

## 작업 로그 템플릿

```md
# Work Log - YYYY-MM-DD

## Branch

`branch-name`

## Task Summary

- 작업 요약

## Files Changed

- `FilePath`: 변경 이유

## Implementation Summary

- 구현 내용 요약

## Networking Impact

- 서버 / 클라이언트 책임
- Replication 영향
- RPC 영향
- Dedicated Server 관련 사항

## GAS Impact

- Ability 변경
- GameplayEffect 변경
- GameplayTag 변경
- Attribute 변경
- Grant / Remove 변경

## Problems

- 발생한 문제

## Decisions / Solutions

- 해결 방식
- 선택한 이유

## TODO

- 남은 작업

## Portfolio Summary

- 포트폴리오에 사용할 수 있는 한 줄 요약
```

---

## 작성 원칙

- 사실 기반으로 작성합니다.
- 과장하지 않습니다.
- 실제로 구현한 것과 계획을 구분합니다.
- 문제와 해결 과정을 남깁니다.
- AI가 한 일과 본인이 판단한 일을 구분할 수 있으면 좋습니다.
