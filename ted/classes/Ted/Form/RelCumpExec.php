<?php

/**
 * Class Ted_Form_RelCumpExec
 */
class Ted_Form_RelCumpExec extends Ted_Form_Abstract
{
    /**
     *
     */
    public function init()
	{
		parent::init();
		
		$this->setName('filtro-ted')
		->setAttrib('id', 'form-filtro-ted')
		->setMethod(Zend_Form::METHOD_POST)
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
		
		$funcao = new Zend_Form_Element_Hidden('funcao');
		$funcao->setAttrib('id', 'funcao');
						
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'relcumpexec-form.php'))));
		$this->addElements(array($ungcod, $funcao));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
		
	}

    /**
     * @return string
     */
    public function getQueryRelatorio()
    {
		$sql = "SELECT 
					numero_termo,
					unidade_proponente,
					unidade_concedente,
					identificacao_objeto,
					to_char(data_execucao, 'DD/MM/YYYY'),
					prazo_cumprimento,	
					to_char(data_execucao::date + (prazo_cumprimento || ' month')::interval , 'DD/MM/YYYY') as data_com_prazo,	
					to_char( (data_execucao::date+(prazo_cumprimento || ' month')::interval)::date+60, 'DD/MM/YYYY') as data_com_prazo_mais_60_dias
					
				FROM (
				
					SELECT 
						tcp.tcpid as numero_termo,
						unp.ungdsc as unidade_proponente,
						unc.ungdsc as unidade_concedente,
						--replace(tcpdscobjetoidentificacao, ' ? ', '') as identificacao_objeto,
						(select identificacao from ted.justificativa where tcpid = tcp.tcpid) as identificacao_objeto,
				
						(select 
							max(hst.htddata) 
						from workflow.historicodocumento hst 
						where aedid in (select aed.aedid from workflow.acaoestadodoc aed  where aed.aedstatus = 'A' and esdiddestino = 639)
						and hst.docid = doc.docid) as data_execucao,
				
						(select distinct 
							max(crdmesexecucao)
						from ted.previsaoorcamentaria pro
						where pro.tcpid = tcp.tcpid 
						and pro.prostatus = 'A')  as prazo_cumprimento
						
					FROM ted.termocompromisso tcp
					JOIN unidadegestora unp ON tcp.ungcodproponente = unp.ungcod
					JOIN unidadegestora unc ON tcp.ungcodconcedente = unc.ungcod
					JOIN workflow.documento doc ON doc.docid = tcp.docid
					WHERE tcp.tcpstatus = 'A'
					AND doc.esdid = 639	
				
				) AS foo
				ORDER BY numero_termo";
		
		return $sql;
	}

    /**
     *
     */
    public function mostraRelatorio()
    {
		$sql = $this->getQueryRelatorio();
		$list = new Simec_Listagem();
		$list->setCabecalho(array(
				"Termo", 
				"Unidade Proponente", 
				"Unidade Concedente", 
				"Objeto",
            	"Data de Execução", 
				"Prazo para o cumprimento", 
				"Data com Prazo", 
				"Data com Prazo + 60 dias"))
		  ->setQuery($sql);
		
		$list->addCallbackDeCampo(array(
				'unidade_proponente',
				'unidade_concedente',
				'identificacao_objeto'
		), 'alinhaParaEsquerda');
		
		$list->addCallbackDeCampo(array(
				'numero_termo',
				'prazo_cumprimento'
		), 'alinhaParaDireita');
		$list->turnOnPesquisator();
		
		$list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
	}

    /**
     *
     */
    public function mostraRelatorioXls()
    {
		global $db;
		
		$sql = $this->getQueryRelatorio();
		$cabecalho = array(
            "Termo",
            "Unidade Proponente",
            "Unidade Concedente",
            "Objeto",
            "Data de Execução",
            "Prazo para o cumprimento",
            "Data com Prazo",
            "Data com Prazo + 60 dias"
        );
		
        ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=rel_unidadegestora_" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=rel_unidadegestora_" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");
		$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');		
	}
}