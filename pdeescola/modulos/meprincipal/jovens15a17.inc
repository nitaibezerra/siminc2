<?php
/*
if( $_POST['ajaxExclui'] && $_SESSION['memid']) {
	$sql = "DELETE FROM pdeescola.meatividade WHERE memid = ".$_SESSION['memid'];
	$del = $db->executar( $sql );
	$sqlUp = "UPDATE pdeescola.memaiseducacao SET memadesaopst = NULL, memdataadesaopst = NULL, memcpfadesaopst=NULL
			 WHERE memid = ".$_SESSION['memid'];
	$up    = $db->executar( $sqlUp );
	$sql2  = "DELETE FROM pdeescola.mealunoparticipante WHERE memid = ".$_SESSION['memid'];
	$del2  = $db->executar( $sql2 );
	$db->commit();
	die();
}
*/
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['memid'] || !$_SESSION['meentid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.\");
			location.href = \"pdeescola.php?modulo=melista&acao=E\";
		  </script>";
	exit;
}



include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$esdid = mePegarEstadoAtual( $docid );

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod IN (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) $somenteConsulta = false;
}

if($_REQUEST["submetido"] && ($_REQUEST["majquantidadeai"] || $_REQUEST["majquantidadeaf"]) ) {
	
	
	$majid = $db->pegaUm("select majid from pdeescola.mealunojovemparticipante WHERE memid = ".$_SESSION['memid']." and majano = ".$_SESSION["exercicio"]);
	
	if(!$majid) {
		$sql = "INSERT INTO pdeescola.mealunojovemparticipante(memid,majano,majquantidadeai,majquantidadeaf)
				   VALUES(".$_SESSION['memid'].", 
				   		  ".$_SESSION["exercicio"].", 
				   		  ".($_REQUEST["majquantidadeai"] > 0 ? "'".$_REQUEST["majquantidadeai"]."'" : 'null').", 
				   		  ".($_REQUEST["majquantidadeaf"] > 0 ? "'".$_REQUEST["majquantidadeaf"]."'" : 'null').")";
	} else {
		$sql = "UPDATE pdeescola.mealunojovemparticipante 
				SET 
					majquantidadeai = ".($_REQUEST["majquantidadeai"] > 0 ? "'".$_REQUEST["majquantidadeai"]."'" : 'null').",
					majquantidadeaf = ".($_REQUEST["majquantidadeaf"] > 0 ? "'".$_REQUEST["majquantidadeaf"]."'" : 'null')."
				WHERE majid = ".$majid;
	}

	$db->executar($sql);
	$db->commit();
	$db->sucesso('meprincipal/jovens15a17');
	
}

echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/jovens15a17&acao=A");

$titulo = "Mais Educação - Educação Integral";
$subtitulo = "Cadastro - Jovens 15 a 17 anos ";

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();


//busca dados da tela
$sql = "SELECT
			majquantidadeai, majquantidadeaf, majtotalmatricula1517
		FROM
			pdeescola.mealunojovemparticipante 
		WHERE
			memid = ".$_SESSION['memid']." AND
			majano = " . $_SESSION["exercicio"];
$dadosAlunoParticipante = $db->pegaLinha($sql);
if($dadosAlunoParticipante) extract($dadosAlunoParticipante);
			
?>
<script type="text/javascript">
	function salvarAlunado(){
		
		var totalAlunado = "<?=$majtotalmatricula1517?>";
		var vlrAi = document.formulario.majquantidadeai.value;
		var vlrAf = document.formulario.majquantidadeaf.value;
		
		if(!totalAlunado) totalAlunado = 0;
		if(!vlrAi) vlrAi = 0;
		if(!vlrAf) vlrAf = 0;
		
		if( (parseFloat(vlrAi) + parseFloat(vlrAf)) > parseFloat(totalAlunado) ){
			alert("A soma do Alunado Anos Iniciais e Anos Finais não pode ser maior que o Censo Escolar");
			return false;
		}
		
		document.formulario.submit();
	}
