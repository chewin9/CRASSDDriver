# SSD

SSD NAND 데이터를 파일 시스템에 저장하고, 다양한 SSD 명령(`write`, `read`, `erase`)을 처리하는 C++ 프로젝트입니다.

## 주요 기능

- **Write**: 지정된 LBA에 값을 기록하고 파일로 저장  
- **Read**: 지정된 LBA의 값을 읽어 출력  
- **Erase**: 지정된 LBA 범위(`erase_size`)만큼 데이터를 삭제  
- **FileIO 유틸리티**: `GenFolderAndEmptyFiles`, `EraseAllFiles`를 통해 버퍼 디렉터리 생성 및 파일 관리  
- **커맨드 패턴**: `ICommand` 인터페이스 기반으로 `WriteCommand`, `ReadCommand`, `EraseCommand` 구현  

## 시스템 요구 사항

- C++17 이상

## 빌드 및 실행
