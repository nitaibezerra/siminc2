<?PHP
class assoc_array2xml {
	var $text;
	var $arrays, $keys, $node_flag, $depth, $xml_parser;

	function array2xml($array) {
		//global $text;
		$this->text="<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><array>";
		$this->text.= $this->array_transform($array);
		$this->text .="</array>";
		return $this->text;
	}

	function array_transform($array,$ident = 1){
		//global $array_text;
		foreach($array as $key => $value){
			if(!is_array($value)){
				$this->text .= str_repeat("   ",$ident)."<$key>$value</$key>\n";
			} else {
				$this->text.= str_repeat("   ",$ident)."<$key>\n";
				$this->array_transform($value,$ident+1);
				$this->text.= str_repeat("   ",$ident)."</$key>\n";
			}
		}
		$ident--;
		return $this->text;
	}
}
?>