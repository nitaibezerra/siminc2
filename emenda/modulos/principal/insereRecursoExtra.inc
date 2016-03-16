<?php

if($_REQUEST['montaFuncionaProgramatica']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaComboFuncionalProgramaticaOrçamento($_POST['resid']);
	exit;
}

if($_POST['acao'] == 'insere'){
	
	$sql = "SELECT 
			  coalesce(max(emecod::integer),0) + 1 as maximo
			FROM 
			  emenda.emenda
			WHERE
			  emetipo = 'X'";
	
	$emecod = $db->pegaUm($sql);	
	
	if(strlen($emecod) > 1){
		$emecod = $emecod;
	} else {
		$emecod = '999000'.$emecod;
	}
	
	$emeid = insereRecursoExtra($_POST, $emecod);
	$retorno = insereRecursoDetalhe($_POST, $emeid);
	
	if($retorno == 1){
		echo "<script>
			  	  alert('Operação realizada com sucesso!');
			  	  //window.location.href = 'emenda.php?modulo=principal/listaDeExtraOrcamento&acao=A';
			  </script>";
	}
}

if($_POST['acao'] == 'alterar'){
	
	$emeid = insereRecursoExtra($_POST, $emecod);
	$retorno = insereRecursoDetalhe($_POST, $emeid);
	
	if($retorno == 1){
		echo "<script>
			  	  alert('Operação realizada com sucesso!');
			  	  //window.location.href = 'emenda.php?modulo=principal/listaDeExtraOrcamento&acao=A';
			  </script>";
	}
}

function insereRecursoExtra($post, $codigo){
	global $db;
	extract($post);
	
	if($emeid){
		$sql = "UPDATE 
				  emenda.emenda  
				SET 
				  emecod = $emecod,
				  ".($resid ? 'autid = (SELECT autid FROM emenda.responsavel WHERE resid = '.$resid.'),': '')."
				  acaid = ".($acaid ? $acaid : 'NULL').",
				  ".($resid ? 'resid = '.$resid.',': '')."
				  emetipo = 'X',
				  emeano = $emeano
				 
				WHERE 
				  emeid = $emeid";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO 
				  emenda.emenda(
				  emecod,
				  autid,
				  acaid,
				  resid,
				  emetipo,
				  emeano
				) 
				VALUES (
				  '$codigo',
				  (SELECT autid FROM emenda.responsavel WHERE resid = $resid),
				  ".($acaid ? $acaid : 'NULL').",
				  $resid,
				  'X',
				  $emeano
				)RETURNING emeid";

		return $db->pegaUm($sql);
	}
}

function permitirAlteracaoGNDResponsavel($emeid){
	global $db;
	
	$sql = "SELECT 
				1
			FROM 
			    emenda.emenda eme
			    inner join emenda.emendadetalhe emd
			        on emd.emeid = eme.emeid
			    inner join emenda.iniciativaemendadetalhe ied
			        on ied.emdid = emd.emdid
			WHERE 
			    ied.iedstatus = 'A'
			    and eme.emeid = $emeid";
	$retorno = $db->pegaUm($sql);
	
	//caso exista alguma iniciativa vinculada ao recurso não será permitida alteração do GND e do responsavel.
	if($retorno){
		return ('false');
	} else {
		return ('true');
	}			  
}

function insereRecursoDetalhe($post, $codigo){
	global $db;
	extract($post);
	
	if($emeid){
		$sql = "UPDATE 
				  emenda.emendadetalhe  
				SET 
				  emeid = $emeid,
				  ".($gndcod ? 'gndcod = '.$gndcod.',' : '')."
				  foncod = ".($foncod ? $foncod : 'NULL').",
				  mapcod = $mapcod,
				  emdvalor = ".retiraPontos($emdvalor).",
				  emdtipo = 'R'
				 
				WHERE 
				  emdid = $emdid";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO 
				  emenda.emendadetalhe(
				  emeid,
				  gndcod,
				  foncod,
				  mapcod,
				  emdvalor,
				  emdtipo
				) 
				VALUES (
				  $codigo,
				  $gndcod,
				  ".($foncod ? $foncod : 'NULL').",
				  $mapcod,
				  ".retiraPontos($emdvalor).",
				  'R'
				)";
	
		$db->executar($sql);
	}
	return $db->commit();
}

