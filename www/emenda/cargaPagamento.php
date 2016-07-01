<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT distinct pta, ptrid, autor, uf, emenda, ptres, funcional_rogramatica, subtitulo, valor_emenda, entidade, valor_total, valor_empenhado,
  			empenho, numero_empenho, termo_compromisso, pagamento, valor_pago
		FROM 
		  	emenda.cargapagamento_par c
            inner join emenda.planotrabalho p on p.ptrcod = cast(substring(trim(c.pta),1,4) as integer)
        where pta is not null order by ptrid";

$arrDados = $db->carregar($sql);

foreach ($arrDados as $v) {
	
	$sql = "SELECT em.empnumeroprocesso, em.empcodigoptres, em.empcodigopi,
				em.empcentrogestaosolic, em.empnumero, em.empsituacao,
				e.eobvalorempenho, e.eobpercentualemp
			FROM par.subacaoemendapta sep
				inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
				inner join par.subacao s on s.sbaid = sd.sbaid
				inner join par.empenhosubacao e on e.sbaid = s.sbaid -- se tiver nesta tabela teve empenho
				inner join par.empenho em on em.empid = e.empid and empstatus = 'A' and eobstatus = 'A'
				inner join emenda.ptemendadetalheentidade ped on ped.ptrid = sep.ptrid
				inner join emenda.v_emendadetalheentidade ved on ved.edeid = ped.edeid
			WHERE
				sep.ptrid = {$v['ptrid']}
				and sep.sepstatus = 'A'
				and ved.edestatus = 'A'";
	
	$arrDadosEmpPar = $db->carregar( $sql );
	$arrDadosEmpPar = $arrDadosEmpPar ? $arrDadosEmpPar : array();
	
	if( $arrDadosEmpPar[0] ){	
		$sql = "SELECT exf.exfid, exf.tpeid, exf.pliid, exf.ptrid, exf.pedid, exf.exfvalor, exf.exfcodmunicipiosiafi, exf.exfnumsolempenho, exf.exfespecieempenho,
	  				exf.exfcodfontesiafi, exf.semid, exf.exfdataalteracao, exf.exfanooriginal, exf.exfnumempenhooriginal, exf.exfidpai, exf.exfnaturezadespesa,
	  				exf.exfverifsiafi, exf.exfverifcadin, exf.exfdatainclusao, exf.usucpf, exf.exfdataemisao, exf.plicod, exf.ptres
				FROM 
	            	emenda.execucaofinanceira exf
				WHERE  
	            	exf.ptrid = {$v['ptrid']} 
	            	and exf.exfstatus = 'A'";
				
		$arrDadosExf = $db->carregar($sql);
		$arrDadosExf = $arrDadosExf ? $arrDadosExf : array();
		
		if( $arrDadosExf[0] ){
			if( sizeof($arrDadosExf) > 1 ) {
				ver($v['pta'], $sql, $arrDadosExf);				
			}
		}
	}
}


?>