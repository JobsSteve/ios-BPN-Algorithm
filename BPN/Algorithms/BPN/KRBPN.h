//
//  KRBPN.h
//  BPN V1.4 ( 倒傳遞類神經網路 ; 本方法使用其中的 EBP 誤差導傳遞類神經網路建構 )
//
//  Created by Kalvar on 13/6/28.
//  Copyright (c) 2013 - 2014年 Kuo-Ming Lin (Kalvar). All rights reserved.
//
/*
 * @ 3 層架構
 *   - 輸入層
 *   - 隱藏層
 *   - 輸出層
 */
#import <Foundation/Foundation.h>

/*
 * @ 儲存訓練過後的 BPN Network 數據資料
 */
#import "KRBPNTrainedNetwork.h"

/*
 * @ 訓練完成時
 *   - success     : 是否訓練成功
 *   - trainedInfo : 訓練後的 Network 資料
 */
typedef void(^KRBPNTrainingCompletion)(BOOL success, NSDictionary *trainedInfo, NSInteger totalTimes);

/*
 * @ 每一次的迭代資料
 *   - times       : 訓練到了第幾代
 *   - trainedInfo : 本次訓練的 Network 資料
 */
typedef void(^KRBPNEachGeneration)(NSInteger times, NSDictionary *trainedInfo);

/*
 * @ 當前訓練的 BPN Network 數據資料
 *   - trainedInfo = @{};
 *      - KRBPNTrainedInfoInputWeights      : NSMutableArray, 調整後的輸入層各向量值到隱藏層神經元的權重
 *      - KRBPNTrainedInfoHiddenWeights     : NSMutableArray, 調整後的隱藏層神經元到輸出層神經元的權重值
 *      - KRBPNTrainedInfoHiddenBiases      : NSMutableArray, 調整後的隱藏層神經元的偏權值
 *      - KRBPNTrainedInfoOutputBias        : double,         調整後的輸出層神經元偏權值
 *      - KRBPNTrainedInfoOutputResults     : NSArray,        輸出結果
 *      - KRBPNTrainedInfoTrainedGeneration : NSInteger,      已訓練到第幾代
 *
 */
static NSString *KRBPNTrainedInfoInputWeights      = @"KRBPNTrainedInfoInputWeights";
static NSString *KRBPNTrainedInfoHiddenWeights     = @"KRBPNTrainedInfoHiddenWeights";
static NSString *KRBPNTrainedInfoHiddenBiases      = @"KRBPNTrainedInfoHiddenBiases";
static NSString *KRBPNTrainedInfoOutputBias        = @"KRBPNTrainedInfoOutputBias";
static NSString *KRBPNTrainedInfoOutputResults     = @"KRBPNTrainedInfoOutputResults";
static NSString *KRBPNTrainedInfoTrainedGeneration = @"KRBPNTrainedInfoTrainedGeneration";

@protocol KRBPNDelegate;

@interface KRBPN : NSObject
{
    
}

//Setup attribute is strong that 'coz we want to keep the delegate when the training run in the other queue.
@property (nonatomic, strong) id<KRBPNDelegate> delegate;
//輸入層各向量值之陣列集合
@property (nonatomic, strong) NSMutableArray *inputs;
//輸入層各向量值到第 1 層隱藏層神經元的權重
@property (nonatomic, strong) NSMutableArray *inputWeights;
//隱藏層神經元到輸出層神經元的權重值
@property (nonatomic, strong) NSMutableArray *hiddenWeights;
//隱藏層神經元的偏權值
@property (nonatomic, strong) NSMutableArray *hiddenBiases;
//隱藏層有幾顆神經元
@property (nonatomic, assign) NSInteger countHiddenNets;
//輸出層神經元偏權值
@property (nonatomic, assign) double outputBias;
//輸出層的輸出值( 輸出結果 )
@property (nonatomic, strong) NSArray *outputResults;
//所有輸入向量( 每一組訓練資料 )的各別輸出期望值
@property (nonatomic, strong) NSMutableArray *outputGoals;
//學習速率
@property (nonatomic, assign) CGFloat learningRate;
//收斂誤差值 ( 10^-3, 10^-6 )
@property (nonatomic, assign) double convergenceError;
//活化函式的 Alpha Value ( LMS 的坡度 )
@property (nonatomic, assign) float fOfAlpha;
//訓練迭代次數上限
@property (nonatomic, assign) NSInteger limitGeneration;
//目前訓練到第幾代
@property (nonatomic, assign) NSInteger trainingGeneration;
//是否正在訓練中
@property (nonatomic, assign) BOOL isTraining;
//當前訓練後的資料
@property (nonatomic, strong) NSDictionary *trainedInfo;
//取出儲存在 NSUserDefaults 裡訓練後的完整 BPN Network 數據資料
@property (nonatomic, readwrite) KRBPNTrainedNetwork *trainedNetwork;

@property (nonatomic, copy) KRBPNTrainingCompletion trainingCompletion;
@property (nonatomic, copy) KRBPNEachGeneration eachGeneration;

+(instancetype)sharedNetwork;
-(instancetype)init;

#pragma --mark Settings Public Methods
-(void)addPatterns:(NSArray *)_patterns outputGoal:(float)_goal;
-(void)addPatternWeights:(NSArray *)_weights;
-(void)addHiddenLayerNetBias:(float)_netBias netWeight:(float)_netWeight;

#pragma --mark Training Public Methods
-(void)randomWeights;
-(void)training;
-(void)trainingDoneSave;
-(void)trainingWithRandom;
-(void)trainingWithRandomAndSave;
-(void)pause;
-(void)continueTraining;
-(void)reset;
-(void)useTrainedNetworkToOutput;

#pragma --mark Trained Network Public Methods
-(void)saveTrainedNetwork;
-(void)removeTrainedNetwork;
-(void)recoverTrainedNetwork:(KRBPNTrainedNetwork *)_recoverNetworks;
-(void)recoverTrainedNetwork;

#pragma --mark Blocks
-(void)setTrainingCompletion:(KRBPNTrainingCompletion)_theBlock;
-(void)setEachGeneration:(KRBPNEachGeneration)_theBlock;

@end

@protocol KRBPNDelegate <NSObject>

@optional
-(void)krBPNDidTrainFinished:(KRBPN *)krBPN trainedInfo:(NSDictionary *)trainedInfo totalTimes:(NSInteger)totalTimes;
-(void)krBPNEachGeneration:(KRBPN*)krBPN trainedInfo:(NSDictionary *)trainedInfo times:(NSInteger)times;

@end
