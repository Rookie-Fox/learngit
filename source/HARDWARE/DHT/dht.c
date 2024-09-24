#include "stm32f1xx_hal.h"
#include "dht.h"

#define DAT_LOW HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define DAT_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
 
#define DAT_VALUE HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)

uint8_t data[5];


void delay_us(uint32_t us)//微妙延时
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
    {
        ;
    }
}

void DHT_GPIO_Init(uint32_t Mode) //初始化GPIO口，PC0接温度传感器DAT
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = Mode;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void DHT11_Start() //DHT11初始化开始信号
{
    DHT_GPIO_Init(GPIO_MODE_OUTPUT_PP);
    DAT_HIGH;
    DAT_LOW;
    HAL_Delay(30);
    DAT_HIGH;
    
    DHT_GPIO_Init(GPIO_MODE_INPUT);
    //下面是DHT信号，如果符合DHT信号时序图就进行运行
    while(DAT_VALUE);
    //dht=0
    while(!DAT_VALUE);
    //dht=1
    while(DAT_VALUE);//之后开始传输数据
    //dht=0
}


uint8_t* DHT11_Read() // 接收DHT11数据
{
    int i;
    int j;
    char temp; //移位，一次性读取8个bit并存放在temp中
    char flag; //标志位
    
    DHT11_Start(); //重启进入高速模式后才发送数据
    
    DHT_GPIO_Init(GPIO_MODE_INPUT); //PC0设置为输入模式
    
    for(i = 0;i<5;i++) //取5个字节
    {
        for(j = 0;j < 8 ;j++) //每个字节取8bit
        {
            while(!DAT_VALUE); //等待数据到来，数据来会从0变为1
            delay_us(40); //延时读取
            if(DAT_VALUE == 1)
            {
                flag = 1;
                while(DAT_VALUE); //等待变位，70us后会变为下一位
            }else{
                flag = 0;
            }
            temp = temp << 1; //左移一位，为了使先出来的bit到高位
            temp |= flag;
        }
        data[i] = temp ;//得到8个bit为一个字节，存放在数组中
    }
    return data;
}
