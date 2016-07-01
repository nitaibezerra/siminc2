<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Boletim Estatístico por advogado.";
monta_titulo( $titulo_modulo, '' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">

function validaFormulario(){
	$('#loader-container').slideDown();
	var inicio = document.getElementById('htddata_inicio');
	var fim = document.getElementById('htddata_fim');
	//valida data
	if(inicio.value != '' && fim.value != ''){
		if(!validaData(inicio)){
			alert("Data Início Inválida.");
			inicio.focus();
			$('#loader-container').slideUp();
			return false;
		}		
		if(!validaData(fim)){
			alert("Data Fim Inválida.");
			fim.focus();
			$('#loader-container').slideUp();
			return false;
		}		
	}
	$('#filtro').submit();
}

$(document).ready(function()
{
	$('#loader-container').slideUp();
});

</script>
<div id="loader-container" style="position: absolute; background-color: white; opacity: .6; width:110%; height:2000%; margin-top: -200px; margin-left: -20px; Z-index:22;" >
	<center>
		<div id="loader" style="margin-top:300px;">
			<img src="../imagens/wait.gif" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</center>
</div>
<form action="" method="post" name="filtro" id="filtro">
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Período do Boletim:</td>
			<td>
				<?php 
				$_REQUEST['htddata_inicio'] = $_REQUEST['htddata_inicio'] ? $_REQUEST['htddata_inicio'] : '01/'.date('m').'/'.date('Y');
				$_REQUEST['htddata_fim']    = $_REQUEST['htddata_fim'] ? $_REQUEST['htddata_fim'] : '01/'.str_pad((date('m')+1), 2, "0", STR_PAD_LEFT).'/'.date('Y');
				$htddata_inicio = explode('/', $_REQUEST['htddata_inicio']);
				$htddata_inicio = $htddata_inicio[1].'/'.$htddata_inicio[0].'/'.$htddata_inicio[2];
				$htddata_fim = explode('/', $_REQUEST['htddata_fim']);
				$htddata_fim = $htddata_fim[1].'/'.$htddata_fim[0].'/'.$htddata_fim[2];
				?>
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Filtrar" style="cursor: pointer;" class="filtrar" onclick="validaFormulario()"/>
			</td>
		</tr>
	</table>
</form>
<form action="" method="post" name="formulario">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
				<b style="font-size: 10pt;"><br/></b>
			</td>
		</tr>
		<?php 
		
		$whereLEFT1 = "";
		$whereLEFT2 = "";
		
		// período
		if( $_REQUEST['htddata_inicio'] && $_REQUEST['htddata_fim'] ){
			$htddata_inicio = formata_data_sql($_REQUEST['htddata_inicio']);
			$htddata_fim = formata_data_sql($_REQUEST['htddata_fim']);
			$whereLEFT = " AND had.dtatribuicao::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
			$whereLEFT1 = " AND hst.htddata::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
			$whereLEFT11 = " AND hst2.htddata::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
			$whereLEFT2 = " AND nud.nuddatageracao::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
			$whereLEFT3 = " WHERE htddata::date < '$htddata_fim'::date";
			$whereLEFT4 = "AND hco.dtatribuicao::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
		}
		
		$sql = "SELECT
					entnome,
					sum(qtd_processos) as qtd_processos,
					sum(qtd_analises) as qtd_analises,
					sum(qtd_numeracao) as qtd_numeracao,
					sum(qtd_processos_atuais) as qtd_processos_atuais,
					sum(qtd_rec) as qtd_rec
				FROM
				(
					(
					--Advogados
					SELECT DISTINCT
						ent.entnome,
						--adv.advid,
						count(DISTINCT hst.docid) as qtd_processos,
						count(DISTINCT hst2.hstid) as qtd_analises,
						count(DISTINCT nudid) as qtd_numeracao,
						count(DISTINCT hta.docid) as qtd_processos_atuais,
						count(DISTINCT hst_acao2.docid) as qtd_rec
					FROM
						conjur.advogados adv
					INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
					--Coluna 1, 2
					LEFT  JOIN conjur.historicoadvogados had ON had.advid = adv.advid $whereLEFT
					LEFT  JOIN workflow.historicodocumento hst ON hst.hstid = had.hstid $whereLEFT1
					LEFT  JOIN workflow.historicodocumento hst2 ON hst2.docid = hst.docid $whereLEFT11 
																   AND hst2.aedid IN (".WF_AEDID_ENCAMINHAR_ADVOGADO.",
																   					  ".WF_AEDID_RETORNAR_ADVOGADO.",
																					  ".WF_AEDID_ENCAMINHAR_CHEFE_DIVISAO_CGAE.",
																					  ".WF_AEDID_ENCAMINHAR_COORD_CGAE.",
																					  ".WF_AEDID_RETORNAR_CHEFE_DIVISAO_CGAE.",
																					  ".WF_AEDID_RETORNAR_COORD_CGAE.")
					--Coluna 3
					LEFT  JOIN conjur.numeracaodocumento nud ON nud.advid = adv.advid $whereLEFT2
					--Coluna 4
					LEFT  JOIN (SELECT max(hstid) as hstid, docid FROM workflow.historicodocumento $whereLEFT3 GROUP BY docid) hta ON hta.hstid = had.hstid
					--Coluna 5
					LEFT  JOIN (SELECT
									h1.hstid,
									min(h2.hstid) as hstid2,
									h1.docid
							    FROM
									workflow.historicodocumento h1
							    LEFT JOIN workflow.historicodocumento h2 ON h2.hstid > h1.hstid AND h1.docid = h2.docid
							    WHERE
									h1.aedid in (".WF_AEDID_RETORNAR_COORDENADOR.",
												 ".WF_AEDID_RETORNAR_COORDENADOR_CGAE.",
												 ".WF_AEDID_RETORNAR_COORDENADOR_CGAE_4.")
							    GROUP BY
									h1.hstid, h1.docid ) hst_acao1 ON hst_acao1.docid = hst.docid
					LEFT  JOIN workflow.historicodocumento hst_acao2 ON hst_acao2.docid = hst_acao1.docid
																	    AND hst_acao2.hstid = hst_acao1.hstid2 
																	    AND hst_acao2.aedid IN (".WF_AEDID_ENCAMINHAR_ADVOGADO.",
																	    						".WF_AEDID_RETORNAR_ADVOGADO.",
																							  	".WF_AEDID_ENCAMINHAR_CHEFE_DIVISAO_CGAE.",
																							  	".WF_AEDID_ENCAMINHAR_COORD_CGAE.",
																							  	".WF_AEDID_RETORNAR_CHEFE_DIVISAO_CGAE.",
																							  	".WF_AEDID_RETORNAR_COORD_CGAE.") 
					WHERE
						adv.advstatus = 'A'
						--AND adv.advid NOT IN (SELECT DISTINCT advid FROM conjur.coordenacao WHERE advid IS NOT NULL )
					GROUP BY
						ent.entnome,
						adv.advid
					ORDER BY 
						ent.entnome
					)
				
					UNION ALL
					--Coordenadores = N da coordenação - N dos Advogados para coluna 1 e 2
					(
					SELECT DISTINCT
						ent.entnome,
						--adv.advid,
						count(DISTINCT hco.prcid)-tot.qtd_processos_adv as qtd_processos,
						count(DISTINCT hco.hstid)-tot.qtd_analises_adv as qtd_analises,
						count(DISTINCT nudid) as qtd_numeracao,
						count(DISTINCT hta.docid) as qtd_processos_atuais,
						0 as qtd_rec
					FROM
						conjur.advogados adv
					INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
					INNER JOIN conjur.coordenacao coo ON coo.advid = adv.advid
					LEFT  JOIN 
						(
						--N dos advogados das colunas 1 e 2
						SELECT DISTINCT
							aco.coonid,
							--adv.advid,
							count(DISTINCT hst.docid) as qtd_processos_adv,
							count(DISTINCT hst2.hstid) as qtd_analises_adv
						FROM
							conjur.advogados adv
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
						INNER JOIN conjur.advogadosxcoordenacao aco ON aco.advid = adv.advid
						--Coluna 1
						LEFT  JOIN conjur.historicoadvogados had ON had.advid = adv.advid $whereLEFT
						LEFT  JOIN workflow.historicodocumento hst ON hst.hstid = had.hstid $whereLEFT1
						LEFT  JOIN workflow.historicodocumento hst2 ON hst2.docid = hst.docid $whereLEFT11 
																	   AND hst2.aedid IN (".WF_AEDID_ENCAMINHAR_ADVOGADO.",
																	   					  ".WF_AEDID_RETORNAR_ADVOGADO.",
																						  	".WF_AEDID_ENCAMINHAR_CHEFE_DIVISAO_CGAE.",
																						  	".WF_AEDID_ENCAMINHAR_COORD_CGAE.",
																						  	".WF_AEDID_RETORNAR_CHEFE_DIVISAO_CGAE.",
																						  	".WF_AEDID_RETORNAR_COORD_CGAE.")
						--Coluna 2
						LEFT  JOIN conjur.numeracaodocumento nud ON nud.advid = adv.advid $whereLEFT2
						WHERE
							adv.advstatus = 'A'
							--AND adv.advid NOT IN (SELECT DISTINCT advid FROM conjur.coordenacao WHERE advid IS NOT NULL )
						GROUP BY
							aco.coonid
						ORDER BY 
							aco.coonid
						) tot ON tot.coonid = coo.coonid
					--Coluna 1, 2
					LEFT  JOIN conjur.historicocoordenacoes hco ON hco.coonid = coo.coonid $whereLEFT4
					--Coluna 3
					LEFT  JOIN conjur.numeracaodocumento nud ON nud.advid = adv.advid $whereLEFT2
					--Coluna 4
					LEFT  JOIN (SELECT max(hstid) as hstid, docid FROM workflow.historicodocumento $whereLEFT3 GROUP BY docid) hta ON hta.hstid = hco.hstid
					--Coluna 5
					--Acho q aqui n tem sentido reanalise dentro da coordenação						
					WHERE
						adv.advstatus = 'A'
					GROUP BY
						tot.qtd_processos_adv,
						tot.qtd_analises_adv,
						ent.entnome,
						adv.advid
					ORDER BY 
						ent.entnome
					)
				) as foo
				GROUP BY
					entnome
				ORDER BY
					1";
		
		/*SQL ANTIGA*/
//		"SELECT DISTINCT
//					ent.entnome,
//					--adv.advid,
//					count(DISTINCT hst.docid) as qtd_processos,
//					count(DISTINCT hst2.hstid) as qtd_analises,
//					count(DISTINCT nudid) as qtd_numeracao,
//					count(DISTINCT hta.docid) as qtd_processos_atuais,
//					count(DISTINCT hst_acao2.docid) as qtd_rec
//				FROM
//					conjur.advogados adv
//				INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
//				--Coluna 1
//				LEFT  JOIN conjur.historicoadvogados had ON had.advid = adv.advid $whereLEFT
//				LEFT  JOIN workflow.historicodocumento hst ON hst.hstid = had.hstid $whereLEFT1
//				LEFT  JOIN workflow.historicodocumento hst2 ON hst2.docid = hst.docid $whereLEFT11 AND hst2.aedid IN (".WF_AEDID_ENCAMINHAR_ADVIGADO.",".WF_AEDID_RETORNAR_ADVIGADO.")
//				--Coluna 2
//				LEFT  JOIN conjur.numeracaodocumento nud ON nud.advid = adv.advid $whereLEFT2
//				--Coluna 3
//				LEFT  JOIN (SELECT max(hstid) as hstid, docid FROM workflow.historicodocumento $whereLEFT3 GROUP BY docid) hta ON hta.hstid = had.hstid
//				--Coluna 4
//				LEFT  JOIN (SELECT
//								h1.hstid,
//								min(h2.hstid) as hstid2,
//								h1.docid
//						    FROM
//								workflow.historicodocumento h1
//						    LEFT JOIN workflow.historicodocumento h2 ON h2.hstid > h1.hstid AND h1.docid = h2.docid
//						    WHERE
//								h1.aedid = ".WF_AEDID_RETORNAR_COORDENADOR."
//						    GROUP BY
//								h1.hstid, h1.docid ) hst_acao1 ON hst_acao1.docid = hst.docid
//				LEFT  JOIN workflow.historicodocumento hst_acao2 ON hst_acao2.docid = hst_acao1.docid
//																    AND hst_acao2.hstid = hst_acao1.hstid2 
//																    AND hst_acao2.aedid IN (".WF_AEDID_ENCAMINHAR_ADVIGADO.",".WF_AEDID_RETORNAR_ADVIGADO.") 
//				WHERE
//					adv.advstatus = 'A'
//				GROUP BY
//					ent.entnome,
//					adv.advid
//				ORDER BY 
//					ent.entnome";
		$cabecalho = Array("Advogado",
							"<center>Processos<br> Recebidos</center>",
							"<center>Processos<br> Análise/Reanálise</center>",
							"<center>Manifestações<br> Proferidas</center>",
							"<center>Estoque ou Saldo</center>",
							"<center>Reincidência ou Retorno</center>");
		$celWidth = Array("50%","10%","10%","10%","10%","10%");
		$celAlign = Array("Left","center","center","center","center","center");
		$db->monta_lista($sql,$cabecalho,100,50,$soma,$alinha,"N","formulario",$celWidth,$celAlign,$tempocache=null,$param=Array());
		?>
	</table>
</form>