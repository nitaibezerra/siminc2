<?php
class TipoDespesa extends ModeloFabrica
{
    protected $stNomeTabela 	= "fabrica.tipodespesa";
    protected $arChavePrimaria 	= array( "tpdpsid" );
    protected $arAtributos 		= array(
								        'tpdpsid'         => null,
								        'tpdpsdsc'        => null,
								        'tpdpsdtcadastro' => null,
								        'tpdpsstatus'     => null
									);
	private $tpdpsid;
    private $tpdpsdsc;
	private $tpdpsdtcadastro;
	private $tpdpsstatus;

    /**
     * Criação de métodos get's e set's
     * 
     * */
    public function setId( $id ){
    	$this->tpdpsid = $id;
    }
    public function setDescricao( $descricao ){
    	$this->tpdpsdsc = $descricao;
    }
    
    public function getId()
    {
        return $this->tpdpsid;
    }
    public function getDescricao( ){
    	return $this->tpdpsdsc;
    }

    //Informações da TAbela
    public function getNomeTabela( ){
    	return $this->stNomeTabela;
    }
    public function getChavePrimaria( ){
    	return $this->arChavePrimaria;
    }
    public function getAtributos( ){
    	return $this->arAtributos;
    }
}   