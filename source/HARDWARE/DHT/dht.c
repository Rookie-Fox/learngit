#include "stm32f1xx_hal.h"
#include "dht.h"

#define DAT_LOW HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define DAT_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
 
#define DAT_VALUE HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)

uint8_t data[5];


void delay_us(uint32_t us)//΢����ʱ
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
    {
        ;
    }
}

void DHT_GPIO_Init(uint32_t Mode) //��ʼ��GPIO�ڣ�PC0���¶ȴ�����DAT
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = Mode;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void DHT11_Start() //DHT11��ʼ����ʼ�ź�
{
    DHT_GPIO_Init(GPIO_MODE_OUTPUT_PP);
    DAT_HIGH;
    DAT_LOW;
    HAL_Delay(30);
    DAT_HIGH;
    
    DHT_GPIO_Init(GPIO_MODE_INPUT);
    //������DHT�źţ��������DHT�ź�ʱ��ͼ�ͽ�������
    while(DAT_VALUE);
    //dht=0
    while(!DAT_VALUE);
    //dht=1
    while(DAT_VALUE);//֮��ʼ��������
    //dht=0
}


uint8_t* DHT11_Read() // ����DHT11����
{
    int i;
    int j;
    char temp; //��λ��һ���Զ�ȡ8��bit�������temp��
    char flag; //��־λ
    
    DHT11_Start(); //�����������ģʽ��ŷ�������
    
    DHT_GPIO_Init(GPIO_MODE_INPUT); //PC0����Ϊ����ģʽ
    
    for(i = 0;i<5;i++) //ȡ5���ֽ�
    {
        for(j = 0;j < 8 ;j++) //ÿ���ֽ�ȡ8bit
        {
            while(!DAT_VALUE); //�ȴ����ݵ��������������0��Ϊ1
            delay_us(40); //��ʱ��ȡ
            if(DAT_VALUE == 1)
            {
                flag = 1;
                while(DAT_VALUE); //�ȴ���λ��70us����Ϊ��һλ
            }else{
                flag = 0;
            }
            temp = temp << 1; //����һλ��Ϊ��ʹ�ȳ�����bit����λ
            temp |= flag;
        }
        data[i] = temp ;//�õ�8��bitΪһ���ֽڣ������������
    }
    return data;
}
