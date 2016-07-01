<?php

// empreendimento || obra || orgao
//verificaSessao( 'obra' );

$preid = $_SESSION['obras2']['preid'];
$obrid = $_SESSION['obras2']['obrid'];

switch ( $_POST['op'] ){
	case 'gravarEmpedimento':
		
		$impedimentoterreno = $db->pegaLinha("SELECT * FROM obras2.impedimentoterreno WHERE obrid='".$obrid."'");
		
		if($impedimentoterreno['iteid']) {
			
			$sql = "UPDATE obras2.impedimentoterreno
   					SET iteimpedimento=".$_POST['iteimpedimento'].", usucpf='".$_SESSION['usucpf']."', itedataatualizacao=NOW(), iteobs=".(($_POST['iteobs'])?"'".$_POST['iteobs']."'":"NULL")." 
					WHERE iteid='".$impedimentoterreno['iteid']."'";
			
			$db->executar($sql);
			
			$db->executar("DELETE FROM obras2.impedimentoresposta WHERE iteid='".$impedimentoterreno['iteid']."'");
			
			if($_POST['tpiid']) {
				foreach($_POST['tpiid'] as $tpiid) {
					$db->executar("INSERT INTO obras2.impedimentoresposta(iteid, tpiid) VALUES ('".$impedimentoterreno['iteid']."','".$tpiid."');");
				}
			}
			
		} else {
			
			$sql = "INSERT INTO obras2.impedimentoterreno(
            		iteimpedimento, usucpf, itedataatualizacao, iteobs, obrid)
    				VALUES (".$_POST['iteimpedimento'].", '".$_SESSION['usucpf']."', NOW(), ".(($_POST['iteobs'])?"'".$_POST['iteobs']."'":"NULL").", '".$obrid."') RETURNING iteid;";
			$iteid = $db->pegaUm($sql);
			
			if($_POST['tpiid']) {
				foreach($_POST['tpiid'] as $tpiid) {
					$db->executar("INSERT INTO obras2.impedimentoresposta(iteid, tpiid) VALUES ('".$iteid."','".$tpiid."');");
				}
			}
		}
		
		$db->commit();
		
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href = '?modulo=principal/analiseTerrenoObjeto&acao=A';
			 </script>");
	
	case 'alterar':
		$_SESSION['obras2']['smiid'] = $_POST['smiid'];
		header('Location: ?modulo=principal/cadSupervisaoMI&acao=E');
		die;
	case 'apagar':
		$supervisaoMi = new SupervisaoMi( $_POST['smiid'] );
		$supervisaoMi->smistatus = 'I';
		$supervisaoMi->salvar();
		
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href = '?modulo=principal/listaSupervisaoMI&acao=A';
			 </script>");
}

