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


# TestShell 

사용자가 SSD Driver를 이용할 수 있게 명령어와 테스트 스크립트를 제공하는 c++ 프로젝트입니다.  

## 시스템 요구사항 
- <filesystem> 라이브러리를 이용하므로 C++17 이상 환경이 필요합니다.  

## 빌드 및 실행

- Debug 구성: 각 feature를 unit test로 검증한 google test를 실행할 수 있습니다. 
- Release 구성: SSDDriver와 TestShell을 통해 아래 기능을 실행할 수 있습니다. 

## 주요 기능

- **write** : SSD Driver를 통해 지정된 LBA에 값을 기록하고 파일로 저장
- **read** : SSD Driver를 통해 지정된 LBA에 적힌 값을 확인 후 출력
- **fullwrite** : SSD Driver를 통해 LBA 0부터 LBA 99까지 동일한 값을 저장
- **fullread** : SSD Driver를 통해 LBA 0부터 LBA 99까지의 값을 출력
- **erase** : SSD Driver를 통해 지정된 LBA부터 size만큼의 데이터를 0으로 삭제
- **erase_range** : SSD Driver를 통해 start LBA부터 end LBA까지의 데이터를 0으로 삭제
- **flush** : SSD driver에 캐싱되어있는 데이터를 nand에 저장
- **help** : 각 명령어에 대한 주요 설명

## TestScript
- **1_FullWriteAndReadCompare** : 1_ 혹은 1_FullWriteAndReadCompare 을 입력하여 실행 가능


- Test Scenario
• 0 ~ 4 번 LBA 까지 Write 명령어를 수행한다
• 0 ~ 4 번 LBA 까지 ReadCompare 수행
• 5 ~ 9 번 LBA 까지 다른 값으로 Write 명령어를 수행한다
• 5 ~ 9 번 LBA 까지 ReadCompare 수행
• 10 ~ 14 번 LBA 까지 다른 값으로 Write 명령어를 수행한다
• 10 ~ 14 번 LBA 까지 ReadCompare 수행
• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare 를 수행한다


- **2_PartialLBAWrite** : 2_ 혹은 2_PartialLBAWrite 을 입력하여 실행 가능

 
- Test Scenario
• Loop 는 30 회
• 4 번 LBA 에 값을 적는다
• 0 번 LBA 에 같은 값을 적는다
• 3 번 LBA 에 같은 값을 적는다
• 1 번 LBA 에 같은 값을 적는다
• 2 번 LBA 에 같은 값을 적는다
• LBA 0 ~ 4 번 , ReadCompare


- **3_WriteReadAging** : 3_ 혹은 3_WriteReadAging 을 입력하여 실행 가능

 
- Test Scenario
• Loop 200 회
• 0 번 LBA 에 랜덤 값을 적는다
• 99 번 LBA 에 랜덤 값을 적는다
• LBA 0 번과 99 번 , ReadCompare 를 수행

- **4_EraseAndWriteAging** : 4_ 혹은 4_EraseAndWriteAging 을 입력하여 실행 가능

 
- Test Scenario
• 0 ~ 2 번 LBA 삭제
• Loop 30 회
• 2 번 LBA Write
• 2 번 LBA OverWrite
• 2 ~ 4 번 LBA 삭제
• 2 4 번 ReadCompar e
• 4 번 LBA Write
• 4 번 LBA OverWrite
• 4 ~ 6 번 LBA 삭제
• 4 ~ 6 번 ReadCompare
• 6 번 LBA Write
• 6 번 LBA OverWrite
• 6 ~ 8 번 LBA 삭제
• 6 ~ 8 번 ReadCompare