</script>

<form method="post" id="formulario" name="formulario">
<input type="hidden" name="submetido" value="1" />

<!-- (Início) Tabela que engloba todo o conteúdo -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<table align="center">
			<tr>
			<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
			<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
			<td>
			<center>
			<div id="div_mensagem_editar_alunado" style="display:<?=$displayTabelaAtividades?>;">
			<? if(!$somenteConsulta) { ?>
			<font color="#DF6161">
				<!-- 
				<b>Para editar o Alunado Participante não pode haver nenhuma atividade cadastrada e nenhuma opção de adesão ao PST pode ter sido realizada.</b>
				 -->
			</font>
			<? } ?>
			</div>
			</center>
			
				<table class="listagem"  cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td bgcolor="#E8E8E8" valign="middle" align="center" colspan="2" ><b>CENSO ESCOLAR</b></td>
					</tr>
					<tr>
						<td valign="middle" align="center"><b>Número de estudantes de 15 a 17 anos:</b></td>
						<td><b><?=$majtotalmatricula1517?></b></td>
					</tr>
				</table>
			
				<br>
				
				<!-- (Início) Tabela com o Alunado Participante -->
				<table width="40%" class="listagem" bgcolor="#e8e8e8" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td rowspan="3" valign="middle" align="center" width="10%"><b>Alunado Participante:</b></td>
						<td colspan="9" valign="middle" align="center" width="60%"><b>Mais Educação para Jovens de 15 a 17 anos no Ensino Fundamental</b></td>
					</tr>
					<tr>
						<!-- Ensino Fundamental -->
						<td valign="middle" align="center"><b>Anos Iniciais</b></td>
						<td valign="middle" align="center"><b>Anos Finais</b></td>
					</tr>
					<tr style="background-color:#FAFAFA;">
						<td valign="middle" align="center"><input type="text" class="normal" size="3" maxlength="3" onkeypress="return somenteNumeros(event);" id="majquantidadeai" name="majquantidadeai"  value="<?=$majquantidadeai?>" <?if(!$somenteConsulta){echo 'disabled';}?>></td>
						<td valign="middle" align="center"><input type="text" class="normal" size="3" maxlength="3" onkeypress="return somenteNumeros(event);" id="majquantidadeaf" name="majquantidadeaf"  value="<?=$majquantidadeaf?>" <?if(!$somenteConsulta){echo 'disabled';}?>></td>
					</tr>
				</table>
				
				
				<?if($somenteConsulta){?>
					<center>
						<br><input type="button" name="salvar" value="Salvar" onclick="salvarAlunado();" style="width:100px; cursor:pointer;">
					</center>
				<?}?>
				
				<br><br>
			
				<table width="45%" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td>
							<font>
								<b>
									Atividade Global: Projetos de Vida
									<br><br>
									Descrição:
									<br>
									Construir propostas de atividades com os jovens que propiciem trabalhos integrados entre diferentes áreas de conhecimento tendo como objetivo principal o de orientar a criação de espaço para: AUTORIA, CRIAÇÃO, PROTAGONISMO e AUTONOMIA do grupo de estudantes. Além disso, se espera que tais atividades permitam aos jovens lançar um olhar sobre suas trajetórias escolares, planejando e executando propostas de caráter investigativo bem como de organização de ações que lhe permitam prosseguir em seus estudos e realizar aproximações com o mundo do trabalho.
									<br><br>
									OBS: será disponibilizado material de orientação e sugestões de propostas de trabalho ao tutor responsável pela realização da atividade.
								</b>
							</font>
						</td>
					</tr>
				</table>
								
				
			</td>
		</tr>
	</table>

		
		</td>
	</tr>
</table>
<!-- (Fim) Tabela que engloba todo o conteúdo -->

</form>

