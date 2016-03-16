<?php

function carregarPerguntas($dados) {
	global $db;
	
	$sql = "SELECT prgid as codigo, prgdesc as descricao FROM pdeinterativo.pergunta WHERE prgstatus='A' AND prgmodulo='".$dados['prgmodulo']."' and prgsubmodulo='".$dados['prgsubmodulo']."' and prgdetalhe='".iconv("UTF-8", "ISO-8859-1", $dados['prgdetalhe'])."' ORDER BY prgdesc";
	$perguntas = $db->carregar($sql);
	
	if($perguntas[0]) {
		echo "<table class=listagem width=100%>";
		foreach($perguntas as $perg) {
			echo "<tr>";
			echo "<td><input type=checkbox name=prgid[] value=".$perg['codigo']."></td>";
			echo "<td>".$perg['descricao']."</td>";
			echo "</tr>";
		}
		echo "</table>";
	} else {
		echo "Não existem perguntas";
	}
}

function carregarTema($dados) {
	global $db;
	
	$sql = "select
					prgdetalhe as codigo,
					prgdetalhe as descricao
				from
					pdeinterativo.pergunta
				where
					prgmodulo = '".$dados['prgmodulo']."' and prgsubmodulo = '".$dados['prgsubmodulo']."'  and prgdetalhe is not null
				group by 
					prgdetalhe
				order by
					prgdetalhe";
	
	$detalhes = $db->carregar($sql);
	
	if($detalhes[0]) {
		echo "<select class=\"CampoEstilo\" name=\"prgdetalhe\" onchange=\"if(this.value!=''){filtraPerguntas('".$dados['prgmodulo']."','".$dados['prgsubmodulo']."', this.value);}else{jQuery('#td_pergunta').html('Selecione um tema');}\">";
		echo "<option value=\"\">Selecione</option>";
		if($dados['prgmodulo']=="G" && $dados['prgsubmodulo']=="D") echo "<option value=\"equipegestora\">Equipe Gestora</option>";
		if($dados['prgmodulo']=="C" && $dados['prgsubmodulo']=="C") echo "<option value=\"Colegiado\">Colegiado\Conselho Escolar</option>";
		if($dados['prgmodulo']=="C" && $dados['prgsubmodulo']=="C") echo "<option value=\"perfil\">perfil</option>";
		if($dados['prgmodulo']=="C" && $dados['prgsubmodulo']=="D") echo "<option value=\"perfil\">Perfil</option>";
		if($dados['prgmodulo']=="C" && $dados['prgsubmodulo']=="P") echo "<option value=\"perfil\">Perfil</option>";
		foreach($detalhes as $dtl) {
			if ($dados['prgmodulo'] != "P" && $dtl['codigo'] != "Colegiado")
			echo "<option value=\"".$dtl['codigo']."\">".$dtl['descricao']."</option>";
		}
		
		
		
		echo "</select>";
	} else {
		echo "Não possui temas";
	}
	//$db->monta_combo("prgdetalhe",$sql,"S","Selecione...","filtraPerguntas","","","","N","","");
}

function carregarEscolas($dados) {
	global $db;
	$sql = "	select
					pdeid as codigo,
					pdenome as descricao
				from
					pdeinterativo.pdinterativo
				where
					muncod = '".$dados['muncod']."'
				order by
					pdenome";
	
	$db->monta_combo("pdeid",$sql,"S","Selecione...","","","","","N","","");
	
	
}

function carregarMunicipios($dados) {
	global $db;
	$sql = "	select
					muncod as codigo,
					mundescricao as descricao
				from
					territorios.municipio
				where
					estuf = '".$dados['estuf']."'
				order by
					mundescricao";
	
	$db->monta_combo("muncod",$sql,"S","Selecione...",$dados['acao'],"","","","N","","");
}

