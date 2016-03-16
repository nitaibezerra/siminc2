<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//Espaço em branco Espaço em branco
if( $_POST['requisicao'] == 'salvar' ){
	salvarImpositivo( $_REQUEST );
	$db->sucesso($_REQUEST['modulo'], '&emdid='.$_REQUEST['emdid'].'&edeid='.$_REQUEST['edeid'].'&valor='.$_REQUEST['valor'].'&tipo='.$_REQUEST['tipo']);
}

if( $_POST['requisicao'] == 'salvaranexos' ){
	$ediid = salvarImpositivo( $_REQUEST );
	
	if( $_FILES["arquivo"] && !$_POST['arqid'] ){
		$campos	= array(
						"anxdsc" => "'{$_POST['anxdescricao']}'",
						"anxdata" => "'now()'",
						"ediid" => $ediid
						);	
		$file = new FilesSimec("anexo", $campos ,"emenda");
		$arquivoSalvo = $file->setUpload();
	} elseif($_POST['arqid']) {
	    $sql = "UPDATE emenda.anexo SET anxdsc = '{$_POST['anxdescricao']}' where arqid=".$_POST['arqid'];
	    $db->executar($sql);
	    $db->commit();
	}
	$db->sucesso($_REQUEST['modulo'], '&emdid='.$_REQUEST['emdid'].'&edeid='.$_REQUEST['edeid'].'&valor='.$_REQUEST['valor'].'&tipo='.$_REQUEST['tipo']);
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    $db->sucesso($_REQUEST['modulo'], '&emdid='.$_REQUEST['emdid'].'&edeid='.$_REQUEST['edeid'].'&valor='.$_REQUEST['valor'].'&tipo='.$_REQUEST['tipo']);
}

#manipular anexos de arquivos para envio de email
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    $db->sucesso($_REQUEST['modulo'], '&emdid='.$_REQUEST['emdid'].'&edeid='.$_REQUEST['edeid'].'&valor='.$_REQUEST['valor'].'&tipo='.$_REQUEST['tipo']);
}

function salvarImpositivo( $dados ){
	global $db;
	
	if( $dados['edivalor'] ){
		$edivalor = str_replace(".","", $dados['edivalor']);
		$edivalor = str_replace(",",".", $edivalor);
	} else {
		$edivalor =  'null';
	}	
	$ediimpositivo = ($dados['ediimpositivo'] ? "'".$dados['ediimpositivo']."'" : 'null');
	$edidescricao = ($dados['edidescricao'] ? "'".$dados['edidescricao']."'" : 'null');
	$edeid = ($dados['edeid'] ? $dados['edeid'] : 'null');
	
	if( $dados['ediid'] ){
		/*if( $dados['ediimpositivo'] == 'NH'){ 
			$edidescricao = 'null';
		}*/
		$sql = "UPDATE emenda.emendadetalheImpositivo SET 
				  emdid = {$dados['emdid']},
				  edivalor = $edivalor,
				  edeid = $edeid,
				  usucpf = '{$_SESSION['usucpf']}',
				  ediimpositivo = $ediimpositivo,
				  edidescricao = $edidescricao				 
				WHERE 
				  ediid = {$dados['ediid']}";
		$db->executar($sql);
		$ediid = $dados['ediid'];		
	} else {
		$sql = "INSERT INTO emenda.emendadetalheimpositivo(emdid, edeid, edidescricao, edivalor, usucpf, ediimpositivo) 
				VALUES ({$dados['emdid']}, $edeid, $edidescricao, $edivalor, '{$_SESSION['usucpf']}', $ediimpositivo) returning ediid";
		$ediid = $db->pegaUm( $sql );
	}
	$db->executar("DELETE FROM emenda.emendaimpositivo_tiposjust WHERE ediid = $ediid");
	if( $dados['emiid'][0] ){
		foreach ($dados['emiid'] as $emiid) {
			$sql = "INSERT INTO emenda.emendaimpositivo_tiposjust(ediid, emiid) 
					VALUES ($ediid, $emiid)";
			$db->executar($sql);
		}
	}
	$db->commit();
	return $ediid;
}

include APPRAIZ . 'includes/Agrupador.php';

$edeid = $_REQUEST['edeid'];

if( !empty($edeid) && $_SESSION['exercicio'] > 2014 ){
	$filtro = " and edeid = $edeid ";
	$coluna = ', eb.enbcnpj, eb.enbnome';
}

$sql = "SELECT ediid, emdid, edivalor, edidata, edistatus,
  			usucpf, ediimpositivo, edidescricao
		FROM emenda.emendadetalheimpositivo WHERE emdid = {$_REQUEST['emdid']} $filtro";
