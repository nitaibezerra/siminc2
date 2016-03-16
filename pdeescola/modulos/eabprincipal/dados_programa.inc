<?php

/*** Verifica se as variáveis de sessão do módulo estão corretamente setadas ***/
eabVerificaSessao();

// AJAX
if($_POST['ajaxEatid'])
{
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
				eaoid as codigo,
				eaodescricao as descricao
			FROM
				pdeescola.eabtipooficina
			WHERE
				eaostatus = 'A'
				AND eatid = ".$_POST["ajaxEatid"]."
			ORDER BY
				eaodescricao ASC";
	die($db->monta_combo("eaoid", $sql, 'S', "Selecione uma Atividade / Oficina", 'alternarExibicaoOficina', '', '', '400', 'S', '',false,null));
}

if( $_REQUEST["submetido"] )
{
	$sql = "DELETE FROM pdeescola.eabatividade WHERE eabid = ".$_SESSION["eabid"];
	$db->executar($sql);
	
	for($i=0; $i<count($_REQUEST['lista_eatid']); $i++)
	{
		$eaeid = $_REQUEST["lista_eaeid"][$i];
		if($_REQUEST["lista_eaetipo"][$i] == "E")
		{
			$eaaoutroespacoescola = ($_REQUEST["lista_eaeid_outros"][$i] == "") ? 'null' : $_REQUEST["lista_eaeid_outros"][$i];
			$eaaoutroespacocomunidade = 'null'; 
		}
		else
		{
			$eaaoutroespacocomunidade = ($_REQUEST["lista_eaeid_outros"][$i] == "") ? 'null' : $_REQUEST["lista_eaeid_outros"][$i];
			$eaaoutroespacoescola = 'null';
		}
		
		$eaaoutraoficina = ( $_REQUEST["lista_eaoid_outros"][$i] == "") ? 'null' : $_REQUEST["lista_eaoid_outros"][$i];
				
		$sql = "INSERT INTO 
					pdeescola.eabatividade(  eabid,
											 eatid,
										     eaoid,
											 eaaturno,
											 edrid,
											 eadid,
											 eaeid,
											 eaaqtdparticipante,
											 eaaoutraoficina,
											 eaaoutroespacoescola,
											 eaaoutroespacocomunidade,
											 eabressarcida)
						VALUES(".$_SESSION["eabid"].",
						   ".$_REQUEST["lista_eatid"][$i].",
						   ".$_REQUEST["lista_eaoid"][$i].",
						   '".$_REQUEST["lista_eaaturno"][$i]."',
						   ".$_REQUEST["lista_edrid"][$i].",
						   ".$_REQUEST["lista_eadid"][$i].",
						   ".$eaeid.",
						   ".$_REQUEST["lista_eaaqtdparticipante"][$i].",
						   '".$eaaoutraoficina."',
						   '".$eaaoutroespacoescola."',
						   '".$eaaoutroespacocomunidade."',
						   '".$_REQUEST['lista_eabressarcida'][$i]."'
						   )";
		$db->executar($sql);
	}
	
	$db->commit();
	echo "<script>
			alert('Dados gravados com sucesso.');
		  </script>";
}

if( $_REQUEST["submetidoea"] )
{
//	$db->executar("DELETE FROM pdeescola.eabescolaanoanterior WHERE eabid = ".$_SESSION["eabid"]);
	if( $_REQUEST["eaaid"] ) {
		
		for($i=0; $i<count($_REQUEST["eaaid"]); $i++) {
			
			$db->executar("INSERT INTO pdeescola.eabescolaanoanterior(eabid,eaaid) VALUES(".$_SESSION["eabid"].",".$_REQUEST["eaaid"][$i].")");
		}
	}
	$eabanoatual 	= ($_REQUEST["eabanoatual"]) ? 't' : 'f';
	$edrid 			= ($_REQUEST["dia"]=="3") ? 3 : $_REQUEST["edrid"];
	$eabqtduexatende= ($_REQUEST["opcao"]=="nao") ? 'null' : (($_REQUEST["eabqtduexatende"]) ? $_REQUEST["eabqtduexatende"] : 'null');
	
	$sql = "UPDATE pdeescola.eabescolaaberta 
				   SET eabanoatual = '".$eabanoatual."',
				   	   edrid = ".$edrid.",
				   	   eabqtduexatende = ".$eabqtduexatende."
				   WHERE eabid = ".$_SESSION["eabid"];
	
	$db->executar($sql);
	if( !empty($_REQUEST["justificativa"]) ){
		
		$sql = "DELETE FROM pdeescola.eabjustificativa WHERE eabid = ".$_SESSION["eabid"];
		
		$db->executar($sql);
	
		$justificativadesc = $_REQUEST["justificativa"] == 4 ? $_REQUEST["justificativadesc"] : "";
	
		$sql = "INSERT INTO pdeescola.eabjustificativa(
	           			eabid, etjid, eajoutrajustificativa)
	    		 VALUES 
	    		 		( {$_SESSION["eabid"]}, {$_REQUEST["justificativa"]}, '{$justificativadesc}')";	
		
		$db->executar($sql);
	}

	$db->commit();
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.location.href = 'pdeescola.php?modulo=eabprincipal/dados_programa&acao=A'; 
		  </script>";
	die;
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

// Planos de atividade de anos anteriores não poderão ser alterados.
if($anosAnteriores){
	$anosAnteriores = ($somenteConsulta = true);
}

$sql = "SELECT docid FROM pdeescola.eabescolaaberta WHERE eabid = ".$_SESSION["eabid"]." AND eabstatus = 'A'";
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = eabCriarDocumento( $_SESSION['entid'] , $_SESSION['eabid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Escola Aberta');
				location.href = 'pdeescola.php?modulo=inicio&acao=C';
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ABERTA.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadEscolaAberta = $db->pegaUm($sql);

if(( integer)$vPerfilCadEscolaAberta > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == eabMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = eabCriarDocumento( $_SESSION['entid'], $_SESSION['eabid'] );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Escola Aberta');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = eabPegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_EAB || (integer)$esdid == AGUARDANDO_CORRECAO_CADASTRAMENTO_EAB) {
		$somenteConsulta = false;
	}
}

echo "<br />";
echo montarAbasArray(carregaAbasEscolaAberta(), "/pdeescola/pdeescola.php?modulo=eabprincipal/dados_programa&acao=A");

