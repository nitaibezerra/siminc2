<?php
// RETIRAR DEPOIS
/* ini_set("memory_limit", "20000024M");
 */
set_time_limit(0);

if( $_REQUEST['requisicao'] == 'cancelarempenho' ){
	
	cancelarEmpenhoSigef($_POST);
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=6 align="center" style="width: 100%">
		<tr>
			<th colspan="4" style="text-align: center;">Dados Processo</th>
		</tr>
		<tr>
		<td class="SubTituloDireita">Número de Processo:</td>
		<td colspan="3">
		<?php
			$numeroprocesso = simec_htmlentities( $_POST['numeroprocesso'] );
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
		?>
		</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php 
			$municipio = simec_htmlentities( $_POST['municipio'] );
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Ano do Processo:</td>
		<td colspan="3">
			<?php
			$anoprocesso = $_POST['anoprocesso'];
			$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao 
					from par.processopar 
					where substring(prpnumeroprocesso,12,4) <> '' 
						and prpstatus = 'A' 
					order by substring(prpnumeroprocesso,12,4) desc";
			$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Somente empenho diferente:</td>
		<td colspan="3">
			<input type="radio" value="S" id="empenhodif" name="empenhodif" <? if($_REQUEST["empenhodif"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="empenhodif" name="empenhodif" <? if( ($_REQUEST["empenhodif"] == "N" || $_REQUEST["empenhodif"] == '') ) { echo "checked"; } ?> /> Não
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Empenho SIMEC/SIGEF:</td>
		<td colspan="3">
			<input type="radio" value="S" id="sequencialexiste" name="sequencialexiste" <? if($_REQUEST["sequencialexiste"] == "S") { echo "checked"; } ?> /> Empenhos que não existem no SIMEC<br>
			<input type="radio" value="N" id="sequencialexiste" name="sequencialexiste" <? if( ($_REQUEST["sequencialexiste"] == "N" || $_REQUEST["sequencialexiste"] == '') ) { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo de Processo:</td>
		<td colspan="3">
			<input type="radio" value="PAR" id="tipoprocesso" name="tipoprocesso" <? if( $_REQUEST["tipoprocesso"] == "PAR" || $_REQUEST["tipoprocesso"] == '' ) { echo "checked"; } ?> /> Subações Genérico do PAR<br>
			<input type="radio" value="OBRA" id="tipoprocesso" name="tipoprocesso" <? if( $_REQUEST["tipoprocesso"] == "OBRA" ) { echo "checked"; } ?> /> Obras PAR<br>
			<input type="radio" value="PAC" id="tipoprocesso" name="tipoprocesso" <? if( $_REQUEST["tipoprocesso"] == "PAC" ) { echo "checked"; } ?> /> Obras do PAC<br>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="4" style="text-align: center;">
			<input class="botao" type="button" name="pesquisar" id= "pesquisar" value="Pesquisar" onclick="submeteFormulario();">
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=sistema/diferencas_sigef_simec_empenho&acao=A';" />
		</td>
	</tr>
	</table>
</form>
<?
if($_REQUEST['requisicao'] == 'pesquisar') {
	
	if($_REQUEST['numeroprocesso']) {
		$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
		$where[] = "processo = '".$_REQUEST['numeroprocesso']."'";
	}
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(municipio) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	
	if($_REQUEST['estuf']) {
		$where[] = "estuf='".$_REQUEST['estuf']."'";
		$filtro = " and iu.mun_estuf='".$_REQUEST['estuf']."'";
	}
	
	if( $_REQUEST['sequencialexiste'] == 'S' ) {
		$filtroExiste = " (select ems_numero_processo as processo from par.empenhosigef where ems_numero_sequencial_da_ne not in (select empprotocolo from par.empenho))";
	} else {
		$filtroExiste = " (select empnumeroprocesso as processo from par.empenho where empstatus = 'A'
			                      union all
			               select ems_numero_processo as processo from par.empenhosigef) ";		
	} 
	
	if( $_REQUEST['tipoprocesso'] == 'PAR' ){
		$filtroJoin = "SELECT distinct p.prpid as codigo, 
					        substring(p.prpnumeroprocesso from 1 for 5)||'.'||
					        substring(p.prpnumeroprocesso from 6 for 6)||'/'||
					        substring(p.prpnumeroprocesso from 12 for 4)||'-'||
					        substring(p.prpnumeroprocesso from 16 for 2) as numeroprocesso,
					        p.prpnumeroprocesso as processo, 
					        substring(p.prpnumeroprocesso, 12, 4) as ano, 
					        case when m.mundescricao is null then e.estdescricao else m.mundescricao end municipio, 
					        case when m.estuf is null then e.estuf else m.estuf end as estuf,
		                    'PAR' as tipo 
					    FROM
					        par.instrumentounidade iu
					        inner join par.processopar p ON p.inuid = iu.inuid $filtro
					        left join territorios.municipio m ON m.muncod = iu.muncod
					        left join territorios.estado e ON e.estuf = iu.estuf
					    WHERE 1=1 /*p.prpstatus = 'A'*/";
	}elseif( $_REQUEST['tipoprocesso'] == 'OBRA' ){
		$filtroJoin = "SELECT distinct p.proid as codigo, 
					        substring(p.pronumeroprocesso from 1 for 5)||'.'||
					        substring(p.pronumeroprocesso from 6 for 6)||'/'||
					        substring(p.pronumeroprocesso from 12 for 4)||'-'||
					        substring(p.pronumeroprocesso from 16 for 2) as numeroprocesso,
					        p.pronumeroprocesso as processo, 
					        substring(p.pronumeroprocesso, 12, 4) as ano, 
					        case when m.mundescricao is null then e.estdescricao else m.mundescricao end municipio, 
					        case when m.estuf is null then e.estuf else m.estuf end as estuf,
		                    'OBRA' as tipo 
					    FROM
					        par.instrumentounidade iu
					        inner join par.processoobraspar p ON p.inuid = iu.inuid
					        left join territorios.municipio m ON m.muncod = iu.muncod
					        left join territorios.estado e ON e.estuf = iu.estuf";
	}elseif( $_REQUEST['tipoprocesso'] == 'PAC' ){
		$filtroJoin = "SELECT distinct p.proid as codigo, 
					        substring(p.pronumeroprocesso from 1 for 5)||'.'||
					        substring(p.pronumeroprocesso from 6 for 6)||'/'||
					        substring(p.pronumeroprocesso from 12 for 4)||'-'||
					        substring(p.pronumeroprocesso from 16 for 2) as numeroprocesso,
					        p.pronumeroprocesso as processo, 
					        substring(p.pronumeroprocesso, 12, 4) as ano, 
					        case when m.mundescricao is null then e.estdescricao else m.mundescricao end municipio, 
					        case when m.estuf is null then e.estuf else m.estuf end as estuf,
		                    'PAC' as tipo 
					    FROM
					        par.processoobra p 
					        left join territorios.municipio m ON m.muncod = p.muncod
					        left join territorios.estado e ON e.estuf = p.estuf
						where 1=1 /*p.prostatus = 'A'*/
";
	}
	if($_REQUEST['anoprocesso']){
		$where[] = "substring(processo,12,4) = '".$_REQUEST['anoprocesso']."'";
	}
	
	$sql = "select 
				codigo,
			    numeroprocesso,
			    processo,
			    ano,
			    municipio,
			    estuf,
                tipo
			from(
				$filtroJoin
			) as foo
			where
			     processo in $filtroExiste
				".(($where)? ' and '.implode(" AND ", $where):"")."
				order by municipio";
	
	$arrProcesso = $db->carregar($sql);
}

$arrProcesso = $arrProcesso ? $arrProcesso : array();

//16216

?>
	<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtitulodireita" width="20%" style="font-weight: bold" valign="top">Legenda:</td>
			<td valign="top" style="font-weight: bold" width="80%">&nbsp;<img src='../imagens/money_cancel.gif' title='Reduzir'>&nbsp;-&nbsp;Cancelar Nota de Empenho(NE)</td>				
		</tr>
	</table>
	<table class="listagem" width="100%" bgcolor="#F5F5F5" cellspacing="1" cellpadding="2" align="center">
	<thead>
		<tr>
			<td align="center" class="title" width="10%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Processo</strong></td>
			<td align="center" class="title" width="05%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Ano</strong></td>
			<td align="center" class="title" width="05%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>UF</strong></td>
			<td align="center" class="title" width="10%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Município/Estado</strong></td>
			<td align="center" class="title" width="35%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Empenho SIMEC</strong></td>
			<td align="center" class="title" width="35%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Empenho SIGEF</strong></td>
		</tr>
	</thead>
	<tbody>
		<?if($arrProcesso[0]){
			$count = 0; 
			?>
			<?foreach ($arrProcesso as $key => $v) {
				$arrSimec = carregarDadosEmpenhoSimec( $v['processo'] );
				$htmlSimec = $arrSimec['html'];
				$retornoSimec = $arrSimec['retorno'];
				
				$arrSigef = carregarDadosEmpenhoSigef( $v['processo'] );
				$htmlSigef = $arrSigef['html'];
				$retornoSigef = $arrSigef['retorno'];
				
				$boDiferenca = false;
				 if( $_POST['empenhodif'] == 'S' ){						
					$programaSimec 		= $retornoSimec[$v['processo']]['programa']; 
					$empenhoSimec 		= $retornoSimec[$v['processo']]['empenho'];
					$empenhopaiSimec 	= $retornoSimec[$v['processo']]['sequencialpai'];
					$valorSimec			= (float)$retornoSimec[$v['processo']]['valor'];
					$situacaoSimec 		= $retornoSimec[$v['processo']]['situacao'];
					
					$programaSigef 		= $retornoSigef[$v['processo']]['programa']; 
					$empenhoSigef 		= $retornoSigef[$v['processo']]['empenho'];
					$empenhopaiSigef 	= $retornoSigef[$v['processo']]['sequencialpai'];
					$valorSigef			= (float)$retornoSigef[$v['processo']]['valor'];
					$situacaoSigef 		= $retornoSigef[$v['processo']]['situacao'];
					
					$pos = strpos($situacaoSimec, $situacaoSigef);
					 
					if( $programaSimec != $programaSigef || $empenhoSimec != $empenhoSigef || $empenhopaiSimec != $empenhopaiSigef || $valorSimec != $valorSigef || $pos === false ){
						$boDiferenca = true;
					}
				}
				if( sizeof($retornoSigef) != sizeof($retornoSimec) ){
					$boDiferenca = true;
				}
				
				if( $_POST['empenhodif'] == 'N' ){
					$boDiferenca = true;
				}
				
				if( $boDiferenca ){
					$count++;
					$valign = ( (strlen($htmlSigef) < 100)  ? 'middle' : 'top');
					
					$key % 2 ? $cor = "#dedfde" : $cor = ""; ?>
					<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td valign="middle"><?=$v['numeroprocesso']; ?></td>
						<td valign="middle"><?=$v['ano']; ?></td>
						<td valign="middle"><?=$v['estuf']; ?></td>
						<td valign="middle"><?=$v['municipio']; ?></td>
						<td valign="top"><?=$htmlSimec; ?></td>
						<td valign="<?=$valign ?>" align="center"><?=$htmlSigef; ?></td>
					</tr>
				<? }?>
			<?} ?>
		<?} ?>
	</tbody>
	</table>
	<?if($arrProcesso[0]){ ?>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros: <?=$count; ?></b></td>
			<tr>
		</table>
	<?}else{ ?>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<tr>
			<td align="center" style="color:#cc0000;" colspan="10">Não foram encontrados Registros de Empenho.</td>
		</tr>
		</table>
	<?} ?>

<div id="div_login" title="Tela de Identificação" style="display: none; text-align: center;">
	<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario", "S", "S", "Usuário", "25","","","","","","", "id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
					onmouseover="MouseOver(this);" value="" size="26" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
	</table>
</div>
<div id="div_dialog" title="Descrição do Retorno" style="display: none; text-align: left;">
	<div id="div_msg"></div>
</div>
<script type="text/javascript">
function submeteFormulario(){
	$('#requisicao').val('pesquisar');
	$('#formulario').submit();
}

function telaLogin( sequencialNE ) {
	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
	
	jQuery( "#div_login" ).show();
	jQuery( '#div_login' ).dialog({
		resizable: false,
		width: 500,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				if( jQuery('[name="wsusuario"]').val() == '' ){
					alert('Informe o usuário.');
					jQuery('[name="wsusuario"]').focus();
					return false;
				}
				if( jQuery('[name="wssenha"]').val() == '' ){
					alert('Informe a senha.');
					jQuery('[name="wssenha"]').focus();
					return false;
				}
				enviaCancelarEmpenho( sequencialNE );
				jQuery( this ).dialog( 'close' );
			},
			'Fechar': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
}

function enviaCancelarEmpenho( sequencialNE ){
	
	$.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=cancelarempenho&sequencial="+sequencialNE+'&wsusuario='+jQuery('[name="wsusuario"]').val()+'&wssenha='+jQuery('[name="wssenha"]').val(),
		async: false,
		success: function(msg){
			$( "#div_dialog" ).show();
			$( "#div_msg" ).html(msg);
			$( '#div_dialog' ).dialog({
					resizable: false,
					width: 900,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Fechar': function() {
							jQuery( this ).dialog( 'close' );
							submeteFormulario();
						}
					}
			});
		}
	});
}
</script>
<?
function carregarDadosEmpenhoSimec( $processo ){
	global $db;
	
	$sql = "select distinct
				e.empprogramafnde as programa,
			    e.empprotocolo as sequencial,
			    e.empnumero as empenho,
			    e.empid,
			    e.empcodigoespecie,
			    (select empprotocolo from par.empenho where empid = e.empidpai and empstatus = 'A') as sequencialpai,
			    e.empnumeroprocesso as processo,
			    --(select sum(es.eobvalorempenho) from par.empenhosubacao es where es.empid = e.empid and es.eobstatus = 'A') as valor,
			    e.empvalorempenho as valor,
			    e.empsituacao as situacao /*,
			    (select sum(total)
				from(
					select count(sbaid) as total, empid from par.empenhosubacao where eobstatus = 'A' group by empid
					union all
					select count(preid), empid from par.empenhoobrapar where eobstatus = 'A' group by empid
					union all
					select count(preid), empid from par.empenhoobra where eobstatus = 'A' group by empid
				) as foo
				where empid = e.empid) as totalemp*/
			from
				par.empenho e
				--inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
			where e.empnumeroprocesso = '{$processo}' and empstatus = 'A'
			order by e.empprotocolo, valor";
	$arrEmpSimec = $db->carregar($sql);
	$arrEmpSimec = $arrEmpSimec ? $arrEmpSimec : array();
	
	$estiloCaption = "style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"";
	$arrRetorno = array();
	if( $arrEmpSimec ){
	$html = '<table class="listagem" width="100%" bgcolor="#F5F5F5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th colspan="10">Dados Empenho</th>
				</tr>
				<tr>
					<td align="center" class="title" '.$estiloCaption.'><strong>Programa</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial Pai</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Empenho</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Especie</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Valor</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Situação</strong></td>
				</tr>';
	
		foreach ($arrEmpSimec as $simec){
			$simec['empenho'] = substr($simec['empenho'], -6);
			//$simec['empenhopai'] = substr($simec['empenhopai'], -6);

			$html.= '<tr>
						<td '.$estiloCaption.'>'.$simec['programa'].'</td>
						<td '.$estiloCaption.'>'.$simec['sequencial'].'</td>
						<td '.$estiloCaption.'>'.$simec['sequencialpai'].'</td>
						<td '.$estiloCaption.'>'.$simec['empenho'].'</td>
						<td '.$estiloCaption.'>'.$simec['empcodigoespecie'].'</td>
						<td '.$estiloCaption.'>'.number_format($simec['valor'], '2', ',', '.').'</td>
						<td '.$estiloCaption.'>'.$simec['situacao'].'</td>
					</tr>';
			
			$arrRetorno[$processo] = array(
										'programa' 		=> trim($simec['programa']),
										'empenho' 		=> trim($simec['empenho']),
										'sequencialpai' 	=> trim($simec['sequencialpai']),
										'valor' 		=> trim($simec['valor']),
										'situacao' 		=> trim($simec['situacao'])
										);
		}
		$html.= '</table>';
	} else {
		$html = '<span style="color:#cc0000;">Não foram encontrados Registros de Empenho SIMEC.</span>';
	}
	return array('html' => $html, 'retorno' => $arrRetorno);
}

