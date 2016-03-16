<?php
class Estatistica{
	private $array = "";
	public $precisao = "";
	
	public function __construct($array="") {
		$this->array = $array;
		
	}
	//pega o valor max do array
	public function getMax(){
		if($this->array){
			return max($this->array);
		}else{
			return 0;		
		}
	}
	
	//pega o valor min do array
	public function getMin(){
		if($this->array){
			return min($this->array);
		}else{
			return 0;		
		}
	}
	
	//mediana
	public function getMediana(){
		sort($this->array);
		$tipo = count($this->array) % 2;
		
		if($tipo != 0){
			 $m = ( ( (count($this->array) + 1) / 2 ) - 1 );
			 return $this->array[$m];
		} else {
			$m = count($this->array) / 2;
			return ( ($this->array[$m - 1] + $this->array[$m]) / 2);
		}
	}
	//soma dos elementos do array
	public function getSomaDosElementos() {
		$total = 0;
		
		for ($counter = 0; $counter < count($this->array); $counter++){
			$total += $this->array[$counter];
		}	
		return $total;
	}
	//media aritmetica
	public function getMediaAritmetica() {
		$total = 0;
		
		for ($counter = 0; $counter < count($this->array); $counter++){
			$total = bcadd($total, $this->array[$counter], $this->precisao);
		}
		if(count($this->array) <= 0){
			return 0;
		}else{
			return bcdiv($total, count($this->array), $this->precisao);
		}
	}	
	// Variância Amostral
	public function getVariancia() {
		if( count($this->array) == 1 ){
			return 0;
		}else{
			$total =  bcsub(count($this->array), 1, $this->precisao);
			return bcdiv($this->getDesvio(), $total, $this->precisao);
		}
	}
	
	public function getDesvio(){
		$total = 0;
		for ($counter = 0; $counter < count($this->array); $counter++){
			$potencia = bcpow( bcsub($this->array[$counter], $this->getMediaAritmetica(), $this->precisao ), 2, $this->precisao );
			$total = bcadd($total, $potencia, $this->precisao) ;
		}
		
		return $total;
	}
	
	public function ordenar() {
		return sort($this->array);
	}
	
	// Array não pode conter valores duplicados
	public function buscaPor($value) {
		return in_array($this->array, $value);
	}
	
	// Desvio Padrão Amostral
	public function getDesvioPadrao() {
		return bcsqrt($this->getVariancia(), $this->precisao);
	}
	
	public function imprimeArray() {
		echo("\nElementos do Array: ");
		for ($count = 0; $count < count($this->array); $count++)
			echo($this->array[$count] . " ");
	}
	
	public function getArray() {
		return $this->array;
	}
	
	public function setArray($array) {
		$this->array = $array;
	}
}
?>