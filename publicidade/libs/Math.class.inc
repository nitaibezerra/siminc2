<?php

/**
 * Trabalha com valores monetários e de ponto flutuante
 * @author Silas Matheus
 */
class Math{
	
	/**
	 * Casas decimais
	 * @name $precision
	 * @access private
	 * @var int
	 */
	private $precision;
	
	/**
	 * Número 1
	 * @name $num1
	 * @access private
	 * @var int
	 */	
	private $num1;
	
	/**
	 * Número 2
	 * @name $num2
	 * @access private
	 * @var int
	 */	
	private $num2;
	
	/**
	 * Resultado das operações
	 * @name $result
	 * @access private
	 * @var float
	 */	
	private $result;

	/**
	 * Incia a classe com os valores para as operações
	 * @name  __construct
	 * @author Silas Matheus
	 * @param float $num1
	 * @param float $num2
	 * @param int $precision
     * @access public
     * @return void 
	 */
	public function __construct($num1, $num2, $precision = 0){

		$this->num1 = str_replace(',', '.', $num1);
		$this->num2 = str_replace(',', '.', $num2);
		$this->precision = $precision;
		
	}
	
	/**
	 * Verifica se o primeiro numero é maior que o segundo
	 * @name  isLarger
	 * @author Silas Matheus
     * @access public
     * @return bool 
	 */
	public function isLarger(){
		
		$this->result = false;
		
		if(bccomp($this->num1, $this->num2, $this->precision) == 1)
			$this->result = true;
			
		return $this->result;
		
	}
	
	/**
	 * Verifica se os dois número são iguais
	 * @name  isEqual
	 * @author Silas Matheus
     * @access public
     * @return bool 
	 */
	public function isEqual(){
		
		$this->result = false;
		
		if(bccomp($this->num1, $this->num2, $this->precision) == 0)
			$this->result = true;
			
		return $this->result;
		
	}
	
	/**
	 * Verifica se o primeiro número é menor que o segundo
	 * @name  isLess
	 * @author Silas Matheus
     * @access public
     * @return bool 
	 */
	public function isLess(){
		
		$this->result = false;
		
		if(bccomp($this->num1, $this->num2, $this->precision) == -1)
			$this->result = true;
			
		return $this->result;
		
	}
	
	/**
	 * Soma os dois valores
	 * @name  sum
	 * @author Silas Matheus
     * @access public
     * @return Math 
	 */
	public function sum(){
		
		$this->result = bcadd($this->num1, $this->num2, $this->precision);
		return $this;
		
	}

	/**
	 * Subtrai o primeiro valor pelo segundo
	 * @name  sub
	 * @author Silas Matheus
     * @access public
     * @return Math 
	 */
	public function sub(){
		
		$this->result = bcsub($this->num1, $this->num2, $this->precision);
		return $this;
		
	}
	
	/**
	 * Divide o primeiro valor pelo segundo
	 * @name  div
	 * @author Silas Matheus
     * @access public
     * @return Math 
	 */
	public function div(){
		
		$this->result = bcdiv($this->num1, $this->num2, $this->precision);
		return $this;
		
	}
	
	/**
	 * Mutilplica os valores
	 * @name  mul
	 * @author Silas Matheus
     * @access public
     * @return Math 
	 */
	public function mul(){
		
		$this->result = bcmul($this->num1, $this->num2, $this->precision);
		return $this;
		
	}	
	
	/**
	 * Formata o resultado como monetário
	 * @name  moneyFormat
	 * @author Silas Matheus
     * @access public
     * @return float 
	 */
	public function moneyFormat($format = "%i"){
		
		return money_format($format, $this->result);
		
	}

	/**
	 * Executa um replace no resultado
	 * @name  replace
	 * @author Silas Matheus
     * @access public
     * @return string 
	 */
	public function replace($search = '.', $replace = ','){
		
		return str_replace($search, $replace, $this->result);
		
	}
	
	/**
	 * Retorna o Resultado
	 * @name  getResult
	 * @author Silas Matheus
     * @access public
     * @return float 
	 */
	public function getResult(){
		
		return $this->result;
		
	}
	
}

?>