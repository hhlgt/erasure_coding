## Erasure_Coding

A repository for encoding and decoding methods of some popular Erasure Codes, including the implementation of partial encoding methods for `encoding-and-transferring`.  

Erasure Codes include 

- Reed-Solomon Codes
- Locally Repairable Codes
  - Azure-LRC
  - Azure-LRC+1
  - Optimal LRC
  - Optimal Cauchy LRC
  - Uniform Cauchy LRC
- Product Codes

------

### Quick Start

- environment

  - cmake 3.15.0
  - gcc 9.4.0

- install third party

  ```
  sh install_jerasure.sh
  ```

- compile

  ```
  sh compile.sh
  ```

- create test data

  ```
  sh create_dirs.sh
  ```

- run

  ```
  cd build
  ./test_xx
  ```

  