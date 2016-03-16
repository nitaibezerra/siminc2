<?php
	
class ControleWorkFlow extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.controleworkflow";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctwid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ctwid' => null, 
									  	'esdid' => null,
									  	'ctwtemporesp' => null, 
    									'ctwtempoexec' => null, 
									  	'ctwfuncao' => null,
    									'ctwstatus' => null
									  );
	
	protected $arrPesquisa = null;
	
	public function showListaControleWorkFlow()
	{
				
		$sql = "select
					'<center><img class=\"middle link\" title=\"Alterar Controle de WorkFlow\" alt=\"Alterar Controle de WorkFlow\" onclick=\"alterarControleWorkFlow(\'' || ctwid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Controle de WorkFlow\" alt=\"Excluir Controle de WorkFlow\" onclick=\"excluirControleWorkFlow(\'' || ctwid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					(select esddsc from workflow.estadodocumento esd where esd.esdid = ctw.esdid) as esddsc,
					ctwtemporesp,
					(CASE WHEN ctwtempoexec IS NULL OR ctwtempoexec = 0
						THEN 'N/A'::text
						ELSE ctwtempoexec::text
					END)::text as ctwtempoexec,
					(CASE WHEN ctwdtexecucao IS NULL
						THEN 'Não Executado'::text
						ELSE (to_char(ctwdtexecucao,'DD/MM/YYYY HH24:MM'))::text
					END)::text as ctwdtexecucao,
					ctwfuncao
				from
					fabrica.controleworkflow ctw
				where
					ctwstatus = 'A'  
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
				order by
					ctwid";
		
		$cabecalho = array("Ações","Estado do WorkFlow","Tempo de Resposta (em horas)","Tempo Mínimo para Execução da Função (em horas)","Última Execução","Função");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function excluirControleWorkFlow()
	{
		$ctwid = $_REQUEST['ctwid'];
		
		$sql = sprintf("update fabrica.controleworkflow set ctwstatus = 'I' where ctwid = %d",(int)$ctwid);
		$this->executar($sql);
		$this->commit($sql);
		return false;
		
	}
	
	public function pesquisarControleWorkFlow()
	{
		foreach($this->arAtributos as $campos => $valor){
			if($_POST[$campos] && $_POST[$campos] != ""){
				$this->arrPesquisa[] = $campos."::text ilike ('%".$_POST[$campos]."%') ";
			}
		}
		
	}
	
	public function antesSalvar()
	{ 
		$this->ctwtemporesp	 = !$this->ctwtemporesp || $this->ctwtemporesp == "" ? 0 : str_replace(".","",$this->ctwtemporesp);
		$this->ctwtempoexec	 = !$this->ctwtempoexec || $this->ctwtempoexec == "" ? 0 : str_replace(".","",$this->ctwtempoexec);
		return true;
	}
		
}