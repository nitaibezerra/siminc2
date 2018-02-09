<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisfor/_constantes.php";
require_once APPRAIZ . "www/sisfor/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();

   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "select i.iusd,
                   r.fpbid,
                   pp.plpvalor,
                   p.pflcod,
                   t.tpeid 
		from sisfor.relatoriomensal r
		inner join sisfor.identificacaousuario i on i.iusd = r.iusd 
		inner join sisfor.tipoperfil t on t.iusd = i.iusd and t.tpebolsa=true and t.pflcod='".PFL_COORDENADOR_INST."'
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		inner join sisfor.pagamentoperfil pp on pp.pflcod = p.pflcod 
		inner join workflow.documento d on d.docid = r.docid 
		left join sisfor.pagamentobolsista pb on pb.iusd = r.iusd and pb.fpbid = r.fpbid
		where d.esdid='".ESD_RELATORIOMENSAL_EMPAGAMENTO."' and i.iustermocompromisso=true and pb.pboid is null and i.iusnaodesejosubstituirbolsa!=true";

$pagamentosinstitucionais = $db->carregar($sql);

if($pagamentosinstitucionais[0]) {
	foreach($pagamentosinstitucionais as $pg) {
		
		$docid = wf_cadastrarDocumento(WF_TPDID_PAGAMENTOBOLSA, "Pagamento SISFOR");
			
		$sql = "INSERT INTO sisfor.pagamentobolsista(
						            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento,
						            pflcod, tpeid)
						    VALUES ('".$pg['iusd']."', '".$pg['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$pg['plpvalor']."',
						            '".$pg['pflcod']."', '".$pg['tpeid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
}



$sql = "select * from sisfor.folhapagamentoprojeto fp 
		inner join workflow.documento d on d.docid = fp.docid 
		where d.esdid=".ESD_ENVIADO_PAGAMENTO;

$folhap = $db->carregar($sql);

if($folhap[0]) {
	foreach($folhap as $f) {
		
		$sql = "select 
				   iu.iusd,
                   m.fpbid,
                   pp.plpvalor,
                   p.pflcod,
                   t.tpeid
	            from sisfor.mensario m
	                inner join sisfor.tipoperfil t on t.tpeid = m.tpeid and t.tpebolsa=true
	                inner join seguranca.perfil p on p.pflcod = t.pflcod 
	                inner join sisfor.pagamentoperfil pp on pp.pflcod = p.pflcod 
	                inner join sisfor.identificacaousuario iu on iu.iusd = t.iusd
	                left  join  sisfor.mensarioavaliacoes ma on ma.menid = m.menid 
	                left join sisfor.identificacaousuario ia on ia.iusd = ma.iusdorientador 
	                left join sisfor.pagamentobolsista pb on pb.iusd = iu.iusd and pb.fpbid = m.fpbid
	            where t.sifid = '{$f['sifid']}' and m.fpbid = '{$f['fpbid']}' and mavatividadesrealizadas='A' and iu.iustermocompromisso=true and pb.pboid is null 
				order by m.fpbid";
		
		$pagamentosbolsistas = $db->carregar($sql);
		
		if($pagamentosbolsistas[0]) {
			foreach($pagamentosbolsistas as $pg) {
				
				$erros = verificarCriacaoPagamento(array('tpeid' => $pg['tpeid'],'fpbid' => $pg['fpbid']));
				
				if(!$erros) {
				
					$docid = wf_cadastrarDocumento(WF_TPDID_PAGAMENTOBOLSA, "Pagamento SISFOR");
					
					$sql = "INSERT INTO sisfor.pagamentobolsista(
						            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento,
						            pflcod, tpeid)
						    VALUES ('".$pg['iusd']."', '".$pg['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$pg['plpvalor']."',
						            '".$pg['pflcod']."', '".$pg['tpeid']."');";
						
					$db->executar($sql);
					$db->commit();
					
					$i++;
				
				} else {
					$x++;
				}

				
			}
			
			echo $i." pagamentos inseridos<br>";
			echo $x." pagamentos com problemas de limites<br>";
		}
	}
}


$sql = "select p.docid from sisfor.pagamentobolsista p 
inner join workflow.documento d on d.docid = p.docid 
where d.esdid=".ESD_PAGAMENTO_RECUSADO;

$docids = $db->carregarColuna($sql);

if($docids) {
	foreach($docids as $docid) {
		wf_alterarEstado( $docid, AED_AUTORIZAR_RECUSADO_PAGAMENTO, '', array());
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sisfor_gerar_pagamentos.php'";
$db->executar($sql);
$db->commit();


$db->close();

echo "fim!!!!";

?>