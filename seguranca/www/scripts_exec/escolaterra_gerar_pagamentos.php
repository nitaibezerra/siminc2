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
require_once APPRAIZ . "www/escolaterra/_constantes.php";
require_once APPRAIZ . "www/escolaterra/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

//gerando bolsas de tutores
$sql = "select r.iusid, r.fpbid, p.plpvalor, t.pflcod, i.ufpid, t.tpeid, p.plpmaximobolsas from escolaterra.relatorioacompanhamento r 
		inner join workflow.documento d on d.docid = r.docid 
		inner join escolaterra.identificacaousuario i on i.iusid = r.iusid 
		inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
		inner join escolaterra.pagamentoperfil p on p.pflcod = t.pflcod 
		left join escolaterra.pagamentobolsista pg on pg.iusid = i.iusid and pg.fpbid = r.fpbid 
		where d.esdid='".ESD_LIBERADO_PAGAMENTO."' and pg.pboid is null and i.iustermocompromisso=true";

$pagamentosbolsistas = $db->carregar($sql);
		
if($pagamentosbolsistas[0]) {
	foreach($pagamentosbolsistas as $pg) {
		
		$qtd_usu = $db->pegaUm("SELECT count(*) FROM escolaterra.pagamentobolsista WHERE iusid='".$pg['iusid']."'");
		$qtd_fun = $db->pegaUm("SELECT count(*) FROM escolaterra.pagamentobolsista WHERE tpeid='".$pg['tpeid']."'");
		
		if(($qtd_usu < $pg['plpmaximobolsas']) && ($qtd_fun < $pg['plpmaximobolsas'])) {
		
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento escola terra us ".$pg['iusid']." tp ".$pg['tpeid']." fp ".$pg['fpbid']);
						
			$sql = "INSERT INTO escolaterra.pagamentobolsista(
				            iusid, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
				            pflcod, ufpid, tpeid)
				    VALUES ('".$pg['iusid']."', '".$pg['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$pg['plpvalor']."', 
				            '".$pg['pflcod']."', '".$pg['ufpid']."', '".$pg['tpeid']."');";
							
			$db->executar($sql);
			$db->commit();
			
			$tutorpg++;
		
		}

				
	}
			
}

// gerando bolsas de coordenadores estaduais
$sql = "select r.iusid, r.fpbid, p.plpvalor, t.pflcod, i.ufpid, t.tpeid, p.plpmaximobolsas from escolaterra.relatorioacompanhamentocoordenadorestadual r
		inner join workflow.documento d on d.docid = r.docid
		inner join escolaterra.identificacaousuario i on i.iusid = r.iusid
		inner join escolaterra.tipoperfil t on t.iusid = i.iusid
		inner join escolaterra.pagamentoperfil p on p.pflcod = t.pflcod
		left join escolaterra.pagamentobolsista pg on pg.iusid = i.iusid and pg.fpbid = r.fpbid
		where d.esdid='".ESD_LIBERADO_PAGAMENTO_COORDENADORESTADUAL."' and pg.pboid is null and i.iustermocompromisso=true";

$pagamentosbolsistas = $db->carregar($sql);

if($pagamentosbolsistas[0]) {
	foreach($pagamentosbolsistas as $pg) {
		
		$qtd_usu = $db->pegaUm("SELECT count(*) FROM escolaterra.pagamentobolsista WHERE iusid='".$pg['iusid']."'");
		$qtd_fun = $db->pegaUm("SELECT count(*) FROM escolaterra.pagamentobolsista WHERE tpeid='".$pg['tpeid']."'");
		
		if(($qtd_usu < $pg['plpmaximobolsas']) && ($qtd_fun < $pg['plpmaximobolsas'])) {

			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento escola terra us ".$pg['iusid']." tp ".$pg['tpeid']." fp ".$pg['fpbid']);
				
			$sql = "INSERT INTO escolaterra.pagamentobolsista(
				            iusid, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento,
				            pflcod, ufpid, tpeid)
				    VALUES ('".$pg['iusid']."', '".$pg['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$pg['plpvalor']."',
				            '".$pg['pflcod']."', '".$pg['ufpid']."', '".$pg['tpeid']."');";
	
			$db->executar($sql);
			$db->commit();
			
			$coordenadorestatualpg++;
		
		}


	}
		
}


echo "Bolsas de tutores inseridas : ".$tutorpg."<br>";
echo "Bolsas de coordenadores estaduais inseridas : ".$coordenadorestatualpg."<br>";


echo "fim!!!!";

?>