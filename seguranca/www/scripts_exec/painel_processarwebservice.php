<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/painel/_constantes.php';
include_once APPRAIZ . 'www/painel/_funcoesagendamentoindicador.php';

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

date_default_timezone_set ('America/Sao_Paulo');

//referente painel.coleta
define("COLETA_AUTOMATICA", 2);// tipo automatica

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
	$_SESSION['usucpforigem'] = '00000000191';
	$auxusucpf = '00000000191';
	$auxusucpforigem = '00000000191';
}else{
	$auxusucpf = $_SESSION['usucpf'];
	$auxusucpforigem = $_SESSION['usucpforigem'];
}

//ver($auxusucpf);

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$HTML .=  "Início do processamento (".date("d/m/Y h:i:s").")<br>";

if(!$_REQUEST['fecharjanela']) {
	/*
	 * 
	 */
	$sql = "SELECT * FROM painel.webservicefiles WHERE wbsstatus='A'";
	$agendwbs = $db->carregar($sql);
	
	if($agendwbs[0]) {
		foreach($agendwbs as $ag) {
			$cc = explode("_",$ag['wbsdsc']);
			$dd_wbs['indid'] = $cc[1];
			$_SESSION['indid'] = $cc[1];
			$_SESSION['usucpf'] = $ag['usucpf'];
			$_SESSION['usucpforigem'] = $ag['usucpf'];
			$dd_wbs['agddataprocessamento'] = formata_data($cc[0]);
			$dd_wbs['csvarray'] = file(DIRFILES . "painel/webservice_files/".$ag['wbsdsc'].".csv");
			$html = date("d/m/Y H:i:s")." :: ".enviarAgendamentoWebService($dd_wbs);
			$sql = "UPDATE painel.webservicefiles SET wbslog=wbslog||'".addslashes($html)."', wbsstatus='P'
 					WHERE wbsid='".$ag['wbsid']."'";
			$db->executar($sql);
			$db->commit();
			$_SESSION['usucpf']=$auxusucpf;
			$_SESSION['usucpforigem']=$auxusucpforigem;
			unset($_SESSION['indid'],$dd_wbs);
			$HTML .= "Arquivo carregado do webservice '".$ag['wbsdsc'].".csv' com sucesso<br>";
			
		}
	} else {
		$HTML .= "Não existem arquivos do webservice para serem carregados<br>";
	}
	
	// limpando variaveis
	unset($agendwbs);
}

$sql = "SELECT * FROM painel.agendamentocarga WHERE (agddataprocessamento='".(($_REQUEST['dataprocessamento'])?$_REQUEST['dataprocessamento']:date("Y-m-d"))."' OR agddataprocessamento= DATE(NOW() - INTERVAL '1 DAY')) AND agdstatus='A' AND agdprocessado=false ".(($_REQUEST['agdid'])?"AND agdid='".$_REQUEST['agdid']."'":"");
$agendamentos = $db->carregar($sql);

/*
 * CARREGANDO TODOS OS AGENDAMENTOS A SEREM PROCESSADOS NAQUELE DIA
 *  
 */
