# Work Log - 2026-04-25

## Branch

`feature/server-target`

## Task Summary

- Dedicated Server용 `Server Target` 추가 가능 여부를 점검했습니다.
- 프로젝트 구조와 문서 기준으로 현재 단계에서 `Server Target` 추가가 적절한지 확인했습니다.
- `Source/OVERDRIVEServer.Target.cs`를 추가해 Dedicated Server 빌드 타깃 구조를 구성했습니다.
- UE 5.7 환경에서 `OVERDRIVEServer` 빌드를 시도해 현재 엔진 배포본 제약을 확인했습니다.
- 이번 작업 내용을 Notion 정리용 마크다운으로 문서화했습니다.

## Files Changed

- `Source/OVERDRIVEServer.Target.cs`: Dedicated Server용 Unreal TargetRules 추가
- `Docs/WorkLogs/2026-04-25.md`: 이번 채팅 작업 로그 기록

## Implementation Summary

- 현재 프로젝트에는 `OVERDRIVE.Target.cs`와 `OVERDRIVEEditor.Target.cs`만 존재하는 상태를 확인했습니다.
- 런타임 모듈이 `OVERDRIVE` 하나인 단순 구조여서 `Server Target` 추가 범위로 작업을 제한했습니다.
- `OVERDRIVEServerTarget` 클래스를 추가하고 `Type = TargetType.Server`로 Dedicated Server 빌드 타깃을 정의했습니다.
- 기존 타깃과 동일하게 `BuildSettingsVersion.V6`와 `EngineIncludeOrderVersion.Unreal5_7`를 사용해 설정 일관성을 유지했습니다.
- 이후 UE 5.7 `Build.bat`로 `OVERDRIVEServer Win64 Development` 빌드를 시도해 실제 인식 여부를 검증했습니다.

## Networking Impact

- 이번 작업은 네트워크 게임플레이 로직 변경이 아니라 Dedicated Server용 빌드 엔트리 추가 작업입니다.
- 서버/클라이언트 책임 분리 로직, RPC, Replication 변수는 변경하지 않았습니다.
- Dedicated Server 환경에서 이후 로비/Ready/커스터마이징 구조를 검증할 수 있는 기반만 추가했습니다.

## GAS Impact

- GAS 관련 변경 없음
- Ability 변경 없음
- GameplayEffect 변경 없음
- GameplayTag 변경 없음
- Attribute 변경 없음
- Grant / Remove 변경 없음

## Problems

- 최초 서버 빌드 시도에서 UnrealBuildTool 로그 파일 접근 권한 문제로 빌드가 중단되었습니다.
- 권한 상승 후 다시 빌드했지만 현재 설치된 UE 5.7 엔진에서 `Server targets are not currently supported from this engine distribution.` 오류가 발생했습니다.

## Decisions / Solutions

- 권한 문제는 로컬 실행 환경 이슈로 보고 권한 상승으로 재시도했습니다.
- 최종 오류는 프로젝트 코드 문제가 아니라 현재 엔진 배포본의 Dedicated Server 빌드 제한으로 판단했습니다.
- 현재 마일스톤에서는 패키징/실서버 빌드가 우선순위가 아니므로 Unreal Engine 소스 빌드 전환은 보류했습니다.
- 이번 작업 범위는 `Server Target` 추가와 빌드 가능성 확인까지로 제한했습니다.

## TODO

- 이후 Dedicated Server 실제 빌드/실행 검증이 필요해지는 시점에 UE 5.7 소스 빌드 환경을 준비
- 소스 빌드 엔진 환경에서 `OVERDRIVEServer` 빌드 재검증
- 다음 마일스톤 기능으로 로비, Ready 상태, 메카 파츠 커스터마이징, Replication 구조 작업 진행

## Portfolio Summary

- Unreal Engine C++ 프로젝트에 Dedicated Server용 빌드 타깃을 직접 추가해 멀티플레이 지향 구조를 정리했습니다.
- 코드 이슈와 엔진 배포 환경 이슈를 분리해 판단했고, 현재 범위를 넘는 소스 빌드 전환은 의도적으로 미뤘습니다.
- 포트폴리오 관점에서 서버 권한 기반 프로젝트를 염두에 둔 빌드 구조 설계와 범위 통제 판단을 함께 보여줄 수 있는 작업입니다.
