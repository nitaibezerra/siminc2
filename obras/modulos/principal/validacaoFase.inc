<?php

if(isset($_POST['cancelar'])){	
	switch ($_POST['cancelar']){
		case 'btnCancelaHmo':
			$sql = "UPDATE obras.validacao SET vldstatushomologacao = '', vldobshomologacao = '' WHERE vldid = ".$_POST['vldid'];
			break;
		case 'btnCancela25':
			$sql = "UPDATE obras.validacao SET vldstatus25exec = '', vldobs25exec = '' WHERE vldid = ".$_POST['vldid'];
			break;
		case 'btnCancela50':
			$sql = "UPDATE obras.validacao SET vldstatus50exec = '', vldobs50exec = '' WHERE vldid = ".$_POST['vldid'];
			break;
	}
	if(isset($sql)){
		$db->executar($sql);
		if($db->commit()){
			die('true');			
		}
	}
	die('false');
}

if(!$_SESSION["obra"]["obrid"]) {
	// se não existir obrid então envie o usuário para a tela de seleção de obras
	header( "location:obras.php?modulo=inicio&acao=A" );
    exit;
}
$obras = new Obras();
switch($_REQUEST['requisicao']) {
	case "salvar":
		extract($_POST);		
		$habilita25 = $obrpercexec >= '25' ? true : false;
		$habilita50 = $obrpercexec >= '50' ? true : false;
		
		if( $vldstatushomologacao ){
			$vldstatushomologacao 	= "'".$vldstatushomologacao."'" ;
			$usucpf_homo 			= $usucpf_homo ? "'".$usucpf_homo."'" : "'".$_SESSION['usucpf']."'" ;
			$vlddtinclusaosthomo	= $vlddtinclusaosthomo ? "'".$vlddtinclusaosthomo."'" : "now()" ;
		} else {
			$vldstatushomologacao 	= "'".''."'";
			$usucpf_homo 			= "null";
			$vlddtinclusaosthomo	= "null";
		}
		
		if( $vldstatus25exec && $habilita25 ){
			$vldstatus25exec 		= "'".$vldstatus25exec."'" ;
			$usucpf_25 				= $usucpf_25 ? "'".$usucpf_25."'" : "'".$_SESSION['usucpf']."'" ;
			$vlddtinclusaost25exec	= $vlddtinclusaost25exec ? "'".$vlddtinclusaost25exec."'" : "now()" ;
		} else {
			$vldstatus25exec 		= "'".''."'";
			$usucpf_25 				= "null";
			$vlddtinclusaost25exec	= "null";
		}
		
		if( $vldstatus50exec && $habilita50 ){
			$vldstatus50exec 		= "'".$vldstatus50exec."'" ;
			$usucpf_50 				= $usucpf_50 ? "'".$usucpf_50."'" : "'".$_SESSION['usucpf']."'" ;
			$vlddtinclusaost50exec	= $vlddtinclusaost50exec ? "'".$vlddtinclusaost50exec."'" : "now()" ;
		} else {
			$vldstatus50exec 		= "'".''."'";
			$usucpf_50 				= "null";
			$vlddtinclusaost50exec	= "null";
		}
		$vldid = $_POST['vldid'];
		$obrid = $_SESSION["obra"]["obrid"];
		
		//die($vldstatus50exec);
		
		if( $_POST['vldid'] ){
			$sql = "UPDATE obras.validacao SET 
					  usucpf_homo = $usucpf_homo,
					  usucpf_25 = $usucpf_25,
					  usucpf_50 = $usucpf_50,
					  obrid = $obrid,
					  vldstatushomologacao = $vldstatushomologacao,
					  vlddtinclusaosthomo = $vlddtinclusaosthomo,
					  vldstatus25exec = $vldstatus25exec,
					  vlddtinclusaost25exec = $vlddtinclusaost25exec,
					  vldstatus50exec = $vldstatus50exec,
					  vlddtinclusaost50exec = $vlddtinclusaost50exec,					  
					  vldobshomologacao = '$vldobshomologacao',
					  vldobs25exec = '$vldobs25exec',
					  vldobs50exec = '$vldobs50exec'					 
					WHERE 
					  vldid = $vldid";
		} else {
			$sql = "INSERT INTO obras.validacao( usucpf_homo, usucpf_25, usucpf_50, obrid, vldstatushomologacao,
	  					vlddtinclusaosthomo, vldstatus25exec, vlddtinclusaost25exec, vldstatus50exec, vlddtinclusaost50exec,
	  					vldobshomologacao, vldobs25exec, vldobs50exec) 
					VALUES ($usucpf_homo, $usucpf_25, $usucpf_50, $obrid, $vldstatushomologacao,
	  					$vlddtinclusaosthomo, $vldstatus25exec, $vlddtinclusaost25exec, $vldstatus50exec, $vlddtinclusaost50exec,
	  					'$vldobshomologacao', '$vldobs25exec', '$vldobs50exec')";
		}		
		$db->executar( $sql );
		if( $db->commit() ){
			$db->sucesso( 'principal/validacaoFase' );
			exit();
		} else {
			echo "<script>alert('Falha na operação');</script>";
		}
		break;
	case "download":
		$obras->DownloadArquivo( $_REQUEST );
	break;
}

