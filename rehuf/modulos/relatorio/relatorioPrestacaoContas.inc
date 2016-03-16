<?php

include APPRAIZ ."includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';
echo '<br>';
monta_titulo( "Relatório Prestação de Contas - REHUF", "" );

if($_POST){
	extract($_POST);
}

//data atual
$data=date("Ymd");
//Verifica se exite alguma prestação com o estado diferente de encerrado e bloqueado
$sql = "SELECT pi.plinumero,
	pi.plidtinicio,
	pi.plidttermino,
	prc.docid,
	edc.esdid 
FROM REHUF.PRESTACAOCONTAS PRC
INNER JOIN REHUF.VINCULOPLANOINTERNO VPI ON VPI.VPIID = PRC.VPIID
INNER JOIN REHUF.PLANOINTERNO PI ON VPI.PLIID = PI.PLIID
INNER JOIN WORKFLOW.DOCUMENTO DC ON DC.DOCID = PRC.DOCID
INNER JOIN WORKFLOW.ESTADODOCUMENTO EDC ON EDC.ESDID = DC.ESDID
WHERE EDC.ESDID <> " .ESTADO_ENCERRADO." AND EDC.ESDID <> " .ESTADO_BLOQUEADO;

$arrDados = $db->carregar($sql);
if($arrDados[0]){
	foreach($arrDados as $dado){
		//Verifica se o vínculo está fora do prazo.
		if((int)$data <= (int)str_replace("-","",$dado['plidttermino'])){
		}else{
		//verifica se ele já foi desbloqueado
		$sql = "select 
						count(1) 
				from 
					workflow.historicodocumento wh
				inner join 
					workflow.acaoestadodoc wa on wa.aedid = wh.aedid
				inner join 
					workflow.estadodocumento we on we.esdid = wa.esdidorigem
				where 
					wh.docid = " .$dado['docid'].
				"and
					wh.aedid in (".WF_ACAO_DESBLOQUEADO.")";
		$bloqueado = $db->pegaUm( $sql );
		if(!$bloqueado){			
				//Pega a ação para boquear (estado atual + estado de bloqueado)
				$sql = "SELECT
								aedid
						FROM
							workflow.acaoestadodoc
						WHERE
							esdidorigem = ".$dado['esdid']. "
						AND
							esdiddestino = " .ESTADO_BLOQUEADO;
				$aedid = $db->pegaUm( $sql );
				//Altera o workflow
				wf_alterarEstado($dado['docid'],$aedid,'',array());
			}
		}
	}
}


?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery(function() {
	escondetr(<?php echo $plitid ?>);
});		

function escondetr(plitid){
	if(plitid == '1'){
		jQuery('.planotrabalho').hide();
		jQuery('.portaria').show();
		jQuery('#pltid').val("");
	}else if(plitid == '2'){
		jQuery('.portaria').hide();
		jQuery('.planotrabalho').show();
		jQuery('#prtid').val("");
	}else{
		jQuery('.planotrabalho').hide();
		jQuery('.portaria').hide();
		jQuery('#prtid').val("");
		jQuery('#pltid').val("");
	}
}

function relatorio(){
	var erro = 0;
	var plitid = jQuery("#plitid").val();
	var prtid = jQuery("#prtid").val();
	var pltid = jQuery("#pltid").val();
	if(!jQuery("#entid").val() && !jQuery("#plitid").val() && !jQuery("#esdid").val()){
		alert('Um dos campos de filtro deve ser informado');
		erro = 1;
		return false;
	}
	if(plitid == '1'){
		if(typeof prtid == 'undefined'){
			alert('Favor selecionar a Portaria.');
			erro = 1;
			return false;
		}else if(!trim(prtid)){
			alert('Favor selecionar a Portaria.');
			erro = 1;
			return false;
		}
	}else if(plitid == '2'){
		if(typeof pltid == 'undefined'){
			alert('Favor selecionar o Plano de Trabalho.');
			erro = 1;
			return false;
		}else if(!trim(pltid)){
			alert('Favor selecionar o Plano de Trabalho.');
			erro = 1;
			return false;
		}
	}
	if(erro == 0){
		jQuery("#formulario").submit();
	}	
}
</script>

