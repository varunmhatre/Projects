package
{
	import Box2D.Dynamics.Contacts.b2Contact;
	import citrus.objects.CitrusSprite;
	import citrus.objects.platformer.box2d.*;
	import citrus.physics.box2d.Box2D;
	import citrus.math.MathVector;
	import citrus.view.starlingview.StarlingCamera;
	import feathers.controls.Label;
	import flash.filesystem.File;
	import starling.events.Event;
	import flash.geom.*;
	import citrus.core.starling.StarlingState
	import citrus.core.CitrusEngine;
	import starling.display.Button;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	import starling.utils.AssetManager;
	    
	/**
	 * ...
	 * @author Varun
	 */
	public class Game2 extends StarlingState
	{
		public var hero:Hero;
		public var count:int;
		public var h:int;
		
		private var btn:Button;
		private var jumpBtn:Button;
		
		private var btn2:Button;
		private var assets:AssetManager;
		private var _isRightTouched:Boolean;
		private var _isLeftTouched:Boolean;
		private var hero2:Hero;
		private var _jumpFlag:Boolean;
		private var heroV:Array;
		private var blockBtn:Button;
		private var _blockFlag:Boolean;
		private var enemy3:Enemy;
		private var enemy:Enemy;
		private var teleN:Number = 1;
		private var teleF:Number = 1;
		private var jumpN:Number = 20;
		private var enemy2:Enemy;
		private var insideRange:int = 0;
		
		public function Game2()
		{
			super();
		
		}
		
		override public function initialize():void
		{
			super.initialize();
			trace("Started level 2");
			var physics:Box2D = new Box2D("box2d");
			physics.visible = true;
			add(physics);
			
			assets = new AssetManager();
			assets.enqueue(File.applicationDirectory.resolvePath("assets"));
			assets.loadQueue(onProgress);
			
			// Different level design
			var floor:Platform = new Platform("floor", {x: 360, y: 430, width: 5000, height: 50});
			add(floor);
			
			var p1:Platform = new Platform("p1", {x: 620, y: 100, width: 130, height: 20});
			add(p1);
			
			var p3:Platform = new Platform("p3", {x: 620, y: -80, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p3:Platform = new Platform("p3", {x: 620, y: -540, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p4:Platform = new Platform("p4", {x: 620, y: -240, width: 130, height: 20, oneWay: true});
			add(p4);
			
			var p5:Platform = new Platform("p5", {x: 920, y: 100, width: 130, height: 20, oneWay: true});
			add(p5);
			
			var p6:Platform = new Platform("p6", {x: 920, y: -80, width: 130, height: 20, oneWay: true});
			add(p6);
			
			var p7:Platform = new Platform("p7", {x: 920, y: -240, width: 130, height: 20, oneWay: true});
			add(p7);
			
			var p11:Platform = new Platform("p11", {x: 920, y: 240, width: 130, height: 20, oneWay: true});
			add(p11);
			
			var p11:Platform = new Platform("p11", {x: 920, y: -420, width: 130, height: 20, oneWay: true});
			add(p11);
			
			var p8:Platform = new Platform("p8", {x: 1220, y: 100, width: 130, height: 20, oneWay: true});
			add(p8);
			
			var p9:Platform = new Platform("p9", {x: 1220, y: -80, width: 130, height: 20, oneWay: true});
			add(p9);
			
			var p10:Platform = new Platform("p10", {x: 1220, y: -240, width: 130, height: 20, oneWay: true});
			add(p10);
			
			var p3:Platform = new Platform("p3", {x: 1220, y: -540, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p11:Platform = new Platform("p11", {x: 1520, y: 240, width: 130, height: 20, oneWay: true});
			add(p11);
			
			var p1:Platform = new Platform("p1", {x: 1820, y: 100, width: 130, height: 20});
			add(p1);
			
			var p3:Platform = new Platform("p3", {x: 1820, y: -80, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p3:Platform = new Platform("p3", {x: 1820, y: -390, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p3:Platform = new Platform("p3", {x: 1820, y: -570, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p1:Platform = new Platform("p1", {x: 2120, y: 100, width: 130, height: 20});
			add(p1);
			
			var p3:Platform = new Platform("p3", {x: 2120, y: -80, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var p4:Platform = new Platform("p4", {x: 2120, y: -240, width: 130, height: 20, oneWay: true});
			add(p4);
			
			var p3:Platform = new Platform("p3", {x: 2120, y: -570, width: 130, height: 20, oneWay: true});
			add(p3);
			
			var mp1:MovingPlatform = new MovingPlatform("moving1", {x: 420, y: 100, width: 100, height: 20, startX: 420, startY: 100, endX: 420, endY: 350, oneWay: true});
			add(mp1);
			
			var mp2:MovingPlatform = new MovingPlatform("moving2", {x: 2500, y: 100, width: 100, height: 20, startX: 2500, startY: 100, endX: 2500, endY: 350, oneWay: true});
			add(mp2);
			
			this.hero = new Hero("hero", {x: 20, y: 20, width: 30, height: 30});
			add(hero);
			
			view.camera.setUp(hero, new Rectangle(-100, -1000, 4500, 5000), new Point(0.25, 0.6), new Point(.5, .5));
			
			enemy = new Enemy("enemy", {x: 1250, y: 100, width: 30, height: 30, leftBound: 970, rightBound: 1530});
			add(enemy);
			enemy.speed = 0;
			
			enemy2 = new Enemy("enemy2", {x: 360, y: 420, width: 30, height: 30, leftBound: 10, rightBound: 680});
			add(enemy2);
			enemy2.speed = 3;
			
			enemy3 = new Enemy("enemy3", {x: 2000, y: 420, width: 30, height: 30, leftBound: 1270, rightBound: 1830});
			add(enemy3);
			enemy3.speed = 0;
			
			var c1:Coin = new Coin("c1", {x: 620, y: 70, width: 40, height: 40});
			c1.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c1");
				});
			add(c1);
			
			var c2:Coin = new Coin("c2", {x: 620, y: -270, width: 40, height: 40});
			c2.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c2");
				});
			add(c2);
			
			var c3:Coin = new Coin("c3", {x: 620, y: -570, width: 40, height: 40});
			c3.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c3");
				});
			add(c3);
			
			var c4:Coin = new Coin("c4", {x: 920, y: -110, width: 40, height: 40});
			c4.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c4");
				});
			add(c4);
			
			var c5:Coin = new Coin("c5", {x: 1220, y: -110, width: 40, height: 40});
			c5.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c5");
				})
			add(c5);
			
			var c6:Coin = new Coin("c6", {x: 1220, y: -570, width: 40, height: 40});
			c6.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c6");
				})
			add(c6);
			
			var c7:Coin = new Coin("c7", {x: 1820, y: 70, width: 40, height: 40});
			c7.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c7");
				})
			add(c7);
			
			var c8:Coin = new Coin("c8", {x: 1820, y: -420, width: 40, height: 40});
			c8.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c8");
				})
			add(c8);
			
			var c9:Coin = new Coin("c9", {x: 2120, y: -110, width: 40, height: 40});
			c9.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c9");
				})
			add(c9);
			
			var c10:Coin = new Coin("c10", {x: 2120, y: -600, width: 40, height: 40});
			c10.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c10");
				})
			add(c10);
			
			hero.onTakeDamage.add(heroHurt);
			
			this.addEventListener(Event.ADDED_TO_STAGE, addedToStageHandler);
		}
		
		private function addedToStageHandler(e:Event):void
		{
		}
		
		private function onProgress(ratio:Number):void
		{
			if (ratio == 1)
			{				
				btn = new Button(assets.getTexture("btnbg"), "Left");
				btn.x = 50;
				btn.y = 950;
				this.addChild(btn);
				
				btn2 = new Button(assets.getTexture("btnbg"), "Right");
				btn2.x = 300;
				btn2.y = 950;
				this.addChild(btn2);
				
				jumpBtn = new Button(assets.getTexture("btnbg"), "Jump");
				jumpBtn.x = 1620;
				jumpBtn.y = 750;
				this.addChild(jumpBtn);
				
				blockBtn = new Button(assets.getTexture("btnbg"), "Block");
				blockBtn.x = 1620;
				blockBtn.y = 950;
				this.addChild(blockBtn);
				
				this.addEventListener(TouchEvent.TOUCH, isPressed);
			}
		}
		
		private function isPressed(event:TouchEvent):void
		{
			var touch:Touch = event.getTouch(btn);
			var touch2:Touch = event.getTouch(btn2);
			var touchJump:Touch = event.getTouch(jumpBtn);
			var touchBlock:Touch = event.getTouch(blockBtn);
			
			heroV = hero.velocity;
			if (touchJump != null)
			{
				if (touchJump.phase == TouchPhase.BEGAN && hero.onGround && !_blockFlag) //on finger down
				{
					heroV[1] -= 15;
					hero.velocity = heroV;
					_jumpFlag = true;
				}
				if (touchJump.phase == TouchPhase.ENDED) //on finger up
				{
					trace("release");
					_jumpFlag = false;
				}
			}
			if (touchBlock != null)
			{
				if (touchBlock.phase == TouchPhase.BEGAN) //on finger down
				{
					_blockFlag = true;
				}
				if (touchBlock.phase == TouchPhase.ENDED) //on finger up
				{
					trace("release");
					_blockFlag = false;
				}
			}
			if (touch != null)
			{
				if (touch.phase == TouchPhase.BEGAN && !_blockFlag) //on finger down
				{
					_isLeftTouched = true;
					if (!hero.onGround)
					{
						heroV[0] -= 4;
						hero.velocity = heroV;
					}
				}
				
				if (touch.phase == TouchPhase.ENDED) //on finger up
				{
					trace("release");
					_isLeftTouched = false;
				}
			}
			if (touch2 != null)
			{
				if (touch2.phase == TouchPhase.BEGAN && !_blockFlag) //on finger down
				{
					_isRightTouched = true;
					if (!hero.onGround)
					{
						heroV[0] += 4;
						hero.velocity = heroV;
					}
				}
				
				if (touch2.phase == TouchPhase.ENDED) //on finger up
				{
					trace("release");
					_isRightTouched = false;
				}
			}
		}
		
		public function heroHurt():void
		{
			if (!_blockFlag)
				h += 1;
		}
		
		// Update function that contains enemy behavior algorithms
		public override function update(n:Number):void
		{
			super.update(n);
			
			if (hero.y > 500)
				CitrusEngine.getInstance().state = new Game2();
			if (count == 10)
				CitrusEngine.getInstance().state = new GameEnd();
			if (h == 5)
				CitrusEngine.getInstance().state = new GameOver();
			
			if (!enemy2.kill)
			{
				jumpN -= 1;
				if (jumpN == 0)
				{
					enemy2.velocity = [0, -10];
					jumpN = Math.round(Math.random() * 70);
				}
			}
					
			if (!enemy3.kill)
			{
				if (Math.abs(enemy3.x - hero.x) < 300 && (hero.y - enemy3.y) < 5)
				{
					if (teleN == 1)
					{
						enemy3.x = enemy3.x - 100;
						teleN = 0;
					}
					if ((enemy3.x - hero.x) > 0)
					{
						enemy3.x -= 2;
					}
					else
					{
						enemy3.x += 2;
					}
				}
				
			}
			
			if (!enemy.kill)
			{
				if (Math.abs(enemy.x - hero.x) < 300 && (hero.y - enemy.y) < 5)
				{
					if (insideRange != 3)
						insideRange = 1;
					if ((enemy.x - hero.x) > 0)
					{
						enemy.x -= 2;
					}
					else
					{
						enemy.x += 2;
					}
				}
				if (Math.abs(enemy.x - hero.x) > 300 && (hero.y - enemy.y) < 5 && insideRange == 1)
				{
					insideRange = 3;
					if ((enemy.x - hero.x) > 0)
					{
						enemy.x -= 200;
					}
					else
					{
						enemy.x += 200;
					}
				}
				
			}
			
			heroMechanics();
		}
		
		private function heroMechanics():void
		{
			if (hero.onGround && !_jumpFlag && !_blockFlag)
			{
				if (_isLeftTouched)
				{
					hero.velocity = [-6, 0];
				}
				else if (_isRightTouched)
				{
					hero.velocity = [6, 0];
				}
			}
			if (_blockFlag)
			{
				hero.hurtVelocityX = 0;
				hero.hurtVelocityY = 0;
				
			}
			else
			{
				hero.hurtVelocityX = 8;
				hero.hurtVelocityY = 8;
			}
		}
	}
}