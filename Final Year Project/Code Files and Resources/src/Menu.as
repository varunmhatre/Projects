package
{
	import citrus.core.starling.StarlingState
	import citrus.core.CitrusEngine;
	import flash.filesystem.File;
	import starling.display.Button;
	import starling.events.EnterFrameEvent;
	import starling.events.Event;
	import starling.utils.AssetManager;
	
	/**
	 * ...
	 * @author Varun
	 */
	public class Menu extends StarlingState
	{
		private var assets:AssetManager;
		private var btn:Button;
		
		public function Menu()
		{
			super();
		}
		
		override public function initialize():void
		{
			super.initialize();
			
			assets = new AssetManager();
			assets.enqueue(File.applicationDirectory.resolvePath("assets"));
			
			assets.loadQueue(onProgress);
			
			this.addEventListener(Event.ADDED_TO_STAGE, addedToStageHandler);
		}
		
		private function onProgress(ratio:Number):void
		{
			if (ratio == 1)
			{
				btn = new Button(assets.getTexture("btnbg"), "Start");
				btn.x = 130;
				btn.y = 130;
				this.addChild(btn);
				
				this.addEventListener(Event.TRIGGERED, onTriggered);
			}
		}
		
		private function onTriggered(e:Event):void 
		{
			switch (e.target) 
			{
				case btn:
					CitrusEngine.getInstance().state = new Game();
				break;
				default:
			}
		}
		
		private function addedToStageHandler(event:Event):void
		{
		
		}
		
		protected function button_triggeredHandler(event:Event):void
		{
			CitrusEngine.getInstance().state = new Game();
		}
	}

}