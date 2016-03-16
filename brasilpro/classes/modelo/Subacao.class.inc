<?php
	
class Subacao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sbaid' => null, 
									  	'sbadsc' => null, 
									  	'sbaordem' => null,
    									'sbaobra' => null,
									    'sbaestrategiaimplementacao' => null,
    									'sbaptres' => null,
    									'sbanaturezadespesa' => null,
    									'sbamonitoratecnico' => null,
    									'docid' => null,
    									'frmid' => null,
									    'indid' => null,
    									'foaid' => null,
									    'undid' => null,
    									'ptsid' => null,
    									'ppsid' => null,
									    'aciid' => null,
									    'sbacronograma' => null,
    									'prgid' => null,
    									'sbastatus' => null
									  );

	public function deletaPorPtoid($ptoid)
	{
		$this->executar("delete FROM par.subacao WHERE aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})");
	}
	
	public function listaSubAcaoPorAcao($aciid)
	{
		$sql = "select
					'<img src=\"../imagens/alterar.gif\" title=\"Listar Subacao\" style=\"cursor:pointer\" onclick=\"listarSubacao(\'' || sbaid || '\')\"  />' as acao,
					sbadsc,
					COALESCE(frmdsc,'N/A') as frmdesc,
					COALESCE(unddsc,'N/A') as unddsc
				from
					par.subacao sba
				left join
					par.formaexecucao frm on frm.frmid = sba.frmid
				left join
					par.unidademedida und on und.undid = sba.undid
				where
					aciid = $aciid AND
					sba.sbastatus = 'A'
				order by
					sbaordem,sbadsc";
		$cabecalho = array("Ação","Subação","Forma de Execução","Unidade de Medida");
		$this->monta_lista($sql,$cabecalho,100,10,"N","center");
	}
	
	public function carregaSubAcaoPorAcao($aciid)
	{
		$sql = "select
					sbaid,
					sbadsc
				from
					par.subacao
				where
					aciid = $aciid AND
					sba.sbastatus = 'A'
				order by
					sbaordem,sbadsc";
		return $this->carregar($sql);
	}
}									  