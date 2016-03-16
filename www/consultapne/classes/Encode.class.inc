<?php
class Encode {
	public static function utf8_decode_recursive($array)
	{
		$utf8_array = array();
	
		if (is_array($array))
		{
			foreach ($array as $key => $val)
			{
				if (is_array($val))
					$utf8_array[$key] = Encode::utf8_decode_recursive($val);
				else if (is_object($val))
					$utf8_array[$key] = $val;
				else
					$utf8_array[$key] = utf8_decode($val);
			}
		}
		else
		{
			$utf8_array = $array;
		}
	
		return $utf8_array;
	}
	
	public static function utf8_encode_recursive($array)
	{
		$utf8_array = array();
	
		if (is_array($array))
		{
			foreach ($array as $key => $val)
			{
				if (is_array($val))
					$utf8_array[$key] = Encode::utf8_encode_recursive($val);
				else if (is_object($val) || is_numeric($val))
					$utf8_array[$key] = $val;
				else
					$utf8_array[$key] = utf8_encode($val);
			}
		}
		else
		{
			$utf8_array = $array;
		}
	
		return $utf8_array;
	}
}