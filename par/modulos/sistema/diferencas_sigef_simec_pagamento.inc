<?php
// RETIRAR DEPOIS
/* ini_set("memory_limit", "20000024M");
 */
set_time_limit(0);

if( $_REQUEST['requisicao'] == 'atualizarProcesso' ){
	$wsusuario 	= 'USAP_WS_SIGARP';
	$wssenha	= '03422625';
	
	$nu_processo = str_replace(".","", $_REQUEST['numeroprocesso']);
	$nu_processo = str_replace("/","", $nu_processo);
	$nu_processo = str_replace("-","", $nu_processo);
	
	$arrParam = array(
			'wsusuario' => $wsusuario,
			'wssenha' => $wssenha,
			'nu_processo' => $nu_processo,
			'method' => 'historicopagamento',
	);
	
	$arrEmpSigef = montaXMLHistoricoProcessoSIGEF( $arrParam );
	
	foreach($arrEmpSigef as $chaves => $dados ){
				
			$sql = "SELECT empid, empnumero FROM par.empenho WHERE empprotocolo  = '{$dados['nu_seq_mov_ne']}'";
			$dadosEmpenhoSIMEC = $db->pegaLinha($sql);
			if($dadosEmpenhoSIMEC['empid']){
				$empid = $dadosEmpenhoSIMEC['empid'];
			}else{
				$empid = 'NULL';
			}

			if( $_REQUEST['tipoprocesso'] == 'PAR' ){
				$prpid = $db->pegaUm("select prpid from par.processopar where prpnumeroprocesso = '$nu_processo'");
				$proidpac = 'null';
				$proidpar = 'null';
			} elseif( $_REQUEST['tipoprocesso'] == 'ObrasPAR' ){
				$prpid = 'null';
				$proidpac = 'null';
				$proidpar = $db->pegaUm("select proid from par.processoobraspar where pronumeroprocesso = '$nu_processo'");
			} else {
				$prpid = 'null';
				$proidpac = $db->pegaUm("select proid from par.processoobra where pronumeroprocesso = '$nu_processo'");
				$proidpar = 'null';
			}
			
			$boTem = $db->pegaUm("select count(nu_seq_mov_ne) from par.historicopagamentosigef where nu_seq_mov_pag = '{$dados['nu_seq_mov_pag']}' and nu_processo = '$nu_processo'");
			
			$dados['dt_emissao'] = ($dados['dt_emissao'] == '--' ? '' : $dados['dt_emissao']);
						
			$nu_parcela 			= $dados['nu_parcela'] 				? "'".$dados['nu_parcela']."'" 				: 'null';
			$an_exercicio 			= $dados['an_exercicio'] 			? "'".$dados['an_exercicio']."'" 			: 'null';
			$processo	 			= $nu_processo 						? "'".$nu_processo."'" 						: 'null';
			$vl_parcela 			= $dados['vl_parcela'] 				? "'".$dados['vl_parcela']."'" 				: 'null';
			$nu_mes 				= $dados['nu_mes'] 					? "'".$dados['nu_mes']."'" 					: 'null';
			$nu_documento_siafi_ne 	= $dados['nu_documento_siafi_ne'] 	? "'".$dados['nu_documento_siafi_ne']."'" 	: 'null';
			$nu_seq_mov_ne 			= $dados['nu_seq_mov_ne'] 			? "'".$dados['nu_seq_mov_ne']."'" 			: 'null';
			$ds_username_movimento 	= $dados['ds_username_movimento'] 	? "'".$dados['ds_username_movimento']."'" 	: 'null';
			$ds_situacao_doc_siafi 	= $dados['ds_situacao_doc_siafi'] 	? "'".$dados['ds_situacao_doc_siafi']."'" 	: 'null';
			$dt_movimento 			= $dados['dt_movimento'] 			? "'".$dados['dt_movimento']."'" 			: 'null';
			$nu_seq_mov_pag 		= $dados['nu_seq_mov_pag'] 			? "'".$dados['nu_seq_mov_pag']."'" 			: 'null';
			$dt_emissao 			= $dados['dt_emissao'] 				? "'".$dados['dt_emissao']."'" 				: 'null';
			$nu_documento_siafi 	= $dados['nu_documento_siafi'] 		? "'".$dados['nu_documento_siafi']."'" 		: 'null';
			$numero_de_vinculacao 	= $dados['numero_de_vinculacao'] 	? "'".$dados['numero_de_vinculacao']."'" 	: 'null';
			
						
			if( (int)$boTem == 0 ){

				$sql = "INSERT INTO par.historicopagamentosigef(prpid, proidpac, proidpar, empid, nu_processo, nu_parcela, an_exercicio, vl_parcela, nu_mes, nu_documento_siafi_ne, nu_seq_mov_ne,
  							ds_username_movimento, ds_situacao_doc_siafi, dt_movimento, nu_seq_mov_pag, dt_emissao, nu_documento_siafi, numero_de_vinculacao, data_atualizacao_rotina)
						VALUES (
							  $prpid,
							  $proidpac,
							  $proidpar,
  							  $empid,
							  $processo,
							  $nu_parcela,
							  $an_exercicio,
							  $vl_parcela,
							  $nu_mes,
							  $nu_documento_siafi_ne,
							  $nu_seq_mov_ne,
							  $ds_username_movimento,
							  $ds_situacao_doc_siafi,
							  $dt_movimento,
							  $nu_seq_mov_pag,
							  $dt_emissao,
							  $nu_documento_siafi,
							  $numero_de_vinculacao, now())";
				
				$db->executar($sql);
			} else {
				$sql = "UPDATE par.historicopagamentosigef SET 
						  	prpid 					= $prpid,
						  	proidpac 				= $proidpac,
						  	proidpar 				= $proidpar,
						  	empid 					= $empid,
						  	nu_processo 			= $processo,
						  	nu_parcela 				= $nu_parcela,
						  	an_exercicio 			= $an_exercicio,
						  	vl_parcela 				= $vl_parcela,
						  	nu_mes 					= $nu_mes,
						  	nu_documento_siafi_ne 	= $nu_documento_siafi_ne,
						  	nu_seq_mov_ne 			= $nu_seq_mov_ne,
						  	ds_username_movimento 	= $ds_username_movimento,
						  	ds_situacao_doc_siafi 	= $ds_situacao_doc_siafi,
						  	dt_movimento 			= $dt_movimento,
						  	nu_seq_mov_pag 			= $nu_seq_mov_pag,
						  	dt_emissao 				= $dt_emissao,
						  	nu_documento_siafi 		= $nu_documento_siafi,
						  	numero_de_vinculacao 	= $numero_de_vinculacao,
						  	data_atualizacao_rotina = now()					 
						WHERE 
						  	nu_seq_mov_pag = $nu_seq_mov_pag";		
				
				$db->executar($sql);
			}
			$db->commit();
		}
	$db->sucesso('sistema/diferencas_sigef_simec_pagamento', '&numeroprocesso='.$_REQUEST['numeroprocesso'].'&tipoprocesso='.$_REQUEST['tipoprocesso']);
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
			$numeroprocesso = simec_htmlentities( $_REQUEST['numeroprocesso'] );
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
	$filtroExiste = " (select empnumeroprocesso as processo from par.empenho where empstatus = 'A'
			           union all
			           select nu_processo as processo from par.historicopagamentosigef) ";
	
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
							 where 1=1 /*p.prostatus = 'A'*/";
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
?>
	<table class="listagem" width="100%" bgcolor="#F5F5F5" cellspacing="1" cellpadding="2" align="center">
	<thead>
		<tr>
			<td align="center" class="title" width="10%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Processo</strong></td>
			<td align="center" class="title" width="05%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Ano</strong></td>
			<td align="center" class="title" width="05%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>UF</strong></td>
			<td align="center" class="title" width="10%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Município/Estado</strong></td>
			<td align="center" class="title" width="35%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Pagamento SIMEC</strong></td>
			<td align="center" class="title" width="35%" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" <?=$estiloCaption ?>><strong>Pagamento SIGEF</strong></td>
		</tr>
	</thead>
	<tbody>
		<?if($arrProcesso[0]){
			$count = 0; 
			?>
			<?foreach ($arrProcesso as $key => $v) {
				$arrSimec = carregarDadosPagamentoSimec( $v['processo'] );
				$htmlSimec = $arrSimec;
				
				if( $_REQUEST['numeroprocesso'] ){
					$arrSigef = carregarDadosPagamentoSigef( $v['processo'], true );
				} else {
					$arrSigef = carregarDadosPagamentoSigef( $v['processo'] );
				}
				$htmlSigef = $arrSigef;
				
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
			<td align="center" style="color:#cc0000;" colspan="10">Não foram encontrados Registros de Pagamento.</td>
		</tr>
		</table>
	<?} ?>

<div id="div_dialog" title="Descrição do Retorno" style="display: none; text-align: left;">
	<div id="div_msg"></div>
</div>
<script type="text/javascript">
function submeteFormulario(){
	$('#requisicao').val('pesquisar');
	$('#formulario').submit();
}

function atualizaProcesso(){
	$('#requisicao').val('atualizarProcesso');
	$('#formulario').submit();
}
</script>
<?
function carregarDadosPagamentoSimec( $processo ){
	global $db;
	
	$sql = "select distinct
			    p.pagparcela as parcela,
			    p.pagmes as mes,
			    p.paganoexercicio as exercicio,
			    p.pagvalorparcela as vrlparcela,
			    p.parnumseqob as sequencial,
			    p.pagnumeroempenho as empenho,
			    p.pagnumeroob as ordermbancaria,
			    p.pagsituacaopagamento as situacao,
			    (select sum(total)
			    from(
			        select count(sbaid) as total, pagid from par.pagamentosubacao where pobstatus = 'A' group by pagid
			        union all
			        select count(preid), pagid from par.pagamentoobrapar group by pagid
			        union all
			        select count(preid), pagid from par.pagamentoobra group by pagid
			    ) as foo
			    where pagid = p.pagid) as totalemp
			from
			    par.pagamento p
			inner join par.empenho e on e.empid = p.empid
			where e.empnumeroprocesso = '{$processo}' AND p.pagstatus = 'A'
			order by p.parnumseqob, p.pagvalorparcela";
	
	$arrEmpSimec = $db->carregar($sql);
	$arrEmpSimec = $arrEmpSimec ? $arrEmpSimec : array();
	
	$estiloCaption = "style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"";
	$arrRetorno = array();
	if( $arrEmpSimec ){
	$html = '<table class="listagem" width="100%" bgcolor="#F5F5F5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th colspan="10">Dados Pagamento</th>
				</tr>
				<tr>
					<td align="center" class="title" '.$estiloCaption.'><strong>Parcela</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Mês</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Exercicio</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>OB</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Empenho</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Valor</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Situação</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Qtd Itens</strong></td>
				</tr>';
	
		foreach ($arrEmpSimec as $simec){
			$simec['empenho'] = substr($simec['empenho'], -6);
			$simec['ordermbancaria'] = substr($simec['ordermbancaria'], -6);

			$html.= '<tr>
						<td '.$estiloCaption.'>'.(str_pad($simec['parcela'], 3, 0, STR_PAD_LEFT)).'</td>
						<td '.$estiloCaption.'>'.(str_pad($simec['mes'], 3, 0, STR_PAD_LEFT)).'</td>
						<td '.$estiloCaption.'>'.$simec['exercicio'].'</td>
						<td '.$estiloCaption.'>'.$simec['sequencial'].'</td>
						<td '.$estiloCaption.'>'.$simec['ordermbancaria'].'</td>
						<td '.$estiloCaption.'>'.$simec['empenho'].'</td>
						<td '.$estiloCaption.'>'.number_format($simec['vrlparcela'], '2', ',', '.').'</td>
						<td '.$estiloCaption.'>'.$simec['situacao'].'</td>
						<td '.$estiloCaption.'>'.$simec['totalemp'].'</td>
					</tr>';
		}
		$html.= '</table>';
	} else {
		$html = '<span style="color:#cc0000;">Não foram encontrados Registros de Empenho SIMEC.</span>';
	}
	return $html;
}

function carregarDadosPagamentoSigef( $processo, $wsService = false ){
	global $db;
	
	if( $wsService ){
		$wsusuario 	= 'USAP_WS_SIGARP';
		$wssenha	= '03422625';

		$arrParam = array(
				'wsusuario' => $wsusuario,
				'wssenha' => $wssenha,
				'nu_processo' => $processo,
				'method' => 'historicopagamento',
		);
			
		$arrEmpSigef = montaXMLHistoricoProcessoSIGEF( $arrParam );
		$linha = '<td align="center" class="title" '.$estiloCaption.'><strong>Ação</strong></td>';
	} else {
		$sql = "select distinct
				    h.nu_parcela,
				    h.nu_mes,
				    h.an_exercicio,
				    h.vl_parcela,
				    h.nu_seq_mov_pag,
				    h.nu_documento_siafi_ne,
				    h.ds_situacao_doc_siafi,
				    h.nu_documento_siafi
				from
				    par.historicopagamentosigef h
				where h.nu_processo = '{$processo}'
				order by h.nu_seq_mov_pag, h.vl_parcela";
		
		$arrEmpSigef = $db->carregar($sql);
	}
	$arrEmpSigef = $arrEmpSigef ? $arrEmpSigef : array();
	
	$estiloCaption = "style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"";
	$arrRetorno = array();
	if( $arrEmpSigef ){
	$html = '<table class="listagem" width="100%" bgcolor="#F5F5F5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th colspan="10">Dados Pagamento</th>
				</tr>
				<tr>
					'.$linha.'
					<td align="center" class="title" '.$estiloCaption.'><strong>Parcela</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Mês</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Exercicio</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Sequencial</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>OB</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Empenho</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Valor</strong></td>
					<td align="center" class="title" '.$estiloCaption.'><strong>Situação</strong></td>
				</tr>';
	
		foreach ($arrEmpSigef as $sigef){
			$linhaTD = '';
			if( $wsService ){
				$boTem = $db->pegaUm("select count(nu_seq_mov_pag) from par.historicopagamentosigef where nu_seq_mov_pag = '{$sigef['nu_seq_mov_pag']}'");
				
				if( (int)$boTem < (int)1 ){
					$linhaTD = "<td ".$estiloCaption."><center><img src=../imagens/money_cancel.gif align=absmiddle style=cursor:pointer; title=\"Atualizar Processo\" onclick=\"atualizaProcesso('".trim($processo)."');\"></center></td>";
				} else {
					$linhaTD = '<td '.$estiloCaption.'></td>';
				}
			}
			
			$html.= '<tr>
					    '.$linhaTD.'
						<td '.$estiloCaption.'>'.$sigef['nu_parcela'].'</td>
						<td '.$estiloCaption.'>'.$sigef['nu_mes'].'</td>
						<td '.$estiloCaption.'>'.$sigef['an_exercicio'].'</td>
						<td '.$estiloCaption.'>'.$sigef['nu_seq_mov_pag'].'</td>
						<td '.$estiloCaption.'>'.$sigef['nu_documento_siafi'].'</td>
						<td '.$estiloCaption.'>'.$sigef['nu_documento_siafi_ne'].'</td>
						<td '.$estiloCaption.'>'.number_format($sigef['vl_parcela'], '2', ',', '.').'</td>
						<td '.$estiloCaption.'>'.$sigef['ds_situacao_doc_siafi'].'</td>
					</tr>';
		}
		$html.= '</table>';
	} else {
		$html = '<span style="color:#cc0000;">Não foram encontrados Registros de Empenho SIGEF.</span>';
	}
	return $html;
}
?>