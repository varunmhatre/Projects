<?php

header('Content-type: text/plain');

//Abstract class to print a value
abstract class ValueModifier 
{
    protected $value = 0;

    final public function __toString() 
    {
    	return $this->value . "\n";
    }

    abstract public function updateValue();  
}

class StoredSum extends ValueModifier
{
    public function __construct() 
    {        
        $init = func_get_args();

        foreach ($init as $arg) 
        {
            $this->value += $arg;
        }
    }

    public function updateValue() 
    {    
        $args = func_get_args();

        if (empty($args)) return $this->value;


        foreach ($args as $arg) {
            $this->value += $arg;
        }

        return $this->value;
    }
}

$var = new StoredSum(1,1);

echo $var;	

echo $var->updateValue(5) . "\n";
$var->updateValue(3, 4) . "\n";

echo $var;	

?>