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

## 🧪 TestScript 목록 및 시나리오

---

### **1. FullWriteAndReadCompare**
- **실행 명령어**: `1_` 또는 `1_FullWriteAndReadCompare`

#### 🔹 Test Scenario
- LBA 0 ~ 4: Write
- LBA 0 ~ 4: ReadCompare  
- LBA 5 ~ 9: 다른 값으로 Write  
- LBA 5 ~ 9: ReadCompare  
- LBA 10 ~ 14: 다른 값으로 Write  
- LBA 10 ~ 14: ReadCompare  
- … 위와 같은 규칙으로 전체 LBA 영역에 대해 Full Write + ReadCompare 수행

---

### **2. PartialLBAWrite**
- **실행 명령어**: `2_` 또는 `2_PartialLBAWrite`

#### 🔹 Test Scenario
- Loop: **30회**
- LBA 4: 값 쓰기  
- LBA 0: 같은 값 쓰기  
- LBA 3: 같은 값 쓰기  
- LBA 1: 같은 값 쓰기  
- LBA 2: 같은 값 쓰기  
- LBA 0 ~ 4: ReadCompare

---

### **3. WriteReadAging**
- **실행 명령어**: `3_` 또는 `3_WriteReadAging`

#### 🔹 Test Scenario
- Loop: **200회**
- LBA 0: 랜덤 값 Write  
- LBA 99: 랜덤 값 Write  
- LBA 0, 99: ReadCompare

---

### **4. EraseAndWriteAging**
- **실행 명령어**: `4_` 또는 `4_EraseAndWriteAging`

#### 🔹 Test Scenario
- LBA 0 ~ 2: Erase  
- Loop: **30회**  
  - LBA 2: Write  
  - LBA 2: Overwrite  
  - LBA 2 ~ 4: Erase  
  - LBA 2, 4: ReadCompare  
  - LBA 4: Write  
  - LBA 4: Overwrite  
  - LBA 4 ~ 6: Erase  
  - LBA 4 ~ 6: ReadCompare  
  - LBA 6: Write  
  - LBA 6: Overwrite  
  - LBA 6 ~ 8: Erase  
  - LBA 6 ~ 8: ReadCompare