$emeid = ($_POST['emeid'] ? $_POST['emeid'] : $emeid);

if( $emeid ){
		
	$sqlEmenda = "SELECT 
					  emeid,
					  emecod,
					  autid,
					  acaid,
					  resid,
					  emetipo,
					  emeano
					FROM 
					  emenda.emenda
					WHERE
					  emeid = {$emeid}";
					  
	$arEmenda = $db->pegaLinha($sqlEmenda);
	
	$sqlEmendaD = "SELECT 
					  emdid,
					  emeid,
					  gndcod,
					  foncod,
					  mapcod,
					  emdvalor
					FROM 
					  emenda.emendadetalhe
					WHERE
					  emeid = {$emeid}";
					  
	$arEmendaD = $db->pegaLinha($sqlEmendaD);
	
	extract($arEmenda);
	extract($arEmendaD);
	
	$permitirAlterarGNDResponsavel = permitirAlteracaoGNDResponsavel($emeid);

	if( $acaid ){
		$sql = "SELECT 
				  fupfuncionalprogramatica ||' - '|| fupdsc as descricao
				FROM 
				  emenda.v_funcionalprogramatica
				WHERE
					acastatus = 'A'
				    --AND prgano = '{$_SESSION['exercicio']}'
				    AND acaid = $acaid";
		
		$acadsc = $db->pegaUm($sql);
		$acaid = array("codigo" => $acaid, "descricao" => $acadsc);
	}
	
	$boPTA = verificaExisteEmendaDetalhePTA($emeid);
	
	$emdvalor = number_format($emdvalor, 2, ',','.');
	$acao = 'alterar';
}else{
	$acao = 'insere';
}

if($permitirAlterarGNDResponsavel == 'true'){
	$desabilita = "";
	$msgGND = "";
	$msgResponsavel = "";
} else if($permitirAlterarGNDResponsavel == 'false'){
	$desabilita = "disabled=\"disabled\"";
	$msgGND = "<span style='color: red;padding-left:5px;'>Este recurso já foi vinculado a uma iniciativa com esse GND!</span>";
	$msgResponsavel = "<span style='color: red;padding-left:5px;'>Este recurso já foi vinculado a uma iniciativa com esse Responsável!</span>";
}
require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
							
 monta_titulo( $titulo_modulo, ''); 