$titulo = 'Escola Aberta - Dados Programa '.$_SESSION["exercicio"];
$subtitulo = '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório';

monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAberta();

		//se $perfis desabilita Funcionamento do Escola Aberta em anos anteriores, Funcionamento do Escola Aberta em 2010, Uex Central.
		$perfis = arrayPerfil();
		
		if( in_array(PDEESC_PERFIL_CAD_ESCOLA_ABERTA,$perfis) || 
			in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA,$perfis) || 
			in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA,$perfis))
			{
				$boBloqueia = true;
			}
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="./js/eabajax.js" type="text/javascript"></script>

<form method="post" id="formEscolaAberta" action="">
<input type="hidden" name="submetidoea" value="1" />
<table align="center" class="Tabela">
 <tbody>
	<tr>
 		<td width="100%" style="text-align: center;" class="SubTituloEsquerda" colspan="2">&nbsp;</td>
 	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Funcionamento do Escola Aberta
		<br />anos anteriores
		</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
			<?php 
			$sql = "SELECT
						eaa.eaaid
					FROM
						pdeescola.eabtipoanoanterior eaa
					INNER JOIN
						pdeescola.eabescolaanoanterior eea ON eea.eaaid = eaa.eaaid
														  AND eea.eabid = ".$_SESSION["eabid"]."
					WHERE
						eaa.eaastatus = 'A'";
			$anosEab = $db->carregarColuna($sql);
			
			$sql = "SELECT eaaid,eaadescricao FROM pdeescola.eabtipoanoanterior WHERE eaastatus = 'A'";
			$anosAnteriores = $db->carregar($sql);
			$inp = array();
			for($i=0; $i<count($anosAnteriores); $i++)
			{
				$ch = "";
				if($anosEab)
				{
					if( in_array($anosAnteriores[$i]["eaaid"], $anosEab) )
					{
						$ch = 'checked="checked"';
					}
				}
				//Alteração Anos anteriores desabilidado a pedido equipe Escola Aberta.
				$inp[] = '<input type="checkbox" name="eaaid[]" value="'.$anosAnteriores[$i]["eaaid"].'" '.$ch.'" '.(($boBloqueia) ? 'disabled="disabled"' : '').'/>'.$anosAnteriores[$i]["eaadescricao"];
			}
			echo implode('&nbsp;', $inp);
			?>
		</td>
	</tr>			 
	<tr>
		<?php 
			
		$eabanoatual = $db->pegaUm("select eabanoatual from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		
		//Alteração Anos anteriores desabilidado a pedido equipe Escola Aberta.
		$ch = ($eabanoatual && $eabanoatual=='t' ) ? 'checked="checked and disabled"': '';
		?>
		<?php if($boBloqueia): ?>
				<input type="hidden" name="eabanoatual" value="<?php echo $eabanoatual ?>">
		<?php endif; ?>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Funcionamento do Escola Aberta em <?php echo $_SESSION['exercicio'] ?>
		<img src="/imagens/ajuda.gif" title="PARA EFETIVAR A PARTICIPAÇÃO DA ESCOLA NO PROGRAMA ESCOLA ABERTA É NECESSÁRIO QUE A UEX TENHA SE RECADASTRADO PDDEWEB, ASSINADO O TERMO DE COMPROMISSO DO PROGRAMA (ANEXO IIB) E PRESTADO CONTAS DO EXERCÍCIO ANTERIOR.">
		</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
			<input type="checkbox" id="eabanoatual" name="eabanoatual" value="t" <?=$ch?> <?=(($boBloqueia) ? 'disabled="disabled"' : '')?>/> <?php echo $_SESSION['exercicio'] ?>
		</td>
	</tr>
	<tr>
		<?php 
		$edrid = $db->pegaUm("select edrid from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		?>
		<td width="100" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
			<b>Dias de funcionamento em 2011/2012</b> 
			<img src="/imagens/ajuda.gif" title="A ESCOLA QUE NÃO RECEBEU RECURSO DO PDDE/FEFS EM 2009 FICA LIMITADA A 1 (UM) DIA DE FUNCIONAMENTO.">
			<br />
		
			<input type="radio" name="dia" value="1" onclick="diaFuncionamento(this.value);"  <?=(($edrid == 1 || $edrid == 2) ? 'checked="checked"' : 'disabled="disabled"')?> /> 1 dia
			&nbsp;&nbsp;&nbsp;
			<span id="sabado_domingo" style="padding:3px;border:1px solid black;<?=(($edrid && ($edrid==1 || $edrid==2)) ? 'display:inline;' : 'display:none;')?>">
				<input type="radio" class="edrid_" name="edrid" value="1" id="sabado1" onclick="verificafds(this);" <?=(((!$edrid || $edrid==3) || ($edrid && $edrid==1)) ? 'checked="checked"' : '')?> /> Sábado
				&nbsp;
				<input type="radio" class="edrid_" name="edrid" value="2" id="domingo1" onclick="verificafds(this);" <?=(($edrid && $edrid==2) ? 'checked="checked"' : '')?> /> Domingo
			</span>
			<br />
			<input type="radio" name="dia" value="3" <?=(($edrid == 3 && date('Y') == $_SESSION['exercicio']) ? 'checked="checked"' : 'disabled="disabled"')?> onclick="diaFuncionamento(this.value);" /> 2 dias (Sábado e Domingo) 
		</td>
		<?php 
		$eabqtduexatende = $db->pegaUm("select eabqtduexatende from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		?>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
			<b>UEX Central</b>
			<br />
			<input type="radio" name="opcao" value="sim" <?=(($eabqtduexatende) ? 'checked="checked"' : '')?> onclick="uexCentral(this);" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?>/> Sim
			&nbsp;&nbsp;&nbsp;
			<span id="escolas" style="padding:3px;border:1px solid black;<?=(($eabqtduexatende) ? 'display:inline;' : 'display:none;')?>">
				Atende:&nbsp;
				<input type="radio" name="eabqtduexatende" value="1" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?> <?=((!$eabqtduexatende || ($eabqtduexatende && $eabqtduexatende==1)) ? 'checked="checked"' : '')?> />1 Escola
				&nbsp;
				<input type="radio" name="eabqtduexatende" value="2" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?> <?=(($eabqtduexatende && $eabqtduexatende==2) ? 'checked="checked"' : '')?> />2 Escolas
				&nbsp;
				<input type="radio" name="eabqtduexatende" value="3" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?> <?=(($eabqtduexatende && $eabqtduexatende==3) ? 'checked="checked"' : '')?> />3 Escolas
				&nbsp;
				<input type="radio" name="eabqtduexatende" value="4" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?> <?=(($eabqtduexatende && $eabqtduexatende==4) ? 'checked="checked"' : '')?> />4 Escolas
				&nbsp;
				<input type="radio" name="eabqtduexatende" value="5" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?> <?=(($eabqtduexatende && $eabqtduexatende==5) ? 'checked="checked"' : '')?> />5 Escolas
				<?php if($boBloqueia): ?>
					<input type="hidden" name="eabqtduexatende" value="<?php echo $eabqtduexatende ?>">
				<?php endif; ?>
			</span>	
			<br />
			<input type="radio" name="opcao" value="nao" <?=((!$eabqtduexatende) ? 'checked="checked"' : '')?> onclick="uexCentral(this);" <?=(($boBloqueia) ? 'disabled="disabled"' : '')?>/> Não
		</td>
	</tr>	
 	<tr id="linhaJustificativa">
 		<?php 
 			$arTipoJustificativa = $db->carregar("select etjid, etjdescricao from pdeescola.eabtipojustificativa where etjstatus = 'A' ");
 			 
			$dadosJust = $db->pegaLinha("select etjid, eajoutrajustificativa from pdeescola.eabjustificativa where eabid = ".$_SESSION["eabid"]);
			
		
 		?>
 		<td width="100%" style="background: rgb(238, 238, 238)" class ="" colspan="2"> 
 			<label><b>Justificativa de mudança de funcionamento para apenas 1 dia</b></label><br />
 			<?php 
 				$i = 1;
 				foreach($arTipoJustificativa as $tipoJustificativa)
 				{
 					if($dadosJust['etjid'] == $tipoJustificativa['etjid'])
 						$checked = 'checked = "checked";';
 					else
 						$checked = '';
 					
 					echo "<input name=\"justificativa\" id=\"{$tipoJustificativa['etjid']}\" {$checked} value=\"{$tipoJustificativa['etjid']}\" onclick=\"toggleJustificativa()\" type=\"radio\" /><label for=\"{$tipoJustificativa['etjid']}\">{$tipoJustificativa['etjdescricao']}</label>";
	 				if(count($arTipoJustificativa) != $i){
	 					echo "<br />";
	 				} else {
	 					echo "";
	 				}
 					$i++;
 				}
 				
 			?>
 			<span id="spanJustificativa" style="display: <?php if(!$dadosJust['eajoutrajustificativa']) echo 'none';?>">
				<label> - Especificar:</label>
	 			<input name="justificativadesc" id="especificar" value="<?=$dadosJust['eajoutrajustificativa']?>" type="text" size="60" maxlength="250" />
			</span> 		
 		</td>  
 	</tr>
	<tr>
 		<td width="100%" style="text-align: center;" class="SubTituloEsquerda" colspan="2">
 		<?php if( !$somenteConsulta ) { ?>
 			<input type="button" value="Salvar" id="btSalvarEA" onclick="salvarEA();" />
 		<?php } ?>
 		</td>
 	</tr>
 </tbody>
</table>
</form>
<br />
<?php 

?>

<table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="0" align="center" border="0">
	<tr>
 		<td width="100%" style="text-align: center;" class="SubTituloEsquerda" colspan="3">Registro das Atividades / Oficinas</td>
 	</tr>
	<tr>
		<td style="width:450px;">
			Área Temática
			<br />
			<?
			$sql = "SELECT
						eatid as codigo,
						eatdescricao as descricao
					FROM
						pdeescola.eabtipoatividade
					WHERE
						eatstatus = 'A'
					ORDER BY
						eatdescricao ASC";
			$db->monta_combo("eatid", $sql, 'S', "Selecione uma área", 'filtraOficina', '', '', '400', 'S', '');
			?>
			<img src="/imagens/ajuda.gif" title="NO SENTIDO DE VALORIZAR A DIVERSIDADE DE ATIVIDADES NO FINAL DE SEMANA, A ESCOLA DEVE PREVER, NO MÍNIMO, 1 (UMA) ATIVIDADE/OFICINA DE CADA ÁREA TEMÁTICA.">
       	</td>
		<td id="nome_oficina" colspan="2"> 
			Nome da Atividade / Oficina <br />
			<select name="eaoid" class="campoEstilo" style="width:400px;">
				<option value="">Selecione uma Atividade / Oficina</option>
			</select>
			<img src="../imagens/obrig.gif" border="0" />
			<img src="/imagens/ajuda.gif" title="ESTE CAMPO TRAZ ATIVIDADES DE REFERÊNCIA, MAS A ESCOLA PODE COLOCAR UMA ATIVIDADE QUE NÃO ESTEJA MENCIONADA.">
		</td>
	</tr>
	<tr>
		<td> 
			Duração 
			<br />
			<?
			$sql = "SELECT
						eadid as codigo,
						eaddescricao as descricao
					FROM
						pdeescola.eabtipoduracaooficina
					WHERE
						eadstatus = 'A'";
			
			$db->monta_combo("eadid", $sql, 'S', "Selecione o Tempo de Duração", '', '', '', '205', 'S', '');
			?>
			<img src="/imagens/ajuda.gif" title="APÓS O PREENCHIMENTO DO PLANO DE ATIVIDADES, A ESCOLA DEVE ATUALIZÁ-LO, NO MÍNIMO, 01(UMA) VEZ DURANTE O EXERCÍCIO DO RECURSO DO PDDE/FEFS 2010/2011, NA OCASIÃO DA REPROGRAMAÇÃO DOS RECURSOS.">
		</td>
		<td colspan="2">
			N° participantes por oficina
			<br />
			<input type="text" class="normal" style="width:200px;" maxlength="3" onKeyUp="this.value=mascaraglobal('###',this.value);" name="eaaqtdparticipante" value="" />
			<img src="../imagens/obrig.gif" border="0">
			<img src="/imagens/ajuda.gif" title="A ESCOLA DEVE REGISTRAR A MÉDIA DE PARTICIPANTES DA ATIVIDADE OU O NÚMERO ESTIMADO DE PARTICIPANTES.">
		</td>
	</tr>
	<tr>
		<td>
			Dia de Realização
			<br />
			<input type="radio" name="diarealizacao" value="1" id="sabado2" onclick="verificafds(this);" <?=(($edrid == 1 || $edrid == 2) ? 'disabled="disabled"' : '')?> /> Sábado
			&nbsp;
			<input type="radio" name="diarealizacao" value="2" id="domingo2" onclick="verificafds(this);" <?=(($edrid == 1 || $edrid == 2) ? 'disabled="disabled"' : '')?> /> Domingo
		</td>
		<td>
			Turno
			<br />
			<input type="radio" name="eaaturno" value="M" checked="checked" /> Manhã
			&nbsp;&nbsp;
			<input type="radio" name="eaaturno" value="T" /> Tarde
		</td>
		<td>
			Ressarcida (PDDE-FEFS)
			<br />
			<input type="radio" name="eabressarcida" value="t" /> Sim
			&nbsp;&nbsp;
			<input type="radio" name="eabressarcida" value="f" /> Não
			&nbsp;
			<?php echo obrigatorio();?>
		</td>
	</tr>
	<tr>
		<td colspan="3">
			Espaço Utilizado
			<img src="../imagens/obrig.gif" border="0">
			<br />
			<input type="radio" value="E" name="eaetipo" checked="checked" onclick="espacoUtilizado(this);" /> Espaços potenciais da escola utilizados para atividades do programa
			&nbsp;
			<span id="espaco_escola" style="display:inline;">
			<?php
			 $sql = "SELECT
						eaeid as codigo,
						eaedescricao as descricao
					FROM
						pdeescola.eabtipoespaco
					WHERE
						eaestatus = 'A'
						AND eaetipo = 'E'
					ORDER BY
						eaedescricao ASC";
			$db->monta_combo("eaeid_escola", $sql, 'S', "Selecione um espaço", 'alternarExibicaoEspacoEscola', '', '', '300', 'N', '');
			?>
			<input type="text" name="eaaoutroespacoescola" id="eaaoutroespacoescola" value="" style="width: 50ex; display: none;" class="" size="300">
			</span>
			<br />
			<input type="radio" value="C" name="eaetipo" onclick="espacoUtilizado(this);" /> Espaços parceiros potenciais da comunidade utilizados para atividades do programa
			&nbsp;
			<span id="espaco_comunidade" style="display:none;">
			<?php
			 $sql = "SELECT
						eaeid as codigo,
						eaedescricao as descricao
					FROM
						pdeescola.eabtipoespaco
					WHERE
						eaestatus = 'A'
						AND eaetipo = 'C'
					ORDER BY
							eaedescricao ASC";
				$db->monta_combo("eaeid_comunidade", $sql, 'S', "Selecione um espaço", 'alternarExibicaoEspacoComunidade', '', '', '300', 'N', '');
				?>
				<input type="text" name="eaaoutroespacocomunidade" id="eaaoutroespacocomunidade" value=""  style="width: 50ex; display: none;" class="" size="300">
				</span>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#c0c0c0" cellSpacing="5" cellPadding="0" align="center" border="0">
		<tr>
			<td align="center">
			<?php if( !$somenteConsulta ) { ?>
				<input type="button" value="Adicionar Atividade" id="btIncluirAtividade" onclick="incluirAtividade();" />
			<?php } ?>
			</td>
		</tr>
</table>

<br />
<table align="center" width="95%">
	<tr>
		<td>
			<span style="color:red;">* Após incluir as atividades desejadas, deve-se clicar no botão abaixo: "Salvar Atividades".</span>
		</td>
	</tr>
</table>
<form method="post" id="formAtividades" action="">
<input type="hidden" name="submetido" value="1" />
<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" id="tabela_listagem" class="listagem">
	<thead>
		<tr>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Alterar/Excluir</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Área Temática</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Nome da Atividade / Oficina</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Duração da oficina</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>N° participantes por oficina</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Turno</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Dia de Realização</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Espaço Utilizado</strong>
			</td>
			<td bgcolor="" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
				<strong>Ressarcida</strong>
			</td>
		</tr>
	</thead>
	<tbody>
	<?php 
	$sql = "SELECT
			'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterar(' || eaa.eaaid || ');\" />
			 <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluir(' || eaa.eaaid || ')\" />' as acao,
			eat.eatdescricao,
			eao.eaodescricao,
			ead.eaddescricao,
			eaa.eaaqtdparticipante,
			CASE WHEN eaaturno = 'M'
			THEN 'Manhã' ELSE 'Tarde' END as eaaturno,
			edr.edrdescricao,
			eae.eaedescricao
		FROM
			pdeescola.eabatividade eaa
		INNER JOIN
			pdeescola.eabtipoatividade eat ON eat.eatid = eaa.eatid
		INNER JOIN
			pdeescola.eabtipooficina eao ON eao.eaoid = eaa.eaoid
		INNER JOIN
			pdeescola.eabtipoduracaooficina ead ON ead.eadid = eaa.eadid
		INNER JOIN
			pdeescola.eabdiarealizacao edr ON edr.edrid = eaa.edrid
		INNER JOIN
			pdeescola.eabtipoespaco eae ON eae.eaeid = eaa.eaeid
		INNER JOIN
			pdeescola.eabescolaaberta eab ON eab.eabid = eaa.eabid
		WHERE
			eaa.eabid = ".$_SESSION["eabid"];
	$dados = $db->carregar($sql);
	
	$sql = "SELECT
				 eaba.eatid,
				 eaba.eaoid,
				 eaba.eaaturno,
				 eaba.edrid,
				 eaba.eadid,
				 eaba.eaeid,
				 eaba.eaaqtdparticipante,
				 eaba.eaaoutroespacoescola,
				 eaba.eaaoutroespacocomunidade,
				 eaba.eaaoutraoficina,
				 eaba.eabressarcida,
				 eat.eatdescricao,
				 eao.eaodescricao,
				 ead.eaddescricao,
				 edr.edrdescricao,
				 eae.eaedescricao,
				 eae.eaetipo
			FROM
				pdeescola.eabatividade eaba
			INNER JOIN
				pdeescola.eabtipoatividade eat ON eat.eatid = eaba.eatid
			INNER JOIN
				pdeescola.eabtipooficina eao ON eao.eaoid = eaba.eaoid
			INNER JOIN
				pdeescola.eabtipoduracaooficina ead ON ead.eadid = eaba.eadid
			INNER JOIN
				pdeescola.eabdiarealizacao edr ON edr.edrid = eaba.edrid
			INNER JOIN
				pdeescola.eabtipoespaco eae ON eae.eaeid = eaba.eaeid
			WHERE
				eaba.eabid = ".$_SESSION["eabid"];
	$dados = $db->carregar($sql);
	
	if($dados)
	{
		for($i=0; $i<count($dados); $i++)
		{
			$cor = ($i%2) ? '#e0e0e0' : '#f4f4f4';
			
			if( $dados[$i]['eaetipo'] == 'E' )
			{
				$outros = $dados[$i]['eaaoutroespacoescola'];
				$texto = ($dados[$i]['eaaoutroespacoescola'] && $dados[$i]['eaaoutroespacoescola']!='null') ? ': '.$dados[$i]['eaaoutroespacoescola'] : '';
			}
			else
			{
				$outros = $dados[$i]['eaaoutroespacocomunidade'];
				$texto = ($dados[$i]['eaaoutroespacocomunidade'] && $dados[$i]['eaaoutroespacocomunidade']!='null') ? ': '.$dados[$i]['eaaoutroespacocomunidade'] : '';
			}
			
			echo '<tr style="background-color:'.$cor.'">
					<td style="text-align:center;">
						<img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="excluir(this.parentNode.parentNode.rowIndex)" />
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eatid'].'" name="lista_eatid[]" />
						'.$dados[$i]['eatdescricao'].'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eaoid'].'" name="lista_eaoid[]" />
						<input type="hidden" value="'.$dados[$i]['eaaoutraoficina'].'" name="lista_eaoid_outros[]" />
						'.$dados[$i]['eaodescricao'].' '.(($dados[$i]['eaaoutraoficina'] && $dados[$i]['eaaoutraoficina']!='null') ? ": ".$dados[$i]['eaaoutraoficina'] : "").'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eadid'].'" name="lista_eadid[]" />
						'.$dados[$i]['eaddescricao'].'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eaaqtdparticipante'].'" name="lista_eaaqtdparticipante[]" />
						'.$dados[$i]['eaaqtdparticipante'].'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eaaturno'].'" name="lista_eaaturno[]" />
						'.(($dados[$i]['eaaturno']=="M") ? "Manhã" : "Tarde").'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['edrid'].'" name="lista_edrid[]" />
						'.$dados[$i]['edrdescricao'].'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eaetipo'].'" name="lista_eaetipo[]" />
						<input type="hidden" value="'.$dados[$i]['eaeid'].'" name="lista_eaeid[]" />
						<input type="hidden" value="'.$outros.'" name="lista_eaeid_outros[]" />
						'.$dados[$i]['eaedescricao'].' '.$texto.'
					</td>
					<td style="text-align:center;">
						<input type="hidden" value="'.$dados[$i]['eabressarcida'].'" name="lista_eabressarcida[]" />
						'.(($dados[$i]['eabressarcida']=="t") ? 'Sim' : 'Não').'
					</td>
				  </tr>';
		}
	}
	else
	{
		echo '<td colspan="9" align="center" style="color: rgb(204,0,0);">Não foram encontrados registros.</td>';
	}
	?>
	</tbody>
</table>
<br />
<?php if( !$somenteConsulta ) { ?>
<center><input type="button" value="Salvar Atividades" id="btSalvar" onclick="salvar();" /></center>
<?php } ?>
</form>
<br />

<script type="text/javascript"><!--

if(document.getElementById('sabado1').checked == true)
{
	document.getElementById('domingo2').disabled = true;
	document.getElementById('sabado2').checked = true; 
}

if(document.getElementById('domingo1').checked == true)
{
	document.getElementById('sabado2').disabled = true;
	document.getElementById('domingo2').checked = true; 
}

if(document.getElementById('sabado2').checked == true)
{
	document.getElementById('domingo1').disabled = false;
	document.getElementById('sabado1').checked = true; 
}

if(document.getElementById('domingo2').checked == true)
{
	document.getElementById('sabado1').disabled = false;
	document.getElementById('domingo1').checked = true;		
}

function incluirAtividade()
{
	// recupera o objeto da tabela
	var tabela = document.getElementById('tabela_listagem');
	// recupera o objeto do botão
	var bt = document.getElementById("btIncluirAtividade");
	// desabilita o botão
	bt.disabled = true;

	// recupera o objeto do campo 'área temática' e valida
	var eatid = document.getElementsByName('eatid')[0];
	if(eatid.value == '')
	{
		alert('O campo "Área Temática" deve ser selecionado.');
		eatid.focus();
		bt.disabled = false;
		return;
	}
	
	// recupera o objeto do campo 'nome da atividade/oficina' e valida
	var eaoid = document.getElementsByName('eaoid')[0];
	var eaaoutraoficina = document.getElementById('eaaoutraoficina');
	if(eaoid.value == "")
	{
		alert('O campo "Nome da Atividade / Oficina" deve ser selecionado.');
		eaoid.focus();
		bt.disabled = false;
		return;
	}
	if( eaoid.options[eaoid.selectedIndex].text.substr(0,6).toLowerCase() == "outros" )
	{
		if( eaaoutraoficina.value == '' )
		{
			alert('O campo "Outros" deve ser preenchido.');
			eaaoutraoficina.focus();
			bt.disabled = false;
			return;
		}
	}
	
	// recupera o objeto do campo 'duração da oficina' e valida
	var eadid =	document.getElementsByName('eadid')[0];
	if(eadid.value == "")
	{
		alert('O campo "Duração da Oficina" deve ser preenchido.');
		eadid.focus();
		bt.disabled = false;
		return;
	}
	
	// recupera o objeto do campo 'n° de participantes por oficina' e valida
	var eaaqtdparticipante = document.getElementsByName('eaaqtdparticipante')[0];
	if(eaaqtdparticipante.value == "")
	{
		alert('O campo "N° participantes por oficina" deve ser preenchido.');
		eaaqtdparticipante.focus();
		bt.disabled = false;
		return;
	}

//	//alerta o preenchimento Ressarcida.
	var eabressarcida = document.getElementsByName('eabressarcida');
	if(eabressarcida[0].checked == false && eabressarcida[1].checked == false)
	{
		alert('O preenchimeto do campo "Ressarcida (PDDE-FEFS)" é obrigatório.')
		eabressarcida[0].focus();
		bt.disabled = false;
		return;
	}
	
	
	// recupera o objeto do campo 'espaço utilizado' e valida
	var eaetipo = document.getElementsByName('eaetipo');
	
	if(eaetipo[0].checked == true)
	{
		var eaaoutroespacoescola = document.getElementById('eaaoutroespacoescola');
		var eaeidEscola	= document.getElementsByName('eaeid_escola')[0];
		if(eaeidEscola.value == "")
		{
			alert('O campo "Espaços potenciais da escola" deve ser selecionado.');
			eaeidEscola.focus();
			bt.disabled = false;
			return;
		}
		else if(eaeidEscola.value == "12" && eaaoutroespacoescola.value == "")
		{
			alert('O campo "Outros" deve ser preenchido.');
			eaaoutroespacoescola.focus();
			bt.disabled = false;
			return;
		}
	}
	if(eaetipo[1].checked == true)
	{
		var eaaoutroespacocomunidade = document.getElementById('eaaoutroespacocomunidade');
		var eaeidComunidade	= document.getElementsByName('eaeid_comunidade')[0];
		if(eaeidComunidade.value == "")
		{
			alert('O campo "Espaços parceiros potenciais da comunidade" deve ser selecionado.');
			eaeidComunidade.focus();
			bt.disabled = false;
			return;
		}
		else if(eaeidComunidade.value == "25" && eaaoutroespacocomunidade.value == "")
		{
			alert('O campo "Outros" deve ser preenchido.');
			eaaoutroespacocomunidade.focus();
			bt.disabled = false;
			return;
		}
	}

	// retira a mensagem, se houver, de que não foram encontrados registros
	if( tabela.rows.length == 2 )
	{
		if( tabela.rows[1].cells[0].innerHTML == 'Não foram encontrados registros.' )
		{
			tabela.deleteRow(1);
		}
	}

	// seta a cor de fundo
	var backgroundColor = "#f4f4f4";
	// tamanho da tabela
	var tamanho = tabela.rows.length;
	
	if( tamanho > 1 )
	{
		if(tabela.rows[tamanho-1].style.backgroundColor == "rgb(224, 224, 224)")
		{
			backgroundColor = "#f4f4f4";					
		}
		else
		{
			backgroundColor = "#e0e0e0";					
		}
	}

	//var arrAlterar = new Array();
	//arrAlterar['eatid'] = eatid.value;
	
	// cria uma linha nova
	var linha = tabela.insertRow(tamanho);
	linha.style.backgroundColor = backgroundColor;
	// cria as células a partir dos valores informados para a atividade
	var celAlterarExcluir = linha.insertCell(0);
	celAlterarExcluir.style.textAlign = 'center';
	celAlterarExcluir.innerHTML = //'<img src="../imagens/alterar.gif" style="cursor:pointer;" onclick="alterar()" />&nbsp;'; +
								  '<img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="excluir(this.parentNode.parentNode.rowIndex)" />';
	
	// insere a célula com o conteúdo adequado
	var celAreaTematica = linha.insertCell(1);
	celAreaTematica.style.textAlign = 'center'; 
	celAreaTematica.innerHTML = '<input type="hidden" name="lista_eatid[]" value="'+(eatid.value)+'" />'
								+ eatid.options[eatid.selectedIndex].text;

	// insere a célula com o conteúdo adequado
	var celNomeAtividade = linha.insertCell(2);
	celNomeAtividade.style.textAlign = 'center';

	var texto = "";
	var textoLista = "";
	if( eaoid.options[eaoid.selectedIndex].text.substr(0,6).toLowerCase() == "outros" )
	{
		texto = ': ' + eaaoutraoficina.value;
	}
	celNomeAtividade.innerHTML =   '<input type="hidden" name="lista_eaoid[]" value="'+(eaoid.value)+'" />'
								 + '<input type="hidden" name="lista_eaoid_outros[]" value="'+(eaaoutraoficina.value)+'" />'
								 + eaoid.options[eaoid.selectedIndex].text + texto;

	// insere a célula com o conteúdo adequado
	var celDuracaoOficina = linha.insertCell(3);
	celDuracaoOficina.style.textAlign = 'center';
	celDuracaoOficina.innerHTML = '<input type="hidden" name="lista_eadid[]" value="'+(eadid.value)+'" />'
								  + eadid.options[eadid.selectedIndex].text;

	// insere a célula com o conteúdo adequado
	var celNumParticipantes = linha.insertCell(4);
	celNumParticipantes.style.textAlign = 'center';
	celNumParticipantes.innerHTML = '<input type="hidden" name="lista_eaaqtdparticipante[]" value="'+(eaaqtdparticipante.value)+'" />'
									+ eaaqtdparticipante.value;

	// recupera o objeto do campo 'turno'
	var eaaturnoManha = document.getElementsByName('eaaturno')[0];
	var eaaturnoTarde = document.getElementsByName('eaaturno')[1];
	// insere a célula com o conteúdo adequado
	var celTurno = linha.insertCell(5);
	celTurno.style.textAlign = 'center';
	if(eaaturnoManha.checked)
	{
		celTurno.innerHTML = '<input type="hidden" name="lista_eaaturno[]" value="'+(eaaturnoManha.value)+'" />'
							 + 'Manhã';
	}
	else
	{
		celTurno.innerHTML = '<input type="hidden" name="lista_eaaturno[]" value="'+(eaaturnoTarde.value)+'" />'
		 					 + 'Tarde';
	}
	
	// recupera o objeto do campo 'dia de realização'
	var diaRealizacaoSabado = document.getElementsByName('diarealizacao')[0];
	var diaRealizacaoDomingo = document.getElementsByName('diarealizacao')[1];
	// insere a célula com o conteúdo adequado
	var celDiaRealizacao = linha.insertCell(6);
	celDiaRealizacao.style.textAlign = 'center';
	if(diaRealizacaoSabado.checked)
	{
		celDiaRealizacao.innerHTML = '<input type="hidden" name="lista_edrid[]" value="'+(diaRealizacaoSabado.value)+'" />'
									 + 'Sábado';
	}
	else
	{
		celDiaRealizacao.innerHTML = '<input type="hidden" name="lista_edrid[]" value="'+(diaRealizacaoDomingo.value)+'" />'
		 							 + 'Domingo';
	}

	// insere a célula com o conteúdo adequado
	var celEspacoUtilizado = linha.insertCell(7);
	celEspacoUtilizado.style.textAlign = 'center';

	var texto = "";
	if(eaetipo[0].checked == true)
	{
		texto = (eaaoutroespacoescola.value != "") ? ': ' + eaaoutroespacoescola.value : '';
		celEspacoUtilizado.innerHTML =   '<input type="hidden" name="lista_eaetipo[]" value="'+(eaetipo[0].value)+'" />'
									   + '<input type="hidden" name="lista_eaeid[]" value="'+(eaeidEscola.value)+'" />'
									   + '<input type="hidden" name="lista_eaeid_outros[]" value="'+(eaaoutroespacoescola.value)+'" />'
									   + eaeidEscola.options[eaeidEscola.selectedIndex].text + texto;
	}
	else
	{
		texto = (eaaoutroespacocomunidade.value != "") ? ': ' + eaaoutroespacocomunidade.value : '';
		celEspacoUtilizado.innerHTML =   '<input type="hidden" name="lista_eaetipo[]" value="'+(eaetipo[1].value)+'" />'
									   + '<input type="hidden" name="lista_eaeid[]" value="'+(eaeidComunidade.value)+'" />'
									   + '<input type="hidden" name="lista_eaeid_outros[]" value="'+(eaaoutroespacocomunidade.value)+'" />'
									   + eaeidComunidade.options[eaeidComunidade.selectedIndex].text + texto;
	}

	// Ressarcida (PDDE-FEFS) 
	var eabRessarcidaSim = document.getElementsByName('eabressarcida')[0];
	var eabRessarcidaNao = document.getElementsByName('eabressarcida')[1];
	// insere a célula com o conteúdo adequado
	var celRessarcida = linha.insertCell(8);
	celRessarcida.style.textAlign = 'center';
	if(eabRessarcidaSim.checked)
	{
		celRessarcida.innerHTML = '<input type="hidden" name="lista_eabressarcida[]" value="'+(eabRessarcidaSim.value)+'" />'
		 						  + 'Sim';
	}
	else
	{
		celRessarcida.innerHTML = '<input type="hidden" name="lista_eabressarcida[]" value="'+(eabRessarcidaNao.value)+'" />'
								  + 'Não';
	}
	
	// limpa os campos do formulário
	eatid.selectedIndex = 0;
	eadid.selectedIndex = 0;
	eaaqtdparticipante.value = "";

	if( eaoid.options[eaoid.selectedIndex].text.substr(0,6).toLowerCase() == "outros" )
	{
		eaaoutraoficina.value = "";
		eaaoutraoficina.style.display = 'none';
	}
	eaoid.selectedIndex = 0;
	
	diaRealizacaoSabado.checked = true;
	eaaturnoManha.checked = true;
	eabRessarcidaSim.checked = true;
	
	if(eaetipo[0].checked == true)
	{
		eaeidEscola.selectedIndex = 0;
		eaaoutroespacoescola.value = "";
		eaaoutroespacoescola.style.display = 'none';
	}
	if(eaetipo[1].checked == true)
	{
		eaeidComunidade.selectedIndex = 0;
		eaaoutroespacocomunidade.value = "";
		eaaoutroespacocomunidade.style.display = 'none';
	}
	
	bt.disabled = false;
	if(document.getElementById('sabado1').checked == true)
	{
		//document.getElementById('domingo2').disabled = true;
		document.getElementById('sabado2').checked = true; 
	}

	if(document.getElementById('domingo1').checked == true)
	{
		document.getElementById('sabado2').disabled = true;
		document.getElementById('domingo2').checked = true; 
	}

	if(document.getElementById('sabado2').checked == true)
	{
		document.getElementById('domingo1').disabled = false;
		document.getElementById('sabado1').checked = true; 
	}

	if(document.getElementById('domingo2').checked == true)
	{
		document.getElementById('sabado1').disabled = false;
		document.getElementById('domingo1').checked = true;		
	}
}

function salvar() {
	var form = document.getElementById("formAtividades");
	var bt = document.getElementById("btSalvar");
	var eatid = document.getElementsByName('lista_eatid[]');

	bt.disabled	= true;

	var arrEatid = new Array();
	var flag;
	for(var i=0; i<eatid.length; i++)
	{
		flag = true;
		for(var j=0; j<arrEatid.length; j++)
		{
			if( arrEatid[j] == eatid[i].value )
			{
				flag = false;
			}
		}

		if(flag)
		{
			arrEatid.push(eatid[i].value);
		}
	}

	if( arrEatid.length < 4 )
	{
		alert('É necessário preencher, no mínimo, 1 (uma) oficina de cada Área Temática.');
		bt.disabled = false;
		return;
	}
	form.submit();
}

function alterar(id)
{
	var form	=	document.getElementById("formAtividades");
	var bt		=	document.getElementById("btSalvar");
	var submet	= 	document.getElementsByName('submetido')[0];
	var eaaid	= 	document.getElementsByName('eaaid')[0];
	
	bt.disabled 	= true;
	eaaid.value		= id;
	submet.value 	= 0;
	form.submit();
}

function excluir(index)
{
	if( confirm("Deseja excluir este item?") )
	{
		// recupera o objeto da tabela
		var tabela = document.getElementById('tabela_listagem');
		tabela.deleteRow(index);

		if( tabela.rows.length == 1 )
		{
			var linha = tabela.insertRow(1);
			var celula = linha.insertCell(0);
			celula.style.textAlign = 'center';
			celula.style.color = 'rgb(204,0,0)';
			celula.colSpan = "9";
			celula.innerHTML = 'Não foram encontrados registros.';
		}
	}
}

function filtraOficina(eatid)
{
	if(!eatid)
		return false;
		
	var td		= document.getElementById('nome_oficina');
	var select	= document.getElementsByName('eaoid')[0];

	select.disabled 			= true;
	select.options[0].text 		= 'Carregando...';
	select.options[0].selected 	= true;

	var req = new Ajax.Request('pdeescola.php?modulo=eabprincipal/dados_programa&acao=A', {
        method:     'post',
        parameters: '&ajaxEatid=' + eatid,
        onComplete: function (res)
        {
        	var campoOutro = "<input type=\"text\" name=\"eaaoutraoficina\" id=\"eaaoutraoficina\" value=\"\" style=\"width: 50ex; display: none;\" class=\"\" size=\"300\">";
			td.innerHTML = 'Nome da Atividade / Oficina<br />' + res.responseText + '<img src="/imagens/ajuda.gif" title="ESTE CAMPO TRAZ ATIVIDADES DE REFERÊNCIA, MAS A ESCOLA PODE COLOCAR UMA ATIVIDADE QUE NÃO ESTEJA MENSIONADA.">  '+ campoOutro;
		}
		
	  });
}

function filtraOficinaCarregar(eatid, eaoid)
{
	var td		= document.getElementById('nome_oficina');
	var select	= document.getElementsByName('eaoid')[0];

	select.disabled 			= true;
	select.options[0].text 		= 'Carregando...';
	select.options[0].selected 	= true;

	var eaaoutraoficina = '<?php echo $eaaoutraoficina; ?>';

	var req = new Ajax.Request('pdeescola.php?modulo=eabprincipal/dados_programa&acao=A', {
        method:     'post',
        parameters: '&ajaxEatid=' + eatid,
        onComplete: function (res)
        {							        	
			var campoOutro = "<input type=\"text\" name=\"eaaoutraoficina\" id=\"eaaoutraoficina\" value="+eaaoutraoficina+" style=\"width: 50ex; display: none;\" class=\"\" size=\"50\">";
			td.innerHTML = 'Nome da Atividade / Oficina<br />' + res.responseText+ '<img src="/imagens/ajuda.gif" title="ESTE CAMPO TRAZ ATIVIDADES DE REFERÊNCIA, MAS A ESCOLA PODE COLOCAR UMA ATIVIDADE QUE NÃO ESTEJA MENSIONADA.">  ' + campoOutro;
			// Seleciona o item certo do combo a partir do 'eaoid'
			var novoSelect 	= document.getElementsByName('eaoid')[0];
			var selecionado	= "";

			for(var i=0; i<novoSelect.options.length; i++) {
				if(novoSelect.options[i].value == eaoid){
					selecionado = novoSelect.options[i].index;
					alternarExibicaoOficina(eaoid);
				}
			}

			novoSelect.selectedIndex = selecionado;

			
        }
  });
}
//trava o dia de funcionamento com dia de realização
function verificafds(obj)
{
	
	if(obj.id == 'sabado1' && obj.checked == true){
		document.getElementById('domingo2').disabled = true;
		document.getElementById('sabado2').checked = true; 
	}
	
	if(obj.id == 'domingo1' && obj.checked == true){
		document.getElementById('sabado2').disabled = true;
		document.getElementById('domingo2').checked = true; 
	}

	if(obj.id == 'sabado2' && obj.checked == true){
		document.getElementById('domingo1').disabled = false;
		document.getElementById('sabado1').checked = true; 
	}
	
	if(obj.id == 'domingo2' && obj.checked == true){
		document.getElementById('sabado1').disabled = false;
		document.getElementById('domingo1').checked = true;		
	}
}

jQuery.noConflict();

if(jQuery('input[name=edrid]:radio:checked').val() == 1){	
	jQuery('#sabado2').attr("checked", true);
	jQuery('input[name=diarealizacao]').attr('inline', true);	
}else{	
	jQuery('#domingo2').attr("checked", true);
	jQuery('input[name=diarealizacao]').attr('inline', true);	
}
if(jQuery('input[name=dia]:radio:checked').val() == 3){
	jQuery('input[name=diarealizacao]').attr('disabled', false);
}
	
jQuery(document).ready(function(){
	diaFuncionamento(jQuery('input[name=dia]:checked').val());	
});

//Se a escola for somente 2011, ou dois dia, não aparece Tipo de justificativa.
function diaFuncionamento(valor)
{
	var boExisteAnoAnterior = false;
	jQuery('input[name=eaaid[]]').each(function(){
		if(jQuery(this).attr('selected') == true){
			boExisteAnoAnterior = true;
		}
	});
	
	if(valor == 3 && boExisteAnoAnterior == false)
	{
		jQuery('#sabado_domingo').hide();
		jQuery('#linhaJustificativa').hide();
	}
}

function toggleJustificativa()
{
	var obOutra = document.getElementById( '4' );
	var spanJustificativa = document.getElementById( 'spanJustificativa' );
	
	spanJustificativa.style.display = obOutra.checked ? '' : 'none';
}

function uexCentral(obj)
{
	if(obj.value == 'sim')
	{
		document.getElementById("escolas").style.display = "inline";
	}
	if(obj.value == 'nao')
	{
		document.getElementById("escolas").style.display = "none";
	}
}


function espacoUtilizado(obj, eatid, eaeid)
{
	if(obj.value == "E")
	{
		document.getElementById("espaco_escola").style.display 		= "inline";
		document.getElementById("espaco_comunidade").style.display 	= "none";
	}
	if(obj.value == "C")
	{

		document.getElementById("espaco_comunidade").style.display 	= "inline";
		document.getElementById("espaco_escola").style.display 		= "none";

		}
	
	}

function salvarEA()
{
	var form	=	document.getElementById("formEscolaAberta");
	var bt		=	document.getElementById("btSalvarEA");
	

	bt.disabled = true;
	form.submit();
}

function alternarExibicaoOficina( valor ){
	var eaaoutraoficina = document.getElementById( 'eaaoutraoficina' );
	if( valor == 94 || valor == 95 || valor == 96 || valor == 97){
		eaaoutraoficina.style.display = "";
		eaaoutraoficina.className = "";
	} else if( valor != 94 || valor != 95 || valor != 96 || valor != 97){
		eaaoutraoficina.value = "";
		eaaoutraoficina.style.display = "none";
	}
}
function alternarExibicaoEspacoEscola( valor ){
	var eaaoutroespacoescola = document.getElementById( 'eaaoutroespacoescola' );
	if( valor == 12 ){
		eaaoutroespacoescola.style.display = "";
		eaaoutroespacoescola.className = "";
	} else if( valor != 12 ){
		eaaoutroespacoescola.value = "";
		eaaoutroespacoescola.style.display = "none";
	}
}
function alternarExibicaoEspacoComunidade( valor ){
	var eaaoutroespacocomunidade = document.getElementById( 'eaaoutroespacocomunidade' );
	if( valor == 25 ){
		eaaoutroespacocomunidade.style.display = "";
		eaaoutroespacocomunidade.className = "";
	} else if( valor != 25 ){
		eaaoutroespacocomunidade.value = "";
		eaaoutroespacocomunidade.style.display = "none";
	}
}
<?php 
if($eaaid)
{
	echo "filtraOficinaCarregar(".$eatid.", ".$eaoid.")";
}
?>

</script>