// cabeçalho padrão SIMEC e permissões do módulo
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "www/obras/permissoes.php";

// Monta as abas e o título da tela
print "<br/>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Validação da Obra", "");
echo $obras->CabecalhoObras();
if($_SESSION["obra"]["obrid"]){	
	$sql = "SELECT v.vldid, u1.usunome as usuhomo, u2.usunome as usu25, u3.usunome as usu50, v.obrid, v.usucpf_homo, v.usucpf_25,
  				v.usucpf_50, v.vldstatushomologacao, v.vlddtinclusaosthomo, to_char(v.vlddtinclusaosthomo, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaosthomo1,
				v.vldstatus25exec, to_char(v.vlddtinclusaost25exec, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaost25exec1, v.vlddtinclusaost25exec, 
				v.vldstatus50exec, to_char(v.vlddtinclusaost50exec, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaost50exec1, v.vlddtinclusaost50exec, fl.flcid, fl.tflid, fl.flchomlicdtprev,
				fl.flcdata, fl.flcmeiopublichomol, fl.flcobshomol, oi.obrpercexec,
				v.vldobshomologacao, v.vldobs25exec, v.vldobs50exec
			FROM obras.obrainfraestrutura oi
			  	left join obras.validacao v on v.obrid = oi.obrid
			  	left join obras.faselicitacao fl on fl.obrid = oi.obrid and fl.tflid = 9 and fl.flcstatus = 'A'
			  	left join seguranca.usuario u1 on u1.usucpf = v.usucpf_homo
				left join seguranca.usuario u2 on u2.usucpf = v.usucpf_25
				left join seguranca.usuario u3 on u3.usucpf = v.usucpf_50
			WHERE 
			  	oi.obrid = ".$_SESSION["obra"]["obrid"];
	
	$arrFases = $db->pegaLinha( $sql );
} 
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	jQuery.noConflict();

	function DownloadArquivoFase(arqid){
		window.location = 'obras.php?modulo=principal/validacaoFase&acao=A&requisicao=download&arqid='+arqid;
	}
	
	function salvarValidacao(){
		var form = document.getElementById('formulario');
		var msg = true;
		for(i=0; i<form.length; i++){
			if(formulario.elements[i].type == 'radio' ){
				if( formulario.elements[i].checked == true ){
					msg = false;
				}
			}			
		}
		
		if( msg ){
			alert('Selecione uma validação!');
			return false;
		}
		if(jQuery('#vldstatushomologacao_n').attr('checked') && jQuery('#vldobshomologacao').val() == ''){
			alert('É obrigatório a justificativa da validação da homologação!');
			jQuery('#vldobshomologacao').focus();
			return false;	
		}
		if(jQuery('#vldstatus25exec_n').attr('checked') && jQuery('#vldobs25exec').val() == ''){
			alert('É obrigatório a justificativa da Execução Física 25%!');
			jQuery('#vldobs25exec').focus();
			return false;	
		}
		if(jQuery('#vldstatus50exec_n').attr('checked') && jQuery('#vldobs50exec').val() == ''){
			alert('É obrigatório a justificativa da Execução Física 50%!');
			jQuery('#vldobs50exec').focus();
			return false;	
		}
		
		document.getElementById('requisicao').value = 'salvar';
		document.getElementById('formulario').submit();
	}
	
	jQuery(function(){
		
		jQuery('#vldobshomologacao, #vldobs25exec, #vldobs50exec').parent().hide();
		
		// Validação da Homologação
		jQuery('#vldstatushomologacao_n').click(function(){
			jQuery('#vldobshomologacao').parent().show();						
		});
		jQuery('#vldstatushomologacao_s').click(function(){		
			jQuery('#vldobshomologacao').val('').parent().hide();
		});
		
		// Validação dos 25%
		jQuery('#vldstatus25exec_n').click(function(){	
			jQuery('#vldobs25exec').parent().show();			
		});
		jQuery('#vldstatus25exec_s').click(function(){
			jQuery('#vldobs25exec').val('').parent().hide();
		});
		
		// Validação dos 50%
		jQuery('#vldstatus50exec_n').click(function(){
			jQuery('#vldobs50exec').parent().show();			
		});
		jQuery('#vldstatus50exec_s').click(function(){
			jQuery('#vldobs50exec').val('').parent().hide();
		});
		
		jQuery('.Cancelar').click(function(){	
			id = this.id.replace('btnCancela','');
			validacao = id == 'Hmo' ? 'Homologação' : id == '25' ? 'Execução Física 25%' : 'Execução Física 50%';
			if(confirm('Deseja cancelar a validação da '+validacao+'?')){		
				jQuery.ajax({
					url: 'obras.php?modulo=principal/validacaoFase&acao=A',
					type: 'post',
					data: 'cancelar='+this.id+'&vldid='+jQuery('input[name=vldid]').val(),
					success: function(e){				
						if(e == 'true')
							document.location.href = 'obras.php?modulo=principal/validacaoFase&acao=A';
					}
				});
			}
		});
		
	});
</script>
<table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtituloesquerda" colspan="2">Fase de Homologação</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Fase:</td>
		<td>
			<?php
			if( $arrFases['tflid'] ){
				$sql = "SELECT tf.tfldesc AS descricao FROM obras.tiposfaseslicitacao tf
	                    WHERE tf.tflid = ".$arrFases['tflid'];
				$tfldesc = $db->pegaUm( $sql );
			}
			
			echo campo_texto( 'tfldesc', 'N', 'N', '', 80, 60, '',  '', 'left', '', 0);?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data:</td>
		<td>
			<? $flchomlicdtprev = $arrFases['flchomlicdtprev'] ?>
			<? echo campo_data( 'flchomlicdtprev', 'N', 'N', '', 'S' )?>
			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Meio de Publicação:</td>
		<td>
			<? $flcmeiopublichomol = $arrFases['flcmeiopublichomol'] ?>
			<? echo campo_texto( 'flcmeiopublichomol', 'N', 'N', '', 80, 60, '',  '', 'left', '', 0);?>			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Observação:</td>
		<td>
			<? $flcobshomol = $arrFases['flcobshomol'] ?>
			<? echo campo_textarea( 'flcobshomol', 'N', 'N', '', '84', '4', '500');?>			
		</td>
	</tr>
	<tr>
		<td class="subtituloesquerda" colspan="2">Anexos</td>
	</tr>
</table>
<?
$sql = "SELECT
			to_char(aqb.aqodtinclusao,'DD/MM/YYYY'),
			tarq.tpadesc,
			'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivoFase(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
			arq.arqtamanho || ' kbs' as tamanho ,
			arq.arqdescricao,								
			usu.usunome
		FROM
			((public.arquivo arq 
				INNER JOIN obras.arquivosobra aqb ON arq.arqid = aqb.arqid) 
				INNER JOIN obras.tipoarquivo tarq ON tarq.tpaid = aqb.tpaid) 
				INNER JOIN seguranca.usuario usu ON usu.usucpf = aqb.usucpf
		WHERE
			aqb.aqostatus = 'A' AND	aqb.obrid = '" . $_SESSION["obra"]["obrid"] . "'
			AND (arqtipo <> 'image/jpeg' AND arqtipo <> 'image/png' AND arqtipo <> 'image/gif')
			AND tarq.tpaid in (24)";

$cabecalho = array( 
					"Data Inclusão",
					"Tipo Arquivo",
					"Nome Arquivo",
					"Tamanho (Mb)",
					"Descrição Arquivo",
					"Responsável");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );

$habilitaHo = !empty($arrFases['tflid']) ? '': 'disabled="disabled"';
$habilita25 = $arrFases['obrpercexec'] >= '25' ? '' : 'disabled="disabled"';
$habilita50 = $arrFases['obrpercexec'] >= '50' ? '' : 'disabled="disabled"';
?>
<form name="formulario" id="formulario" method="post">
<input type="hidden" id="requisicao" name="requisicao" value="">
<input type="hidden" id="obrpercexec" name="obrpercexec" value="<?=$arrFases['obrpercexec']; ?>">
<input type="hidden" id="vldid" name="vldid" value="<?=$arrFases['vldid']; ?>">

<table class="tabela" width="95%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<? if( $arrFases['vldstatushomologacao'] == 'S' ){ ?>
			<tr>
				<td class="subtitulodireita" width="20%">Ordem de serviço inserida em:</td>
				<td><?=$arrFases['vlddtinclusaosthomo1']; ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="20%">Responsável:</td>
				<td><?=$arrFases['usuhomo']; ?></td>
			</tr>
			<tr>
				<td><input type="hidden" name="vldstatushomologacao" id="vldstatushomologacao" value="<?=$arrFases['vldstatushomologacao']; ?>">
				<input type="hidden" name="vlddtinclusaosthomo" id="vlddtinclusaosthomo" value="<?=$arrFases['vlddtinclusaosthomo']; ?>">
				<input type="hidden" name="usucpf_homo" id="usucpf_homo" value="<?=$arrFases['usucpf_homo']; ?>"></td>
				<td><input type="button" name="btnCancelaHmo" id="btnCancelaHmo" class="Cancelar" value="Cancelar"></td>
			</tr>	
		<?} else { ?>
			<tr>
				<td class="subtitulodireita" width="20%">Documento de Ordem de Serviço Inserido?</td>
				<td>
					<?php if($arrFases['vldstatushomologacao'] == 'N'): ?>
						<script>
							jQuery(function(){
								jQuery('#vldobshomologacao').parent().show();
							});
						</script>
					<?php else: ?>
						<script>
							jQuery(function(){
								jQuery('#vldobshomologacao').val('').parent().hide();
							});
						</script>
					<?php endif; ?>
					<input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_s" <?=$habilitaHo; ?> value="S" <?php echo $arrFases['vldstatushomologacao'] == 'S' ? 'checked' : '' ?>>Sim
					<input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_n" <?=$habilitaHo; ?> value="N" <?php echo $arrFases['vldstatushomologacao'] == 'N' ? 'checked' : '' ?>>Não					
					<?php 
					
					$var = 'vldobshomologacao'; 
					$obrig = 'N'; 
					$habil = 'S'; 
					$label = ''; 
					$cols = 80; 
					$rows = 5; 
					$max = 5000; 
					$funcao = ''; 
					$acao = 0; 
					$txtdica = ''; 
					$tab = false; 
					$title = NULL; 
					$value = $arrFases['vldobshomologacao'];
					
					echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value); 
					?>
				</td>
			</tr>
		<?} ?>
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>
<table class="listagem" width="95%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="subtituloesquerda" colspan="2">Execução Física 25%</td>
	</tr>
	<? if( $arrFases['vldstatus25exec'] == 'S' ){ ?>
			<tr>
				<td class="subtitulodireita" width="20%">Homologação Validade em:</td>
				<td><?=$arrFases['vlddtinclusaost25exec1']; ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="20%">Responsável:</td>
				<td><?=$arrFases['usu25']; ?></td>
			</tr>
			<tr>
				<td><input type="hidden" name="vldstatus25exec" id="vldstatus25exec" value="<?=$arrFases['vldstatus25exec']; ?>">
				<input type="hidden" name="vlddtinclusaost25exec" id="vlddtinclusaost25exec" value="<?=$arrFases['vlddtinclusaost25exec']; ?>">
				<input type="hidden" name="usucpf_25" id="usucpf_25" value="<?=$arrFases['usucpf_25']; ?>"></td>
				<td><input type="button" name="btnCancela25" id="btnCancela25" class="Cancelar" value="Cancelar"></td>
			</tr>	
		<?} else { ?>
			<tr>
				<td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
				<td>
					<input type="radio" name="vldstatus25exec" id="vldstatus25exec_s" <? echo $habilita25; ?> value="S" <?php echo $arrFases['vldstatus25exec'] == 'S' ? 'checked' : '' ?>>Sim
					<input type="radio" name="vldstatus25exec" id="vldstatus25exec_n" <? echo $habilita25; ?> value="N" <?php echo $arrFases['vldstatus25exec'] == 'N' ? 'checked' : '' ?>>Não
					<?php if($arrFases['vldstatus25exec'] == 'N'): ?>
						<script>
							jQuery(function(){
								jQuery('#vldobs25exec').parent().show();
							});
						</script>
					<?php else: ?>
						<script>
							jQuery(function(){
								jQuery('#vldobs25exec').val('').parent().hide();
							});
						</script>
					<?php endif; ?>					
					<?php 
					
					$var = 'vldobs25exec'; 
					$obrig = 'N'; 
					$habil = 'S'; 
					$label = ''; 
					$cols = 80; 
					$rows = 5; 
					$max = 5000; 
					$funcao = ''; 
					$acao = 0; 
					$txtdica = ''; 
					$tab = false; 
					$title = NULL; 
					$value = $arrFases['vldobs25exec'];					
					
					echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value); 
					?>
				</td>
			</tr>
		<?} ?>
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>
<table class="listagem" width="95%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="subtituloesquerda" colspan="2">Execução Física 50%</td>
	</tr>
	<? if( $arrFases['vldstatus50exec'] == 'S' ){ ?>
			<tr>
				<td class="subtitulodireita" width="20%">Homologação Validade em:</td>
				<td><?=$arrFases['vlddtinclusaost50exec1']; ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="20%">Responsável:</td>
				<td><?=$arrFases['usu50']; ?></td>
			</tr>
			<tr>
				<td><input type="hidden" name="vldstatus50exec" id="vldstatus50exec" value="<?=$arrFases['vldstatus50exec']; ?>">
				<input type="hidden" name="vlddtinclusaost50exec" id="vlddtinclusaost50exec" value="<?=$arrFases['vlddtinclusaost50exec']; ?>">
				<input type="hidden" name="usucpf_50" id="usucpf_50" value="<?=$arrFases['usucpf_50']; ?>"></td>
				<td><input type="button" name="btnCancela50" id="btnCancela50" class="Cancelar" value="Cancelar"></td>
			</tr>	
		<?} else { ?>
			<tr>
				<td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
				<td>
					<input type="radio" name="vldstatus50exec" id="vldstatus50exec_s" <? echo $habilita50; ?> value="S" <?php echo $arrFases['vldstatus50exec'] == 'S' ? 'checked' : '' ?>>Sim
					<input type="radio" name="vldstatus50exec" id="vldstatus50exec_n" <? echo $habilita50; ?> value="N" <?php echo $arrFases['vldstatus50exec'] == 'N' ? 'checked' : '' ?>>Não
					<?php if($arrFases['vldstatus50exec'] == 'N'): ?>
						<script>
							jQuery(function(){
								jQuery('#vldobs50exec').parent().show();
							});
						</script>
					<?php else: ?>
						<script>
							jQuery(function(){
								jQuery('#vldobs50exec').val('').parent().hide();
							});
						</script>
					<?php endif; ?>						
					<?php 
					
					$var = 'vldobs50exec'; 
					$obrig = 'N'; 
					$habil = 'S'; 
					$label = ''; 
					$cols = 80; 
					$rows = 5; 
					$max = 5000; 
					$funcao = ''; 
					$acao = 0; 
					$txtdica = ''; 
					$tab = false; 
					$title = NULL; 
					$value = $arrFases['vldobs50exec'];
					
					echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value); 
					?>
				</td>
			</tr>
		<?} ?>
	<tr bgcolor="#C0C0C0">
		<td class="subtitulocentro" colspan="2">
		<? if( empty($habilitaHo) || empty($habilita25) || empty($habilita50) ){ ?>
			<input type="button" value="Salvar" onclick="salvarValidacao();">
		<?} else { ?>
			<input type="button" value="Salvar" disabled="disabled">
		<?} ?>
		</td>
	</tr>
</table>
</form>