if($agendamentos[0]) {
	foreach($agendamentos as $agen) {
		$sql = "SELECT DISTINCT COALESCE(agd.indid,0)||'.'||COALESCE(agd.dpeid,0)||'.'||COALESCE(agd.ddiid,0)||'.'||COALESCE(agd.acdmuncod,'0')||'.'||COALESCE(agd.acduf,'0')||'.'||COALESCE(agd.acdesciescod,'0')||'.'||COALESCE(agd.tidid1,'0')||'.'||COALESCE(agd.tidid2,'0')||'.'||COALESCE(agd.iepid,'0')||'.'||COALESCE(agd.entid,'0')||'.'||COALESCE(agd.unicod,'0')||'.'||COALESCE(agd.polid,'0')||'.'||COALESCE(agd.iecid,'0') as identificador, * FROM painel.agendamentocargadados agd 
				LEFT JOIN painel.indicador ind ON ind.indid = agd.indid 
				WHERE agdid='".$agen['agdid']."'";
		
		$dadosagendamento = $db->carregar($sql);
		
		unset($dadosconsolidados, $dadosduplicados);
		/*
		 * CARREGANDO OS REGISTROS DO AGENDAMENTOS
		 */
		if($dadosagendamento[0]) {
			/*
			 * ARMAZENANDO OS DADOS DO AGENDAMENTO POR INDICADOR E PERÍODO DETALHES
			 */
			$dadosduplicados = false;
			foreach($dadosagendamento as $dadosagen) {
				$tot += $dadosagen['acdqtde'];
				if(!$dadosconsolidados[$dadosagen['indid']][$dadosagen['dpeid']][$dadosagen['identificador']]) {
					$dadosconsolidados[$dadosagen['indid']][$dadosagen['dpeid']][$dadosagen['identificador']] = $dadosagen;
				} else {
					$dadosduplicados[] = array('indid' => $dadosagen['indid'], 'dpeid' => $dadosagen['dpeid'], 'identificador' => $dadosagen['identificador']);
				}
			
			}

			/*
			 * VARRENDO OS DADOS E APLICANDO AS REGRAS
			 */

			if($dadosconsolidados) {
				foreach($dadosconsolidados as $indid => $valores1) {
					foreach($valores1 as $dpeid => $valores2) {
						
						$regidseh = $db->pegaUm("SELECT regid FROM painel.indicador WHERE indid='".$indid."'");
						
						$sql = "INSERT INTO painel.seriehistorica(indid, sehstatus, dpeid, sehdtcoleta, sehqtde, regid)
	    				 		VALUES ('".$indid."', 'H', '".$dpeid."', NOW(), '0',".(($regidseh)?"'".$regidseh."'":"NULL").") RETURNING sehid;";
						
						$sehid = $db->pegaUm($sql);
						
						if($valores2) {
							foreach($valores2 as $vlrdetalhe) {
								$valorcumulativo += $vlrdetalhe['acdvalor'];
								$qtdecumulativo += $vlrdetalhe['acdqtde'];
								$sqls = "INSERT INTO painel.detalheseriehistorica(
								            ddiid, sehid, dshvalor, dshcod, dshcodmunicipio, dshuf, dshqtde, tidid1, tidid2, iepid, entid, unicod, polid, iecid)
								    	VALUES (".(($vlrdetalhe['ddiid'])?"'".$vlrdetalhe['ddiid']."'":"NULL").", '".$sehid."', ".(($vlrdetalhe['acdvalor'])?"'".$vlrdetalhe['acdvalor']."'":"NULL").",
								    			".(($vlrdetalhe['acdesciescod'])?"'".$vlrdetalhe['acdesciescod']."'":"NULL").", ".(($vlrdetalhe['acdmuncod'])?"'".$vlrdetalhe['acdmuncod']."'":"NULL").", 
								    			".(($vlrdetalhe['acduf'])?"'".$vlrdetalhe['acduf']."'":"NULL").", ".((trim($vlrdetalhe['acdqtde']))?trim($vlrdetalhe['acdqtde']):"0").", 
								    			".(($vlrdetalhe['tidid1'])?"'".$vlrdetalhe['tidid1']."'":"NULL").", ".(($vlrdetalhe['tidid2'])?"'".$vlrdetalhe['tidid2']."'":"NULL").",
								    			".(($vlrdetalhe['iepid'])?"'".$vlrdetalhe['iepid']."'":"NULL").", ".(($vlrdetalhe['entid'])?"'".$vlrdetalhe['entid']."'":"NULL").",
								    			".(($vlrdetalhe['unicod'])?"'".$vlrdetalhe['unicod']."'":"NULL").", ".(($vlrdetalhe['polid'])?"'".$vlrdetalhe['polid']."'":"NULL").", 
								    			".(($vlrdetalhe['iecid'])?"'".$vlrdetalhe['iecid']."'":"NULL").")";
								$db->executar($sqls, false);
							}
						}
						$db->executar("UPDATE painel.seriehistorica SET sehqtde=(SELECT sum(qtde) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."'), sehvalor=(SELECT sum(valor) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."') WHERE sehid='".$sehid."'");
						
					}

					/*
					 * limpando series historicas
					 */
					$sql = "UPDATE painel.seriehistorica SET sehstatus='H' WHERE indid='".$indid."' AND sehstatus='A'";
					$db->executar($sql);
					$sql = "SELECT seh.sehid, seh.dpeid FROM painel.seriehistorica seh 
							LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
							WHERE indid='".$indid."' AND (sehstatus='A' OR sehstatus='H') ORDER BY dpedatainicio DESC, sehid DESC LIMIT 1";
					$seriemaior = $db->pegaLinha($sql);
					
					$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehid='".$seriemaior['sehid']."'";
					$db->executar($sql);
					
					// Apaga dados do detalhe serie historica
					$sql = "DELETE FROM painel.detalheseriehistorica WHERE sehid in (SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$seriemaior['dpeid']."' AND indid='".$indid."' AND sehid!='".$seriemaior['sehid']."')";
					$db->executar($sql);

					$sql = "DELETE FROM painel.seriehistorica WHERE dpeid='".$seriemaior['dpeid']."' AND indid='".$indid."' AND sehid!='".$seriemaior['sehid']."'";
					$db->executar($sql);

					$sql = "SELECT * FROM painel.seriehistorica WHERE indid='".$indid."' AND sehstatus='H' ORDER BY sehid DESC";
					$serieoutros = $db->carregar($sql);
					unset($dpe);
					if($serieoutros[0]) {
						foreach($serieoutros as $ser) {
							if($dpe[$ser['dpeid']]) {
								$sql = "DELETE FROM painel.tipoconteudografico WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
								$sql = "DELETE FROM painel.detalheseriehistorica WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
								$sql = "DELETE FROM painel.seriehistorica WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
							} else {
								$dpe[$ser['dpeid']]=true;
							}
						}
					}
					/*
					 * limpando series historicas
					 */
				}
			}
			$db->executar("UPDATE painel.agendamentocarga SET agdprocessado=true WHERE agdid='".$dadosagen['agdid']."'");
			$db->commit();
			
			$HTML .=  "Agendamento #".$dadosagen['agdid']."(ind.".$indid.") foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		} else {
			$HTML .=  "O agendamento #".$agen['agdid']." não possui dados<br>";
			$db->executar("UPDATE painel.agendamentocarga SET agdprocessado=true WHERE agdid='".$agen['agdid']."'");
			$db->commit();
		}
	}
} else {
	$HTML .= "Não existem agendamentos ate a data '".date("d/m/Y h:i:s")."'<br>";
}

