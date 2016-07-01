<?php
include_once APPRAIZ . "demandasfies/classes/html_table.class.php";
class HtmlTableCustom extends HTML_Table
{

	function __construct($id = '', $klass = '', $attr_ar = array())
	{
		parent::__construct($id, $klass, $attr_ar);
	}

	public function setHeader($camposDaTabela, $class = 'text-center')
	{
		$this->addTSection('thead');
		$this->addRow($class);
		foreach ($camposDaTabela as $campo) {
			$this->addCell($campo, '', 'header', array('class' => $class));
		}
	}

	public function setBody($dados, $camposDaTabela, $classRow = '')
	{
		$class = '';
		foreach ($dados as $key => $dado) {
			$this->addRow($classRow);
			foreach ($camposDaTabela as $atributo => $campo) {
				$valor = $dado[$atributo];
				$this->addCell($valor, 'text-center');
			}
		}
	}

}

?>