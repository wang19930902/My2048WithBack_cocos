#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	auto layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(180, 170, 160, 255));
	this->addChild(layerColorBG);

	//在上方加入游戏的分数  
	int txtAllHeight = visibleSize.height - visibleSize.width;
	int txtItemHeight = txtAllHeight / 8;


	//加入“分数”label  
	auto labelTTFCardNumberName = LabelTTF::create("SCORE", "HiraKakuProN-W6", txtItemHeight * 2);
	labelTTFCardNumberName->setPosition(Point(visibleSize.width / 2, visibleSize.height - txtItemHeight * 2));
	addChild(labelTTFCardNumberName);

	//加入具体的分数  
	labelTTFCardNumber = LabelTTF::create("0", "HiraKakuProN-w6", txtItemHeight * 2);
	labelTTFCardNumber->setPosition(Point(visibleSize.width / 2, visibleSize.height - txtItemHeight * 4));
	addChild(labelTTFCardNumber);

	// 加入撤销按钮
	auto backBtnText = LabelTTF::create("back", "HiraKakuProN-w6", txtItemHeight * 3);
	auto backMenuItem = MenuItemLabel::create(backBtnText,CC_CALLBACK_1(HelloWorld::btnCallback, this));
	auto backMenu = Menu::create(backMenuItem, NULL);
	backMenu->setPosition(Point(visibleSize.width / 2, visibleSize.height - txtItemHeight * 7));
	addChild(backMenu);

	createCardSprite(visibleSize);
	//创建2张自动生成卡片  
	autoCreateCardNumber();
	

    backDate();
    return true;
}


//事件监听回调：触摸开始  
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//获取触摸的X轴和Y轴  
	Point touchPoint = touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）  
	touch->getLocationInView();
	firstX = touchPoint.x;
	firstY = touchPoint.y;

	return true;
}

//事件监听回调：触摸结束  
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//获取X轴和Y轴的移动范围  
	Point touchPoint = touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）  
	endX = firstX - touchPoint.x;
	endY = firstY - touchPoint.y;

	//是否移动
	auto isDo = false;

	if(abs(endX) + abs(endY) < 50)
		return ;
	//判断X轴和Y轴的移动距离，如果X轴的绝对值大，则向左右滑动，如果Y轴的绝对值大，则向上下滑动  
	backDate();
	if (abs(endX) > abs(endY))
	{
		//手势向左右  
		//判断向左还是向右  
		if (endX >0)
		{
			//向左  
			isDo = doLeft();
			//判断游戏是否还能继续  
			doCheckGameOver();
			if (isDo){
				autoCreateCardNumber();
			}
		}
		else
		{
			//向右  
			isDo = doRight();
			//判断游戏是否还能继续  
			doCheckGameOver();
			if (isDo){
				autoCreateCardNumber();
			}
		}
	}
	else
	{
		//手势向上下  
		//判断手势向上还是向下  
		if (endY >0)
		{
			//向下  
			isDo = doDown();
			//判断游戏是否还能继续  
			doCheckGameOver();
			if (isDo){
				autoCreateCardNumber();

			}
		}
		else
		{
			//向上  
			isDo = doUp();
			//判断游戏是否还能继续  
			doCheckGameOver();
			if (isDo){
				autoCreateCardNumber();

			}
		}

	}
}

//滑向上下左右的方法  
bool HelloWorld::doUp()
{
	bool isdo = false;
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {

			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						//改变分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;
				}
			}

		}
	}
	return isdo;
}

bool HelloWorld::doDown()
{
	bool isdo = false;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y--;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						//改变分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;
				}
			}

		}
	}
	return isdo;
}

bool HelloWorld::doLeft()
{
	bool isdo = false;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			for (int x1 = x + 1; x1 < 4; x1++) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x--;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						//改变分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;
				}
			}

		}
	}
	return isdo;
}

bool HelloWorld::doRight()
{
	bool isdo = false;
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {

			for (int x1 = x - 1; x1 >= 0; x1--) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						//改变分数  
						score += cardArr[x][y]->getNumber();
						labelTTFCardNumber->setString(String::createWithFormat("%i", score)->getCString());
						isdo = true;
					}
					break;
				}
			}

		}
	}
	return isdo;
}

//创建卡片  
void HelloWorld::createCardSprite(cocos2d::Size size)
{
	//求出单元格的宽度和高度  
	int unitSize = (size.width - 28) / 4;

	//4*4的单元格  
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			CardSprite* card = CardSprite::createCardSprite(0, unitSize, unitSize, unitSize*i + 14, unitSize*j + 20);
			addChild(card);
			//把卡片加入到数组中  
			cardArr[i][j] = card;
		}
	}
}

//自动生成卡片  
void HelloWorld::autoCreateCardNumber()
{
	bool add_flag = false;
	for (int m = 0; m<4; m++){
		for (int n = 0; n<4; n++){
			if (cardArr[m][n]->getNumber() == 0){
				add_flag = true;
			}
		}
	}
	if (add_flag){
		int i = CCRANDOM_0_1() * 4;
		int j = CCRANDOM_0_1() * 4;
		//求出单元格的宽度和高度
		//判断是否这个位置已存在卡片 
		if (cardArr[i][j]->getNumber() > 0)
		{
			log("chuangjian");
			autoCreateCardNumber();
		}
		else
		{
			log("begin to create");
			//CardSprite* card = CardSprite::createCardSprite(2, unitSize, unitSize, unitSize*i+140, unitSize*j+20); 
			//cardArr[i][j]=card; 
			cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 5 ? 2 : 4);
		}
	}
}

//判断游戏是否还能继续  
void HelloWorld::doCheckGameOver(){
	bool isGameOver = true;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (cardArr[x][y]->getNumber() == 0 ||
				(x>0 && (cardArr[x][y]->getNumber() == cardArr[x - 1][y]->getNumber())) ||
				(x<3 && (cardArr[x][y]->getNumber() == cardArr[x + 1][y]->getNumber())) ||
				(y>0 && (cardArr[x][y]->getNumber() == cardArr[x][y - 1]->getNumber())) ||
				(y<3 && (cardArr[x][y]->getNumber() == cardArr[x][y + 1]->getNumber()))) {
				isGameOver = false;
			}
		}
	}

	if (isGameOver) {
		//游戏结束，重新开始游戏  
		log("游戏结束");
		Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
	}
	
}

void HelloWorld::btnCallback(Ref* pSender){
	/*for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				cardArr[i][j]->setNumber(backCardArr[i][j]);
			}
		}*/
	int x = cardBackArr.size();
	if ( x == 0)
	{
		return;
	}
	auto item = cardBackArr.back();
	int* back = item->GetData();
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			cardArr[i][j]->setNumber(back[i * 4 + j]);
		}
	}
	cardBackArr.popBack();
}

void HelloWorld::backDate(){
	auto item = CardBackItem::create();
	int back[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			back[i * 4 + j] = cardArr[i][j]->getNumber();
		}
	}
	item->AddData(back);
	cardBackArr.pushBack(item);
}
