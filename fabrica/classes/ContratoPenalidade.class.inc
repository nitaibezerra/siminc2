<?php
	
class ContratoPenalidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.contratopenalidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctpid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ctpid' => null, 
									  	'pnlid' => null,
									  	'ctrid' => null, 
    									'ctpvalor' => null, 
									  	'odsid' => null
									  );
	
	protected $arrPesquisa = null;
	
	public function showListaPenalidade()
	{
				
		$sql = "select
					'<center><img class=\"middle link\" title=\"Alterar Penalidade\" alt=\"Alterar Penalidade\" onclick=\"alterarPenalidade(\'' || ctpid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Penalidade\" alt=\"Excluir Penalidade\" onclick=\"excluirPenalidade(\'' || ctpid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					ctpid,
					(select pnldsc from fabrica.penalidade pnl  where ctp.pnlid = pnl.pnlid) as pnldsc,
					(select ctrnumero from fabrica.contrato ctr  where ctp.ctrid = ctr.ctrid) as ctrnumero,
					(select ods.odsdetalhamento from fabrica.ordemservico ods where ods.odsid = ctp.odsid),
					ctpvalor
				from
					fabrica.contratopenalidade ctp
				where
					1 = 1 
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
				order by
					ctpid";
		
		$cabecalho = array("Ações","Código","Penalidade","Número do Contrato","Ordem de Serviço","Porcentagem da Penalidade (%)");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function excluirPenalidade()
	{
		$ctpid = $_REQUEST['ctpid'];
		
		$sql = sprintf("delete from fabrica.contratopenalidade where ctpid = %d",(int)$ctpid);
		$this->executar($sql);
		$this->commit($sql);
		return false;
		
	}
	
	public function pesquisarContrato()
	{
		foreach($this->arAtributos as $campos => $valor){
			if($_POST[$campos] && $_POST[$campos] != ""){
				$this->arrPesquisa[] = $campos."::text ilike ('%".$_POST[$campos]."%') ";
			}
		}
		
	}
		
}