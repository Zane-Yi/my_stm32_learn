# STM32 项目架构说明

## 目录结构

```
my_stm32_learn/
├── Hard/           # 硬件抽象层 (HAL)
├── Driver/         # 驱动层
├── App/            # 应用层
└── src/            # 系统启动和中断
```

## 分层架构

### 1. Hard (硬件抽象层)
**职责**: 直接操作硬件寄存器，提供基础硬件初始化接口

- `LED.c/h` - LED GPIO初始化
- `KEY.c/h` - 按键GPIO初始化
- `BEEP.c/h` - 蜂鸣器GPIO初始化
- `PWM.c/h` - PWM定时器初始化
- `USART.c/h` - 串口初始化

**特点**:
- 只负责硬件配置，不包含业务逻辑
- 提供最底层的硬件访问接口

### 2. Driver (驱动层)
**职责**: 封装硬件操作，提供功能性驱动接口

#### `breathing_led.c/h` - 呼吸灯驱动
提供呼吸灯控制接口：
- `BreathingLed_Init()` - 初始化呼吸灯
- `BreathingLed_Enable()` - 启用呼吸灯
- `BreathingLed_Disable()` - 禁用呼吸灯
- `BreathingLed_SetColor()` - 设置颜色（红/绿/蓝）
- `BreathingLed_Update()` - 更新呼吸灯状态（周期调用）

**设计亮点**:
- 封装了PWM渐变逻辑
- 支持三种颜色切换
- 状态机管理（启用/禁用）

#### `key_event.c/h` - 按键事件管理器
提供事件驱动的按键处理机制：
- `KeyEvent_Init()` - 初始化事件系统
- `KeyEvent_Register()` - 注册按键事件回调
- `KeyEvent_Process()` - 处理按键事件（周期调用）

**设计亮点**:
- 事件注册机制，解耦按键检测和业务逻辑
- 支持按下/释放两种事件
- 可注册多个按键，每个按键独立回调
- 自动消抖处理

### 3. App (应用层)
**职责**: 实现业务逻辑，协调各驱动模块

#### `Task_Scheduler.c/h` - 任务调度器
- `Task_Scheduler_Init()` - 初始化应用（注册按键回调）
- `System_Task_Runner()` - 主循环任务调度

**业务逻辑**:
- KEY1 (PA0): 切换呼吸灯开/关 + 触发蜂鸣器
- KEY2 (PC13): 切换呼吸灯颜色（红→绿→蓝）

**特点**:
- 不直接调用硬件层
- 通过驱动层接口实现功能
- 业务逻辑清晰，易于修改

## 数据流向

```
用户按键 → Hard层检测GPIO → Driver层触发事件 → App层回调处理 → Driver层执行动作 → Hard层操作硬件
```

## 重构前后对比

### 重构前
```c
// Task_Scheduler.c 中混杂了所有逻辑
if(KEY_Process(&KEY_beep) == KEY_ON) {
    Start_Beep_Task();
    led_enabled++;
    __HAL_TIM_SET_COMPARE(...);  // 直接操作寄存器
}
```

### 重构后
```c
// App层只处理业务逻辑
void OnKey_Beep_Pressed(void* user_data) {
    BEEP_ON;
    BreathingLed_Enable();  // 通过驱动接口
}

// 在初始化时注册
KeyEvent_Register(&key_beep_handler, GPIOA, GPIO_PIN_0,
                  GPIO_PIN_SET, OnKey_Beep_Pressed, NULL, NULL);
```

## 优势

1. **分层清晰**: 硬件/驱动/应用三层分离
2. **解耦合**: 按键事件通过回调机制，不需要在调度器中轮询
3. **可维护**: 每层职责单一，修改某层不影响其他层
4. **可扩展**: 添加新按键只需注册回调，添加新驱动只需实现接口
5. **可测试**: 每层可独立测试

## 如何添加新功能

### 添加新按键
```c
// 1. 定义回调函数
void OnKey_New_Pressed(void* user_data) {
    // 你的逻辑
}

// 2. 在Task_Scheduler_Init()中注册
KeyEvent_Register(&key_new_handler, GPIOX, GPIO_PIN_X,
                  GPIO_PIN_SET, OnKey_New_Pressed, NULL, NULL);
```

### 添加新驱动
1. 在Driver/创建新的.c/.h文件
2. 实现初始化、控制、更新接口
3. 在App层调用驱动接口