$arImpositivo = $db->pegaLinha($sql);
$arImpositivo = $arImpositivo ? $arImpositivo : array();
extract($arImpositivo);

$habilita = $habilitabtn;

$sql = "SELECT
		    emecod as numero,
		    pu.unidsc as unidade,
		    ef.fupfuncionalprogramatica as funcional,
		    ef.fupdsc as subtitulo,
		    CASE WHEN ve.resid is not null THEN er.resdsc ELSE 'Não informado' END as responsavel,
		    ve.gndcod||' - '||gn.gnddsc as gnd,
		    ve.mapcod||' - '||map.mapdsc as mod,
		    ve.foncod||' - '||fon.fondsc as fonte,
		    case when cast(ve.emeano as integer) > 2014 then ve.edevalor else ve.emdvalor end as valor,
		    et.etodescricao $coluna
		FROM
		    emenda.v_emendadetalheentidade ve
		    inner join emenda.v_funcionalprogramatica ef ON ef.acaid = ve.acaid
		    inner join emenda.emendatipoorigem et on et.etoid = ve.etoid
		    inner join emenda.entidadebeneficiada eb on eb.enbid = ve.entid and eb.enbstatus = 'A'
		    left join emenda.responsavel er ON er.resid = ve.resid
		    left join public.unidade pu ON pu.unicod = ef.unicod
		    left join public.gnd gn on gn.gndcod = ve.gndcod and gn.gndstatus = 'A'
		    left join public.modalidadeaplicacao map on map.mapcod = ve.mapcod
		    left join public.fonterecurso fon on fon.foncod = ve.foncod and fon.fonstatus = 'A' 
		WHERE
		    ve.emdid = {$_REQUEST['emdid']}
		    and ve.edestatus = 'A' $filtro";
$arDetalheEmenda = $db->pegaLinha($sql);
?>
<html>
	<head>
		<title>SIMEC - Emenda Impendimento</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/agrupador.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<style>
			#emiid_, #emiid { width: 300px; }
		</style>
	</head>
<body>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="emdid" id="emdid" value="<?=$_REQUEST['emdid']; ?>">
    <input type="hidden" name="edeid" id="edeid" value="<?=$_REQUEST['edeid']; ?>">
    <input type="hidden" name="edevalor" id="edevalor" value="<?=$_REQUEST['valor']; ?>">
    <input type="hidden" name="ediid" id="ediid" value="<?=$arImpositivo['ediid']; ?>">
    <input type="hidden" name="edivalorutilizado" id="edivalorutilizado" value="<?=$arImpositivo['edivalor']; ?>">
    
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita" colspan="4"><center><b>Dados do detalhe da Emenda</b></center></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Unidade Orçamentária:</b></td>
			<td><?php echo $arDetalheEmenda["unidade"];?></td>
			<td class="subtitulodireita"><b>Responsável:</b></td>
			<td><?php echo $arDetalheEmenda["responsavel"];?></td>
						
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Modalidade de Aplicação:</b></td>
			<td><?php echo $arDetalheEmenda["mod"];?></td>
			<td class="subtitulodireita"><b>Fonte:</b></td>
			<td><?php echo $arDetalheEmenda["fonte"];?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>GND:</b></td>
			<td><?php echo $arDetalheEmenda["gnd"];?></td>
			<td class="subtitulodireita"><b>Origem Emenda:</b></td>
			<td><?php echo $arDetalheEmenda["etodescricao"];?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Funcional Programática:</b></td>
			<td colspan="3">
				<b><?php echo $arDetalheEmenda["funcional"].'</b> - '.$arDetalheEmenda["subtitulo"];?>
			</td>
		</tr>