<form method="POST" id="formulario" name="formulario" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita">Tipo:</td>
			<td>
				<?php
					$sql = "SELECT
							 p.plitid AS codigo,
							 p.plitdsc AS descricao
							FROM
							 rehuf.planointernotipo p
							WHERE
							 p.plitstatus = 'A'
							ORDER BY
							 p.plitdsc desc";
					$db->monta_combo("plitid",$sql, 'S' ,'Selecione...','escondetr','','','','','plitid');
				?>
			</td>
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">Portaria:</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
									prt.*,
									pi.plinumero,
									vp.vpiid,
									arq.arqnome	
							FROM
									rehuf.vinculoplanointerno vp
							LEFT JOIN 
									rehuf.planointerno pi ON vp.pliid=pi.pliid
							INNER JOIN 
									rehuf.portaria prt ON vp.prtid=prt.prtid
							LEFT JOIN
									public.arquivo arq ON arq.arqid = vp.arqid AND arq.arqstatus = 'A'
							LEFT JOIN 
									rehuf.vinculointernoquestionario vpiq ON vp.vpiid = vpiq.vpiid
							GROUP BY
									prt.prtid,prt.prtnumero,prt.prtdata,prt.prtdtpublicacao,prt.prtnumerodou,pi.plinumero,vp.vpiid,arq.arqnome";
					$arrPortaria = $db->carregar($sql);
					if($arrPortaria){
						//Início foreach portaria
						$n = 0;
						foreach($arrPortaria as $dado){
							$arrLista[$n]['codigo'] = $dado['prtid'];
					 	 	$arrLista[$n]['descricao'] = "Portaria Nº {$dado['prtnumero']}, de ".exibeDataPorExtenso($dado['prtdata'])." - DOU {$dado['prtnumerodou']}, de ".formata_data($dado['prtdtpublicacao'])."."; 
					 		$n++;
					}// Fim foreach portaria 
					$db->monta_combo("prtid",$arrLista, 'S' ,'Selecione...','','','','','','prtid');
				}//fim if
			?> 
			</td>
		</tr>
		<tr class="planotrabalho">
			<td width="25%" class="SubTituloDireita">Plano de Trabalho:</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
									plt.*,
									pi.plinumero,
									vp.vpiid,
									arq.arqnome
							FROM
									rehuf.vinculoplanointerno vp
							LEFT JOIN 
									rehuf.planointerno pi ON vp.pliid=pi.pliid
							INNER JOIN 
									rehuf.planotrabalho plt ON vp.pltid=plt.pltid
							LEFT JOIN
									public.arquivo arq ON arq.arqid = vp.arqid AND arq.arqstatus = 'A'
							INNER JOIN 
									rehuf.vinculointernoquestionario vpiq ON vp.vpiid = vpiq.vpiid
							GROUP BY
									plt.pltid,plt.plttitulo,pi.plinumero,vp.vpiid,arq.arqnome";
					$arrPlanotrabalho = $db->carregar($sql);
					if($arrPlanotrabalho){
						//Início foreach portaria
						$n = 0;
						foreach($arrPlanotrabalho as $dado){
							 $arrListaPlanoTrabalho[$n]['codigo'] = $dado['pltid'];
							 $arrListaPlanoTrabalho[$n]['descricao'] = $dado['plttitulo']; 
				  			$n++;
					}// Fim foreach portaria 
					$db->monta_combo("pltid",$arrListaPlanoTrabalho, 'S' ,'Selecione...','','','','','','pltid');
				}//fim if
			?> 
			</td>
		</tr>
		<tr>
				<td class="SubTituloDireita">IFES:</td>
				<td>
					<?php
						$sql = "SELECT 
									ent.entid as codigo,
									ena.entsig ||' - ' || upper(ena.entnome) as descricao 
								FROM 
									entidade.entidade ent
								LEFT JOIN 
									entidade.funcaoentidade fen ON fen.entid = ent.entid 
								INNER JOIN 
									entidade.funentassoc fue ON fue.fueid = fen.fueid
								LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid
								LEFT JOIN 
									rehuf.estruturaunidade esu ON esu.entid = ent.entid
								WHERE 
									fen.funid IN ('16','93') 
								AND 
									(esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
								GROUP BY ena.entid, ena.entnome, ena.entsig,ent.entunicod,ent.entid
								ORDER BY ena.entsig";
					 	$db->monta_combo("entid",$sql, 'S' ,'Selecione...','','','','','','entid');
					?>
				</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Estado Atual:</td>
			<td>
				<?php
						$sql = 
								"SELECT
										esdid as codigo,
			 							esddsc as descricao
								 FROM
			 							workflow.estadodocumento
								 WHERE
			 						esdid in(" .ESTADO_EM_ELABORACAO.",".ESTADO_EM_ANALISE.",".ESTADO_EM_AJUSTE.",".ESTADO_BLOQUEADO.",".ESTADO_ENCERRADO.")";
						$arrDados = $db->carregar($sql);
						$n = 0;
						foreach($arrDados as $dado){
							$arrLista[$n]['codigo'] = $dado['codigo'];
							$arrLista[$n]['descricao'] = $dado['descricao'];
							$n++;
						}
							$arrLista[$n]['codigo'] .= ESTADO_NAO_INICIADO;
							$arrLista[$n]['descricao'] .= 'Não Iniciado';	
						$db->monta_combo("esdid",$arrLista, 'S' ,'Selecione...','','','','','','esdid');
				 ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<input type="button" name="btn_salvar" value="Gerar Relatório" onclick="relatorio()" />
				<input type="button" name="btn_limpar" value="Limpar" onclick="window.location=window.location" />
			</td>
		</tr>
	</table>
</form>
<?php
$arrCabecalho = array('UG','IFES - DESCRIÇÃO','PI','Estado Atual');
if($_POST['entid']){
	$where.= " AND ent.entid = ". $_POST['entid'];
}

if($_POST['esdid']){
	if($_POST['esdid'] != ESTADO_NAO_INICIADO){
		$where.= " AND edc.esdid = ". $_POST['esdid'];
	}else{
		$ug .= " '<span>' || ent.entungcod || '</span>'  as ug";
		$where.= " AND edc.esddsc IS NULL AND ena.entnome IS NOT NULL AND ent.entungcod IS NOT NULL";
	}
	
}

if($_POST['prtid']){
	$where.= " AND vpi.prtid = ". $_POST['prtid'];
}else if($_POST['pltid']){
	$where.= " AND vpi.pltid = ". $_POST['pltid'];
}

if(!$ug){
	$ug = "case when vpi.pltid is null
		then '<a style=\'color:blue\' href=\'rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&entungcod=' || ent.entungcod || '&vpiid=' || vpi.vpiid || '&prtid=' || vpi.prtid || '&prtid=' || vpi.prtid || '\'  >' || ent.entungcod || '</a>'
		else '<a style=\'color:blue\' href=\'rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&entungcod=' || ent.entungcod || '&vpiid=' || vpi.vpiid || '&pltid=' || vpi.pltid || '\'  >' || ent.entungcod || '</a>'
	end as ug";
}

$sql = "
SELECT DISTINCT
{$ug},
	upper(ena.entsig) ||' - ' || ena.entnome as ifes,
	'<span></span>' || upper(pi.plinumero) as PI,
	coalesce(edc.esddsc,'Não Iniciado') as estado_atual
FROM 
	entidade.entidade ent
LEFT JOIN
	rehuf.prestacaocontas prc ON ent.entungcod = prc.prcug
LEFT JOIN 
	rehuf.vinculoplanointerno vpi ON prc.vpiid = vpi.vpiid 
LEFT JOIN
	rehuf.planointerno pi ON pi.pliid = vpi.pliid
LEFT JOIN
	entidade.funcaoentidade fen ON fen.entid = ent.entid
LEFT JOIN
	entidade.funentassoc fue ON fue.fueid = fen.fueid
LEFT JOIN 
	entidade.entidade ena ON ena.entid = fue.entid
LEFT JOIN
	rehuf.estruturaunidade esu ON esu.entid = ent.entid AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)
LEFT JOIN
	workflow.documento dc ON dc.docid = prc.docid
LEFT JOIN
	workflow.estadodocumento edc ON edc.esdid = dc.esdid
WHERE 1=1
AND fen.funid IN ('16','93')
       {$where}";
//dbg(simec_htmlentities($sql));
if($_POST){
	$db->monta_lista( $sql, $arrCabecalho,  25, 10, 'N', 'center','','');
}
 ?>