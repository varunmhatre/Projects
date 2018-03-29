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
	public class GameOver extends StarlingState
	{
		
		public function GameOver() 
		{
			super();
		}
		
		override public function initialize():void
		{
			super.initialize();
			var lbStatus:Label = new Label();
			lbStatus.text = "Game Over";
			lbStatus.visible = true;
			add(new CitrusSprite("lbStatus", { x:330, y:230, view:lbStatus } ));			
		}
	}

}