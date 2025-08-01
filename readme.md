# SSD

SSD NAND 데이터를 파일 시스템에 저장하고, 다양한 SSD 명령(`write`, `read`, `erase`)을 처리하는 C++ 프로젝트입니다.

## 주요 기능

- **Write**: 지정된 LBA에 값을 기록하고 파일로 저장  
- **Read**: 지정된 LBA의 값을 읽어 출력  
- **Erase**: 지정된 LBA 범위(`erase_size`)만큼 데이터를 삭제  
- **Flush**: SSD가 캐싱 중인 데이터를 NAND에 저장
- **FileIO 유틸리티**: `GenFolderAndEmptyFiles`, `EraseAllFiles`를 통해 버퍼 디렉터리 생성 및 파일 관리  
- **커맨드 패턴**: `ICommand` 인터페이스 기반으로 `WriteCommand`, `ReadCommand`, `EraseCommand` 구현  

## 시스템 요구 사항

- C++17 이상

## 빌드 및 실행


# TestShell 

사용자가 SSD Driver를 이용할 수 있게 명령어와 테스트 스크립트를 제공하는 c++ 프로젝트입니다.  

## 주요 기능

- **write** : SSD Driver를 통해 지정된 LBA에 값을 기록하고 파일로 저장
- **read** : SSD Driver를 통해 지정된 LBA에 적힌 값을 확인 후 출력
- **fullwrite** : SSD Driver를 통해 LBA 0부터 LBA 99까지 동일한 값을 저장
- **fullread** : SSD Driver를 통해 LBA 0부터 LBA 99까지의 값을 출력
- **erase** : SSD Driver를 통해 지정된 LBA부터 size만큼의 데이터를 0으로 삭제
- **erase_range** : SSD Driver를 통해 start LBA부터 end LBA까지의 데이터를 0으로 삭제
- **flush** : SSD driver에 캐싱되어있는 데이터를 nand에 저장
- **help** : 각 명령어에 대한 주요 설명

## 시스템 요구사항 
- <filesystem> 라이브러리를 이용하므로 C++17 이상 환경이 필요합니다.  

## 스크립트 사용 지원 