//Chamada de programa
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
echo cabecalhoObra($obrid);
monta_titulo( 'Análise de Terreno do Objeto', '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script>

function abreMapaEntidade(tipoendereco){

	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;

	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}

</script>
<?


include_once APPRAIZ . "par/classes/modelo/PreObra.class.inc";
include_once APPRAIZ . "par/classes/controle/SubacaoControle.class.inc";


$obSubacaoControle = new SubacaoControle();
	
$arDados = $obSubacaoControle->recuperarPreObra($preid);
$arDados = ($arDados) ? $arDados : array();
	
$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
	
?>
<body>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="subtitulodireita">Nome do terreno:</td>
	<td>
	<?php			
	$predescricao = $arDados['predescricao'];
	echo campo_texto( "predescricao", 'S', $boAtivo, '', 40, '', '', '','','','','id="predescricao"','',$predescricao);
	?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita">Tipo da Obra:</td>
	<td>
	<?php
	$sql = "SELECT ptoid as codigo, ptodescricao as descricao FROM obras.pretipoobra WHERE ptocategoria IS NOT NULL";
	$arTipoObra = $db->carregar($sql);

	$arTipoObra = $arTipo ? $arTipo : $arTipoObra;
	$ptoid = $arDados['ptoid'];
	$db->monta_combo( "ptoid", $arTipoObra, $boAtivo, 'Selecione...', '', '', '', '', 'S', 'ptoid',false,$ptoid,'Tipo da Obra');
	?> 
	</td>
</tr>
<tr>
	<td class="subtitulodireita">Unidade de Medida:</td>
	<td>Unidade Escolar</td>
</tr>
<?php
	$endereco = new Endereco();
	$entidade->enderecos[0] = $endereco;
?>
	<tr>
		<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
			<label>CEP:</label>
		</td>
		<td>
			<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);" class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10" <?php echo $stAtivo ?> /> 
			<!--<img src="../imagens/obrig.gif" />-->
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Logradouro:</label>
		</td>
		<td>
			<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1" value="<?php echo $arDados['prelogradouro']; ?>" size="48" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Número:</label>
		</td>
		<td>
			<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1" value="<?php echo $arDados['prenumero']; ?>" size="6" maxlength="4" onkeypress="return somenteNumeros(event);" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Complemento:</label>
		</td>
		<td>
			<input type="text" name="endcom" class="CampoEstilo" id="endcom1" value="<?php echo $arDados['precomplemento']; ?>" size="48" maxlength="100" <?php echo $stAtivo ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Bairro:</label>
		</td>
		<td>
			<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1" value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>

	<tr id="tr_estado">
		<td class="subtitulodireita">Estado:</td>
		<td>
			<?php
				$estuf = $arDados['estuf'];
		
				if($_SESSION['par']['estuf']){
					$where = " where e.estuf = '{$_SESSION['par']['estuf']}' ";
				}
		
				$sql = "
					Select	e.estuf as codigo, 
							e.estdescricao as descricao 
					from territorios.estado e
					$where
					order by  e.estdescricao asc
				";
				$db->monta_combo( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
			?>
		</td>
	</tr>
	<tr id="tr_municipio">
		<td class="subtitulodireita">Município:<br/></td>
		<td id="municipio">
			<?php
				if ($arDados['estuf']) {
					$sql = "select
								 muncod as codigo, 
								 mundescricao as descricao 
								from
								 territorios.municipio
								where
								 estuf = '".$arDados['estuf']."' 
								order by
								 mundescricao asc";
					$muncod_ = $arDados['muncod'];
					$db->monta_combo( "muncod_", $sql, $boAtivo, 'Selecione o Estado', '', '', '','','S', 'muncod_',false,$muncod_,'Município');
				} else {
					$db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município');
				}
			?>
		</td>
	</tr>
	<script> document.getElementById('endcep1').value = mascaraglobal('##.###-###', document.getElementById('endcep1').value);</script>
	<?php
		$latitude = explode('.',$arDados['prelatitude']);
		$longitude = explode('.',$arDados['prelongitude']);
	?>
	<tr>
		<td class="SubTituloDireita">Latitude :</td>
		<td>
			<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<?php echo $latitude[0]; ?>" class="normal" type="hidden"> 
				<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º 
			<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<?php echo $latitude[1]; ?>" class="normal" type="hidden"> 
				<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
			<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<?php echo $latitude[2]; ?>" class="normal" type="hidden"> 
				<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
			<input name="latitude[]" id="pololatitude1" value="<?php echo $latitude[3]; ?>" type="hidden">
				<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude :</td>
		<td>
			<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<?php echo $longitude[0]; ?>" type="hidden"> 
				<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º 
			<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<?php echo $longitude[1]; ?>" type="hidden"> 
				<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
			<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<?php echo $longitude[2]; ?>" type="hidden"> 
				<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
			<input name="longitude[]" id="pololongitude1" value="<?php echo $longitude[3]; ?>" type="hidden"> 
				<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
			<input type="hidden" name="endzoom" id="endzoom" value="<?php echo $obCoendereCoentrega->endzoom; ?>" />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td>
			<a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a> 
			<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
		</td>
	</tr>

</table>
				
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">

		<tr>
			<td colspan="2" class="SubTituloCentro">
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>

				<?php
				$sql = "SELECT 
							arqnome, arq.arqid, 
							arq.arqextensao, arq.arqtipo, 
							arq.arqdescricao,							
						 	to_char(arq.arqdata, 'DD/MM/YYYY') as data, 
						 	arc.arqvalidacao
							{$excluir} 
						FROM 
							public.arquivo arq 
						LEFT JOIN 
							public.arquivo_recuperado arc ON arc.arqid = arq.arqid 
						INNER JOIN 
							obras.preobrafotos pof ON arq.arqid = pof.arqid
						INNER JOIN 
							obras.preobra pre ON pre.preid = pof.preid
						--INNER JOIN 
							--seguranca.usuario seg ON seg.usucpf = oar.usucpf 
						WHERE							
							pre.preid = {$preid} 
						AND							
							(substring(arqtipo,1,5) = 'image') 
						ORDER BY 
							arq.arqid";
						//LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
				$fotos = ($db->carregar($sql));				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){
					$_SESSION['imgparams'] = array("filtro" => "cnt.preid={$preid}", 
												   "tabela" => "obras.preobrafotos");

					for( $k=0; $k < count($fotos); $k++ ){
						
						echo "<div style=\"{$alerta}float:left; width:90px; height:140px; text-align:center; margin:2px;\" >
								<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=obras' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=obras&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
								" . $fotos[$k]["data"] . " <br/>
								" . $fotos[$k]["acao"] . "
							  </div>";
						
					}
					
				}else {
					echo "Não existem fotos cadastradas";
				}
				?>
			</td>
		</tr>					