<?php 	if( $_SESSION['exercicio'] > 2014 ){ ?>
		<tr>
			<td class="subtitulodireita"><b>Entidade Beneficiada:</b></td>
			<td colspan="3">
				<b><?php echo formatar_cnpj($arDetalheEmenda["enbcnpj"]).'</b> - '.$arDetalheEmenda["enbnome"];?>
			</td>
		</tr>
<?php 	}?>
		<tr>
			<td class="subtitulodireita"><b>Valor:</b></td>
			<td colspan="3">
				<?php echo number_format( $arDetalheEmenda["valor"], 2, ',', '.');?>
			</td>
		</tr>
	</table>
    
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita" colspan="2"><center><b>Existe algum impedimento para Execução da Emenda?</b></center></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Impedimento:</b></td>
			<td>
				<input type="radio" name="ediimpositivo" id="ediimpositivo_nh" <?=($ediimpositivo == 'NH' ? 'checked="checked"' : '' )?> value="NH"> Não Há
				<input type="radio" name="ediimpositivo" id="ediimpositivo_to" <?=($ediimpositivo == 'TO' ? 'checked="checked"' : '' )?> value="TO"> Total
				<input type="radio" name="ediimpositivo" id="ediimpositivo_pa" <?=($ediimpositivo == 'PA' ? 'checked="checked"' : '' )?> value="PA"> Parcial
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Valor de Impedimento:</b></td>
			<td><?=campo_texto( 'edivalor', 'N', ($habilita ? 'S' : 'N'), 'Valor de Impedimento', 30, 15, '[###.]###,##', '','','','','id="edivalor" class="valorimpositivo"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
		</tr>
		<tr>
			<td colspan="2">
				<table align="center" border="0" class="tabela" style="width: 100%" cellpadding="3" cellspacing="2">
					<tr>
						<td class="SubTituloDireita" colspan="2"><center><b>Justificativa</b></center></td>
					</tr>
					<tr id="tr_justificaitva">
						<td class="SubTituloDireita"><b>Tipos de Impedimento:</b></td>
						<td><?php
							$agrupador = new Agrupador( "formulario" );
							
							$sql = sprintf( "SELECT distinct ei.emiid, emidescricao, eit.ediid FROM emenda.emendaimpositivo ei
												left join emenda.emendaimpositivo_tiposjust eit on eit.emiid = ei.emiid WHERE emistatus = 'A' ;" );
							$tipos = $db->carregar( $sql );
							
							$origem = array();
							$destino = array();
							if ( $tipos ) {
								foreach ( $tipos as $tipo ) {
									if ( in_array( $tipo['ediid'], (array) $ediid ) ) {
										$destino[] = array(
											'codigo'    => $tipo['emiid'],
											'descricao' => $tipo['emidescricao']
										);
									} else {
										$origem[] = array(
											'codigo'    => $tipo['emiid'],
											'descricao' => $tipo['emidescricao']
										);
									}
								}
							}
							$agrupador->setOrigem( "emiid_", null, $origem );
							$agrupador->setDestino( "emiid", null, $destino );
							$agrupador->exibir();
							?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Descrição:</b></td>
						<td><?=campo_textarea('edidescricao', 'N', 'S', 'Descrição', 115, 5, 2000, '', '', '', '', 'Descrição');?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" colspan="2"><center><b>Anexos</b></center></td>
					</tr>
					<tr>
						<td colspan="2">
							<table align="center" border="0" class="tabela" style="width: 100%" cellpadding="3" cellspacing="2">
								<tr>
									<th>Arquivo</th>
									<th>Descrição</th>
									<th>Ação</th>
								</tr>
								<tr>
									<td><input type="file" name="arquivo" id="arquivo"></td>
									<td><?=campo_texto( 'anxdescricao', 'N', 'S', 'Descrição do Anexo', 30, 100, '', '', '', '', '', 'id="anxdescricao"', '', '', ""); ?>
									<td align="center">
									<?if( $habilita ){ ?>
										<input type="button" name="btnSalvarAnexo" id="btnSalvarAnexo" value="Salvar" onclick="salvarImpositivo('A');">
									<?}else{ ?>
										<input type="button" name="btnSalvarAnexo" id="btnSalvarAnexo" value="Salvar" disabled="disabled">
									<?} ?>
										</td>
								</tr>
								 <?php 
								   $sql = "select anx.anxid,
								   				anx.arqid,
												anx.ptrid,
												anx.anxdsc,
												arq.arqnome || '.' || arq.arqextensao as arquivo
											from emenda.anexo anx
												inner join public.arquivo arq on anx.arqid = arq.arqid
									   			inner join emenda.emendadetalheimpositivo ed on ed.ediid = anx.ediid
									   		where ed.emdid = {$_REQUEST['emdid']} 
		                                    	and ed.edistatus = 'A'";
							   
							$arDados = $db->carregar($sql);
						    
						    $count = 1;
						    
						    if($arDados) {
							    foreach($arDados as $dados){
							    ?>
								<tr>		    
							        <td align="left">
							        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/emendaImpositivo&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>&emdid=<?php echo $_REQUEST['emdid']?>&edeid=<?php echo $_REQUEST['edeid']?>'"><?php echo $dados['arquivo'];?></a>
							        </td>
							        <td align="left">
							        	<?php echo $dados['anxdsc']; ?>
							        </td>
							        <td align="center">
							        	<?if( $habilita ){ ?>
							        		<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
							        	<?} else { ?>
							        		<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
							        	<?} ?>
							        </td>
								   	</tr>
							    <?php
							    $count++;
							    }
							} else {
								echo '<tr>
								    	<td style="color: red">Nenhum anexo disponível!</td>
								    </tr>';
							}
							    ?>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td align="center" colspan="2">
				<?if($pedid || !$habilita ){ ?>
					<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" disabled="disabled">
					<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fechar();">
				<?} else { ?>
					<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" onclick="salvarImpositivo('');">
					<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fechar();">
				<?} ?>
			</td>
		</tr>
	</table>
</form>
</body>
<script type="text/javascript">

function fechar(){
	window.opener.location.href = window.opener.location;
	window.close();
}

$(document).ready(function(){
	
	if( $('[name="ediid"]').val() != '' ){
		var edivalor = $('[name="edivalor"]').val();
		
		if( $('[name="ediimpositivo"]:checked').val() != 'NH'){
			$('#tr_justificaitva').css('display', '');
			$('[name="edivalor"]').val( mascaraglobal('[###.]###,##', edivalor ));
		} else {
			$('[name="edivalor"]').attr('readonly', true);
			$('[name="edivalor"]').val( mascaraglobal('[###.]###,##', edivalor ));
			$('#tr_justificaitva').css('display', 'none');
		}
	} else {
		$('[name="edivalor"]').attr('readonly', true);
		$('#tr_justificaitva').css('display', 'none');
		$('[name="edivalor"]').val('0,00');
	}
	
	$('[name="ediimpositivo"]').click(function(){
		
		if( $(this).val() != 'NH'){
			$('[name="edivalor"]').attr('readonly', false);
			$('#tr_justificaitva').css('display', '');
			var edivalorutilizado 	= $('[name="edivalorutilizado"]').val();
			var valor 				= $('[name="edevalor"]').val();
			
			if( $(this).val() == 'TO' ){
				if( valor == '0.00' || valor == '00' || valor == '' ){
					$('[name="edivalor"]').val( mascaraglobal('[###.]###,##', parseFloat(edivalorutilizado).toFixed(2) ));
				} else {
					$('[name="edivalor"]').val( mascaraglobal('[###.]###,##', parseFloat(valor).toFixed(2) ));
				}
			} else {
				if( edivalorutilizado != '' ){
					$('[name="edivalor"]').val( mascaraglobal('[###.]###,##', parseFloat(edivalorutilizado).toFixed(2) ) );
				} else {
					$('[name="edivalor"]').attr('readonly', false);
					$('[name="edivalor"]').val('0,00');
				}				
			}
		} else {
			$('[name="edivalor"]').attr('readonly', true);
			$('#tr_justificaitva').css('display', 'none');
			$('[name="edivalor"]').val('0,00');
		}
	});
	
	$('.valorimpositivo').focusin(function(){
		if( parseInt( $(this).val() ) == '0,00' || parseInt( $(this).val() ) == '00' ){
			$(this).val('');
		}
	});
	
	$('.valorimpositivo').focusout(function(){
		if( $(this).val() == '' || $(this).val() == '0' || $(this).val() == '00' ){
			$(this).val('0,00');
		}
	});
});

function salvarImpositivo(tipo){
	if( $('[name="ediimpositivo"]:checked').length == 0 ){
		alert('Informe se existe orçamento impedimento para a emenda!');
		$('[name="ediimpositivo"]').focus();
		return false;
	} else {	
		if( $('[name="ediimpositivo"]:checked').val() != 'NH' ){
			if( $('[name="edivalor"]').val() == '' || $('[name="edivalor"]').val() == '0' || $('[name="edivalor"]').val() == '00' || $('[name="edivalor"]').val() == '0,00' ){
				alert('O campo Valor do Orçamento Impedimento é preenchimento obrigatório!');
				$('[name="edivalor"]').focus();
				return false;
			}
			
			selectAllOptions( document.getElementById( 'emiid' ) );
			
			var emiid = document.getElementById( 'emiid' ).length;
			if( emiid == 0 ){
				alert('O campo Tipos de Orçamento Impedimento é preenchimento obrigatório!');
				return false;
			}
		}
		if( $('[name="edidescricao"]').val() == '' ){
			alert('O campo Descrição é preenchimento obrigatório!');
			$('[name="edidescricao"]').focus();
			return false;
		}
			
		if( tipo == 'A' ){
			if( $('[name="arquivo"]').val() == '' ){
				alert('Você deve escolher um arquivo!');
				return false;
			}
			if( $('[name="anxdescricao"]').val() == '' ){
				alert('O campo Descrição do Arquivo é preenchimento obrigatório!');
				$('[name="anxdescricao"]').focus();
				return false;
			}
			$('[name="requisicao"]').val('salvaranexos');
		} else {
			$('[name="requisicao"]').val('salvar');
		}
		$('[name="formulario"]').submit();
	}
}

function excluirAnexo( arqid, tipo ){
	var edeid = $('[name="edeid"]').val();
	var emdid = $('[name="emdid"]').val();
	
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid+'&edeid='+edeid+'&emdid='+emdid;
 	}
}
</script>
</html>