<?php

if ($_REQUEST['requisicao'] == 'salvar') {
	extract($_POST);
	
	$rescontrapartidapadrao = $rescontrapartidapadrao ? "'" .str_replace( ",", ".", $rescontrapartidapadrao ) . "'" : 'null';
	
	if( $_POST['resid'] ){	
		$sql = "UPDATE
				  emenda.responsavel  
				SET 
				  unicod = '$unicod',
				  resdsc = '$resdsc',
				  resassunto = '$resassunto',
				  rescontrapartidapadrao = $rescontrapartidapadrao
				WHERE 
				  resid = $resid";
		
		$db->executar( $sql );
	} else {
		$sql = "SELECT resid, resstatus FROM emenda.responsavel WHERE lower(resdsc) = lower('$resdsc')";
		$arResp = $db->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
		if($arResp){
			if($arResp['resstatus'] == 'I'){	
				echo "<script>
						alert('Impossível realizar o cadastro, porque já existe um responsavel no banco com status inativo');
					</script>";
				exit();
			}else{
				echo "<script>
						alert('Impossível realizar o cadastro, porque já existe um responsavel no banco com status ativo');
					</script>";
				exit();
			}
		} else {
	
			$sql = "INSERT INTO
					  emenda.responsavel(
					  unicod,
					  unitpocod,
					  resdsc,
					  resassunto,
					  resstatus,
					  rescontrapartidapadrao
					) 
					VALUES (
					  '$unicod',
					  'U',
					  '$resdsc',
					  '$resassunto',
					  'A',
					  $rescontrapartidapadrao
					) returning resid";
	
			$resid = $db->pegaUm( $sql );
		}
	}
	
	if( is_array($_REQUEST["reenalisemerito"]) ){
		foreach ($_REQUEST["reenalisemerito"] as $prsano => $reenalisemerito) {
			$arrPrsano = explode( '_', $prsano );
			$reeid  = $arrPrsano[0];
			$prsano = $arrPrsano[1];
			
			if(is_numeric($reeid)){
				$sql = "UPDATE emenda.responsavelexercicio SET 
						  resid = $resid,
						  reenalisemerito = '$reenalisemerito',
						  prsano = '$prsano'
						 
						WHERE 
						  reeid = $reeid";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO emenda.responsavelexercicio(resid, reenalisemerito, prsano) 
						VALUES ($resid, '$reenalisemerito', '$prsano')";
				$db->executar($sql);
			}
		}
	}
	$db->commit();
	$db->sucesso('principal/cadastraResponsavel');

	exit;
}
if ($_REQUEST['alteraResponsavel']) {
	header('content-type: text/html; charset=ISO-8859-1');
	alteraResponsavelAjax($_POST);
	exit;
}

if( $_REQUEST['resid'] || $_SESSION["emenda"]["resid"] ){
	
	$_SESSION["emenda"]["resid"] = !empty($_REQUEST['resid']) ? $_REQUEST['resid'] : $_SESSION["emenda"]["resid"];
	
	verificaResponsavel( $_SESSION["emenda"]["resid"] );
	
	$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"] );
	
	if($residPossiveis) {
		if ( count($residPossiveis) > 1 ){
			foreach( $residPossiveis as $chave=>$valor ){
				if ( $valor == $_SESSION["emenda"]["resid"]){
					$habSalvarRes = ' onclick="salvarResponsavel();"';
					$habCamposRes = 'S';
					break;
				}else{
					$habSalvarRes = 'disabled="disabled"';
					$habCamposRes = 'N';
				}
			}
		}else{
			$habSalvarRes = ($residPossiveis[0] == $_SESSION["emenda"]["resid"]) ? ' onclick="salvarResponsavel();"' : 'disabled="disabled"';
			$habCamposRes = ($residPossiveis[0] == $_SESSION["emenda"]["resid"]) ? 'S' : 'N';
		}
	} else {
		$habSalvarRes = ' onclick="salvarResponsavel();"';
		$habCamposRes = 'S';
	}
	
	$sql = "SELECT 
			  res.resid,
			  res.unicod,
			  res.unitpocod,
			  res.resdsc,
			  res.resassunto,
			  res.resstatus,
			  res.rescontrapartidapadrao
			FROM 
			  emenda.responsavel res
			WHERE res.resid = ".$_SESSION["emenda"]["resid"];

	$arResp = $db->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
	
	extract($arResp);
} else {
	$habSalvarRes = ' onclick="salvarResponsavel();"';
	$habCamposRes = 'S';
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);

monta_titulo( 'Cadastro de Responsável', "" );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$retorno = '';
} else {
	$retorno = 'disabled="disabled"';
}

?>
<html>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>

<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" id="resid" name="resid" value="<?=$resid ?>">
<input type="hidden" id="reeid" name="reeid" value="<?=$reeid ?>">
<input type="hidden" id="requisicao" name="requisicao" value="">
 
