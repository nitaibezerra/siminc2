<?php
if ( $_GET['acao'] == 'O' ){
	verificaSessao( 'obra' );
}else{
	verificaSessao( 'empreendimento' );
}

switch ( $_POST['req'] ){
	case 'salvar':
		$endid						= $_POST['endid'];
		
		$endereco 					= new Endereco( $endid );
		$dadosEnd 			 		= $_POST['endereco'];
		$dadosEnd['tpeid'] 			= TIPO_ENDERECO_OBJETO;
		$dadosEnd['endcep'] 		= str_replace(Array('.','-'),'',$_POST['endereco']['endcep']);
		$dadosEnd['medlatitude']	= $_POST['graulatitude'].".".$_POST['minlatitude'].".".$_POST['seglatitude'].".S";
		$dadosEnd['medlongitude']	= $_POST['graulongitude'].".".$_POST['minlongitude'].".".$_POST['seglongitude'].".W";
		$endid 						= $endereco->popularDadosObjeto( $dadosEnd )
											   ->salvar();

		$regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );
											   
		if ( $_GET['acao'] == 'O' ){
			$arDado['obrid'] = $_SESSION['obras2']['obrid'];
		}else{
			$arDado['empid'] = $_SESSION['obras2']['empid'];
		}
		$arDado['usucpf']			  = $_SESSION['usucpf'];
		$arDado['rgadscsimplificada'] = 'Alteração de endereço da obra';
		$arDado['rgadsccompleta'] 	  = $_POST['endereco']['justificativa'];
		$arDado['rgaautomatica'] 	  = 't';
		
		$regAtividade->popularDadosObjeto( $arDado )
				  	 ->salvar();
		$db->commit();				  		   
											   
		die('<script>
				var strUrl = window.opener.location.href;
				alert(\'Operação realizada com sucesso!\');
				window.opener.location = strUrl.replace(\'#\', \'\');
				window.close();
			 </script>');
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";

if ( $_GET['acao'] == 'O' ){
	$obrid 	  = $_SESSION['obras2']['obrid'];
	$obra 	  = new Obras( $obrid );
	$endid    = $obra->endid;
}else{
	$empid 	  		= $_SESSION['obras2']['empid'];
	$empreendimento = new Empreendimento( $empid );
	$endid    		= $empreendimento->endid;	
}

$endereco = new Endereco( $endid );

$habilitadoEnd = true;
$habilitaEnd   = 'S';
$classEnd      = 'CampoEstilo';
//if ( $endereco->endid || $habilitado == false ){
//	$habilitadoEnd = false;
//	$habilitaEnd   = 'N';
//	$classEnd      = 'disabled';
//}

?>

<html>
	<head>
		<title>Edição de Endereço</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>jQuery.noConflict();</script>
		<script src="/includes/prototype.js"></script>
		<script src="/includes/entidades.js"></script>
		<script src="/includes/calendario.js"></script>	
		
		<script type="text/javascript">
		jQuery(document).ready(function(){
			
			jQuery('[type="text"]').keyup();
			jQuery('.salvar').click(function(){
				var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
				var stop  	 = false;
				
				if (jQuery('#endcep').val() == ""){
					mensagem += 'CEP \n';
					stop = true;
				}
				
				if (jQuery('[name*=justificativa]').val() == ""){
					mensagem += 'Justificativa \n';
					stop = true;
				}
				
				if( stop ){
					alert( mensagem );
					return false;
				}
				jQuery('#formObra').submit();
			});
		});			
		</script>
		
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
<body marginheight="0" marginwidth="0">
<form method="post" name="formObra" id="formObra" enctype="multipart/form-data">
	<input type="hidden" name="req" id="req" value="salvar"/>	
	<input type="hidden" name="endid" id="endid" value="<?php echo $endid ?>"/>	
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">	
	<tr>
		<td colspan="3">Local da Obra</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
		<td colspan="2">
			<input type="text" name="endereco[endcep]" 
				   onkeyup="this.value=mascaraglobal('##.###-###', this.value);" 
				   onblur="Entidade.__getEnderecoPeloCEP(this);" class="<?php echo $classEnd; ?>" id="endcep" 
				   value="<?=$endereco->endcep ?>" size="13" maxlength="10" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?>/>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
		</td>
	</tr>
	<tr id="escolha_logradouro_id" style="display:none">
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
		<td colspan="2">
			<input type="text" name="endlog" class="<?php echo $classEnd; ?>" id="endlog" value="<?=$endereco->endlog ?>" size="48" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
        	<label id="lbLogadouro"> Logradouro: </label>
		</td>
        <td colspan="2">
        	<input type="text" name="endereco[endlog]" class="<?php echo $classEnd; ?>" id="endlogradouro" value="<?=$endereco->endlog ?>" size="65" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
      	</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
      	<td colspan="2">
  			<input type="text" name="endereco[endnum]" class="<?php echo $classEnd; ?>" id="endnum" value="<?=$endereco->endnum ?>" size="13" maxlength="8" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
        </td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
      	<td colspan="2">
        	<input type="text" name="endereco[endcom]" class="<?php echo $classEnd; ?>" id="endcom" value="<?=$endereco->endcom ?>" size="65" maxlength="100" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
		</td>
	</tr>
	<tr>
    	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
     	<td colspan="2">
    		<input type="text" name="endereco[endbai]" class="<?php echo $classEnd; ?>" id="endbai" value="<?=$endereco->endbai ?>" size="20" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
        </td>
	</tr>
    <tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
        <td colspan="2">
        	<input type="text" name="mundescricao"  class="<?php echo $classEnd; ?>" id="mundescricao" value="<?=$endereco->getMunDescricao()?>" size="20" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            <input type="hidden" name="endereco[muncod]" id="muncod" class="<?php echo $classEnd; ?>" value="<?=$endereco->muncod ?>" />
            <input type="text" name="endereco[estuf]" class="<?php echo $classEnd; ?>" id="estuf" value="<?=$endereco->estuf ?>" style="width: 5ex; padding-left: 2px" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
		</td>
	</tr>
    <tr>
		<td colspan="3">Coordenadas Geográficas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Latitude</td>
		<td colspan="2">
			<?php 
			$medlatitude = $endereco->medlatitude;
			$latitude = explode(".", $medlatitude);
			$graulatitude = $latitude[0];
			$minlatitude = $latitude[1];
			$seglatitude = $latitude[2];
			$pololatitude = $latitude[3];
			?> 
			<?= campo_texto( 'graulatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?> ° 
			<?= campo_texto( 'minlatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?> ' 
			<?= campo_texto( 'seglatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?> '' 
			<?php 
			if (trim($pololatitude) == "S") {
				echo "&nbsp;<span id=pololatitude_>S</span>"; 
			}elseif(trim($pololatitude) == "N") {
				echo "&nbsp;<span id=pololatitude_>N</span>";
			}else {
				echo "&nbsp;<span id=pololatitude_></span>";
			}
			?>
			<input type="hidden" name="pololatitude" id="pololatitude" value="<?=trim($pololatitude) ?>">
			<?php print obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude</td>
		<td colspan="2">
			<?php 
			$medlongitude = $endereco->medlongitude;
			$longitude = explode(".", $medlongitude);
			$graulongitude = $longitude[0];
			$minlongitude = $longitude[1];
			$seglongitude = $longitude[2];
			$pololongitude = $longitude[3];
			?> 
			<?= campo_texto( 'graulongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?> ° 
			<?= campo_texto( 'minlongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?> ' 
			<?= campo_texto( 'seglongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?> ''
			<?php 
			if (trim($pololongitude) == "W") {
				echo "&nbsp;<span id=pololatitude_>W</span>"; 
			}elseif(trim($pololongitude) == "E") {
				echo "&nbsp;<span id=pololatitude_>E</span>";
			}else {
				echo "&nbsp;<span id=pololatitude_></span>";
			}
			?>
			<input type="hidden" name="pololatitude" id="pololatitude" value="<?=trim($pololongitude) ?>">
			<?php print obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td colspan="2">
<?php 
		if ( $habilitadoEnd ):
?>
			<a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
<?php 
		endif;
?>				
				<input style="display: none;" 
					   type="text" 
					   name="endereco[endzoom]" 
					   id="endzoom"
				   	   value=<? if ( $endereco->endzoom == null ) echo "15"; else echo $endereco->endzoom;?>>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td>
			<?=campo_textarea('endereco[justificativa]', 'S', $habilitaEnd, '', 80, 5, '', '', '', '', '', '', '', array('id' => 'justificativa')) ?>
		</td>
	</tr>	
	<tr>
		<td style="background-color:#DCDCDC" colspan="3" align="center">
			<?php if( $habilitadoEnd ): ?>
				<input type="button" name="salvar" class="salvar" value="Salvar"/>
			<?php endif; ?>
		</td>
	</tr>	
</table>
</form>
</body>
</html>