</table>

<script>
function identificouEmpedimento(vl) {
	if(vl=='TRUE') {
		jQuery('#tr_quais').css('display','');
		jQuery('#tr_comentarios').css('display','');
	} else {
		jQuery('#tr_quais').css('display','none');
		jQuery('#tr_comentarios').css('display','none');
		jQuery("[name^='tpiid[]']").attr('checked',false);
		jQuery('#iteobs').val('');
	}
}

function gravarEmpedimento() {

	if(jQuery("[name^='iteimpedimento']:enabled:checked").length==0) {
		alert('Marque se a empresa identificou algum empedimento para execução do objeto contratado decorrente do terreno apresentado');
		return false;
	}
	
	if(jQuery("[name^='iteimpedimento'][value=TRUE]:enabled:checked").length==1) {
	
		if(jQuery("[name^='tpiid[]']:enabled:checked").length==0) {
			alert('Marque pelo menos 1 empedimento.');
			return false;
		}
		
		if(jQuery("#iteobs").val()=='') {
			alert('Preencha Comentários');
			return false;
		}
	
	}
	
	jQuery('#formulario').submit();
}
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2" class="SubTituloEsquerda">Viabilidade do Terreno</td>
</tr>

<tr>
	<td colspan="2">
	<?php $impedimentoterreno = $db->pegaLinha("SELECT * FROM obras2.impedimentoterreno WHERE obrid='".$obrid."'"); ?>
	<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="op" value="gravarEmpedimento">
	<fieldset>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td>A empresa identificou algum empedimento para execução do objeto contratado decorrente do terreno apresentado?</td>
		<td><input type="radio" name="iteimpedimento" value="TRUE" <?=(($impedimentoterreno['iteimpedimento']=='t')?"checked":"") ?> onclick="identificouEmpedimento(this.value);"> Sim <input type="radio" name="iteimpedimento" value="FALSE" <?=(($impedimentoterreno['iteimpedimento']=='f')?"checked":"") ?> onclick="identificouEmpedimento(this.value);"> Não</td>
	</tr>
	<tr id="tr_quais" <?=(($impedimentoterreno['iteimpedimento']=='t')?"":"style=\"display:none;\"") ?>>
		<td colspan="2">
		<?
		$impedimentoresposta = array(); 
		if($impedimentoterreno['iteid']) {
			$impedimentoresposta = $db->carregarColuna("SELECT tpiid FROM obras2.impedimentoresposta WHERE iteid='".$impedimentoterreno['iteid']."'");
		} 
		?>
		<?php $tipoimpedimento = $db->carregar("SELECT * FROM obras2.tipoimpedimento WHERE tpistatus='A'"); ?>
		<?php if(is_array($tipoimpedimento) && $tipoimpedimento[0]) : ?>
		<b>Quais?</b><br/><br/>
		<?php foreach($tipoimpedimento as $ti) : ?>
		<input type="checkbox" name="tpiid[]" value="<?=$ti['tpiid'] ?>" <?=((in_array($ti['tpiid'], $impedimentoresposta))?"checked":"") ?> > <?=$ti['tpidescricao'] ?><br/>		
		<?php endforeach; ?>
		<?php endif; ?>
		</td>
	</tr>
	<tr id="tr_comentarios" <?=(($impedimentoterreno['iteimpedimento']=='t')?"":"style=\"display:none;\"") ?>>
		<td colspan="2"><b>Comentários:</b><br/><br/>
		<?php $iteobs = $impedimentoterreno['iteobs']; echo campo_textarea( 'iteobs', 'S', 'S', '', '70', '4', '1000'); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro"><input type="button" value="Salvar" onclick="gravarEmpedimento();"></td>
	</tr>
	</table>
	</fieldset>
	</form>
	</td>
</tr>
</table>