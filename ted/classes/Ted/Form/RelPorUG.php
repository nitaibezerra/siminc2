<?php

/**
 * Class Ted_Form_RelPorUG
 */
class Ted_Form_RelPorUG extends Ted_Form_Abstract
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
		
		$ungcod = new Zend_Form_Element_Select('ungcod');
		$ungcod->setAttrib('id', 'ungcod')
		->setAttrib('multiple', 'multiple')
		->setAttrib('data-placeholder', 'Selecione uma ou mais Unidades Gestoras')
		->setAttrib('class', 'form-control chosen-select-no-single')
		->addMultiOptions($this->carregaUngcod());
				
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'relporug-form.php'))));
		$this->addElements(array($ungcod, $funcao));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
		
	}

    /**
     * @return array
     */
    public function carregaUngcod()
    {
		global $db;
		
		$sql = "
		    SELECT
		        ungcod as codigo,
		        ungcod ||' - '|| ungdsc as descricao
		    FROM public.unidadegestora
		    where
		        unicod in (". UNIDADES_OBRIGATORIAS. ")";
		
		$list = $db->carregar($sql);
		$options = array();
		if ($list) {
			foreach($list as $item) {
				$options[$item['codigo']] = $item['descricao'];
			}
		}
		
		return ($options) ? $options : array();
	}

    /**
     * @return string
     */
    public function getQueryRelatorio()
    {
		global $db;
		
		if (count($_REQUEST['ungcod']) > 0){
			$whereG = " and tcp.ungcodconcedente in ('".implode("','", $_REQUEST['ungcod'])."')";
		} else {
			$whereG = " and tcp.ungcodconcedente IS NOT NULL";
		}
		
		$sql = "SELECT 
					numero_termo,
					unidade_proponente,
					unidade_concedente,
					resp_proponente,
					resp_concedente,
					situacao,
					coordenacao,
					tcpdscobjetoidentificacao,
					ano_referencia,
					sum(valor) as total
				FROM (
					SELECT DISTINCT 	       
					       foo.tcpid as numero_termo,
					       foo.unidadegestorap as unidade_proponente,
					       foo.unidadegestorac as unidade_concedente,
					       case when foo.altprop is not null then nomerpc else coalesce(coalesce(foo.prop, foo.nomerpc), ' - ') end as resp_proponente,
					       coalesce(coalesce(conc , foo.nomerpp), ' - ') as resp_concedente,
					       foo.esddsc as situacao,
					       foo.coodsc as coordenacao,
					       (select identificacao from ted.justificativa where tcpid = foo.tcpid) as tcpdscobjetoidentificacao,
					       pro1.proanoreferencia as ano_referencia,
					       pro1.valor as valor	
					FROM (			
							SELECT DISTINCT 
									tcp.tcpid,						
									unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
									unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,					
									rpc.nome as nomerpc,
									rpp.nome as nomerpp,
									( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1597 and hd.docid = tcp.docid order by hstid desc limit 1 ) as prop,
									( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid in (1612, 2442) and hd.docid = tcp.docid order by hstid desc limit 1 ) as conc,
									( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1620 and hd.docid = tcp.docid order by hstid desc limit 1 ) as altprop,									
									esd.esddsc as esddsc
									,coalesce(cdn.coodsc, '-') as coodsc,
									(select identificacao from ted.justificativa where tcpid = tcp.tcpid) as tcpdscobjetoidentificacao
							FROM ted.termocompromisso tcp
				
							LEFT JOIN ted.coordenacao cdn
								ON cdn.cooid = tcp.cooid
							LEFT JOIN public.unidadegestora unp 
								ON unp.ungcod = tcp.ungcodproponente			
							LEFT JOIN public.unidadegestora unc 
								ON unc.ungcod = tcp.ungcodconcedente
							LEFT JOIN ted.representantelegal rpp
								ON rpp.ug = tcp.ungcodconcedente
							LEFT JOIN ted.representantelegal rpc
								ON rpc.ug = tcp.ungcodproponente
							LEFT JOIN workflow.documento doc  
								ON doc.docid = tcp.docid
							LEFT JOIN workflow.estadodocumento esd  
								ON esd.esdid = doc.esdid
										
							WHERE tcpstatus = 'A' 
							AND tcp.tcpid in (select distinct tc.tcpid from ted.termocompromisso tc
								left join ted.previsaoorcamentaria po on tc.tcpid = po.tcpid
								where (po.proanoreferencia >= {$_SESSION['exercicio']} or po.proanoreferencia is null) and tcpstatus = 'A')			
				 {$whereG}
					) as foo
					left join (
						SELECT DISTINCT
							pro.tcpid,
							pro.proid,
							ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
							substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
							substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
							pro.ptrid,
							a.acacod,
							pro.pliid,
							case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
							pro.ndpid,
							to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
							coalesce(pro.provalor, 0) as valor,
							crdmesliberacao,
							crdmesexecucao,
							pro.proid,
							pro.proanoreferencia,
							pro.prodata
						FROM ted.previsaoorcamentaria pro
						LEFT JOIN monitora.pi_planointerno pi 		   ON (pi.pliid = pro.pliid)
						LEFT JOIN monitora.pi_planointernoptres pts    ON (pts.pliid = pi.pliid)
						LEFT JOIN public.naturezadespesa ndp 		   ON (ndp.ndpid = pro.ndpid)
						LEFT JOIN monitora.ptres p 			           ON (p.ptrid = pro.ptrid)
						LEFT JOIN monitora.acao a 		               ON (a.acaid = p.acaid)
						LEFT JOIN public.unidadegestora u 		       ON (u.unicod = p.unicod)
						LEFT JOIN monitora.pi_planointernoptres pt 	   ON (pt.ptrid = p.ptrid)
						WHERE pro.prostatus = 'A'
						--AND pro.proanoreferencia IS NOT NULL
						--AND crdmesliberacao IS NOT NULL
						--AND crdmesexecucao IS NOT NULL
					) as pro1 on pro1.tcpid = foo.tcpid
					order by foo.tcpid, pro1.proanoreferencia
				) AS foa
				GROUP BY numero_termo,unidade_proponente,unidade_concedente,resp_proponente,resp_concedente,situacao,
				coordenacao,tcpdscobjetoidentificacao,ano_referencia
				ORDER BY numero_termo, ano_referencia";
		
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
				"Numero do Termo", 
				"Unidade Proponente", 
				"Unidade Concedente", 
				"Responsável Proponente", 
				"Responsável Concedente", 
				"Situação",
				"Coordenação", 
				"Objeto", 
				"Ano Referência",
				"Valor Total"))
		  ->setQuery($sql);
		$list->addCallbackDeCampo(array('unidade_proponente', 'unidade_concedente', 'resp_proponente', 'resp_concedente', 'situacao', 'coordenacao', 'tcpdscobjetoidentificacao'), 'alinhaParaEsquerda');
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
            "Numero do Termo",
            "Unidade Proponente",
            "Unidade Concedente",
            "Responsável Proponente",
            "Responsável Concedente",
            "Situação",
            "Coordenação",
            "Objeto",
            "Ano Referência",
            "Valor Total"
        );
		
		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=rel_unidadegestora_".date("Ymdhis").".xls");
		header("Content-Disposition: attachment; filename=rel_unidadegestora_".date("Ymdhis").".xls");
		header("Content-Description: MID Gera excel");
		$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');		
	}
}