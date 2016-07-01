<?php
	
class AutorizacaoPagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.autorizacaopagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "apgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	  'apgid',
										  'apgcpf' => null,
										  'apgnome' => null,
										  'apgtipo' => null,
										  'apgmes' => null,
										  'apgano' => null,
										  'rmdid' => null,
										  'apgcpfresponsavel' => null,
										  'apgnomeresponsavel' => null,
										  'apgstatus' => null,
										  'apgjustificativa' => null,
										  'apgenviado' => null
    								);
    
    public function checkTutoresSelecionados($arrTutid)
    {
    	$sql = "update $this->stNomeTabela set apgenviado = true where rmdid is null and apgcpf in (select tutcpf from maismedicos.tutor where tutid in (".implode(",",$arrTutid)."))";
    	$this->executar($sql);
    	$this->commit();
    }
    
}