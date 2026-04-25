# Project Overview

## 프로젝트 이름

OVERDRIVE

---

## 프로젝트 목적

이 프로젝트는 취업 포트폴리오용 Unreal Engine 게임 프로젝트입니다.

상업 출시나 인디게임 완성도보다, 게임 클라이언트 프로그래머로서의 기술 역량 증명이 우선입니다.

핵심 목표는 다음입니다.

- Unreal Engine C++ 기반 개발 역량 증명
- GAS 활용 능력 증명
- Dedicated Server 기반 멀티플레이 구조 경험
- AI 활용 개발 파이프라인 구축
- 가능하면 Unreal Insights 등을 활용한 최적화 경험 확보

---

## 엔진 / 기술 스택

- Engine: Unreal Engine 5.7
- Language: C++
- Ability System: GAS
- Server: Dedicated Server 기준
- AI 활용:
  - 코딩 보조
  - 3D 에셋 생성
  - 언리얼 에이전트 활용
- 최적화:
  - Unreal Insights 활용 가능성 고려
  - Replication 최적화
  - AI 최적화
  - 이펙트 / 프레임 최적화

---

## 게임 장르 / 구조

장르는 다음 방향입니다.

**Dedicated Server 기반 Co-op PvE Mission TPS**

플레이어는 로비에서 메카 파츠를 선택하고, 미션 지역에 진입해 선형 전투 구간을 돌파한 뒤, 거대 보스를 처치하고 결과 화면으로 이동합니다.

---

## 핵심 게임 루프

```text
로비 생성 / 접속
→ 로비 커스터마이징
→ 게임 시작
→ 전진형 PvE 전투
→ 거대 보스 처치
→ 결과 화면
```

---

## 포함할 요소

- 로비 생성 / 접속
- 로비 내 플레이어 Ready 상태
- 로비 내 메카 파츠 커스터마이징
- Dedicated Server 기반 게임 시작 흐름
- 선형 미션형 전투
- 다수 적 처치
- 거대 보스전
- 결과 화면

---

## 초기 버전에서 제외할 요소

- 로그라이크 성장
- 오픈월드
- PvP
- 과도한 커스터마이징
- 상용 출시 수준의 콘텐츠 양

---

## 메인 플레이어 에셋

메인 플레이어 에셋은 Fab의 **Mech Constructor**를 사용합니다.

선택 이유:

- Unreal Engine 지원 확인
- 모든 파츠가 SkeletalMesh로 존재
- 파츠 포즈 동기화 가능
- 런타임 파츠 교체 가능
- `weapon_l`, `weapon_r` 본이 있어 무기 소켓 / 발사 위치 확보 가능
- 파츠 기반 GAS 시스템과 잘 맞음

---

## 개발 시작 우선순위

첫 개발 목표는 다음입니다.

**Dedicated Server 로비 + 플레이어 파츠 커스터마이징 프로토타입**

초기 개발 우선순위:

1. UE 5.7 C++ 프로젝트 세팅
2. Dedicated Server 빌드 / 실행 구조 확인
3. 로비 생성 / 접속 흐름 구현
4. 로비 내 플레이어 Ready 상태 구현
5. Mech Constructor 에셋 적용
6. 파츠 SkeletalMesh 동기화 구조 구현
7. 런타임 파츠 교체 구현
8. 파츠 장착 정보 Replication
9. 파츠 장착에 따른 GAS Ability Grant / Remove 구조 구현
