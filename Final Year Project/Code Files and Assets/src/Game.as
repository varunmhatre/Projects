package
{
	import adobe.utils.CustomActions;
	import Box2D.Dynamics.Contacts.b2Contact;
	import citrus.objects.platformer.box2d.*;
	import citrus.physics.box2d.Box2D;
	import citrus.math.MathVector;
	import citrus.view.starlingview.StarlingCamera;
	import flash.filesystem.File;
	import starling.core.Starling;
	import starling.events.Event;
	import flash.geom.*;
	import citrus.core.starling.StarlingState
	import citrus.core.CitrusEngine;
	import starling.display.Button;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	import starling.utils.AssetManager;
	import flash.display.*;
	    
	/**
	 * ...
	 * @author Varun
	 */
	public class Game extends StarlingState
	{
		[Embed(source="../../../bin/assets/enemy3.png")]
		private var enem:Class;
		
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
		private var _enemyLeft:Boolean;
		private var _enemyRight:Boolean;
		
		public function Game()
		{
			super();
		}
		
		override public function initialize():void
		{
			super.initialize();
			
			// Loading Assets
			assets = new AssetManager();
			assets.enqueue(File.applicationDirectory.resolvePath("assets"));
			assets.loadQueue(onProgress);
			
			// Initializing Variables
			_jumpFlag = false;
			_blockFlag = false;
			_isLeftTouched = false;
			_isRightTouched = false;
			_enemyLeft = true;
			_enemyRight = false;
			
			// Creating the game world
			var physics:Box2D = new Box2D("box2d");
			physics.visible = true;
			add(physics);
			
			var floor:Platform = new Platform("floor", {x: 1000, y: 450, width: 3500, height: 100});
			add(floor);
			
			var floor2:Platform = new Platform("floor2", {x: 4000, y: 430, width: 750, height: 50});
			add(floor2);
			
			var p1:Platform = new Platform("p1", {x: 620, y: 100, width: 200, height: 20});
			add(p1);
			
			var mp1:MovingPlatform = new MovingPlatform("moving1", {x: 420, y: 100, width: 100, height: 20, startX: 420, startY: 100, endX: 100, endY: 300, oneWay: true});
			add(mp1);
			
			var p2:Platform = new Platform("p2", {x: 1250, y: 140, width: 600, height: 20});
			add(p2);
			
			var crate:Crate = new Crate("crate", {x: 2500, y: 300, width: 80, height: 80});
			add(crate);
			
			var mp3:MovingPlatform = new MovingPlatform("moving3", {x: 3250, y: 300, width: 100, height: 20, startX: 3250, startY: 300, endX: 3000, endY: 300, oneWay: true});
			add(mp3);
			
			// Creating the hero, camera movement based around the hero and the enemies.
			this.hero = new Hero("hero", {x: 20, y: 20, width: 30, height: 30});
			add(hero);
			
			heroV = hero.velocity;
			
			//var _cam:StarlingCamera;
			//_cam = view.camera as StarlingCamera;
			//_cam.setUp(hero, new Rectangle( -100, -100, 4500, 550), new Point(0.25, 0.6), Point(.5,.5));
			//_cam.bounds = new Rectangle( -100, -100, 4500, 550);
			//_cam.easing.setTo(.5, .5);
			//view.camera.enabled = true;
			view.camera.setUp(hero, new Rectangle(-100, -200, 4500, 750), new Point(0.25, 0.6), new Point(.5, .5));
			
			enemy = new Enemy("enemy", {x: 1250, y: 100, width: 30, height: 30, leftBound: 970, rightBound: 1530});
			add(enemy);
			enemy.startingDirection = "left";
			enemy.speed = 0;
			
			enemy3 = new Enemy("enemy3", {x: 1270, y: 300, width: 30, height: 30, leftBound: 1270, rightBound: 2000});
			enemy3.view = new enem();
			add(enemy3);
			
			// Creating coins the hero must collect to progress
			var c1:Coin = new Coin("c1", {x: 2250, y: 150, width: 40, height: 40});
			c1.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c1");
				});
			add(c1);
			
			var c2:Coin = new Coin("c2", {x: 670, y: 70, width: 40, height: 40});
			c2.onBeginContact.add(function(c:b2Contact):void
				{
					count += 1;
					trace("Collected c2");
				});
			add(c2);
						
			var c5:Coin = new Coin("c5", {x: 4100, y: 370, width: 40, height: 40});
			c5.onBeginContact.add(function(c:b2Contact):void
				{
					CitrusEngine.getInstance().state = new Game2();
				});
			add(c5);
			
			// Function call for hero when taking damage
			hero.onTakeDamage.add(heroHurt);
			
			this.addEventListener(Event.ADDED_TO_STAGE, addedToStageHandler);
		}
		
		private function addedToStageHandler(e:Event):void
		{
		}
		
		// Creating on screen buttons
		private function onProgress(ratio:Number):void
		{
			if (ratio == 1)
			{
				// add touch event to buttons
				// you can use 2 seperate functions or a single touch handler
				// then pass the respected btn object to the event.getTouch()
				// accordingly change the boolean values _isLeftTouched, _isRightTouched, etc.
				
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
		
		// Adding functionality to the buttons and part of the enemy behavior algorithm
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
					if (Math.round(Math.random() * 100) <= 61 && Math.abs(enemy3.x - hero.x) < 300)
						enemy3.velocity = [0, -13];
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
		
		// Function for hero when taking damage
		public function heroHurt():void
		{
			if (!_blockFlag)
				h += 1;
		}
		
		// Update functoin which contains game behavior based on hero's actions and part of the enemy behavior algorithm
		public override function update(n:Number):void
		{
			super.update(n);
			if (hero.y > 500)
				CitrusEngine.getInstance().state = new Game();
			if (count == 2)
				CitrusEngine.getInstance().state = new Game2();
			if (h == 5)
				CitrusEngine.getInstance().state = new GameOver();
			
			if (!enemy.kill)
			{
				if (Math.abs(enemy.x - hero.x) < 300 && (hero.y - enemy.y) < 5)
				{
					if ((enemy.x - hero.x) > 0)
					{
						if (_enemyLeft == false)
						{
							_enemyLeft == true;
							enemy.turnAround();
						}
						enemy.x -= 3;
						_enemyRight == false;
					}
					else
					{
						enemy.x += 3;
					}
				}
				
			}
			// Function call for additional hero movement
			heroMechanics();
		}
		
		// Function for additional hero movement
		private function heroMechanics():void
		{
			if (hero.onGround && !_jumpFlag && !_blockFlag)
			{
				if (_isLeftTouched)
				{
					//heroV[0] = -6;
					hero.velocity = [-6, 0];
				}
				else if (_isRightTouched)
				{
					//heroV[0] = 6;
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