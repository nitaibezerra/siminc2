<?php

/*** Verifica se as variáveis de sessão do módulo estão corretamente setadas ***/
eabVerificaSessao();

if( $_REQUEST["submetido"] )
{
	if($_REQUEST["excluir"] != "1")
	{
		$_REQUEST["eelressarcido"] = ($_REQUEST["eelressarcido"]=='S') ? 't' : 'f';
		// insert
		if(!($_POST["eelid"]))
		
		{
			$sql = "INSERT INTO 
						pdeescola.eabequipelocal(eabid,
											     eelnome,
												 etoid,
											     eaeid,
												 eelidade,
												 tfoid,
												 eteid,
												 eelsexo,
												 eelressarcido)
						VALUES(".$_SESSION["eabid"].",
							   '".$_REQUEST["eelnome"]."',
							   ".$_REQUEST["etoid"].",
							   ".$_REQUEST["eaeid"].",
							   ".$_REQUEST["eelidade"].",
							   ".$_REQUEST["tfoid"].",
							   ".$_REQUEST["eteid"].",
							   '".$_REQUEST["eelsexo"]."',
							   '".$_REQUEST["eelressarcido"]."')";
		}
		// update
		else
		{
			$sql = "UPDATE
						pdeescola.eabequipelocal
					SET
						eelnome = '".$_REQUEST["eelnome"]."',
						etoid = ".$_REQUEST["etoid"].",
						eaeid = ".$_REQUEST["eaeid"].",
						eelidade = ".$_REQUEST["eelidade"].",
						tfoid = ".$_REQUEST["tfoid"].",
						eteid = ".$_REQUEST["eteid"].",
						eelsexo = '".$_REQUEST["eelsexo"]."',
						eelressarcido = '".$_REQUEST["eelressarcido"]."'
					WHERE
						eelid = ".$_REQUEST["eelid"];
		}
	}
	else
	{
		$sql = "DELETE FROM pdeescola.eabequipelocal WHERE eelid = ".$_REQUEST["eelid"];
	}
	
	$db->executar($sql);
	$db->commit();
	unset($_REQUEST["eelid"]);
	echo "<script>
			alert('Dados gravados com sucesso.');
			location.href = 'pdeescola.php?modulo=eabprincipal/equipe_local&acao=A';
		  </script>";
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$sql = "SELECT eaaid,eaadescricao FROM pdeescola.eabtipoanoanterior WHERE eaastatus = 'A'";
$anosAnteriores = $db->carregar($sql);

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

if((integer)$vPerfilCadEscolaAberta > 0)
{
	if(!$existeDocid)
	{
		if((integer)$_SESSION["exercicio"] == eabMaxProgramacaoExercicio())
		{
			// Recupera ou cria o docid.
			$docid = eabCriarDocumento( $_SESSION['entid'], $_SESSION['eabid'] );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Escola Aberta');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		}
		else
		{
			$docid = false;
		}
	}

	$esdid = eabPegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_EAB || (integer)$esdid == AGUARDANDO_CORRECAO_CADASTRAMENTO_EAB || !$anosAnteriores)
	{
		$somenteConsulta = false;
	}
}

echo "<br />";
echo montarAbasArray(carregaAbasEscolaAberta(), "/pdeescola/pdeescola.php?modulo=eabprincipal/equipe_local&acao=A");

$titulo = 'Escola Aberta - Equipe Local';
$subtitulo = '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório';

monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAberta();

if($_REQUEST["eelid"])
{
	$sql = "SELECT
				eelid,
				eelnome,
				eaeid,
				etoid,
				eelidade,
				tfoid,
				eteid,
				eelsexo,
				eelressarcido
			FROM
				pdeescola.eabequipelocal
			WHERE
				eelid = ".$_REQUEST["eelid"];
	$dados = $db->carregar($sql);
	// extrai os dados
	extract($dados[0]);
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/eabajax.js" type="text/javascript"></script>

<form method="post" id="formEquipeLocal" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="excluir" value="0" />
<input type="hidden" name="eelid" value="<?=$eelid?>" />

<table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="0" align="center" border="0">
	<tr>
		<td style="width:450px;">
			Nome Completo do Integrante
			<br />
			<input type="text" class="normal" style="width:400px;" maxlength="200" id="eelnome" name="eelnome" value="<?=$eelnome?>" />
			<img src="../imagens/obrig.gif" border="0">
		</td>
 		<td>
			Atividade Atual
			<br />
			<? 
			$sql = "SELECT
						eaeid as codigo,
						eaedescricao as descricao
					FROM
						pdeescola.eabtipoatividadeequipe
					WHERE
						eaestatus = 'A'
					ORDER BY
						eaedescricao ASC";
			$db->monta_combo("eaeid", $sql, 'S', "Selecione uma atividade", '', '', '', '400', 'S', '');
			?>
			<img src="/imagens/ajuda.gif" title="É necessário existir um Executor Oficineiro e mais um Coordenador ou Organização Pedagógica">
		</td>
		<td align="right" rowspan="5">
		<?
		$perfis = arrayPerfil();
		
		if($docid)
		{
			if( !in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ABERTA, $perfis) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
			{
				// Barra de estado atual e ações e Historico
				wf_desenhaBarraNavegacao( $docid , array( 'eabid' => $_SESSION['eabid'] ) );
			}
		}
		?>
		<table border="0" cellpadding="0" cellspacing="0"
			style="background-color: #f4f4f4; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
					 	<span title="Relatório do Plano de Atividades da Escola"> 
					 		<strong>Plano de Atividades da Escola</strong> 
					 	</span>
					</td>
				</tr>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a href="javascript:void(0);" onclick="abrePlanoAtividades();" title="Relatório do Plano de Atividades da Escola">Gerar Relatório</a>
					</td>
				</tr>
		</table>
	</tr>
	<tr>
		<td>
			Origem
			<br />
			<? 
			$sql = "SELECT
						etoid as codigo,
						etodescricao as descricao
					FROM
						pdeescola.eabtipoorigem
					WHERE
						etostatus = 'A'
					ORDER BY
						etodescricao ASC";
			$db->monta_combo("etoid", $sql, 'S', "Selecione uma origem", '', '', '', '400', 'S', '');
			?>
		</td>
		<td> 
			Idade
			<br />
			<input type="text" class="normal" style="width:50px;" maxlength="3" id="eelidade" name="eelidade" onkeypress="return somenteNumeros(event);" value="<?php echo $eelidade; ?>" />
			<img src="../imagens/obrig.gif" border="0">
		</td>
		
	</tr>
	
	<tr>
		<td>
			Formação
			<br />
			<? 
			$sql = "SELECT
						tfoid as codigo,
						tfodsc as descricao
					FROM
						public.tipoformacao
					WHERE
						tfostatus = 'A'
					ORDER BY
						oid ASC";
			$db->monta_combo("tfoid", $sql, 'S', "Selecione uma formação", '', '', '', '400', 'S', '');
			?>
		</td>
		<td>
			Tempo Total na Equipe do Programa
			<br />
			<? 
			$sql = "SELECT
						eteid as codigo,
						etedescricao as descricao
					FROM
						pdeescola.eabtempoequipe
					WHERE
						etestatus = 'A'";
			
			$db->monta_combo("eteid", $sql, 'S', "Selecione um tempo", '', '', '', '400', 'S', '');
			?>
		</td>
	</tr>
	<tr>
		<td>
			Sexo
			<br />
			<input type="radio" value="M" name="eelsexo" id="eelsexo" <?=(($eelsexo=='M' || !$eelsexo) ? 'checked="checked"' : '')?> /> Masculino
			&nbsp;&nbsp;&nbsp;
			<input type="radio" value="F" name="eelsexo" id="eelsexo" <?=(($eelsexo=='F') ? 'checked="checked"' : '')?> /> Feminino
			<img src="../imagens/obrig.gif" border="0">
		</td>
		<td>
			Ressarcido
			<br />
			<input type="radio" value="S" name="eelressarcido" id="eelressarcido" <?=(($eelressarcido=='t' || !$eelressarcido) ? 'checked="checked"' : '')?> /> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" value="N" name="eelressarcido" id="eelressarcido" <?=(($eelressarcido=='f') ? 'checked="checked"' : '')?> /> Não
			<img src="../imagens/obrig.gif" border="0">
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#c0c0c0" cellSpacing="5" cellPadding="0" align="center" border="0">
	<tr>
		<td align="left">
			<?php if( !$somenteConsulta ) { ?>
			<input type="button" value="Salvar" id="btSalvar" onclick="salvar();" />
			<?php } ?>
		</td>
	</tr>
</table>
</form>

<?

$sql = "SELECT
			'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterar(' || eel.eelid || ');\" />
			 <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluir(' || eel.eelid || ')\" />' as acao,
			eel.eelnome,
			eae.eaedescricao,
			eto.etodescricao,
			eel.eelidade,
			tfo.tfodsc,
			ete.etedescricao,
			CASE WHEN eel.eelsexo = 'M'
				 THEN 'Masculino'
				 ELSE 'Feminino'
			END as sexo,
			CASE WHEN eel.eelressarcido = 't'
				 THEN 'Sim'
				 ELSE 'Não'
			END as ressarcido
		FROM
			pdeescola.eabequipelocal eel
		INNER JOIN
			pdeescola.eabtipoatividadeequipe eae ON eae.eaeid = eel.eaeid
		INNER JOIN
			pdeescola.eabtipoorigem eto ON eto.etoid = eel.etoid
		INNER JOIN
			public.tipoformacao tfo ON tfo.tfoid = eel.tfoid
		INNER JOIN
			pdeescola.eabtempoequipe ete ON ete.eteid = eel.eteid
		WHERE
			eel.eabid = ".$_SESSION["eabid"];

$cabecalho 	 = array("Alterar/Excluir", "Nome do Integrante", "Atividade", "Origem", "Idade", "Formação", "Tempo na equipe do programa", "Sexo", "Ressarcido");
$tamanho	 = array('5%', '20%', '10%', '20%', '5%', '10%', '10%', '10%', '10%');
$alinhamento = array('center', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center');
$db->monta_lista($sql, $cabecalho, 20, 10, 'N', 'center', '', '', $tamanho, $alinhamento);

?>

<script type="text/javascript">

function salvar() {
	var form	=	document.getElementById("formEquipeLocal");
	var bt		=	document.getElementById("btSalvar");
	var nome	=	document.getElementById('eelnome');
	var ativ	=	document.getElementsByName('eaeid')[0];
	var origem	=	document.getElementsByName('etoid')[0];
	var idade	=	document.getElementById('eelidade');
	var formac	=	document.getElementsByName('tfoid')[0];
	var tempo	=	document.getElementsByName('eteid')[0];
	
	bt.disabled 	= true;
	
	if(nome.value == "") {
		alert('O campo "Nome" deve ser preenchido.');
		nome.focus();
		bt.disabled = false;
		return;
	}
	if(ativ.value == "") {
		alert('O campo "Atividade Atual" deverá ter um obrigatorimente um "Executor Oficineiro" mais um "Coordenador" ou "Organização Pedagógica".');
		ativ.focus();
		bt.disabled = false;
		return;
	}
	if(origem.value == "") {
		alert('O campo "Origem" deve ser selecionado.');
		origem.focus();
		bt.disabled = false;
		return;
	}
	if(idade.value == "") {
		alert('O campo "Idade" deve ser preenchido.');
		idade.focus();
		bt.disabled = false;
		return;
	}
	if (idade.value < 16 || idade.value > 99){
		alert('A Idade mínima deverá ser 16 anos e a idade máxima deverá ser 99 anos.');
		idade.focus();
		bt.disabled = false;
		return;
	}
	if(formac.value == "") {
		alert('O campo "Formação" deve ser selecionado.');
		formac.focus();
		bt.disabled = false;
		return;
	}
	if(tempo.value == "") {
		alert('O campo "Tempo na equipe do programa" deve ser selecionado.');
		tempo.focus();
		bt.disabled = false;
		return;
	}
	if(tempo.value == "") {
		alert('O campo "Tempo na equipe do programa" deve ser selecionado.');
		tempo.focus();
		bt.disabled = false;
		return;
	}
	
	form.submit();
}

function alterar(id)
{
	var form	=	document.getElementById("formEquipeLocal");
	var bt		=	document.getElementById("btSalvar");
	var submet	= 	document.getElementsByName('submetido')[0];
	var eelid	= 	document.getElementsByName('eelid')[0];
	if(bt){
		bt.disabled 	= true;
	}
	eelid.value		= id;
	submet.value 	= 0;
	form.submit();
}

function excluir(id)
{
	if( confirm("Deseja excluir este item?") )
	{
		var form	=	document.getElementById("formEquipeLocal");
		var bt		=	document.getElementById("btSalvar");
		var excluir	=	document.getElementsByName('excluir')[0];
		var eelid	= 	document.getElementsByName('eelid')[0];
		
		bt.disabled 	= true;
		excluir.value 	= 1;
		eelid.value		= id;
		form.submit();
	}
}

function abrePlanoAtividades(){
		windowOpen('pdeescola.php?modulo=eabrelatorio/relatorio_plano_atividade_escola&acao=A','fgfgf', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	}

</script>