function carregarFiltroplanoacao($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="naovalidar">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Plano Ação</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Priorização:</td>
			<td width="80%">
			<?php
				$pditempdeescola[] = array("codigo"=>"t","descricao"=>"Priorizada pelo PDE Escola");
				$pditempdeescola[] = array("codigo"=>"f","descricao"=>"Não priorizada pelo PDE Escola");
				$db->monta_combo('pditempdeescola', $pditempdeescola, 'S', 'Selecione', '', '', '', '200', 'N', 'pditempdeescola');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Fonte:</td>
			<td width="80%">
			<?php
				$pabfonte[] = array("codigo"=>"P","descricao"=>" Fonte PDE Escola");
				$pabfonte[] = array("codigo"=>"O","descricao"=>"Outras fontes");
				$db->monta_combo('pabfonte', $pabfonte, 'S', 'Selecione', '', '', '', '200', 'N', 'pabfonte');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'N', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","N") ?>
			</td>
		</tr>	
	</table>
<?	
}

function carregarFiltrodimensao6($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="estuf,Selecione um Estado">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 6</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Subdimensão :</td>
			<td>
			<select class="CampoEstilo" name="prgsubmodulo" onchange="if(this.value!=''){filtraTema('I', this.value);}else{jQuery('#td_tema').html('Selecione uma subdimensão');}">
			<option value="">Selecione</option>
			<option value="I">6.1. Instalações</option>
			<option value="E">6.2. Equipamentos</option>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Tema :</td>
			<td id="td_tema">Selecione uma Subdimensão</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Perguntas :</td>
			<td id="td_pergunta">Selecione um Tema</td>
		</tr>
		
		<tr>
		<td class="SubTituloDireita" valign="top">Equipamentos :</td>
		<td>
		<?
		$sql = "SELECT tmeid as codigo, tmedesc as descricao FROM pdeinterativo.tipomaterialequipamento WHERE tmestatus='A'";
		combo_popup( "tmeid", $sql, "Equipamento", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
		?>
		</td>
		</tr>

		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","estuf","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="td_escola">
			<?php $db->monta_combo("pdeid",array(),"N","Selecione a Escola","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}


function carregarFiltrodimensao5($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="estuf,Selecione um Estado">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 5</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Subdimensão :</td>
			<td>
			<select class="CampoEstilo" name="prgsubmodulo" onchange="if(this.value!=''){filtraTema('C', this.value);}else{jQuery('#td_tema').html('Selecione uma subdimensão');}">
			<option value="">Selecione</option>
			<option value="E">5.1. Estudantes</option>
			<option value="D">5.2. Docentes</option>
			<option value="P">5.3. Demais Profissionais</option>
			<option value="C">5.4. Pais e comunidade</option>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Tema :</td>
			<td id="td_tema">Selecione uma Subdimensão</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Perguntas :</td>
			<td id="td_pergunta">Selecione um Tema</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","estuf","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="td_escola">
			<?php $db->monta_combo("pdeid",array(),"N","Selecione a Escola","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}


function carregarFiltrodimensao4($dados) {
	global $db;
	if ( in_array(PDEINT_PERFIL_EQUIPE_MEC, arrayPerfil()) || in_array(PDEINT_PERFIL_SUPER_USUARIO, arrayPerfil()) )
		echo'<input type="hidden" name="camposvalidar" id="camposvalidar" value="naovalidar">';
	else
		echo'<input type="hidden" name="camposvalidar" id="camposvalidar" value="estuf,Selecione um Estado">';
		
	?>	
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 4</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Subdimensão :</td>
			<td>
			<select class="CampoEstilo" name="prgsubmodulo" onchange="if(this.value!=''){filtraTema('G', this.value);}else{jQuery('#td_tema').html('Selecione uma subdimensão');}">
			<option value="">Selecione</option>
			<option value="D">4.1. Direção</option>
			<option value="P">4.2. Processos</option>
			<option value="F">4.3. Finanças</option>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Tema :</td>
			<td id="td_tema">Selecione uma Subdimensão</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Perguntas :</td>
			<td id="td_pergunta">Selecione um Tema</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","estuf","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="td_escola">
			<?php $db->monta_combo("pdeid",array(),"N","Selecione a Escola","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}


function carregarFiltrodimensao3($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="estuf,Selecione um Estado">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 3</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Subdimensão :</td>
			<td>
			<select class="CampoEstilo" name="prgsubmodulo" onchange="if(this.value!=''){filtraTema('E', this.value);}else{jQuery('#td_tema').html('Selecione uma subdimensão');}">
			<option value="">Selecione</option>
			<option value="P">3.1. Planejamento Pedagógico</option>
			<option value="T">3.2. Tempo de Aprendizagem</option>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Tema :</td>
			<td id="td_tema">Selecione uma Subdimensão</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Perguntas :</td>
			<td id="td_pergunta">Selecione um Tema</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","estuf","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="td_escola">
			<?php $db->monta_combo("pdeid",array(),"N","Selecione a Escola","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}


function carregarFiltrodimensao2($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="estuf,Selecione um Estado">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 2</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Subdimensão :</td>
			<td>
			<select class="CampoEstilo" name="prgsubmodulo" onchange="if(this.value!=''){filtraTema('D', this.value);}else{jQuery('#td_tema').html('Selecione uma subdimensão');}">
			<option value="">Selecione</option>
			<option value="D">2.2. Distorção idade-série</option>
			<option value="A">2.3. Aproveitamento escolar</option>
			<option value="C">2.4. Áreas de conhecimento</option>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Tema :</td>
			<td id="td_tema">Selecione uma Subdimensão</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Perguntas :</td>
			<td id="td_pergunta">Selecione um Tema</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","estuf","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="td_escola">
			<?php $db->monta_combo("pdeid",array(),"N","Selecione a Escola","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}



function carregarFiltrodimensao1($dados) {
	global $db;
?>	
		<input type="hidden" name="camposvalidar" id="camposvalidar" value="intensino,Selecione um ensino">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td>Filtros - Dimensão 1</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Ensino :</td>
			<td width="80%">
			<?
			$ens[] = array("codigo"=>"I","descricao"=>"Ensino Fundamental Regular - Séries Iniciais (até a 4ª série)");
			$ens[] = array("codigo"=>"F","descricao"=>"Ensino Fundamental Regular - Séries Finais (5ª a 8ª série)");
			$ens[] = array("codigo"=>"M","descricao"=>"Ensino Médio Regular");
			$db->monta_combo('intensino', $ens, 'S', 'Selecione', '', '', '', '200', 'S', 'intensino');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Rede :</td>
			<td width="80%">
			<?
			$rede[] = array("codigo"=>"Municipal","descricao"=>"Municipal");
			$rede[] = array("codigo"=>"Estadual","descricao"=>"Estadual");
			$db->monta_combo('intesfera', $rede, 'S', 'Selecione', '', '', '', '200', 'S', 'intesfera');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
			<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
			</td>
		</tr>
	</table>
<?	
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit();
}



switch($_REQUEST['buscar']) {
	case '1':
		if($_REQUEST['dimensao']) {
			/* configurações */
			ini_set("memory_limit", "2048M");
			set_time_limit(0);
			/* FIM configurações */
			
			echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
			
			if ($_REQUEST['xls'] != 1)
			{
				echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
				echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
			}
			
			include "relatoriogeral_".$_REQUEST['dimensao'].".inc";				
		}
		exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( "Relatório Dimensões", "" );
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	// submete formulario
	if(validarsubmit()) {
		formulario.target = 'exibirrelatorio';
		var janela = window.open('','exibirrelatorio','width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		jQuery("#xls").val('0');
		formulario.submit();
		janela.focus();
	}
}

function exibirRelatorioXLS() {
	var formulario = document.formulario;
	// submete formulario
	if(validarsubmit()) {
		formulario.target = 'exibirrelatorioxls';
		var janela = window.open('pdeinterativo.php?modulo=relatorio/relatoriogeralxls_dimensao4&acao=A','exibirrelatorioxls');
		jQuery("#xls").val('1');
		formulario.submit();
		janela.focus();
		//jQuery.post("pdeinterativo.php?modulo=relatorio/relatoriogeralxls_dimensao4&acao=A",jQuery("#formulario").serialize());
	}
}

function validarsubmit() {
	if(document.getElementById('tmeid')) {
		selectAllOptions( document.getElementById( 'tmeid' ) );
	}
	if(document.getElementById('camposvalidar')) {
		var erro = document.getElementById('camposvalidar').value;
		if(erro=="errovalidar") {
			alert('Relátorio não se encontra disponível');
			return false;
		} else if(erro=="naovalidar") {
			return true;
		} else {
			var campos = erro.split(";");
			for(var i=0;i<campos.length;i++) {
				var c = campos[i].split(',');
				if(document.getElementById(c[0]).value=='') {
					alert(c[1]);
					return false;
				}
			}
		}
	} else {
		alert('Selecione uma dimensão');
		return false;
	}
	return true;
}

function selecionaDimensao(obj) {
	document.getElementById('div_dimensao1').style.display='none';
	document.getElementById('div_'+obj.value).style.display='';
}

function selecionaDimensao(obj) {
	if(jQuery("#dimensao").val() == 'dimensao4' || jQuery("#dimensao").val() == 'dimensao5' ){
		jQuery("#rel_xls").show();
	} else {
		jQuery("#rel_xls").hide();
	}	
	if(obj.value!='') {
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A",
	   		data: {relatorio: jQuery("#dimensao").val(), requisicao:"carregarFiltro"+obj.value},
	   		async: false,
	   		success: function(html){document.getElementById('div_filtros').innerHTML = html;}
	 		});
 	} else {
		jQuery('#div_filtros').html('<p align=center>Selecione uma dimensão</p>');
 	}
}

function filtraMunicipio(estuf) {
	var pescola='';
	if(document.getElementById('td_escola')) { var pescola="&acao=filtraEscola";}
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A",
   		data: "requisicao=carregarMunicipios"+pescola+"&estuf="+estuf,
   		async: false,
   		success: function(html){document.getElementById('td_muncod').innerHTML = html;}
 		});
}

function filtraEscola(muncod) {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A",
   		data: "requisicao=carregarEscolas&muncod="+muncod,
   		async: false,
   		success: function(html){document.getElementById('td_escola').innerHTML = html;}
 		});
}

function filtraTema(prgmodulo, prgsubmodulo) {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A",
   		data: "requisicao=carregarTema&prgmodulo="+prgmodulo+"&prgsubmodulo="+prgsubmodulo,
   		async: false,
   		success: function(html){document.getElementById('td_tema').innerHTML = html;}
 		});
}

function filtraPerguntas(prgmodulo, prgsubmodulo, prgdetalhe) {

	if (prgdetalhe == 'Colegiado')
		prgsubmodulo = 'P';
		
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A",
   		data: "requisicao=carregarPerguntas&prgdetalhe="+prgdetalhe+"&prgmodulo="+prgmodulo+"&prgsubmodulo="+prgsubmodulo,
   		async: false,
   		success: function(html){document.getElementById('td_pergunta').innerHTML = html;}
 		});
}
</script>

<form id="formulario" action="" method="post" name="formulario">
	<input id="buscar" type="hidden" name="buscar" value="1"/>
	<input id="xls" type="hidden" name="xls" value="0"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Dimensões :</td>
			<td width="80%">
				<select id="dimensao" name="dimensao" class="CampoEstilo" onchange="selecionaDimensao(this);">
					<option value="">Selecione</option>
					<option value="dimensao1">1. Indicadores e taxas</option>
					<option value="dimensao2">2. Distorção e aproveitamento</option>
					<option value="dimensao3">3. Ensino e aprendizagem</option>
					<option value="dimensao4">4. Gestão</option>
					<option value="dimensao5">5. Comunidade escolar</option>
					<option value="dimensao6">6. Infraestrutura</option>
					<option value="planoacao">7. Plano de ação</option>
				</select>
			</td>
		</tr>
	</table>
	<div id="div_filtros"><p align=center>Selecione uma dimensão</p></div>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	    <tr>
			<td class="SubTituloDireita" style="text-align:center;">
				<div style="width:100%;">
					<input id="rel" type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" />
					<input id="rel_xls" type="button" name="filtrar_xls" value="Visualizar XLS" onclick="exibirRelatorioXLS();" style="display: none;" />
				</div>
			</td>
		</tr>
	</table>
</form>