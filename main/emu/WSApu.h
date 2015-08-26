typedef struct sound {
    int on;
    int freq;
    int volL;
    int volR;
} SOUND;
typedef struct sweep {
    int on;
    int time;
    int step;
    int cnt;
} SWEEP;
typedef struct noise {
    int on;
    int pattern;
} NOISE;

extern SOUND Ch[4];
extern SWEEP Swp;
extern NOISE Noise;

extern char VoiceOn;
extern short Sound[7];

inline int apuBufLen(void);
inline void apuWaveVolume(int);
inline void apuWaveCreate(void);
inline void apuWaveDel(void);
inline void apuWaveClear(void);
inline void apuInit(void);
inline void apuEnd(void);
unsigned int apuMrand(unsigned int);
inline void apuSetPData(int, unsigned char);
unsigned char apuVoice(void);
inline void apuSweep(void);
WORD apuShiftReg(void);
inline void apuWaveSet(void);
inline void apuStartupSound(void);