function carregarDadosEmpenhoSigef( $processo ){
	global $db;
	
	$sql = "select distinct
				case when coalesce(emp.totemp, 0) = 0 and coalesce(eds.total, 0) = 0 and e.ems_codigo_especie = '01' and coalesce(totcancel, 0) < 1 then
				'<center><img src=../imagens/money_cancel.gif align=absmiddle style=cursor:pointer; title=\"Cancelar NE\" onclick=\"telaLogin('||trim(e.ems_numero_sequencial_da_ne) || ');\"></center>'
				else '' end as acao, 
				e.ems_programa_fnde as programa, 
				e.ems_numero_sequencial_da_ne as sequencial, 
			    e.ems_numero_do_empenho as empenho, 
			    e.ems_numero_do_empenho_pai as empenhopai,
			    e.ems_valor_empenho as valor,
			    e.empid,
			    e.ems_nu_seq_mov_ne as sequencialpai,
			    e.ems_codigo_especie,
			    e.ems_situacao_do_empenho as situacao,
			    e.ems_numero_processo
			from par.empenhosigef e
				left join (
                	select count(empid) as totemp, empnumerooriginal, empnumeroprocesso, empprotocolo 
                	from par.empenho where empstatus = 'A'
                    group by empnumerooriginal, empnumeroprocesso, empprotocolo
               ) emp on emp.empprotocolo = e.ems_numero_sequencial_da_ne
               left join 
                	(select count(edsid) as total, edsempenho, edsprocesso, edssequencial 
                        from par.empenhodivergentesigef 
                        group by edsempenho, edsprocesso, edssequencial 
               ) eds on eds.edssequencial = e.ems_numero_sequencial_da_ne
               left join (
               		select count(emsid) as totcancel, ems_numero_do_empenho, ems_numero_do_empenho_pai, ems_numero_sequencial_da_ne 
                    from par.empenhosigef 
                    where ems_codigo_especie = '03'
                    group by ems_numero_do_empenho, ems_numero_do_empenho_pai, ems_numero_sequencial_da_ne 
               ) ems on ems.ems_numero_do_empenho_pai = e.ems_numero_do_empenho 
			where e.ems_numero_processo = '{$processo}'
			order by e.ems_numero_sequencial_da_ne, e.ems_valor_empenho";
	
	$arrEmpSigef = $db->carregar($sql);
	$arrEmpSigef = $arrEmpSigef ? $arrEmpSigef : array();
	
	$estiloCaption = "style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"";
	$arrRetorno = array();
	if( $arrEmpSigef ){
		//<td align="center" class="title" '.$estiloCaption.'><strong>Empenho Pai</strong></td>
	$html = '<table class="listagem" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th colspan="10">Dados Empenho</th>
				</tr>
				<tr>
					<td align="center" class="title" '.$estiloCaption.'><strong>Ação</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Programa</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial Pai</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Empenho</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Especie</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Valor</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Situação</strong></td>
				</tr>';
	
		foreach ($arrEmpSigef as $simec){
			$html.= '<tr>
						<td '.$estiloCaption.'>'.$simec['acao'].'</td>
						<td '.$estiloCaption.'>'.$simec['programa'].'</td>
						<td '.$estiloCaption.'>'.$simec['sequencial'].'</td>
						<td '.$estiloCaption.'>'.$simec['sequencialpai'].'</td>
						<td '.$estiloCaption.'>'.$simec['empenho'].'</td>
						<td '.$estiloCaption.'>'.$simec['ems_codigo_especie'].'</td>
						<td '.$estiloCaption.'>'.number_format($simec['valor'], '2', ',', '.').'</td>
						<td '.$estiloCaption.'>'.$simec['situacao'].'</td>
					</tr>';
			
			$simec['empenho'] = substr($simec['empenho'], -6);
			
			$arrRetorno[$processo] = array(
										'programa' 		=> trim($simec['programa']),
										'empenho' 		=> trim($simec['empenho']),
										'sequencialpai' 	=> trim($simec['sequencialpai']),
										'valor' 		=> trim($simec['valor']),
										'situacao' 		=> trim($simec['situacao'])
										);
			
		}
		$html.= '</table>';
	} else {
		$html = '<span style="color:#cc0000;">Não foram encontrados Registros de Empenho SIGEF.</span>';
	}
	return array('html' => $html, 'retorno' => $arrRetorno);
}