<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita"">Unidade:</td>
		<?
		$sql = "SELECT 
				  unicod as codigo,
				  unidsc as descricao
				FROM 
				  public.unidade
				WHERE 
				  unitpocod = 'U'
				  AND unicod like '26%'
				  AND unistatus = 'A'
				ORDER BY
					unidsc";
		?>
		<td><?= $db->monta_combo("unicod",$sql, $habCamposRes,'-- Selecione --','', '', '','','S','unicod','','','Unidade');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Responsável:</td>
		<td><?=campo_texto( 'resdsc', 'S', $habCamposRes, 'Responsável', 40, 100, '', '','','','','id="resdsc"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Tema:</td>
		<td><?=campo_texto( 'resassunto', 'S', $habCamposRes, 'Tema', 40, 100, '', '','','','','id="resassunto"'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Contrapartida mínima padrão</td>
		<td>
			<?php $rescontrapartidapadrao = str_replace(".", ",", $rescontrapartidapadrao) ?>
			<?=campo_texto( 'rescontrapartidapadrao', 'N', $habCamposRes, '', 7, 6, '###,##', '','','','','id="rescontrapartidapadrao"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value)"); ?>
			%
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Realizará a Análise de Mérito?</td>
		<td>
			<?
				if($resid){
					$sql = "SELECT
								pre.prsano,
							    ree.reenalisemerito,
							    ree.reeid
							FROM emenda.programacaoexercicio pre
								left join emenda.responsavelexercicio ree on ree.prsano = pre.prsano and ree.resid = $resid
							WHERE 
								pre.prsstatus = 'A' 
							order by pre.prsano";
				} else {
					$sql = "SELECT
								pre.prsano,
							    '' as reenalisemerito,
							    '' as reeid
							FROM emenda.programacaoexercicio pre
							WHERE 
								pre.prsstatus = 'A' 
							order by pre.prsano";
				}
				$arrAno = $db->carregar($sql);
				$arrAno = $arrAno ? $arrAno : array();
				
				echo '<table class="tabela" style="width: 30%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="left">
				<tr>
					<th style="width: 10%">Exercicio</th>
					<th>Ano</th>
				</tr>';
				foreach ($arrAno as $key => $valor) {
					$cor = ($key % 2) ? "#e0e0e0": "#f4f4f4";
					$checkNao = (empty($valor['reenalisemerito']) || $valor['reenalisemerito'] == 'N' ? 'checked="checked"' : '');
					$checkSim = ($valor['reenalisemerito'] == 'S' ? 'checked="checked"' : '');
					
					echo '<tr bgcolor='.$cor.'>
								<td>'.$valor['prsano'].'</td>
								<td><input type="radio" name="reenalisemerito['.($valor['reeid'] ? $valor['reeid'] : 'null').'_'.$valor['prsano'].']" '.$checkSim.' id="reenalisemeritosim" value="S">Sim
									<input type="radio" name="reenalisemerito['.($valor['reeid'] ? $valor['reeid'] : 'null').'_'.$valor['prsano'].']" '.$checkNao.' id="reenalisemeritonao" value="N">Não</td>
							</tr>';
				}
				echo '</table>';
			?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#C0C0C0" colspan="2" style="text-align: center">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" <?php echo $habSalvarRes; ?> <?=$retorno;?> />
			<input type="button" value="Voltar" name="btnCancelar" id="btnCancelar" onclick="history.back(-1);">
		</td>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
<script type="text/javascript">

function pesquisaResponsavel(){	
	$('loader-container').show();
	var myajax = new Ajax.Request('emenda.php?modulo=principal/listaResponsavel&acao=A', {
		        method:     'post',
		        parameters: '&pesquisaAjax=true&'+formulario.serialize(),
		        asynchronous: false,
		        onComplete: function (res){
					$('lista').innerHTML = res.responseText;
		        }
		  });
	$('loader-container').hide();
}

function salvarResponsavel(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "unicod";
	campos[1] 			= "resdsc";
	campos[2] 			= "restema";
					 
	tiposDeCampos[0] 	= "select";
	tiposDeCampos[1] 	= "texto";
	tiposDeCampos[2] 	= "texto";
	
	if( validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		
		//$('loader-container').show();
		
		/*selectAllOptions( document.getElementById( 'prsano' ) );		
		var ano = prsano.options.length;	
		if(ano == 0) {
			alert("O campo 'Ano' deve ser informado.");
			btnSalvar.disabled = false;
			btnVoltar.disabled = false;
			prsano.focus();
			return false;
		}*/
		$('requisicao').value = 'salvar';
		$('formulario').submit();
		/*if( $F('resid') != "" ){
			var pars = '&alteraResponsavel=true&'+$('formulario').serialize();
		} else {
			var pars = '&insereResponsavel=true&'+$('formulario').serialize();
		}
		
		$('loader-container').show();
	
		var myAjax = new Ajax.Request('emenda.php?modulo=principal/cadastraResponsavel&acao=A', {
					        method:     'post',
					        parameters: pars,
					        onComplete: function (res){	
								if(Number(res.responseText) ){
									alert('Operação realizada com sucesso!');
									cancelarResponsavel(); 
								}else{
									if(res.responseText == "I"){
										alert("Impossível realizar o cadastro, porque já existe um responsavel no banco com status inativo");
									} else if(res.responseText == "A"){
										alert("Impossível realizar o cadastro, porque já existe um responsavel no banco com status ativo");										
									} else {
										$('erro').innerHTML = res.responseText; 
										alert('Operação não realizada!');
									}
								}
					        }
					  });
		$('loader-container').hide();*/
	}
}

function cancelarResponsavel(){
	window.location.href = 'emenda.php?modulo=principal/listaResponsavel&acao=A';
}
</script>
</html>