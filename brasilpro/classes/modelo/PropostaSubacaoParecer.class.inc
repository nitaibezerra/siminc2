<?php
	
class PropostaSubacaoParecer extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacaoparecer";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pspid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pspid' => null, 
									  	'ppsid' => null, 
									  	'frmid' => null, 
									  	'pspdescricao' => null,
    									'pspstatus' => null,
    									'pspordem' => null,
    									'pspdata' => null
									  );
									  
	public function recuperarPareceresPorPpsid($ppsid)
	{
		$sql = "SELECT
					'<img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" title=\"Alterar\" onclick=\"editarParecer(\'' || pspid || '\')\" /> <img style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" title=\"Excluir\" onclick=\"excluirParecer(\'' || pspid || '\')\" />' as acao,
					pspdescricao,
					to_char(pspdata,'DD/MM/YYYY') as pspdata
					FROM 
				{$this->stNomeTabela} 
				WHERE 
					ppsid = '{$ppsid}' 
				AND 
					pspstatus = 'A'";
		return $this->carregar($sql);
	}
}									  