function cancelarEmpenhoSigef( $post ){
	global $db;
	
	$sql = "SELECT emsid, prpid, proidpac, proidpar, empid, teeid, ems_numero_processo, ems_cnpj, ems_programa_fnde, ems_unidade_gestora, ems_numero_do_empenho, ems_numero_do_empenho_pai,
			  	ems_valor_empenho, ems_numero_sequencial_da_ne, ems_nu_seq_mov_ne, ems_data_empenho, ems_cpf, ems_numero_sistema, ems_descricao_do_empenho, ems_ano_do_empenho,
			  	ems_centro_de_gestao, ems_codigo_nat_despesa, ems_fonte_recurso, ems_ptres, ems_esfera, ems_pi, ems_codigo_especie, ems_situacao_do_empenho
			FROM
			  	par.empenhosigef e
			where ems_numero_sequencial_da_ne = '".$post['sequencial']."'";
	$arrSigef = $db->pegaLinha($sql);
	
	$wsusuario 	= $post['wsusuario'];
	$wssenha 	= $post['wssenha'];
	$prpid	 	= $arrSigef['prpid'];
	$proidpar 	= $arrSigef['proidpar'];
	$proidpac 	= $arrSigef['proidpac'];
	
	if( !empty($prpid) ){
		$tabela = 'par.historicowsprocessopar';
		$campo = 'prpid';
		$codigo = $prpid;
	} elseif( !empty($proidpar) ){
		$tabela = 'par.historicowsprocessoobrapar';
		$campo = 'proid';
		$codigo = $proidpar;
	}elseif( !empty($proidpac) ){
		$tabela = 'par.historicowsprocessoobra';
		$campo = 'proid';
		$codigo = $proidpac;
	}
	//23400.004668/2012-61
	
	$nu_seq_ne 						= $post['sequencial'];	
	$nu_processo					= trim($arrSigef['ems_numero_processo']);
	$co_natureza_despesa_solic		= trim($arrSigef['ems_codigo_nat_despesa']);
	$co_plano_interno_solic			= trim($arrSigef['ems_pi']);
	$co_ptres_solic					= trim($arrSigef['ems_ptres']);
	$nu_cnpj_favorecido				= trim($arrSigef['ems_cnpj']);	
	$nu_empenho_original			= trim($arrSigef['ems_numero_do_empenho']);
	$an_exercicio_original			= trim($arrSigef['ems_ano_do_empenho']);
	
	if( $an_exercicio_original == date('Y') ){
		$co_especie_empenho			= "03";
	} else {
		$co_especie_empenho			= "13";
	}	
	$vl_empenho						= trim($arrSigef['ems_valor_empenho']);
	$co_esfera_orcamentaria_solic 	= "1";
	$co_centro_gestao_solic			= trim($arrSigef['ems_centro_de_gestao']);
	$an_convenio					= null;
	$nu_convenio					= null;
	$co_observacao					= "2";
	$co_tipo_empenho				= "3";
	$co_descricao_empenho			= "0011";
	$co_programa_fnde				= trim($arrSigef['ems_programa_fnde']);
	$co_fonte_recurso_solic			= trim($arrSigef['ems_fonte_recurso']);	
	$co_gestao_emitente				= "15253";
	$co_unidade_gestora_emitente	= "153173";
	$co_unidade_orcamentaria_solic	= "26298";	
	$nu_proposta_siconv				= null;
	$nu_sistema						= trim($arrSigef['ems_numero_sistema']);
	$data_created 					= date("c");
	
	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;
	
	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	}
	
	$xml = Fnde_Webservice_Client::CreateRequest()
	->setURL($urlWS)
	->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
	->execute();
	
	$xmlRetorno = $xml;	
	$xml = simplexml_load_string( stripslashes($xml));
		
	$result = (integer) $xml->status->result;
	$result = (integer) trim($result);
	
	if($result == 1) {
		$errodecancelamento = false;
		$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
				SOLICITAÇÃO DE CANCELAMENTO DE EMPENHO
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " > ';
		$mensagem .= $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text).' ';
		$mensagem .='</div>
						</div>
		 				<br>';
		echo $mensagem;
				
		$sql = "INSERT INTO ".$tabela."(
				    	".$campo.",
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$codigo."',
				    		'cancelarEmpenhoSIGEF - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
		$db->executar($sql);
		$db->commit();
					
		$sql = "INSERT INTO par.empenhodivergentesigef(prpid, proidpar, proidpac, edscnpj, edsprocesso, edssequencial, edsempenho, edsanoempenho, edsvalorempenho, edsespecie, edscpf, edssequencialcancelamento) 
				VALUES (
				  ".(($prpid)?"'".$prpid."'":"NULL").",
				  ".(($proidpar)?"'".$proidpar."'":"NULL").",
				  ".(($proidpac)?"'".$proidpac."'":"NULL").",
				  ".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
				  ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
				  ".(($nu_seq_ne)?"'".$nu_seq_ne."'":"NULL").",
				  ".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
				  ".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
				  ".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
				  ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
				  '".$_SESSION['usucpf']."',
				  '".$xml->body->nu_seq_ne."'
				)";
			
		$db->executar($sql);
		$db->commit();
		
		//atualizaEmpenhoSigef($wsusuario, $wssenha, $nu_processo, $arrSigef);
	
	} else {
		$errodecancelamento = true;		
		$sql = "INSERT INTO ".$tabela."(
				    	".$campo.",
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$codigo."',
				    		'cancelarEmpenhoSIGEF - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
		
		$db->executar($sql);
		$db->commit();
	}
	
	if($errodecancelamento) {
	
		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
	<request>
		<header>
			<app>string</app>
			<version>string</version>
			<created>$data_created</created>
		</header>
		<body>
			<auth>
				<usuario>$wsusuario</usuario>
				<senha>$wssenha</senha>
			</auth>
			<params>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
			</params>
		</body>
	</request>
XML;
	
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}
	
		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
		->execute();
	
		$xmlRetorno = $xml;
	
		$xml = simplexml_load_string( stripslashes($xml));
	
		/* echo "------ SOLICITAÇÃO DE CANCELAMENTO DE EMPENHO ------<br>";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."<br>";
		echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."<br>"; */
		
		$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
				SOLICITAÇÃO DE CANCELAMENTO DE EMPENHO
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " > ';
		$mensagem.= $xml->status->message->code.' - '.iconv("UTF-8", "ISO-8859-1", $xml->status->message->text).'<br>';
		$result = (integer) $xml->status->result;
		$result = (integer) trim($result);
		
		if($result){			
			$sql = "INSERT INTO ".$tabela."(
				    	".$campo.",
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$codigo."',
				    		'cancelarEmpenhoSIGEF2 - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();
			
			$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $xml->status->message->text).'<br>';
	
			$sql = "INSERT INTO par.empenhodivergentesigef(prpid, proidpar, proidpac, edscnpj, edsprocesso, edssequencial, edsempenho, edsanoempenho, edsvalorempenho, edsespecie, edscpf, edssequencialcancelamento)
					VALUES (
					  ".(($prpid)?"'".$prpid."'":"NULL").",
					  ".(($proidpar)?"'".$proidpar."'":"NULL").",
					  ".(($proidpac)?"'".$proidpac."'":"NULL").",
					  ".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					  ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					  ".(($nu_seq_ne)?"'".$nu_seq_ne."'":"NULL").",
					  ".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					  ".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					  ".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					  ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					  '".$_SESSION['usucpf']."',
					  '".$xml->body->nu_seq_ne."'
					)";
					
			$db->executar($sql);
			$db->commit();
			
			//atualizaEmpenhoSigef( $wsusuario, $wssenha, $nu_processo, $arrSigef );
		}else{			
			$sql = "INSERT INTO ".$tabela."(
				    	".$campo.",
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$codigo."',
				    		'cancelarEmpenhoSIGEF2 - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();
				
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).'<br>';
				}
			}
		}
		
		$mensagem .='</div>
						</div>
		 				<br>';
		echo $mensagem;
	}
}