?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<body>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="acao" id="acao" value="<?=$acao; ?>">
	<input type="hidden" name="emeid" id="emeid" value="<?=$emeid ?>">
	<input type="hidden" name="emdid" id="emdid" value="<?=$emdid ?>">
	<input type="hidden" name="emecod" id="emecod" value="<?=$emecod ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="subtitulodireita"><b>Responsável:</b></td>
		<td><?=comboResponsavel($_SESSION['usucpf'], $resid, 'carregaFuncProgramatica(this.value)', $desabilita, 'S' ); echo $msgResponsavel;?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Funcional Programática:</b></td>
		<td id="funcProgramatica"><?=montaComboFuncionalProgramaticaOrçamento($resid,''); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>GND:</b></td>
		<?
		$sql = "SELECT 
				  gndcod as codigo,
				  gndcod||' - '||gnddsc as descricao						  
				FROM 
				  public.gnd";
		?>
		<td><?=$db->monta_combo("gndcod", $sql, ($boPTA || $desabilita ? "N" : "S"), "Selecione..", '', '', '', '', ($desabilita ? "N" : "S"),'gndcod', '', '', 'GND'); echo $msgGND?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Fonte:</b></td>
		<?
		$sql = "SELECT
				  foncod as codigo,
				  foncod||' - '||fondsc as descricao
				FROM 
				  public.fonterecurso
				WHERE
				  fonstatus = 'A'
				ORDER BY
				  fondsc, foncod";
		?>
		<td><?=$db->monta_combo("foncod", $sql, "S" , "Selecione..", '', '', '', '', 'N','foncod', '', '', 'Fonte'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Modalidade de Aplicação:</b></td>
		<?
		$sql = "SELECT 
				  mapcod as codigo,
				  mapcod||' - '||mapdsc as descricao
				FROM 
				  public.modalidadeaplicacao
				ORDER BY
				  mapcod";
		?>
		<td><?=$db->monta_combo("mapcod", $sql, ($boPTA ? "N" : "S") , "Selecione..", '', '', '', '', 'S','mapcod', '', '', 'MOD'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Valor:</b></td>
		<td><?=campo_texto( 'emdvalor', 'S', 'S', 'Valor', 30, 13, '[###.]###,##', '', 'left', '', 0, 'onchange=\'verificaValorAcima(this)\'','','', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Ano do Orçamento:</b></td>
		<td><?=campo_texto( 'emeano', 'S', 'S', 'Ano', 10, 4, '[#]', '', 'left', '', 0, '','','', "this.value=mascaraglobal('[#]',this.value);"); ?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<?
			if( $boPTA ){
				//echo "<span style='color: red;'>Este recurso já está sendo utilizado em um plano de trabalho.</span>";
				$retorno = 'disabled="disabled"';
			}
			?>
			<input type="button" value="Salvar" onclick="salvarRecurso();" style="cursor: pointer;"/>
			<input type="button" value="Cancelar" onclick="voltarRecurso();" style="cursor: pointer;"/>
		</td>
	</tr>
	<?if($emeid){ ?>
	<tr>
		<td><a href="emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A&emdid=<?=$emdid; ?>"><img src="/imagens/alterar.gif" border=0 title="Distribuir Recurso" style="cursor:pointer;" onclick="alterarRecursos();"> Distribuir Recurso</a></td>
	</tr>
	<?} ?>
	</table>
</form>
</body>
<script type="text/javascript">

	function salvarRecurso(){
		var nomeform 		= 'formulario';
		var submeterForm 	= false;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "resid";
		campos[1] 			= "emecod";
		campos[2] 			= "autid";
		campos[3] 			= "acaid";
		campos[4] 			= "gndcod";
		campos[5] 			= "mapcod";
		campos[6] 			= "emdvalor";
		campos[7] 			= "emeano";
						 
		tiposDeCampos[0] 	= "select";
		tiposDeCampos[1] 	= "texto";
		tiposDeCampos[2] 	= "select";
		tiposDeCampos[3] 	= "select";
		tiposDeCampos[4] 	= "select";
		tiposDeCampos[5] 	= "select";
		tiposDeCampos[6] 	= "texto";
		tiposDeCampos[7] 	= "texto";
		
		if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
			//document.getElementById('acao').value = 'insere';
			document.getElementById('formulario').submit();			
		}
	}
	
	function carregaFuncProgramatica(resid){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/insereRecursoExtra&acao=A', {
			        method:     'post',
			        parameters: '&montaFuncionaProgramatica=true&resid='+resid,
			        asynchronous: false,
			        onComplete: function (res){
						$('funcProgramatica').innerHTML = res.responseText;
			        }
			  });
	}
	
	function voltarRecurso(){
		window.location.href = 'emenda.php?modulo=principal/listaDeExtraOrcamento&acao=A';
	}
	
	function verificaValorAcima(obj){
		<?php if($arEmendaD['emdvalor'] && $boPTA){ ?>
			var emdvalor = '<?=$arEmendaD['emdvalor']?>';
			var valor = obj.value;
			for(i=0; i< valor.length; i++){
			 	if(valor[i] == "."){
					valor = valor.replace(".","");
				}
			}
			valor = valor.replace(",",".");
			
			valorAntigo = new Number(emdvalor);
			valorNovo = new Number(valor);
			
			if(valorAntigo > valorNovo){
				alert('O novo valor deve ser maior do que o valor informado anteriormente!');
				obj.value = emdvalor;
				obj.value = mascaraglobal('[###.]###,##',obj.value);
				return false;
			}else{
				return true;
			}
		<?php } ?>
	}
</script>
</html>