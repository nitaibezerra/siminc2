<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT true FROM par.controle_relatorio_demandas WHERE crddata::date = '".date('Y-m-d')."'::date"; 

$booEnviado = $db->pegaUm($sql);

if( $booEnviado == '' ){
	
	$cabecalho = "Demandas Concluidas Hoje";
	
	$sql = "-- SQL concluídas do dia
			SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
				to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				cmd.cmddsc,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM 
				par.demanda dmd
			INNER JOIN par.demandatipo  			dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento			doc ON doc.docid = dmd.docid
			INNER JOIN workflow.historicodocumento 	hst ON hst.docid = dmd.docid 
			INNER JOIN workflow.comentariodocumento cmd ON cmd.hstid = hst.hstid 
			INNER JOIN workflow.acaoestadodoc   	aed ON aed.aedid = hst.aedid AND aed.esdiddestino = 834
			WHERE 
				to_char(htddata, 'DD/MM/YYYY') = to_char(now(), 'DD/MM/YYYY')
			ORDER BY 
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr><td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>Demandas do PAR</b></label></td> </tr>
			</table><br>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr> <td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td> </tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
					<td width=35% align=center > <label class=TituloTela style=color:#000000; > Comentário </label> </td>
				</tr>
			";
	if( count($arrDados) > 0 ){
	
		foreach( $arrDados as $k => $dados ){
		
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
		
			$html .= "<tr >
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['cmddsc']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=7 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=7 ></td></tr>
			</table>
				</td></tr>
			</table><br>";
	
	$cabecalho = "Demandas Concluidas Este Mês";
	
	$sql = "SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
				to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				cmd.cmddsc,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM 
				par.demanda dmd
			INNER JOIN par.demandatipo  			dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento			doc ON doc.docid = dmd.docid
			INNER JOIN workflow.historicodocumento 	hst ON hst.docid = dmd.docid 
			INNER JOIN workflow.comentariodocumento cmd ON cmd.hstid = hst.hstid 
			INNER JOIN workflow.acaoestadodoc   	aed ON aed.aedid = hst.aedid AND aed.esdiddestino = 834
			WHERE 
				to_char(htddata, 'MMYYYY') = to_char(now(), 'MMYYYY')
			ORDER BY 
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr> <td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td> </tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=20% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=35% align=center > <label class=TituloTela style=color:#000000; > Comentário </label> </td>
				</tr>
			";
	if( count($arrDados) > 0 ){
	
		foreach( $arrDados as $k => $dados ){
		
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
		
			$html .= "<tr >
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['cmddsc']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=7 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=7 ></td></tr>
			</table>
				</td></tr>
			</table><br>";
	
	
	$cabecalho = "Demandas Devolvidas Hoje";
	
	$sql = "-- SQL concluídas do dia
			SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
				to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				cmd.cmddsc,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM
				par.demanda dmd
			INNER JOIN par.demandatipo  			dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento			doc ON doc.docid = dmd.docid
			INNER JOIN workflow.historicodocumento 	hst ON hst.docid = dmd.docid
			INNER JOIN workflow.comentariodocumento cmd ON cmd.hstid = hst.hstid
			INNER JOIN workflow.acaoestadodoc   	aed ON aed.aedid = hst.aedid AND aed.esdiddestino = 832
			WHERE
				to_char(htddata, 'DD/MM/YYYY') = to_char(now(), 'DD/MM/YYYY')
			ORDER BY
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr><td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td> </tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
					<td width=35% align=center > <label class=TituloTela style=color:#000000; > Comentário </label> </td>
				</tr>";
	
	if( count($arrDados) > 0 ){
	
		foreach( $arrDados as $k => $dados ){
	
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
			
			$html .= "<tr >
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
						<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['cmddsc']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=7 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=7 ></td></tr>
			</table>
				</td></tr>
			</table><br>";
	
	$cabecalho = "Demandas Devolvidas Este Mês";
	
	$sql = "SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
					to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				cmd.cmddsc,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM
				par.demanda dmd
			INNER JOIN par.demandatipo  			dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento			doc ON doc.docid = dmd.docid
			INNER JOIN workflow.historicodocumento 	hst ON hst.docid = dmd.docid
			INNER JOIN workflow.comentariodocumento cmd ON cmd.hstid = hst.hstid
			INNER JOIN workflow.acaoestadodoc   	aed ON aed.aedid = hst.aedid AND aed.esdiddestino = 832
			WHERE
				to_char(htddata, 'MMYY') = to_char(now(), 'MMYY')
			ORDER BY
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr><td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td> </tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=20% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
					<td style=\"border:1pt solid #eeeeee;\" width=35% align=center > <label class=TituloTela style=color:#000000; > Comentário </label> </td>
				</tr>";
	
	if( count($arrDados) > 0 ){
	
		foreach( $arrDados as $k => $dados ){
		
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
		
			$html .= "<tr >
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['cmddsc']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=7 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=7 ></td></tr>
				</table>
				</td></tr>
			</table><br>";
	
	$cabecalho = "Demandas Cadastradas Hoje";
	
	$sql = "-- SQL cadastradas do dia
			SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
				to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM 
				par.demanda dmd
			INNER JOIN par.demandatipo  	dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento 	doc ON doc.docid = dmd.docid
			WHERE 
				to_char(docdatainclusao, 'DD/MM/YYYY') = to_char(now(), 'DD/MM/YYYY')
			ORDER BY 
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td>
				</tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td width=55% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
				</tr>
			"; 
	if( count($arrDados) > 0 ){
		
		foreach( $arrDados as $k => $dados ){
		
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
		
			$html .= "<tr >
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=6 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=6 ></td></tr>
			</table>
				</td></tr>
			</table><br>";
	
	$cabecalho = "Demandas Cadastradas Neste Mês";
	
	$sql = "-- SQL cadastradas do mês
			SELECT DISTINCT
				dmd.dmdprioridade,
				dmt.dmtnome,
				'('||dmd.dmdid||') '||dmd.dmdnome as  nome_da_demanda,
				to_char(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
				dmd.dmddescricao,
				doc.esdid,
				dmd.dmdurgente,
				dmd.dmdqtdpriorizado
			FROM 
				par.demanda dmd
			INNER JOIN par.demandatipo  	dmt ON dmt.dmtid = dmd.dmtid
			INNER JOIN workflow.documento 	doc ON doc.docid = dmd.docid
			WHERE 
				to_char(docdatainclusao, 'MMYY') = to_char(now(), 'MMYY')
			ORDER BY 
				dmd.dmdprioridade;";
	
	$arrDados = $db->carregar($sql);
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
				<tr> <td width=100% colspan=3 align=center><label class=TituloTela style=color:#003366;><b>$cabecalho - (".count($arrDados).")</b></label></td> </tr>
				<tr><td>";
	
	$html .= "<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"font-family: Arial; font-size: 11px; border-top: none; border-bottom: none;\" width=98% >
				<tr>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Prioridade </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > N° de Priorizações </label> </td>
					<td width=10% align=center > <label class=TituloTela style=color:#000000; > Tipo </label> </td>
					<td width=20% align=center > <label class=TituloTela style=color:#000000; > Titulo da Demanda </label> </td>
					<td width=55% align=center > <label class=TituloTela style=color:#000000; > Demanda </label> </td>
					<td width=5% align=center > <label class=TituloTela style=color:#000000; > Data de Criação<br> da Demanda </label> </td>
				</tr>
			";
	if( count($arrDados) > 0 ){
	
		foreach( $arrDados as $k => $dados ){
		
			$cor 		= $k%2 == 0 ? 'white' : '#eeeeee';
			$borda 		= $k%2 == 0 ? "border-right: 1pt solid #eeeeee;" : '';
			$urgente	= $dados['dmdurgente'] == 't' ? 'color:red;' : '';
		
			$html .= "<tr >
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdprioridade']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmdqtdpriorizado']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmtnome']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['nome_da_demanda']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['dmddescricao']} </td>
							<td align=center bgcolor=$cor style=\"$borda $urgente\" > {$dados['data_inclusao']} </td>
					</tr>";
		}
	}else{
		$html .= "<tr bgcolor=white > <td align=center colspan=6 style=color:red; > Nenhuma demanda </td> </tr>";
	}
	
	$html .= "	<tr><td align=center colspan=6 ></td></tr>
			</table>
				</td></tr>
			</table><br>";
	
	echo $html;

	if( !$_REQUEST['n_envia'] ){
		
		echo "<br>enviando...<br>";
		
		$us = array($_SESSION['email_sistema']);
		
		enviar_email(array("nome"=>"EQUIPE ". SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br"), $us, "Relatório Diário de Demandas - PAR", $html);
		
		$sql = "INSERT INTO par.controle_relatorio_demandas( crddsc ) VALUES ( 'Relatório enviado as ".date('H:i:s')." de ".date('d/m/Y')."' ); ";
		
		$db->executar( $sql );
		$db->commit();
		$db->close();
	}
}else{
?>
	<table border=0 cellspacing=0 cellpadding=3 align=center bgcolor=#DCDCDC class=tabela style=\"border-top: none; border-bottom: none;\" width=98% >
		<tr><td width=100% colspan=3 align=center><label class=TituloTela style=color:red;><b>E-mail já foi enviado hoje.</b></label></td></tr>
	</table>
<?php 
}
?>