function atualizaEmpenhoSigef( $wsusuario, $wssenha, $nu_processo, $arrSigef ){
	global $db;
	
	$arrParam = array(
			'wsusuario' => $wsusuario,
			'wssenha' => $wssenha,
			'nu_processo' => $nu_processo,
			'method' => 'historicoempenho',
	);

	$prpid	 	= $arrSigef['prpid'];
	$proidpar 	= $arrSigef['proidpar'];
	$proidpac 	= $arrSigef['proidpac'];
	
	if( !empty($prpid) ){
		$campo = 'prpid';
		$codigo = $prpid;
	} elseif( !empty($proidpar) ){
		$campo = 'proid';
		$codigo = $proidpar;
	}elseif( !empty($proidpac) ){
		$campo = 'proid';
		$codigo = $proidpac;
	}

	$arrRetorno = montaXMLHistoricoProcessoSIGEF( $arrParam );
	
	foreach($arrRetorno as $chaves => $dados ){
		$cnpj 						= trim((string)$dados['cnpj']);
		$programa_fnde 				= trim((string)$dados['programa_fnde']);
		$unidade_gestora 			= trim((string)$dados['unidade_gestora']);
		$numero_da_proposta_siconv 	= trim((string)$dados['numero_da_proposta_siconv']);
		$numero_da_ne 				= trim((string)$dados['numero_da_ne']);
		$numero_de_vinculacao_ne 	= trim((string)$dados['numero_de_vinculacao_ne']);
		$valor_da_ne 				= trim((string)$dados['valor_da_ne']);
		$numero_sequencial_da_ne 	= trim((string)$dados['numero_sequencial_da_ne']);
		$nu_seq_mov_ne 				= trim((string)$dados['nu_seq_mov_ne']);
		$data_do_empenho 			= trim((string)$dados['data_do_empenho']);
		$cpf 						= trim((string)$dados['cpf']);
		$nu_id_sistema 				= trim((string)$dados['nu_id_sistema']);
		$descricao_do_empenho 		= utf8_decode(trim((string)$dados['descricao_do_empenho']));
		$ano_do_empenho 			= trim((string)$dados['ano_do_empenho']);
		$centro_de_gestao 			= trim((string)$dados['centro_de_gestao']);
		$natureza_de_despesa 		= trim((string)$dados['natureza_de_despesa']);
		$fonte_de_recurso 			= trim((string)$dados['fonte_de_recurso']);
		$ptres 						= trim((string)$dados['ptres']);
		$esfera 					= trim((string)$dados['esfera']);
		$pi 						= trim((string)$dados['pi']);
		$cod_especie 				= trim((string)$dados['cod_especie']);
		$numero_do_processo 		= trim((string)$dados['numero_do_processo']);
		$situacao_do_empenho 		= trim((string)$dados['situacao_do_empenho']);
		
		if($cod_especie == '01'){
			$teeid = 1;
		}elseif($cod_especie == '02'){
			$teeid = 2;
		}elseif($cod_especie == '03'){
			$teeid = 3;
		}elseif($cod_especie == '04'){
			$teeid = 4;
		}

		$sql = "SELECT empid, empnumero FROM par.empenho WHERE empprotocolo  = '{$numero_sequencial_da_ne}' and empstatus = 'A'";
		$dadosEmpenhoSIMEC = $db->pegaLinha($sql);
		if($dadosEmpenhoSIMEC['empid']){
			$empid = $dadosEmpenhoSIMEC['empid'];
		}else{
			$empid = 'NULL';
		}

		$numero_de_vinculacao_ne 	= $numero_de_vinculacao_ne ? "'".$numero_de_vinculacao_ne."'" : 'NULL';
		$nu_seq_mov_ne 				= $nu_seq_mov_ne ? $nu_seq_mov_ne : 'NULL';
		$nu_id_sistema 				= $nu_id_sistema ? "'".$nu_id_sistema."'" : 'NULL';

		$boTem = $db->pegaUm("select count(ems_numero_sequencial_da_ne) from par.empenhosigef where ems_numero_sequencial_da_ne = '$numero_sequencial_da_ne' and ems_numero_processo = '$numero_do_processo'");

		$numero_da_ne 				= $numero_da_ne ? "'".$numero_da_ne."'" : 'NULL';
		$numero_sequencial_da_ne 	= $numero_sequencial_da_ne ? "'".$numero_sequencial_da_ne."'" : 'NULL';

		if( (int)$boTem == 0 ){		
			$sql = "INSERT INTO par.empenhosigef(
						prpid, proidpac, proidpar, empid, teeid,
						ems_numero_processo, ems_cnpj, ems_programa_fnde, ems_unidade_gestora,
						ems_numero_do_empenho, ems_numero_do_empenho_pai, ems_valor_empenho, ems_numero_sequencial_da_ne, ems_nu_seq_mov_ne, ems_data_empenho,
						ems_cpf, ems_numero_sistema, ems_descricao_do_empenho, ems_ano_do_empenho, ems_centro_de_gestao, ems_codigo_nat_despesa, ems_fonte_recurso,
						ems_ptres,ems_esfera,ems_pi,ems_codigo_especie,ems_situacao_do_empenho
					) VALUES (
						{$prpid}, {$proidpac}, {$proidpar}, {$empid}, {$teeid},
						'{$numero_do_processo}', '{$cnpj}', '{$programa_fnde}', '{$unidade_gestora}',
						{$numero_da_ne}, {$numero_de_vinculacao_ne}, '{$valor_da_ne}', {$numero_sequencial_da_ne}, {$nu_seq_mov_ne}, {$data_do_empenho},
						'{$cpf}', {$nu_id_sistema}, '{$descricao_do_empenho}', '{$ano_do_empenho}', '{$centro_de_gestao}', '{$natureza_de_despesa}', '{$fonte_de_recurso}',
						'{$ptres}','{$esfera}','{$pi}','{$cod_especie}','{$situacao_do_empenho}');";
			$db->executar($sql);
		} else {
			$sql = "UPDATE par.empenhosigef SET
						prpid 						= $prpid,
						proidpac 					= $proidpac,
						proidpar 					= $proidpar,
						empid 						= $empid,
						teeid 						= $teeid,
						ems_numero_processo 		= '$numero_do_processo',
						ems_cnpj 					= '$cnpj',
						ems_programa_fnde 			= '$programa_fnde',
						ems_unidade_gestora 		= '$unidade_gestora',
						ems_numero_do_empenho 		= $numero_da_ne,
						ems_numero_do_empenho_pai 	= $numero_de_vinculacao_ne,
						ems_valor_empenho 			= '$valor_da_ne',
						ems_numero_sequencial_da_ne = $numero_sequencial_da_ne,
						ems_nu_seq_mov_ne 			= $nu_seq_mov_ne,
						ems_data_empenho 			= $data_do_empenho,
						ems_cpf 					= '$cpf',
						ems_numero_sistema 			= $nu_id_sistema,
						ems_descricao_do_empenho 	= '$descricao_do_empenho',
						ems_ano_do_empenho 			= '$ano_do_empenho',
						ems_centro_de_gestao 		= '$centro_de_gestao',
						ems_codigo_nat_despesa 		= '$natureza_de_despesa',
						ems_fonte_recurso 			= '$fonte_de_recurso',
						ems_ptres 					= '$ptres',
						ems_esfera 					= '$esfera',
						ems_pi 						= '$pi',
						ems_codigo_especie 			= '$cod_especie',
						ems_situacao_do_empenho 	= '$situacao_do_empenho'		
					WHERE
						ems_numero_processo = '$numero_do_processo'
						and ems_numero_sequencial_da_ne = $numero_sequencial_da_ne";
			$db->executar($sql);
		}
	}
	$db->commit();
	
	return true;
}
?>