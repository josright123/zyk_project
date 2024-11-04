---
Title: Mermaid Blocks

---

# Mermaid Blocks
```mermaid
graph LR
    A[Application Layer] --> B[Driver API Layer]
    B --> C[Implementation Layer]
    C --> D[Hardware Access Layer]
```
```mermaid
graph LR
    subgraph "Hardware Layer"
    A1[cspi_read_reg]
    A2[cspi_write_reg]
    A3[cspi_read_mem]
    end
        
    subgraph "Implementation Layer"
    C1[impl_dm9051_init]
    C2[impl_dm9051_rx]
    C3[cspi_core_reset]
    end
    subgraph "API Layer"
    B1[dm9051_init]
    B2[dm9051_rx]
    B3[dm9051_tx]
    end
    subgraph "Application"
    D1(main)
    end
```