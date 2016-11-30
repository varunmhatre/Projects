package 
{
	import citrus.core.starling.StarlingState;
	import Box2D.Dynamics.Contacts.b2Contact;
	import citrus.objects.CitrusSprite;
	import citrus.objects.platformer.box2d.*;
	import citrus.physics.box2d.Box2D;
	import feathers.controls.Label;
	import starling.display.Image;
    
	/**
	 * ...
	 * @author Varun
	 */
	public class GameEnd extends StarlingState
	{
		
		public function GameEnd() 
		{
			super();
		}
		
		override public function initialize():void
		{
			super.initialize();
			var lbStatus:Label = new Label();
			lbStatus.text = "You have completed the game.";
			lbStatus.visible = true;
			add(new CitrusSprite("lbStatus", { x:330, y:230, view:lbStatus } ));			
		}
	}

}