if($_REQUEST['fecharjanela']==true) {
	echo "<script>
			alert('Processamento efetuado com sucesso.');
			window.close();
		  </script>";
} else {
	$db->executar("delete from painel.agendamentocargadados where agdid in(select agdid from painel.agendamentocarga where agdstatus='I' or agdprocessado is true)");
	$HTML .=  "DELETE TBL_agendamentocargadados foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.detalheseriehistorica where dshid in(select d.dshid from painel.seriehistorica s inner join painel.detalheseriehistorica d on s.sehid=d.sehid where sehstatus='I')");
	$HTML .=  "DELETE TBL_detalheseriehistorica foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.tipoconteudografico where sehid in(select sehid from painel.seriehistorica where sehstatus='I')");
	$HTML .=  "DELETE TBL_tipoconteudografico foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.seriehistorica where sehstatus='I'");
	$HTML .=  "DELETE TBL_seriehistorica foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->commit();
	//Retirado o REINDEX pois o Banco de Dados já está realizando essa ação. 22/05/2013
	//if(!$_REQUEST['indidat']) {
	//	$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_entid;");
	//	$HTML .=  "REINDEX idx_detalheseriehistorica_entid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.idx_dshsehid;");
	//	$HTML .=  "REINDEX idx_dshsehid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_iepid;");
	//	$HTML .=  "REINDEX idx_detalheseriehistorica_iepid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_unicod;");
	//	$HTML .=  "REINDEX idx_detalheseriehistorica_unicod foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshcod;");
	//	$HTML .=  "REINDEX ix_detalheserirhistorica_dshcod foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshcodmunicipio;");
	//	$HTML .=  "REINDEX ix_detalheserirhistorica_dshcodmunicipio foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshuf;");
	//	$HTML .=  "REINDEX ix_detalheserirhistorica_dshuf foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_sehiddshcodmunicipio;");
	//	$HTML .=  "REINDEX ix_detalheserirhistorica_sehiddshcodmunicipio foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_sehiddshuf;");
	//	$HTML .=  "REINDEX ix_detalheserirhistorica_sehiddshuf foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	//	$db->commit();
	//}
	
	/*
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "SISTEMA DE PROCESSAMENTO DE AGENDAMENTOS";
	$mensagem->From 		= $_SESSION['email_sistema'];
	$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
	$mensagem->Subject = "Processamento do webservice";
	
	ob_start();
	echo "<pre>";
	print_r($_SERVER);
	$dadosserv = ob_get_contents();
	ob_end_clean();
	
	$mensagem->Body = $inicioscript."<br /><br />".$HTML."<br /><br />".$dadosserv;
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */
}
?>