package
{
	import citrus.core.starling.StarlingCitrusEngine;
	import flash.filesystem.File;
	import starling.core.Starling;
	    
	/**
	 * ...
	 * @author Varun
	 */
	[SWF(width="720",height="480",frameRate="50",backgroundColor="#FFFFFF")]
	
	public class Main extends StarlingCitrusEngine
	{
		Starling.multitouchEnabled = true;
		
		public function Main()
		{
			setUpStarling();
			state = new Menu();
		}
	}
}

