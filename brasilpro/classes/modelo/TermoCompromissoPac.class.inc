<?php
	
class TermoCompromissoPac extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.termocompromissopac";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "terid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'terid' => null, 
									  	'terdatainclusao' => null, 
									  	'usucpf' => null, 
									  	'arqid' => null,
    									'muncod' => null,
    									'terstatus' => null,
									  );

	public function recuperaTermoPorMunicipio($muncod)
	{
		$sql = "select terid, arqid, to_char(terdatainclusao, 'YYYY') as teranoinclusao from par.termocompromissopac  where muncod = '$muncod' and terstatus = 'A'";
		return $this->carregar($sql);	
	}
	
}