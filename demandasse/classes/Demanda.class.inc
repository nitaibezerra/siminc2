<?php
	
class Demanda extends Modelo{

    private $view;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasse.demanda";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dmdid");

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dmdid' => null, 
									  	'dmdtitulo' => null, 
									  	'dmddsc' => null, 
									  	'docid' => null,
									  	'usucpfinclusao' => null,
									  	'dmddtinclusao' => null,
									  	'usucpfalteracao' => null,
									  	'dmddtalteracao' => null,
									  	'dmdstatus' => null,
									  );
}