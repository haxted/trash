#include <cstdint>
#include <iostream>

class Processor {
public:
  typedef struct {
    uint8_t regs[16];
    uint16_t sp, ip;
  } Cpu;
  Cpu initcpup = {0};
  Cpu* cpu = &initcpup;
  uint8_t mem[320];
  bool loggingOn;
  void exec() {
    if(loggingOn) {
    printf("IP: 0x%04x  OP: 0x%02x\n", this->cpu->ip, this->mem[this->cpu->ip]);
    for(int i = 0; i < 16; i++) {
      printf("R%d: 0x%02x \n", i, this->cpu->regs[i]);
    }
    }
    switch (this->mem[this->cpu->ip]) {
    case 0xEA: {
      this->cpu->ip += 1;
      break;
    }
    case 0xEF: {
      std::exit(-1);
      break;
    }
    case 0x00: {
      this->cpu->regs[0] = this->mem[this->cpu->ip+1];
      this->cpu->ip += 2;
      break;
    }
    case 0x10: {
      this->cpu->regs[0] += this->mem[this->cpu->ip + 1];
      this->cpu->ip += 2;
      break;
      
    }
    case 0x20: {
      this->cpu->ip = this->mem[this->cpu->ip + 1] | (this->mem[this->cpu->ip + 2] << 8);
      break; 
    }
    case 0x30: {
      this->cpu->regs[0] -= this->mem[this->cpu->ip+1];
      this->cpu->ip += 2;
      break;
    }
    case 0x40: {
      this->cpu->regs[0] *= this->mem[this->cpu->ip+1];
      this->cpu->ip += 2;
      break;
    }
    case 0x50: {
      this->cpu->regs[0] /= this->mem[this->cpu->ip+1];
      this->cpu->ip += 2;
      break;
    }
    case 0x60: {
      uint8_t op = this->mem[this->cpu->ip+1];
      this->cpu->ip+=3;
      break;
    }
    case 0x61: {
      char c = (char)this->mem[this->cpu->ip+1];
      printf("%c", c);
      this->cpu->ip += 2;
      break;
    }
    default: {
      printf("Invalid opcode 0x%2x \nHalting.\n", this->mem[this->cpu->ip]);
      std::exit(-1);
    }  
  }
  }
};

int main(int argc, char **argv) {
  Processor *processor = new Processor();
  if(strcmp(argv[1], "--logging-off")) {
    processor->loggingOn = false;
  }
  printf("H1825 Emulator, %lu bytes of memory.\n", sizeof(processor->mem));
  uint8_t program[320] = {
    0x61, 'H',
    0x61, 'e',
    0x61, 'l',
    0x61, 'l',
    0x61, 'o',
    0x61, ',',
    0x61, ' ',
    0x61, 'W',
    0x61, 'o',
    0x61, 'r',
    0x61, 'l',
    0x61, 'd',
    0x61, '!',
    0x61, '\n',
    0xEF
  };
  std::memcpy(processor->mem, program, sizeof(program));
//  while(processor->cpu->ip < sizeof(processor->mem)) processor->exec();
    std::string in;
    for(;;) {
      std::cout << "# ";
      std::cin >> in;
      if(in == "d") {
        for(int i = 0; i < sizeof(processor->mem); i++) {
          printf("0x%04x: 0x%02x\n", i, processor->mem[i]);
        }
      } else if(in == "q") {
        std::exit(0);
      } else if(in == "ed") {
        std::string mem;
        std::cin >> mem;
        uint16_t addr = static_cast<uint16_t>(std::stoul(mem, nullptr, 16));
        std::string val;
        std::cin >> val;
        uint8_t value = static_cast<uint16_t>(std::stoul(val, nullptr, 16));
        processor->mem[addr] = value;
        printf("0x%04x: 0x%02x\n", addr, value);
      } else if(in == "ex") {
        std::string addr;
        std::cin >> addr;
        uint16_t actualAddr = static_cast<uint16_t>(std::stoul(addr, nullptr, 16));
        processor->cpu->ip = actualAddr;
        while(processor->cpu->ip < sizeof(processor->mem)) processor->exec();
      }
    }
  delete processor;
}
