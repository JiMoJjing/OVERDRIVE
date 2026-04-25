# Dedicated Server / Networking Rules

## 기본 원칙

게임플레이 핵심 동작은 서버 권한을 기준으로 구현합니다.

클라이언트는 요청할 수 있지만, 최종 검증과 실행은 서버가 담당합니다.

---

## 서버 권한으로 처리할 것

- 장비 변경 검증
- Ability 부여 / 제거
- Ready 상태 확정
- 미션 시작 / 종료
- 피해 처리
- 사망 처리
- 보상 처리
- 결과 화면 상태
- 적 AI 및 보스 상태
- 중요한 게임플레이 판정

---

## 클라이언트가 담당할 수 있는 것

- 입력 수집
- UI 표시
- 로컬 예측 또는 시각적 피드백
- 서버 요청 전송
- Replicated 상태를 기반으로 한 화면 갱신

클라이언트의 로컬 결과를 권한 있는 결과로 취급하지 않습니다.

---

## Replication 규칙

- 클라이언트에게 필요한 데이터만 Replication합니다.
- 지속되어야 하는 상태는 Multicast보다 Replicated State를 우선 고려합니다.
- RPC는 이벤트성 요청이나 일회성 연출에 사용합니다.
- Replicated 변수는 목적이 명확해야 합니다.
- Late Join을 고려해야 하는 상태는 Replicated State로 관리합니다.

---

## RPC 사용 기준

### Server RPC

클라이언트가 서버에 요청할 때 사용합니다.

예시:

```text
ServerRequestChangePart
ServerSetReady
ServerRequestStartMission
```

서버는 요청을 검증해야 합니다.

### Client RPC

서버가 특정 클라이언트에게만 알려야 할 때 사용합니다.

예시:

```text
ClientShowErrorMessage
ClientOpenResultScreen
```

### Multicast RPC

모든 클라이언트에게 일회성 이벤트를 보여줄 때 사용합니다.

예시:

```text
MulticastPlayBossIntroEffect
```

지속 상태 동기화 용도로 남용하지 않습니다.

---

## 확인해야 할 네트워크 상황

네트워크 기능은 가능하면 다음 상황을 고려합니다.

- Listen Server
- Dedicated Server
- Client Join
- Late Join
- Server Travel
- Client Disconnect
- Replicated State 초기화
