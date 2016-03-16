<?php

if( $_POST['ajaxSalvarFds'] && $_SESSION['memid']) {
	
	if($_POST['aberturafds']){
		if($_POST['aberturafds'] == 'S'){
			$sqlUp = "UPDATE pdeescola.memaiseducacao SET mamescolaaberta = 't' WHERE memid = ".$_SESSION['memid'];
		}
		else{
			$sqlUp = "UPDATE pdeescola.memaiseducacao SET mamescolaaberta = 'f' WHERE memid = ".$_SESSION['memid'];
		}
		$db->executar( $sqlUp );
		$db->commit();
	}
	if($_POST['memjovem1517']){
		if($_POST['memjovem1517'] == 'S'){
			$sqlUp = "UPDATE pdeescola.memaiseducacao SET memjovem1517 = 't' WHERE memid = ".$_SESSION['memid'];
		}
		else{
			$sqlUp = "UPDATE pdeescola.memaiseducacao SET memjovem1517 = 'f' WHERE memid = ".$_SESSION['memid'];
		}
		$db->executar( $sqlUp );
		$db->commit();
	}
	die();
}

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

// Verifica se o 'memid' existe na sessão.
/*if(!$_SESSION['memid']) {
	echo "<script type=\"text/javascript\">
			alert(\"O cadastro de Diretor e Coordenador deve ser realizado para acessar esta aba.\");
			location.href = \"pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A\";
		  </script>";
	exit;
}*/

/*** Pode fazer o plano em 2011? ***/
if( (integer)$_SESSION["exercicio"] < 2013){
	

	if( $_SESSION["meentid"] )
	{
		$msgx = "";
		$existeAnoAnterior = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
		
		if($existeAnoAnterior)
		{
			
			$atividadesAnoAnterior = $db->carregarColuna("SELECT meacomecounoano FROM pdeescola.meatividade WHERE memid = ".$existeAnoAnterior[0]["memid"]." AND meaano = ".((integer)$_SESSION["exercicio"] - 1)." AND meacomecounoano is not null");
			
			if( count($atividadesAnoAnterior) > 0 )
			{
				$flagAnoAnterior = false;
				foreach($atividadesAnoAnterior as $comecouano)
				{
					if( $comecouano == 't' )
					{
						$flagAnoAnterior = true;
						break;
					}
				}
				
				if( $flagAnoAnterior )
				{
					$memvlrpago = $existeAnoAnterior[0]["memvlrpago"];
					if( $memvlrpago && (float)$memvlrpago > 0 )
					
					if( $existeAnoAnterior[0]["mempagofnde"] == 'f' )
					{
						$msgx = 'A Escola não tem direito ao plano corrrente de '.($_SESSION["exercicio"]);
					}
					
					
				}
				else
				{
					if( $existeAnoAnterior[0]["mempagofnde"] == 't' )
					{
						$msgx = 'A Escola não tem direito ao plano corrrente de '.($_SESSION["exercicio"]);
					}
				}
			}
			else
			{
				if( $existeAnoAnterior[0]["mempagofnde"] == 't' )
				{
					$msgx = 'É necessário responder a aba Atividades '.($_SESSION["exercicio"]-1);
				}
			}
		}
		
		if($msgx){
			?>
			<script type="text/javascript">
				alert("<?=$msgx?>");
				location.href = "pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A";
			</script>
			<?
			die;
		}
		
	}
	
}
/*******fim***********/
	
	
//if( !podeFazerPlano2011() )
//{
	?>
		<script type="text/javascript">
			//alert("A Escola não tem direito ao plano corrrente de <?//=$_SESSION["exercicio"]?>.");
			//location.href = "pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A";
		</script>
	<?
	/*
	echo "<script type=\"text/javascript\">
			alert(\"A Escola não tem direito ao plano corrrente de 2011.\");
			location.href = \"pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A\";
		  </script>";
	exit;
	*/
//}

$anoAnterior = date("Y") - 1;
	
/*
$val = verificaQuestionarioME( $_SESSION['memid'] );


$entidResp = $db->pegaUm( "select entid from pdeescola.memaiseducacao where entid = ".$_SESSION['meentid']." AND memanoreferencia = ".$anoAnterior." AND memstatus = 'A'" );
 

if( $_SESSION['exercicio'] > $anoAnterior && $val != 0 && $entidResp ){
	if( $val == 1000 ){
		echo '<script type="text/javascript">
				alert("Você não pode continuar até responder\nas perguntas da Aba Questionário de Monitoramento.");
				location.href = "pdeescola.php?modulo=meprincipal/questionario&acao=A";
			  </script>';
	} else {
		echo '<script type="text/javascript">
				alert("Você não pode continuar até responder as perguntas da Aba\nQuestionário de Monitoramento. Faltam '.$val.' perguntas.");
				location.href = "pdeescola.php?modulo=meprincipal/questionario&acao=A";
			  </script>';
	}
	exit;
}
*/



/*
 * Regra para atividades de 2010 que não foram pagas em 2009.
 * 23/11/2009
 * Felipe Carvalho
 */
$_SESSION["boAtivNaoPagas2009"] = false;

// regra escola do ano de 2010
if((integer)$_SESSION["exercicio"] == 2010) {
	$dadosEscola = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
	
	if($dadosEscola) {
		// existia em 2009 
		if(count($dadosEscola) > 0) {
			// se foi paga
			//if($dadosEscola[0]["memvlrpago"]) {
			if( $dadosEscola[0]["mempagofnde"] == 't' )
			{
				$ativAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola[0]["memid"]." AND meacomecounoano = 't'");
				
				// se não iniciou
				if((integer)$ativAnoAnterior == 0) {
					$_SESSION["boAtivNaoPagas2009"] = true;
				}
			}
			// se não foi paga em 2009
			else {
				// se não foi paga em 2009 mas existe em 2008, segue normal...
				$dadosEscola2008 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = 2008");
				
				if(!$dadosEscola2008) {
					$_SESSION["boAtivNaoPagas2009"] = false;
				}
			}
		}
	}
}
// fim regra escola do ano de 2010


include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';

// Recupera a modalidade de ensino.
// Tipos de modalidade de ensino: F - Fundamental, M - Médio e T - Todos.
$modalidadeEnsino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");


//pega quantidade de alunos por escola
//$qtdAlunos = $db->pegaUm("SELECT memquantidadealuno FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
//dbg("SELECT memquantidadealuno FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
//if($qtdAlunos > 100) $maisDeCem = "true";
//else $maisDeCem = "false";

//dbg($maisDeCem,1);

//U - URBANO, R - RURAL
$classificacaoEscola = $db->pegaUm("SELECT memclassificacaoescola FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
if(!$classificacaoEscola) $classificacaoEscola = "U";

/*
 $tplid = $db->pegaUm("SELECT tplid FROM entidade.entidade WHERE entid = ".$_SESSION['meentid']);
if($tplid == '2'){	
	$classificacaoEscola = "R";
}
else{
	$classificacaoEscola = "U";
}
 */
//$classificacaoEscola = "R";
//dbg($classificacaoEscola);
//dbg($maisDeCem);
 

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;


$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'";
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = mePegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) {
		$somenteConsulta = false;
	}
}

if($_REQUEST["submetido"]) {
	// Insere os valores dos Saldos
	/*** Retirado devido determinação do pessoal responsável pelo sistema ***/
	/*$saldoDisponivel = str_replace(",", ".", str_replace(".", "", $_REQUEST["saldo_reprogramavel_parcela"]));
	$saldoSuplementar = str_replace(",", ".", str_replace(".", "", $_REQUEST["saldo_reprogramavel_suplementar"]));
	
	$sql = "UPDATE pdeescola.memaiseducacao SET memvlrsaldodisponivel = ".$saldoDisponivel.",
												memvlrsaldosuplementar = ".$saldoSuplementar."
											WHERE
												memid = ".$_SESSION['memid'];
	$db->executar($sql);*/
	
	// Insere os valores do Alunado Participante
	//$existeAlunado = $db->pegaUm("SELECT count(*) FROM pdeescola.mealunoparticipante WHERE memid = ".$_SESSION['memid']." AND mapano = ".date('Y'));
	
	//insere valores capital e custeio
	$mesaldocapital = str_replace(",", ".", str_replace(".", "", $_REQUEST["mesaldocapital"]));
	$mesaldocusteio = str_replace(",", ".", str_replace(".", "", $_REQUEST["mesaldocusteio"]));
	
	if($mesaldocapital && $mesaldocusteio){
		$sql = "UPDATE pdeescola.memaiseducacao SET mesaldocapital = ".$mesaldocapital.",
													mesaldocusteio = ".$mesaldocusteio."
												WHERE
													memid = ".$_SESSION['memid'];
		$db->executar($sql);
	}
	
	
	// insere alunado
	$alunado = $_REQUEST["alunado_participante"];
	
	for($i=0; $i<=11; $i++) {
		$sceid = ($i<=8) ? ($i+1) : ($i+11);
		
		if($alunado[$i] != "") {
			$existeAlunado = $db->pegaUm("SELECT count(*) FROM pdeescola.mealunoparticipante WHERE memid = ".$_SESSION['memid']." AND mapano = " . $_SESSION["exercicio"] . " AND sceid = ".$sceid);
			
			if((integer)$existeAlunado > 0) {
				$sql = "UPDATE pdeescola.mealunoparticipante SET mapquantidade = ".$alunado[$i]." 
						WHERE memid = ".$_SESSION['memid']." AND mapano = " . $_SESSION["exercicio"] . " AND sceid = ".$sceid;
			} else {
				$sql = "INSERT INTO pdeescola.mealunoparticipante(memid,mapano,mapquantidade,sceid)
						   VALUES(".$_SESSION['memid'].", " . $_SESSION["exercicio"] . ", ".$alunado[$i].", ".$sceid.");";
			}
			$db->executar($sql);
		}
	}
	
	// Insere as Atividades cadastradas
	$sql = "DELETE FROM pdeescola.meatividade WHERE memid = ".$_SESSION['memid']." AND meaano = " . $_SESSION["exercicio"];
	$db->executar($sql);
	
	$atividade 	 = $_REQUEST["mtaid"];
	$localizacao = $_REQUEST["loc_atividade"];
	for($i=0; $i<count($atividade); $i++) {
		$sql = "INSERT INTO pdeescola.meatividade(memid, meaano, mtaid, mealocalizacao)
							VALUES(".$_SESSION['memid'].", " . $_SESSION["exercicio"] . ", ".$atividade[$i].", '".$localizacao[$i]."')
				RETURNING meaid";
		$meaid = $db->pegaUm($sql);
		
		$numAlunosAtividade = $_REQUEST["num_alunos_atividade"][$atividade[$i]];
		
		for($z=0; $z<=11; $z++) {
			$sceid = ($z<=8) ? ($z+1) : ($z+11);
			
			if($numAlunosAtividade[$z] != "") {
				$sql = "INSERT INTO pdeescola.mealunoparticipanteatividade(meaid, mpaquantidade, sceid)
											VALUES(".$meaid.", ".$numAlunosAtividade[$z].", ".$sceid.")";
				$db->executar($sql);
			}
		}
	}
	
	//dbg($_REQUEST["loc_atividade"]);
	$db->commit();
	$db->sucesso('meprincipal/atividades_ano_atual');
}

echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A");

$titulo = "Mais Educação - Educação Integral";
$subtitulo = "Cadastro - Atividades " . $_SESSION["exercicio"];

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();

// recupera a situação da escola com o programa Segundo Tempo
if( $_SESSION['exercicio'] == $anoAnterior )
{
	$memadesaopst = $db->pegaUm("SELECT memadesaopst FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
}
else
{
	$memadesaopst = $db->pegaUm("SELECT memadesaopst FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']);
}
if(!$memadesaopst) $memadesaopst = 'B';

//verifica pergunta para jovem15a17
$majtotalmatricula1517 = $db->pegaUm("select majtotalmatricula1517 from pdeescola.mealunojovemparticipante WHERE memid = ".$_SESSION['memid']." and majano = ".$_SESSION["exercicio"]);
?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>

<script src="../includes/prototype.js" type="text/javascript"></script>
<script src="./js/meajax.js" type="text/javascript"></script>
<script type="text/javascript">
	var contMacroCampos   = 0;
	var contAtividades    = 0;
	var arrMacroCampos	  = new Array();
	var modalidadeEnsino  = '<?=$modalidadeEnsino?>';
	var contAtividadesPST = 0;
	var ctAtividadeTecEduc = 0;
	var ctMacroCampoAgroecologia = 0;
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(700,600);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function displayStaticMessage(messageContent,cssClass){
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(400,250);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
		
		
	}
	
	function closeMessage(){
		messageObj.close();	
	}
	
	function salvarFds(){
	
		if(document.formAtividadesAnoAtual.aberturafds[0].checked == false && document.formAtividadesAnoAtual.aberturafds[1].checked == false){
			alert("Favor informar a pergunta 'Deseja ofertar atividades educativas, culturais, esportivas, de formação inicial para o trabalho e geração de renda aos alunos e suas famílias, visando fortalecer a integração entre escola e comunidade, ocupando criativamente o espaço escolar nos finais de semana?'!");
			document.formAtividadesAnoAtual.aberturafds[0].focus();
			return false;
		}
		
		var valor = "";
		if(document.formAtividadesAnoAtual.aberturafds[0].checked == true) valor = "S";
		if(document.formAtividadesAnoAtual.aberturafds[1].checked == true) valor = "N";
		
		
		var valor2 = "";
		<?if( (int)$majtotalmatricula1517 > 0){?>
			if(document.formAtividadesAnoAtual.memjovem1517[0].checked == false && document.formAtividadesAnoAtual.memjovem1517[1].checked == false){
				alert("Favor informar a pergunta 'Deseja ofertar atividade específica para jovens de 15 a 17 anos do ensino fundamental?'!");
				document.formAtividadesAnoAtual.memjovem1517[0].focus();
				return false;
			}
			
			if(document.formAtividadesAnoAtual.memjovem1517[0].checked == true) valor2 = "S";
			if(document.formAtividadesAnoAtual.memjovem1517[1].checked == true) valor2 = "N";
		<?}?>
		
		
		
		var req = new Ajax.Request('pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A', {
							        method:     'post',
							        parameters: '&ajaxSalvarFds=t&aberturafds='+valor+'&memjovem1517='+valor2,							         
							        onComplete: function (res)
							        {		 
										alert('Operação realizada com sucesso!');
										window.location.href = "pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A";
							        }
							        
							  });
	}
</script>

<form method="post" id="formAtividadesAnoAtual" name="formAtividadesAnoAtual">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" id="libera_validacao_alunado" value="1" />
<input type="hidden" id="requisitos_pst" value="0" />
<input type="hidden" id="segundo_tempo" value="<?=$memadesaopst?>" />
<input type="hidden" id="quantidade_alunos" value="" />
<!-- (Início) Tabela que engloba todo o conteúdo -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?
$sql = "SELECT
				mtaid
			FROM
				pdeescola.metipoatividade
			WHERE
				mtasituacao = 'A' AND
				mtaanoreferencia = " . $_SESSION["exercicio"] . " AND
				mtamodalidadeensino = '".$modalidadeEnsino."' AND 
				mtapst = 't'";
	$mtaidPST = $db->pegaUm($sql);
	
?><input type="hidden" name="mtaidPST" id="mtaidPST" value="<?=$mtaidPST?>" >
	<tr>
		<td>
		
			<?
			$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'";
			$possuiAnoAnterior = $db->pegaUm($sql);
			
			if((integer)$possuiAnoAnterior > 0) {
			?>
				<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td align="center" colspan="2" >
							<B>OUTRAS AÇÕES (Sugestões)</B>
						</td>
					</tr>					
					<?php $boMostraBtnSalvarAcao = false; ?>
					<?php $esdid = mePegarEstadoAtual( $docid );?>
					<?php if(!in_array($esdid, array(CADASTRAMENTO_ME, false))): ?>
						<?php $boMostraBtnSalvarAcao = true; ?>
						<tr>
							<td>
								Deseja ofertar atividades educativas, culturais, esportivas, de formação inicial para o trabalho e geração de renda aos alunos e suas famílias, visando fortalecer a integração entre escola e comunidade, ocupando criativamente o espaço escolar nos finais de semana?
							</td>
							<td width="15%">
								<?
								$mamescolaaberta = $db->pegaUm("SELECT mamescolaaberta FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
								?>
								<INPUT type="radio" name="aberturafds" value="S" <?if($mamescolaaberta == 't') echo 'checked';?>><b>SIM</b>
								&nbsp;&nbsp;&nbsp;
								<INPUT type="radio" name="aberturafds" value="N" <?if($mamescolaaberta == 'f') echo 'checked';?>><b>NÃO</b>				
							</td>
						</tr>
					<?php endif; ?>
					<?
					if( (int)$majtotalmatricula1517 > 0){
					?>
						<?php $boMostraBtnSalvarAcao = true; ?>
						<tr>
							<td>
								Sua escola possui jovens de 15 a 17 anos no Ensino Fundamental. O PME possui agora uma ação específica para esses jovens. Deseja oferecer essa ação?
							</td>
							<td nowrap="nowrap">
								<?
								$memjovem1517 = $db->pegaUm("SELECT memjovem1517 FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
								?>
								<INPUT type="radio" name="memjovem1517" value="S" <?if($memjovem1517 == 't') echo 'checked';?>><b>SIM</b>
								&nbsp;&nbsp;&nbsp;
								<INPUT type="radio" name="memjovem1517" value="N" <?if($memjovem1517 == 'f') echo 'checked';?>><b>NÃO</b>				
							</td>
						</tr>
					<?}?>
					<tr style="background-color:#FAFAFA;">
						<td align="right" colspan="2">
							<?if(!$somenteConsulta && $boMostraBtnSalvarAcao){?>
							<input type="button" name="salvar_fds" value="Salvar" onclick="salvarFds();">
							<?}?>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						</td>
					</tr>
				</table>
				<br />
			<?}?>
		
			<br />
			<?
				if($modalidadeEnsino == "F")
					$inSeries = "1,2,3,4,5,6,7,8,9";
				else if($modalidadeEnsino == "M")
					$inSeries = "20,21,22";
				else
					$inSeries = "1,2,3,4,5,6,7,8,9,20,21,22";
					
				$sql = "SELECT
							map.mapquantidade as qtd
						FROM
							pdeescola.mealunoparticipante map
						INNER JOIN
							pdeescola.seriecicloescolar sce ON sce.sceid = map.sceid
										AND sce.sceid IN (".$inSeries.")
						WHERE
							map.memid = ".$_SESSION['memid']." AND
							map.mapano = " . $_SESSION["exercicio"] . "
						ORDER BY
							map.sceid";
				$alunadoParticipante = $db->carregar($sql);
				
				$displayTabelaAlunado 	 = (!$alunadoParticipante) ? 'block' : 'none';
				$displayTabelaAtividades = (!$alunadoParticipante) ? 'none' : 'block';
				$displayTabelaValores = (!$alunadoParticipante) ? 'none' : 'block';
			?>
			<!-- Instruções -->
			
				<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td valign="middle" align="center" colspan="13"><b>Total do Alunado (Censo)</b></td>
					</tr>
					<tr>
						<!-- Ensino Fundamental -->
						<td valign="middle" align="center"><b>1° Ano</b></td>
						<td valign="middle" align="center"><b>2° Ano</b></td>
						<td valign="middle" align="center"><b>3° Ano</b></td>
						<td valign="middle" align="center"><b>4° Ano</b></td>
						<td valign="middle" align="center"><b>5° Ano</b></td>
						<td valign="middle" align="center"><b>6° Ano</b></td>
						<td valign="middle" align="center"><b>7° Ano</b></td>
						<td valign="middle" align="center"><b>8° Ano</b></td>
						<td valign="middle" align="center"><b>9° Ano</b></td>
						<!-- Ensino Médio -->
						<?if((int)$_SESSION["exercicio"] < 2013){ ?>
							<td valign="middle" align="center"><b>1° Ano</b></td>
							<td valign="middle" align="center"><b>2° Ano</b></td>
							<td valign="middle" align="center"><b>3° Ano</b></td>
						<?}?>
					</tr>
					<tr style="background-color:#FAFAFA;">
					<?
						for($i=0; $i<=11; $i++) {
							$vSerie = (($i>=0) && ($i<=8)) ? ($i + 1) : ($i + 11);
							
							$sql = "SELECT
										mecquantidadealunos
									FROM
										pdeescola.mecenso
									WHERE
										entid = '".$_SESSION["meentid"]."' AND
										mecanoreferencia = " . $_SESSION["exercicio"] . " AND
										mecserie = ".$vSerie."
									ORDER BY
										mecserie ASC";
							$quantidadeCenso = $db->carregar($sql);
							
							$qtdCenso = ($quantidadeCenso[0]["mecquantidadealunos"]) ? $quantidadeCenso[0]["mecquantidadealunos"] : 0;
							
							// Ensino Fundamental
							if($modalidadeEnsino == "F") {
								if(($i>=0) && ($i<=8)) {
									echo "<td valign=\"middle\" align=\"center\">".$qtdCenso." <input type=\"hidden\" class=\"normal\"  id=\"alunado_censo_".($i + 1)."\" name=\"alunado_censo[]\"  value=\"".$qtdCenso."\" /></td>";
								} else {
									if((int)$_SESSION["exercicio"] < 2013){
										echo "<td valign=\"middle\" align=\"center\">- <input type=\"hidden\" class=\"normal\"  id=\"alunado_censo_".($i + 1)."\" name=\"alunado_censo[]\"  value=\"".$qtdCenso."\" /></td>";
									}
								}
							}
							// Ensino Médio
							else if($modalidadeEnsino == "M") {
								if((int)$_SESSION["exercicio"] < 2013){
									if(($i>=9) && ($i<=11)) {
										echo "<td valign=\"middle\" align=\"center\">".$qtdCenso." <input type=\"hidden\" class=\"normal\"  id=\"alunado_censo_".($i + 1)."\" name=\"alunado_censo[]\"  value=\"".$qtdCenso."\" /></td>";
									} else {
										echo "<td valign=\"middle\" align=\"center\">- <input type=\"hidden\" class=\"normal\"  id=\"alunado_censo_".($i + 1)."\" name=\"alunado_censo[]\"  value=\"".$qtdCenso."\" /></td>";
									}
								}
							}
							// Ensino Fundamental e Médio
							else {
								echo "<td valign=\"middle\" align=\"center\">".$qtdCenso." <input type=\"hidden\" class=\"normal\"  id=\"alunado_censo_".($i + 1)."\" name=\"alunado_censo[]\"  value=\"".$qtdCenso."\" /></td>";
							}
						}
					?>
					</tr>
				</table>
			
			<br />
			<table align="center">
			<tr>
			<td>
				<div id="div_valores_pagos" style="display:<?=$displayTabelaValores?>;">
				<?
					$sql = "select sum(dshqtde) AS memvlrpago --dshcod as inep, sum(dshqtde) as totalescola 
							from painel.indicador i
							inner join painel.seriehistorica sh on sh.indid = i.indid
							inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
							inner join pdeescola.memaiseducacao me on me.entcodent = dsh.dshcod and memanoreferencia = ".($_SESSION['exercicio']-1)."
							where i.indid = 1420
							and sh.sehstatus <> 'I'
							and sh.dpeid = 1148 --2012
							and  me.entid = ".$_SESSION['meentid']."  ---entcodent
							group by dshcod";
					$vlrPago2013 = $db->pegaUm($sql);
					
					$sql = "select memvlrsaldodisponivel as saldo 
							from pdeescola.memaiseducacao 
							where memanoreferencia = ".($_SESSION['exercicio']-1)." 
							and entid = ".$_SESSION['meentid'];
					$mesaldototal = $db->pegaUm($sql);
		
					$sql = "SELECT
								mme.memvlrpago as memvlrprimeiraparcela,
								mme.memvlrsuplementar,
								mme.mempagofnde,
								mme2.memvlrsaldodisponivel,
								mme2.memvlrsaldosuplementar,
								--mme.mesaldototal,
								mme.mesaldocusteio,
								mme.mesaldocapital
							FROM
								pdeescola.memaiseducacao mme
							INNER JOIN
								pdeescola.memaiseducacao mme2 ON mme2.entid = ".$_SESSION['meentid']." AND
																 mme2.memanoreferencia = " . $_SESSION["exercicio"] . " AND 
																 mme2.memstatus = 'A'
							WHERE
								mme.entid = ".$_SESSION['meentid']." AND
								mme.memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND 
								mme.memstatus = 'A'";
					
					$valoresPagos = $db->carregar($sql);
					
					$saldo_reprogramavel_parcela = ($valoresPagos[0]["memvlrsaldodisponivel"]) ? number_format($valoresPagos[0]["memvlrsaldodisponivel"], 2, ",", ".") : "0,00";
					//$saldo_reprogramavel_suplementar = ($valoresPagos[0]["memvlrsaldosuplementar"]) ? number_format($valoresPagos[0]["memvlrsaldosuplementar"], 2, ",", ".") : "0,00";
					
					if( count( $perfis ) == 1 && pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO)){
						echo "<script>
								document.getElementById('btngravar').disabled = 1;
							  </script> 
						";				
						$consultaMunicipal = 1; 
					}else{
						$consultaMunicipal = 0;
					}
					
					$habil = ($somenteConsulta) ? 'N' : 'S';
					
					$validacaoSaldo = true;
					//if( ( !$valoresPagos[0]["memvlrprimeiraparcela"] && !$valoresPagos[0]["memvlrsuplementar"] ) || ($consultaMunicipal == 1) ) {
					//if( ( $valoresPagos[0]["mempagofnde"] == 'f' && !$valoresPagos[0]["memvlrsuplementar"] ) || ($consultaMunicipal == 1) ) {
					if( ( $valoresPagos[0]["mempagofnde"] == 'f' ) || ($consultaMunicipal == 1) ) {
						$habil = 'N';
						$validacaoSaldo = false;
					} else {
				?>
				<!-- Retirado -->
				<!--<center>
				<font color="#DF6161">
					<b>Preenchimento Obrigatório</b>
				</font>
				</center>-->
				<!-- Retirado -->
				<? } 
				
				if((integer)$possuiAnoAnterior > 0) {
			
					if((int)$_SESSION["exercicio"] < 2013){
						?>
						<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td></td>
								<td valign="middle" align="center"><b><?= ((integer)$_SESSION["exercicio"] - 1) ?></b></td>
								<!--<td valign="middle" align="center"><b>Saldo reprogramado</b></td>-->
							</tr>
							<tr>
								<td valign="middle" align="right">Valor&nbsp;Pago</td>
								<?php /*($valoresPagos[0]["memvlrprimeiraparcela"]) ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00"*/ ?>
								<td bgcolor="#FAFAFA"><input type="text" size="20" value="<?= ($valoresPagos[0]["mempagofnde"] == 't') ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>
								<!--<td bgcolor="#FAFAFA"><? echo campo_texto("saldo_reprogramavel_parcela", 'N', $habil, '', 20, '', '###.###.###,##', '', 'right', '', 0, '', '') ?></td>-->
							</tr>
							<tr style="display: none">
								<td valign="middle" align="right">Complementar</td>
								<td bgcolor="#FAFAFA"><input type="text" size="20" value="<?= ($valoresPagos[0]["memvlrsuplementar"]) ? number_format($valoresPagos[0]["memvlrsuplementar"], 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>
								<!--<td bgcolor="#FAFAFA"><? echo campo_texto("saldo_reprogramavel_suplementar", 'N', $habil, '', 20, '', '###.###.###,##', '', 'right', '', 0, '', '') ?></td>-->
							</tr>
						</table>
					<?}else{?>
						<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td></td>
								<td valign="middle" align="center"><b><?= ((integer)$_SESSION["exercicio"] - 1) ?></b></td>
								<!--<td valign="middle" align="center"><b>Saldo reprogramado</b></td>-->
							</tr>
							<tr>
								<td valign="middle" align="right">Valor&nbsp;Pago</td>
								<td bgcolor="#FAFAFA"><input type="text" size="20" value="<?= ($vlrPago2013) ? number_format($vlrPago2013, 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>
								<?php /*($valoresPagos[0]["memvlrprimeiraparcela"]) ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00"*/ ?>
								<!--<td bgcolor="#FAFAFA"><input type="text" size="20" value="<?//= ($valoresPagos[0]["mempagofnde"] == 't') ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>-->
								<!--<td bgcolor="#FAFAFA"><? //echo campo_texto("saldo_reprogramavel_parcela", 'N', $habil, '', 20, '', '###.###.###,##', '', 'right', '', 0, '', '') ?></td>-->
							</tr>
							<tr>
								<td valign="middle" align="right">Saldo</td>
								<td bgcolor="#FAFAFA"><input type="text" name="mesaldototal" id="mesaldototal" size="20" value="<?= ($mesaldototal) ? number_format($mesaldototal, 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>
							</tr>
							<tr>
								<td valign="middle" align="right">Valor&nbsp;Capital</td>
								<td bgcolor="#FAFAFA" nowrap="nowrap">
									<?
										$mesaldocapital = $valoresPagos[0]["mesaldocapital"] ? number_format($valoresPagos[0]["mesaldocapital"], 2, ",", ".") : "0,00";
										echo campo_texto("mesaldocapital", $habil, $habil, '', 19, '', '###.###.###,##', '', 'right', '', 0, 'id="mesaldocapital"', '')
									?>
								</td>
							</tr>
							<tr>
								<td valign="middle" align="right">Valor&nbsp;Custeio</td>
								<td bgcolor="#FAFAFA" nowrap="nowrap">
									<?
										$mesaldocusteio = $valoresPagos[0]["mesaldocusteio"] ? number_format($valoresPagos[0]["mesaldocusteio"], 2, ",", ".") : "0,00";
										echo campo_texto("mesaldocusteio", $habil, $habil, '', 19, '', '###.###.###,##', '', 'right', '', 0, 'id="mesaldocusteio"', '')
									?>
								</td>
							</tr>
						</table>
					<?}
					}?>	
				</div>
			</td>
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
			
			<!-- (Início) Tabela com o Alunado Participante -->
			<div align="center" id="div_instrucoes_alunado" style="display:<?=$displayTabelaAlunado?>;">
				<font color="#DF6161">
					<b>Informe abaixo o alunado que participará do programa em <?=$_SESSION["exercicio"]?>:</b>
				</font>
			</div>
				<?if((int)$_SESSION["exercicio"] < 2013){?>
					<table class="listagem" bgcolor="#e8e8e8" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td rowspan="3" valign="middle" align="center" width="10%"><b>Alunado Participante:</b></td>
							<td colspan="9" valign="middle" align="center" width="60%"><b>Ensino Fundamental</b></td>
							<td colspan="3" valign="middle" align="center" width="20%"><b>Ensino Médio</b></td>
							<td rowspan="2" valign="middle" align="center" width="10%"><b>Total</b></td>
						</tr>
						<tr>
							<!-- Ensino Fundamental -->
							<td valign="middle" align="center"><b>1° Ano</b></td>
							<td valign="middle" align="center"><b>2° Ano</b></td>
							<td valign="middle" align="center"><b>3° Ano</b></td>
							<td valign="middle" align="center"><b>4° Ano</b></td>
							<td valign="middle" align="center"><b>5° Ano</b></td>
							<td valign="middle" align="center"><b>6° Ano</b></td>
							<td valign="middle" align="center"><b>7° Ano</b></td>
							<td valign="middle" align="center"><b>8° Ano</b></td>
							<td valign="middle" align="center"><b>9° Ano</b></td>
							<!-- Ensino Médio -->
							<td valign="middle" align="center"><b>1° Ano</b></td>
							<td valign="middle" align="center"><b>2° Ano</b></td>
							<td valign="middle" align="center"><b>3° Ano</b></td>
						</tr>
						<tr style="background-color:#FAFAFA;">
						<?
							//$vDisabled = ($alunadoParticipante) ? "disabled=\"disabled\"" : "";
							
							//$vDisabled = ($somenteConsulta) ? "disabled=\"disabled\"" : $vDisabled;
							
							$somaAlunado = 0;
							for($i=0; $i<=11; $i++) {
								// Ensino Fundamental
								if($modalidadeEnsino == "F") {
									if(($i>=0) && ($i<=8)) {
										$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" maxlength=\"3\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"calculaTotalAlunado();\" id=\"alunado_participante_".($i + 1)."\" name=\"alunado_participante[]\"  value=\"".$alunadoParticipante[$i]["qtd"]."\" ".$vDisabled." /></td>";
									} else {
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" name=\"alunado_participante[]\" style=\"background-color:#C0C0C0;\" value=\"\" disabled=\"disabled\" /></td>";
									}
								}
								// Ensino Médio
								else if($modalidadeEnsino == "M") {
									if(($i>=9) && ($i<=11)) {
										$somaAlunado += (integer)$alunadoParticipante[($i - 9)]["qtd"];
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" maxlength=\"3\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"calculaTotalAlunado();\" id=\"alunado_participante_".($i + 11)."\" name=\"alunado_participante[]\"  value=\"".$alunadoParticipante[($i - 9)]["qtd"]."\" ".$vDisabled." /></td>";
									} else {
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" name=\"alunado_participante[]\" style=\"background-color:#C0C0C0;\" value=\"\" disabled=\"disabled\" /></td>";
									}
								}
								// Ensino Fundamental e Médio
								else {
									$idCampo = (($i>=0) && ($i<=8)) ? "id=\"alunado_participante_".($i + 1)."\"" : "id=\"alunado_participante_".($i + 11)."\"";
									
									$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
									echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" maxlength=\"3\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"calculaTotalAlunado();\" ".$idCampo." name=\"alunado_participante[]\"  value=\"".$alunadoParticipante[$i]["qtd"]."\" ".$vDisabled." /></td>";
								}
							}
							
							echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"4\" disabled=\"disabled\" id=\"total_alunado_participante\" value=\"".$somaAlunado."\" /></td>";
							echo "<script> document.getElementById('quantidade_alunos').value = ".$somaAlunado."; </script>";
						?>
						</tr>
					</table>
					
				<?}else{?>
				
					<table class="listagem" bgcolor="#e8e8e8" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td rowspan="3" valign="middle" align="center" width="10%"><b>Alunado Participante:</b></td>
							<td colspan="9" valign="middle" align="center" width="60%"><b>Ensino Fundamental</b></td>
							<td rowspan="2" valign="middle" align="center" width="10%"><b>Total</b></td>
						</tr>
						<tr>
							<!-- Ensino Fundamental -->
							<td valign="middle" align="center"><b>1° Ano</b></td>
							<td valign="middle" align="center"><b>2° Ano</b></td>
							<td valign="middle" align="center"><b>3° Ano</b></td>
							<td valign="middle" align="center"><b>4° Ano</b></td>
							<td valign="middle" align="center"><b>5° Ano</b></td>
							<td valign="middle" align="center"><b>6° Ano</b></td>
							<td valign="middle" align="center"><b>7° Ano</b></td>
							<td valign="middle" align="center"><b>8° Ano</b></td>
							<td valign="middle" align="center"><b>9° Ano</b></td>
						</tr>
						<tr style="background-color:#FAFAFA;">
						<?
							//$vDisabled = ($alunadoParticipante) ? "disabled=\"disabled\"" : "";
							
							//$vDisabled = ($somenteConsulta) ? "disabled=\"disabled\"" : $vDisabled;
							
							$somaAlunado = 0;
							for($i=0; $i<=8; $i++) {
								// Ensino Fundamental
								if($modalidadeEnsino == "F") {
									if(($i>=0) && ($i<=8)) {
										$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" maxlength=\"3\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"calculaTotalAlunado();\" id=\"alunado_participante_".($i + 1)."\" name=\"alunado_participante[]\"  value=\"".$alunadoParticipante[$i]["qtd"]."\" ".$vDisabled." /></td>";
									} else {
										echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" name=\"alunado_participante[]\" style=\"background-color:#C0C0C0;\" value=\"\" disabled=\"disabled\" /></td>";
									}
								}
								// Ensino Fundamental e Médio
								else {
									$idCampo = (($i>=0) && ($i<=8)) ? "id=\"alunado_participante_".($i + 1)."\"" : "id=\"alunado_participante_".($i + 11)."\"";
									
									$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
									echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"3\" maxlength=\"3\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"calculaTotalAlunado();\" ".$idCampo." name=\"alunado_participante[]\"  value=\"".$alunadoParticipante[$i]["qtd"]."\" ".$vDisabled." /></td>";
								}
							}
							
							echo "<td valign=\"middle\" align=\"center\"><input type=\"text\" class=\"normal\" size=\"4\" disabled=\"disabled\" id=\"total_alunado_participante\" value=\"".$somaAlunado."\" /></td>";
							echo "<script> document.getElementById('quantidade_alunos').value = ".$somaAlunado."; </script>";
						?>
						</tr>
					</table>
					
				<?}?>
			</td>
			<td align="left" valign="bottom">
				<div id="div_editar_alunado">
				<? if(!$somenteConsulta) { ?>
					<a href="javascript:void(0);" onclick="editarAlunadoParticipante();">Editar Alunado</a>
				<? } ?>
				</div>
			</td>
			</tr>
			</table>
			<br />
			<div align="center" id="div_botao_alunado" style="display:<?=$displayTabelaAlunado?>;">
				<?if(!$somenteConsulta){?>
				<input type="button" style="width:100px; cursor:pointer;" id="botao_alunado" onclick="informaAlunadoParticipante();" value="OK" />
				<?}?>
			</div>
			<!-- (Fim) Tabela com o Alunado Participante -->
			<br /><br />
			<!-- (Início) DIV da tabela de atividades -->
			<div align="center" id="div_tabela_atividades" style="display:<?=$displayTabelaAtividades?>;">
			<!-- Instruções -->
			<div align="center">
				<font color="#DF6161">
					<?
					if( (integer)$_SESSION["exercicio"] < 2013){
						
						if($classificacaoEscola == 'U'){
							echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas. (Máximo de 4 Macrocampos e 6 Atividades)<br />Informe nas quadrículas abaixo o número de alunos que participará em cada Atividade.</b>";
						}else{
							echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas. (Máximo de 4 Macrocampos e 4 Atividades)<br />Informe nas quadrículas abaixo o número de alunos que participará em cada Atividade.</b>";
						}
						
					}
					else{
						
						if($classificacaoEscola == 'U'){
							if((integer)$possuiAnoAnterior > 0) {
								echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas (4 atividades). Caso queira optar por uma 5ª atividade, essa será, obrigatoriamente,<br> 'Esporte na Escola/Atletismo e múltiplas vivências esportivas (basquete, futebol, futsal, handebol, voleibol e xadrez)'.</b>";
							}
							else{
								echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas. (Mínimo de 3 Macrocampos, com 5 ou 6 Atividades).</b>";
							}
							//echo "<br><br><b>O alunado participante informado pela escola deverá ser o mesmo, obrigatoriamente, em todas as atividades.</b>";
							echo "<br><br><b>* Escolas que compõem o Pacto Nacional da Alfabetização na idade certa devem contemplar estudantes dos anos iniciais nas atividades do PME envolvendo as atividades de Alfabetização.</b>";
						}else{
							/*
							if((integer)$possuiAnoAnterior > 0) {
								echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas (4 atividades). Caso queira optar por uma 5ª atividade, essa será, obrigatoriamente,<br> 'Esporte na Escola/Atletismo e múltiplas vivências esportivas (basquete, futebol, futsal, handebol, voleibol e xadrez)'.</b>";
							}
							else{
								echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas. (Máximo de 4 Macrocampos e 4 Atividades).</b>";
							}
							*/
							echo "<b>Escolha os Macrocampos e as Atividades que serão desenvolvidas. (Máximo de 4 Macrocampos e 4 Atividades).</b>";
							echo "<br><br><b>O alunado participante informado pela escola deverá ser o mesmo, obrigatoriamente, em todas as atividades.</b>";
						}
						
					}
					?>
				</font>
			</div>
			<br />
			<!-- (Início) Tabela com as informações sobre as Atividades -->
			<table id="tabela_atividades" width="100%" align="center" border="0" cellspacing="2" cellpadding="5" class="listagem">
				<thead>
					<tr>
						<td width="30%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Macrocampo/Atividade</strong></td>
						<td width="30%" colspan="9" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ensino Fundamental</strong></td>
						<?if( (integer)$_SESSION["exercicio"] < 2013){ ?>
						<td width="10%" colspan="3" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ensino Médio</strong></td>
						<?}?>
						<td width="5%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Total</strong></td>
						<td width="20%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Localização da Atividade</strong></td>
						<td width="5%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
					<tr>
						<!-- Ensino Fundamental -->
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>1°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>3°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>4°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>5°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>6°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>7°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>8°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>9°</strong></td>
						<!-- Ensino Médio -->
						<?if( (integer)$_SESSION["exercicio"] < 2013){ ?>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>1°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>3°</strong></td>
						<?}?>
					</tr>
				</thead>				
				<tbody>
					<tr style="background-color:#C0C0C0;">
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;">
							
							<select name="macrocampo" id="macrocampo" style="width:200px;" class="CampoEstilo" onchange="comboAtividade();" <? if($somenteConsulta) echo "disabled=\"disabled\""; ?>>
								<option value="">-- Selecione o Macrocampo --</option>
								<?
									if( (integer)$_SESSION["exercicio"] < 2013){
										$whEsporteLazer = ($_SESSION["boAtivNaoPagas2009"]) ? " AND UPPER(mtm.mtmdescricao) like '%ESPORTE E LAZER%' " : "";
										$whRural = ($classificacaoEscola == 'R') ? " AND mtm.mtmid not in (43,47,48,49,50,51) " : " AND mtm.mtmid not in (52,53,54) ";
									}
									else{
										$whRural = ($classificacaoEscola == 'R') ? " AND mtm.mtmid not in (56,58,64,65) " : " AND mtm.mtmid not in (60,62,63) ";
									}
									
									
									
									if( (integer)$possuiAnoAnterior > 0 ) {
										//$whAno2013 = " AND mtm.mtmid >= 64 or mtm.mtmid in (55) ";
										$whAno2013 = " AND mtm.mtmid not in (58) ";
									}
									else{
										$whAno2013 = " AND mtm.mtmid not in (64,65) ";
									}
									
									
									$sql = "SELECT
												mtm.mtmid AS codigo, mtm.mtmdescricao AS descricao
											FROM
												pdeescola.metipomacrocampo mtm
											WHERE
												mtm.mtmsituacao = 'A' AND
												mtm.mtmanoreferencia = " . $_SESSION["exercicio"] . " 
												".$whEsporteLazer.$whRural.$whAno2013."
											ORDER BY 2";
									$selMacroCampo = $db->carregar($sql);
									
									for($i=0; $i<count($selMacroCampo); $i++) {
										echo "<option value=\"".$selMacroCampo[$i]["codigo"]."\">".$selMacroCampo[$i]["descricao"]."</option>";
									}
								?>
							</select>
							
							<br />
							<div id="combo_atividade">
								<select name="atividade" id="atividade" style="width:200px;" class="CampoEstilo" onchange="alertAtividade(this);" <? if($somenteConsulta) echo "disabled=\"disabled\""; ?>>
									<option value="">-- Selecione a Atividade --</option>
								</select>
							</div>
						</td>
						<?
							$vDisEnsFundamental = ($modalidadeEnsino == "M") ? "style=\"background-color:#C0C0C0;\" disabled=\"disabled\"" : "";
							$vDisEnsMedio		= ($modalidadeEnsino == "F") ? "style=\"background-color:#C0C0C0;\" disabled=\"disabled\"" : "";
						?>
						<!-- Ensino Fundamental -->
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]" value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsFundamental?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<!-- Ensino Médio -->
						<?if( (integer)$_SESSION["exercicio"] < 2013){ ?>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsMedio?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsMedio?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="5" maxlength="5" onkeypress="return somenteNumeros(event);" onkeyup="calculaTotalAtividade();" name="num_alunos[]"  value="" <?=$vDisEnsMedio?> <? if($somenteConsulta) echo "disabled=\"disabled\""; ?> /></td>
						<?}?>
						<!-- Total -->
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;"><input type="text" class="normal" size="8" id="total_num_alunos" value="" disabled="disabled" /></td>
						<!-- Localização da Atividade -->
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;">
							<select class="CampoEstilo" id="localizacao_atividade" style="width:110px;" <? if($somenteConsulta) echo "disabled=\"disabled\""; ?>>
								<option value="">-- Selecione --</option>
								<option value="E">Externa</option>
								<option value="I">Interna</option>
							</select>
						</td>
						<!-- Ação -->
						<td valign="middle" align="center" style="border-bottom: 1px solid #000000;">
							<? if(!$somenteConsulta) { ?>
							<img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="insereAtividade();" title="Inclui uma nova atividade">
							<? } else {?>
							<img src="/imagens/gif_inclui_d.gif" style="cursor:pointer;">
							<? } ?>
						</td>
					</tr>
					<?
						$sql = "SELECT DISTINCT
									mtm.mtmid
								FROM
									pdeescola.meatividade mea
								INNER JOIN
									pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
												AND mta.mtasituacao = 'A'
								INNER JOIN
									pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
												AND mtm.mtmsituacao = 'A'
								WHERE
									mea.memid = ".$_SESSION['memid']." AND
									mea.meaano = " . $_SESSION["exercicio"] . "";
						$nMacroCampos = $db->carregarColuna($sql);
						
						if($nMacroCampos) {
							for($i=0; $i<count($nMacroCampos); $i++) {
								echo "<script>
										contMacroCampos++;
										arrMacroCampos.push(".$nMacroCampos[$i].");
									  </script>";
							}
						}
						
						$sql = "SELECT
									mea.meaid,
									mea.mealocalizacao,
									mtm.mtmid,
									mtm.mtmdescricao,
									mta.mtaid,
									mta.mtadescricao
								FROM
									pdeescola.meatividade mea
								INNER JOIN
									pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
												AND mta.mtasituacao = 'A'
								INNER JOIN
									pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
												AND mtm.mtmsituacao = 'A'
								WHERE
									mea.memid = ".$_SESSION['memid']." AND
									mea.meaano = " . $_SESSION["exercicio"] . "
								ORDER BY
									mea.meaid";
						//dbg($sql,1);
						
						$dadosAtividades = $db->carregar($sql);
						
						if($dadosAtividades) {
							
							echo "<script> 
									contAtividades  = ".count($dadosAtividades).";
								 </script>";
							
							$somaGeralAtividades = array();
							for($i=0; $i<count($dadosAtividades); $i++) {
								$bgColor = ($i % 2) ? "#E8E8E8" : "#FAFAFA";
								
								//conta a quantidade de atividades (Tecnologias Educacionais) independente dos macrocampos
								if($dadosAtividades[$i]["mtadescricao"] == 'Tecnologias Educacionais'){
									echo "<script> 
											ctAtividadeTecEduc++;
								 		</script>";
								}
								
								//conta a quantidade de macrocampos AGROECOLOGIA
								if($dadosAtividades[$i]["mtmid"] == 52 || $dadosAtividades[$i]["mtmid"] == 62){
									echo "<script> 
											ctMacroCampoAgroecologia++;
								 		</script>";
								}
								
								echo "<tr style=\"background-color:".$bgColor.";\">
										<td valign=\"middle\" align=\"center\">
											<input type=\"hidden\" name=\"mtmid[]\" value=\"".$dadosAtividades[$i]["mtmid"]."\" />
											<input type=\"hidden\" name=\"mtaid[]\" value=\"".$dadosAtividades[$i]["mtaid"]."\" />
											".$dadosAtividades[$i]["mtmdescricao"]." / ".$dadosAtividades[$i]["mtadescricao"]."
										</td>";
								
								$sql = "SELECT
											map.mpaquantidade
										FROM
											pdeescola.mealunoparticipanteatividade map
										WHERE
											map.meaid = ".$dadosAtividades[$i]["meaid"]." AND 
											map.sceid IN (".$inSeries.")
										ORDER BY
											map.sceid";
								$numAlunosAtividade = $db->carregar($sql);
								
								if( (integer)$_SESSION["exercicio"] < 2013){
									$totalz = 11;
								}
								else{
									$totalz = 8;
								}
								
								
								$somaAtividade = 0;
								for($z=0; $z<=$totalz; $z++) {
									// Ensino Fundamental
									if($modalidadeEnsino == "F") {
										if(($z>=0) && ($z<=8)) {
											$somaAtividade += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
											$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
											echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[$z]["mpaquantidade"];
											echo "<input type=\"hidden\" name=\"num_alunos_atividade[".$dadosAtividades[$i]["mtaid"]."][]\" value=\"".$numAlunosAtividade[$z]["mpaquantidade"]."\" />";
											echo "</td>";
										} else {
											$somaGeralAtividades[$z] = "-";
											echo "<td valign=\"middle\" align=\"center\">-";
											echo "<input type=\"hidden\" name=\"num_alunos_atividade[".$dadosAtividades[$i]["mtaid"]."][]\" value=\"\" />";
											echo "</td>";
										}
									}
									// Ensino Médio
									else if($modalidadeEnsino == "M") {
										if(($z>=9) && ($z<=11)) {
											$somaAtividade += (integer)$numAlunosAtividade[($z-9)]["mpaquantidade"];
											$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[($z-9)]["mpaquantidade"];
											echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[($z-9)]["mpaquantidade"];
											echo "<input type=\"hidden\" name=\"num_alunos_atividade[".$dadosAtividades[$i]["mtaid"]."][]\" value=\"".$numAlunosAtividade[($z-9)]["mpaquantidade"]."\" />";
											echo "</td>";
										} else {
											$somaGeralAtividades[$z] = "-";
											echo "<td valign=\"middle\" align=\"center\">-";
											echo "<input type=\"hidden\" name=\"num_alunos_atividade[".$dadosAtividades[$i]["mtaid"]."][]\" value=\"\" />";
											echo "</td>";
										}
									}
									// Ensino Fundamental e Médio
									else {
										$somaAtividade += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
										$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
										echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[$z]["mpaquantidade"];
										echo "<input type=\"hidden\" name=\"num_alunos_atividade[".$dadosAtividades[$i]["mtaid"]."][]\" value=\"".$numAlunosAtividade[$z]["mpaquantidade"]."\" />";
										echo "</td>";
									}
								}
								
								echo "<td valign=\"middle\" align=\"center\">".$somaAtividade."</td>";
								$somaGeralAtividades[12] += (integer)$somaAtividade;
								
								$descLocalizacao = ($dadosAtividades[$i]["mealocalizacao"] == "E") ? "Externa" : "Interna";
								
								if(!$somenteConsulta) {
									$vBotoes = "<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraAtividade(this.parentNode.parentNode.rowIndex, ".$dadosAtividades[$i]["mtmid"].", ".$dadosAtividades[$i]["mtaid"].", '".$dadosAtividades[$i]["mealocalizacao"]."','".$dadosAtividades[$i]["mtadescricao"]."');\" title=\"Alterar a atividade\">
							   					<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiAtividade(this.parentNode.parentNode.rowIndex, ".$dadosAtividades[$i]["mtmid"].",'".$dadosAtividades[$i]["mtadescricao"]."');\" title=\"Excluir a atividade\">";
								} else {
									$vBotoes = "<img src=\"/imagens/alterar_01.gif\" style=\"cursor:pointer;\">
							   					<img src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer;\">";
								}
								
								echo "<td valign=\"middle\" align=\"center\">
											".$descLocalizacao."
											<input type=\"hidden\" name=\"loc_atividade[]\" value=\"".$dadosAtividades[$i]["mealocalizacao"]."\" />
									  </td>
									  <td valign=\"middle\" align=\"center\">
									  		".$vBotoes."
							   				<input type=\"hidden\" name=\"meaid[]\" value=\"".$dadosAtividades[$i]["meaid"]."\" />
									  </td>
									</tr>";
							}
							
							// Imprime a linha com os Totais
							/*echo "<tr id=\"total_atividades\" style=\"background-color:#C0C0C0;\">
									<td valign=\"middle\" align=\"right\"><b>Total:</b></td>";
							
							for($i=0; $i<count($somaGeralAtividades); $i++) {
								echo "<td valign=\"middle\" align=\"center\">".$somaGeralAtividades[$i]."</td>";
							}
							
							echo "<td valign=\"middle\" align=\"center\">-</td>
								  <td valign=\"middle\" align=\"center\">-</td>
								</tr>";*/
						}
					?>
				</tbody>
				<tfoot>
					<tr>
						<td colspan="17" align="right">
							<? if(!$somenteConsulta) { ?>
							<!--<div id="bt_fazer_adesao" style="float:right;display:none;">
								<input type="button" id="bt_faz_adesao" style="cursor:pointer;" onclick="desfazerEscolhaAdesao();" value="Fazer Adesão ao PST" />
							</div>
							<div id="bt_desfazer_adesao" style="float:right;display:none;">
								<input type="button" id="bt_des_adesao" style="cursor:pointer;" onclick="desfazerEscolhaAdesao();" value="Desfazer Adesão ao PST" />
							</div>-->
							<div style="float:right;">
								<input type="button" id="bt_voltar" style="width:100px; cursor:pointer;" onclick="history.back(-1);" value="Voltar" />
								<input type="button" id="bt_salvar" style="width:100px; cursor:pointer;" onclick="submeteAtividades();" value="Salvar" />
							</div>
							<? } ?>
						</td>
					</tr>
				</tfoot>
			</table>
			<!-- (Fim) Tabela com as informações sobre as Atividades -->
			<? $esdid = mePegarEstadoAtual( $docid );
			if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) { 
		 	?>
				<input type="button" name="btnExcluirAtividade" id="btnExcluirAtividade" value="Excluir Atividades" onclick="excluirAtividade();">
			<? } ?></div>
			<!-- (Fim) DIV da tabela de atividades -->
		</td>
	</tr>
</table>
<!-- (Fim) Tabela que engloba todo o conteúdo -->
</form>

<script type="text/javascript"><!--

<?
/*if(!$somenteConsulta) {
	if($memadesaopst=='S')
		echo "document.getElementById('bt_desfazer_adesao').style.display = \"block\";";
	elseif($memadesaopst=='N')
		echo "document.getElementById('bt_fazer_adesao').style.display = \"block\";";
}*/
?>

<? if(!$_SESSION["boAtivNaoPagas2009"]) { ?>
selecionaAcompPedagogico();
<? } ?>

exibeEditarAlunado();

selecionaAcompPedagogico();

function selecionaAcompPedagogico() {
	if(contAtividades == 0) {
		var selMacroCampo = document.getElementById("macrocampo");
		var index;
		
		for(var i=0; i<selMacroCampo.length; i++) {
			if(selMacroCampo[i].value == 22 || selMacroCampo[i].value == 32 || selMacroCampo[i].value == 42 || selMacroCampo[i].value == 55) 	
				selMacroCampo.selectedIndex = selMacroCampo[i].index;
		}
		
		selMacroCampo.disabled = true;
		comboAtividade();
	}
}

/**
 * Função para verificar se a opção de 'Editar Alunado' deve ser exibida.
 *
 * @return void
 *
 * Since: 27/04/2009
 */
function exibeEditarAlunado() {
	var divEditarAlunado 	= document.getElementById("div_editar_alunado");
	var divMsgEditarAlunado = document.getElementById("div_mensagem_editar_alunado");
	var divTabelaAtividades	= document.getElementById("div_tabela_atividades");

	var segundoTempo 		= document.getElementById('segundo_tempo');
	
	if((divTabelaAtividades.style.display == 'block') && (contAtividades == 0)) {
		if(segundoTempo.value == "B") {
			divEditarAlunado.style.display = "block";
			divMsgEditarAlunado.style.display = "none";
		}
	} else if((divTabelaAtividades.style.display == 'block') && (contAtividades > 0)) { 
		divEditarAlunado.style.display = "none";
		divMsgEditarAlunado.style.display = "block";
	} else {
		divEditarAlunado.style.display = "none";
		divMsgEditarAlunado.style.display = "none";
	}
}

/**
 * Função para excluir uma atividade cadastrada.
 *
 * @return void
 *
 * Since: 28/04/2009
 */
function excluiAtividade(index, macrocampo, txtAtividade) {
	var tabela 				= document.getElementById("tabela_atividades");
	var mtmid				= document.getElementsByName("mtmid[]");
	var existeMacroCampo 	= false;
	
	tabela.deleteRow(index);
	contAtividades--;

	if(txtAtividade == 'Tecnologias Educacionais') ctAtividadeTecEduc--;
	if(macrocampo == '44' || macrocampo == '59') contAtividadesPST--;
	
	for(var i=0; i<mtmid.length; i++) {
		if(mtmid[i].value == macrocampo) existeMacroCampo = true;
	}
	
	if(!existeMacroCampo) {
		for(var i=0; i<arrMacroCampos.length; i++)
			if(arrMacroCampos[i] == macrocampo) {
				arrMacroCampos.splice(i,1);
				contMacroCampos--;
				break;
			}
	}
	
	/*if(contAtividades == 0) {
		var trTotalAtividades = document.getElementById('total_atividades');
		tabela.deleteRow(trTotalAtividades.rowIndex);
	} else {
		atualizaTotalAtividades();
	}*/
	
	exibeEditarAlunado();
	
	selecionaAcompPedagogico();
}

/**
 * Função para alterar uma atividade cadastrada.
 *
 * @return void
 *
 * Since: 28/04/2009
 */
function alteraAtividade(index, macroCampoId, atividadeId, localizacaoId, txtAtividade) {
	// Declaração das variáveis a serem utilizadas.
	var macroCampo 		= document.getElementById("macrocampo");
	var localizacao		= document.getElementById("localizacao_atividade");
	var numAlunos 		= document.getElementsByName("num_alunos[]");
	var totalNumAlunos	= document.getElementById("total_num_alunos");
	
	var numAlunosAtividade = document.getElementsByName("num_alunos_atividade[" + atividadeId + "][]");
	
	for(var i=0; i<macroCampo.options.length; i++) {
		if(macroCampo.options[i].value == macroCampoId)
			macroCampo.selectedIndex = macroCampo.options[i].index;
	}
	
	// Força a chamada da função AJAX, para carregar a combo de Atividades.
	comboAtividade();
	// Espera 100 milisegundos para dar tempo da função AJAX ser executada.
	window.setTimeout('setaComboAtividade('+atividadeId+')', 100);
	
	var contador = 0;
	for(var i=0; i<numAlunos.length; i++) {
		// Ensino Fundamental
		if(modalidadeEnsino == "F") {
			if((i>=0) && (i<=8)) {
				numAlunos[i].value = numAlunosAtividade[i].value;
				contador += Number(numAlunosAtividade[i].value);
			}
		}
		// Ensino Médio
		else if(modalidadeEnsino == "M") {
			if((i>=9) && (i<=11)) {
				numAlunos[i].value = numAlunosAtividade[i].value;
				contador += Number(numAlunosAtividade[i].value);
			}
		}
		// Ensino Fundamental e Médio
		else {
			numAlunos[i].value = numAlunosAtividade[i].value;
			contador += Number(numAlunosAtividade[i].value);
		}
	}
	
	totalNumAlunos.value = contador;
	
	for(var i=0; i<localizacao.options.length; i++) {
		if(localizacao.options[i].value == localizacaoId)
			localizacao.selectedIndex = localizacao.options[i].index;
	}
	
	excluiAtividade(index, macroCampoId, txtAtividade);
}

/**
 * Função para setar a atividade na combo de acordo com a atividade selecionada.
 *
 * @return void
 *
 * Since: 28/04/2009
 */
function setaComboAtividade(atividadeId) {
	var atividade = document.getElementById("atividade");
	
	for(var i=0; i<atividade.options.length; i++) {
		if(atividade.options[i].value == atividadeId)
			atividade.selectedIndex = atividade.options[i].index;
	}	
}

/**
 * Função para habilitar a edição do Alunado Participante.
 *
 * @return void
 *
 * Since: 27/04/2009
 */
function editarAlunadoParticipante() {
	var alunadoParticipante  = document.getElementsByName("alunado_participante[]");
	var divInstrucoesAlunado = document.getElementById("div_instrucoes_alunado");
	var divValoresPagos 	 = document.getElementById("div_valores_pagos");
	var divEditarAlunado 	 = document.getElementById("div_editar_alunado");
	var divBotaoAlunado 	 = document.getElementById("div_botao_alunado");
	var divTabelaAtividades  = document.getElementById("div_tabela_atividades");
	
	for(var i=0; i<alunadoParticipante.length; i++) {
		// Ensino Fundamental
		if(modalidadeEnsino == "F") {
			if((i>=0) && (i<=8))
				alunadoParticipante[i].disabled = false;
		}
		// Ensino Médio
		else if(modalidadeEnsino == "M") {
			if((i>=9) && (i<=11))
				alunadoParticipante[i].disabled = false;
		}
		// Ensino Fundamental e Médio
		else {
			alunadoParticipante[i].disabled = false;
		}
	}
	
	divInstrucoesAlunado.style.display  = "block";
	divValoresPagos.style.display 		= "none";
	divEditarAlunado.style.display 		= "none";
	divBotaoAlunado.style.display 		= "block";
	divTabelaAtividades.style.display 	= "none";
}


/* Função para mudar a cor da coluna, alertando o usuário */
function alertaColunaTabela(indexColuna) {
	for(var i=0; i<=5; i++) {
		setTimeout('mudaBackgroundColorColuna('+indexColuna+')', (200 * (i+1)));
	}
}


/* Muda a cor do backgroundcolor da Coluna */ 
function mudaBackgroundColorColuna(indexColuna) {
	var tabela = document.getElementById("tabela_atividades");
	
	for(var i=3; i<(tabela.rows.length - 1); i++) {
		var celula = tabela.rows[i].cells;
		
		if(celula[indexColuna].style.backgroundColor == "#ffee77" || celula[indexColuna].style.backgroundColor == "rgb(255, 238, 119)") {
			celula[indexColuna].style.backgroundColor = celula[(indexColuna - 1)].style.backgroundColor;
		} else {
			celula[indexColuna].style.backgroundColor = "#ffee77";
		}
	}
}


/* Função para mudar a cor da linha, alertando o usuário */
function alertaLinhaTabela(indexLinha, corLinha) {
	for(var i=0; i<=5; i++) {
		setTimeout('mudaBackgroundColor('+indexLinha+', "'+corLinha+'")', (200 * (i+1)));
	}
}


/* Muda a cor do backgroundcolor da TR */ 
function mudaBackgroundColor(indexLinha, corLinha) {
	var tabela = document.getElementById("tabela_atividades");
	
	if(tabela.rows[indexLinha].style.backgroundColor == "#ffee77" || tabela.rows[indexLinha].style.backgroundColor == "rgb(255, 238, 119)")
		tabela.rows[indexLinha].style.backgroundColor = corLinha;
	else
		tabela.rows[indexLinha].style.backgroundColor = "#ffee77";
}


/**
 * Função para validar e submeter os dados da tabela de 'Atividades'.
 *
 * @return void
 *
 * Since: 23/04/2009
 */
function submeteAtividades() {
<?
/*
 * Validação retirada pois os campos editáveis do saldo foram retirados
 * de acordo com determinação do pessoal responsável pelo sistema.
 * 24/11/2009
 * Felipe Carvalho
 */ 
$validacaoSaldo = false;
?>
<? if($validacaoSaldo) { ?>
	var saldoReprogramavelParcela 	= document.getElementsByName("saldo_reprogramavel_parcela");
	var sReprParcela 				= saldoReprogramavelParcela[0].value.replace(/\./g, "");
	sReprParcela 					= sReprParcela.replace(/\,/g, ".");
	
	// Valida o preenchimento do saldo reprogramado (parcela).
	if(sReprParcela == "" || sReprParcela == 0) {
		alert("O Saldo reprogramado é de preenchimento obrigatório.");
		saldoReprogramavelParcela[0].select();
		return;
	}

	/*
	var saldoReprogramavelSuplementar 	= document.getElementsByName("saldo_reprogramavel_suplementar");
	var sReprSuplementar 				= saldoReprogramavelSuplementar[0].value.replace(/\./g, "");
	sReprSuplementar 					= sReprSuplementar.replace(/\,/g, ".");
	
	// Valida o preenchimento do saldo reprogramado (suplementar).
	if(sReprSuplementar == "" || sReprSuplementar == 0) {
		alert("O Saldo reprogramado é de preenchimento obrigatório.");
		saldoReprogramavelSuplementar[0].select();
		return;
	}
	*/
<? } ?>	


	//verifica valores
	<?if((integer)$possuiAnoAnterior > 0) {?>
		var mesaldocapital			= document.getElementById("mesaldocapital");
		var mesaldocusteio			= document.getElementById("mesaldocusteio");
		var mesaldototal			= document.getElementById("mesaldototal");
		
		if(mesaldocapital.value == '' || mesaldocusteio.value == ''){
			alert("Favor informar os campos: 'Valor Capital' e 'Valor Custeio'");
			return false;
		}
		else{
			var valorCapital = mesaldocapital.value; 
			valorCapital = valorCapital.replace(/\./gi,"");
			valorCapital = valorCapital.replace(/\,/gi,".");
			
			var valorCusteio = mesaldocapital.value; 
			valorCusteio = valorCusteio.replace(/\./gi,"");
			valorCusteio = valorCusteio.replace(/\,/gi,".");
			
			var valorSaldo = mesaldototal.value; 
			valorSaldo = valorSaldo.replace(/\./gi,"");
			valorSaldo = valorSaldo.replace(/\,/gi,".");
			
			if( parseFloat(valorSaldo) < ( parseFloat(valorCapital) + parseFloat(valorCusteio) ) ){
				alert("A soma dos valores Capital e Custeio não podem ultrapassar o valor do Saldo!");
				return false;
			} 
		}
	<?}?>




	var classificacaoEscola = "<?=$classificacaoEscola?>";
	
	// Valida se a quantidade de Atividades cadastradas está correta para Escola Rural.
	if(classificacaoEscola == "R" && contAtividades < 4) {
		alert("Você deve cadastrar 4 Atividades.");
		return;
	}


	// Valida se a quantidade de Atividades cadastradas está correta.
	<? if(!$_SESSION["boAtivNaoPagas2009"]) { ?>
		if(classificacaoEscola == "U") {
			<?if((integer)$possuiAnoAnterior > 0) {?>
				if(contAtividades < 4) {
					alert("Você deve cadastrar 4 Atividades.");
					return;
				}
			<?}else{?>
				if(contAtividades < 5) {
					alert("Você deve cadastrar no mínimo 5 Atividades.");
					return;
				}
			<?}?>
		}
	<? } ?>
	

	// recupera o 'mtaid' da atividade 'Programa Segundo Tempo'
	var mtaidPST = document.getElementById('mtaidPST');
	if( mtaidPST.value == '' ){	
		//alert( 'O programa 2º Tempo não está disponível para o ensino médio.' );
		//return false;
	} 
	var atividade 		= document.getElementsByName("mtaid[]");
	var alunado 		= document.getElementsByName("alunado_participante[]");
	var tabela 			= document.getElementById("tabela_atividades");
	var segundoTempo 	= document.getElementById('segundo_tempo');
	
	var flagValidacaoPST = false;
	var flagAtivAtletismo2011 = false;
	 
	var somaGeralSeriesAtividade = new Array();
	for(var i=0; i<atividade.length; i++) {
		if(segundoTempo.value == "S") {
			if(atividade[i].value == mtaidPST.value) flagValidacaoPST = true;
		}
	
		var indexLinha 	= atividade[i].parentNode.parentNode.rowIndex;
		var corLinha 	= tabela.rows[indexLinha].style.backgroundColor;
		var serie 		= document.getElementsByName("num_alunos_atividade[" + atividade[i].value + "][]");
		
		var somaSeriesAtividade = 0;
		var vAlunadoAux = 0;
		
		<?if( (integer)$_SESSION["exercicio"] < 2013){?>
		
			for(var z=0; z<=11; z++) {
				if(isNaN(somaGeralSeriesAtividade[z])) somaGeralSeriesAtividade[z] = 0;
			
				somaSeriesAtividade += Number(serie[z].value);
				somaGeralSeriesAtividade[z] = (Number(somaGeralSeriesAtividade[z]) + Number(serie[z].value));
				
				if(contAtividades == 5) {
					vAlunadoAux = (alunado[z].value == "") ? 0 : alunado[z].value;
					
					if(serie[z].value != vAlunadoAux) {
						alert("Com 5 atividades cadastradas o n° de alunos das séries de todas as Atividades \ndeve ser igual ao valor referente do Alunado Participante.");
						alertaLinhaTabela(indexLinha, corLinha);
						return;
					}
				}
			}
			
		<?}else{?>
		
			for(var z=0; z<=8; z++) {
				if(isNaN(somaGeralSeriesAtividade[z])) somaGeralSeriesAtividade[z] = 0;
			
				somaSeriesAtividade += Number(serie[z].value);
				somaGeralSeriesAtividade[z] = (Number(somaGeralSeriesAtividade[z]) + Number(serie[z].value));
				
				<?if((integer)$possuiAnoAnterior > 0) {?>
					if(contAtividades == 4) {
						vAlunadoAux = (alunado[z].value == "") ? 0 : alunado[z].value;
						
						if(serie[z].value != vAlunadoAux) {
							alert("Com 4 atividades cadastradas o n° de alunos das séries de todas as Atividades \ndeve ser igual ao valor referente do Alunado Participante.");
							alertaLinhaTabela(indexLinha, corLinha);
							return;
						}
					}
				<?}else{?>
					if(contAtividades == 5) {
						vAlunadoAux = (alunado[z].value == "") ? 0 : alunado[z].value;
						
						if(serie[z].value != vAlunadoAux) {
							alert("Com 5 atividades cadastradas o n° de alunos das séries de todas as Atividades \ndeve ser igual ao valor referente do Alunado Participante.");
							alertaLinhaTabela(indexLinha, corLinha);
							return;
						}
					}
				<?}?>
			}
			
		<?}?>

		/*
		if((classificacaoEscola != "R" && maisDeCem != 'false') && somaSeriesAtividade <= 20) {
			alert("A soma do n° de alunos das séries de cada Atividade deve ser superior a 20.");
			alertaLinhaTabela(indexLinha, corLinha);
			return;
		}
		*/
		
		//verifica Atividade Atletismo 2011
		if(atividade[i].value == 824) flagAtivAtletismo2011 = true;
		
	}
	
	// verifica se existe memid em 2011
	<?
		$sql = "SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = 2011 AND memstatus='A'";
		$memid_ano_2011 = $db->pegaUm($sql);

		
	?>
	var memid_ano_2011 = "<?=$memid_ano_2011?>";
	
	if(flagAtivAtletismo2011 == false && contAtividades == 6 && memid_ano_2011){
		alert("Para as 15 mil escolas que aderiram ao Programa Mais Educação até 2011, a atividade de atletismo torna-se obrigatória, caso a escola opte por desenvolver seis atividades.");
		return;
	} 
	
	
	//if(segundoTempo.value == "S" && !flagValidacaoPST) {
		//alert("Como esta escola aderiu ao Programa Segundo Tempo, é obrigatório a inclusão desta atividade.");
		//return;
	//}
	
	if(contAtividades > 5) {
		for(var i=0; i<11; i++) {
			// Ensino Fundamental
			if(modalidadeEnsino == "F") {
				if((i>=0) && (i<=8)) {
					if(Number(somaGeralSeriesAtividade[i]) < (Number(alunado[i].value) * 5)) {
						alert("Com mais de 5 Atividades cadastradas a soma do n° de alunos das mesmas séries de todas\nas Atividades deve ser maior ou igual a 5x o valor referente do Alunado Participante.");
						alertaColunaTabela(i+1);
						return;
					}
				}
			}
			// Ensino Médio
			else if(modalidadeEnsino == "M") {
				if((i>=9) && (i<=11)) {
					if(Number(somaGeralSeriesAtividade[i]) < (Number(alunado[i].value) * 5)) {
						alert("Com mais de 5 Atividades cadastradas a soma do n° de alunos das mesmas séries de todas\nas Atividades deve ser maior ou igual a 5x o valor referente do Alunado Participante.");
						alertaColunaTabela(i+1);
						return;
					}
				}
			}
			// Ensino Fundamental e Médio
			else {
				if(Number(somaGeralSeriesAtividade[i]) < (Number(alunado[i].value) * 5)) {
					alert("Com mais de 5 Atividades cadastradas a soma do n° de alunos das mesmas séries de todas\nas Atividades deve ser maior ou igual a 5x o valor referente do Alunado Participante.");
					alertaColunaTabela(i+1);
					return;
				}
			}
		}
		
	}
	
	
	<?
		$sql = "SELECT
					mtmid
				FROM
					pdeescola.metipomacrocampo
				WHERE
				 	mtmsituacao = 'A' AND
				 	mtmanoreferencia = " . $_SESSION["exercicio"] . " AND
				 	UPPER(mtmdescricao) LIKE '%ACOMPANHAMENTO PEDAGÓGICO%'";
		$vMtmId = $db->pegaUm($sql);
		/*
		$sql = "SELECT
					mtmid
				FROM
					pdeescola.metipomacrocampo
				WHERE
				 	mtmsituacao = 'A' AND
				 	mtmanoreferencia = " . $_SESSION["exercicio"] . " AND
				 	UPPER(mtmdescricao) LIKE '%AGROECOLÓGIA%'";
		$vMtmEcoId = $db->pegaUm($sql);
		*/
	?>
	var mtmid 					= document.getElementsByName("mtmid[]");
	var vControle 				= false;
	var vControleEsporteLazer 	= false;
	//var vControleEco 	= false;
	
	var arrContrl 	= new Array();
	for(var i=0; i<mtmid.length; i++) {
		
		if(Number(mtmid[i].value) == <?= (integer)$vMtmId ?>) vControle = true;
		//if(Number(mtmid[i].value) == 59) vControleEsporteLazer = true;
		//if(Number(mtmid[i].value) == <?= (integer)$vMtmEcoId ?>) vControleEco = true;
		
		arrContrl.push(mtmid[i].value);
	}
	
	<? if(!$_SESSION["boAtivNaoPagas2009"]) { ?>
		if(!vControle) {
			alert("Pelo menos uma das atividades cadastradas deve pertencer\n     ao Macrocampo 'ACOMPANHAMENTO PEDAGÓGICO'.");
			return;
		}
		<?//if((integer)$possuiAnoAnterior > 0) {?>
			//if(!vControleEsporteLazer) {
			//	alert("Pelo menos uma das atividades cadastradas deve pertencer\n     ao Macrocampo 'ESPORTE E LAZER'.");
			//	return;
			//}
		<?//}?>
		/*
		if(!vControleEco) {
			alert("Pelo menos uma das atividades cadastradas deve pertencer\n     ao Macrocampo 'ACOMPANHAMENTO ECOLÓGICO'.");
			return;
		}
		*/
	<? } ?>
	
	arrContrl.sort();
	var val = "";
	var cont = 0;
	for(var i=0; i<arrContrl.length; i++) {
		if(val != arrContrl[i]) {
			val = arrContrl[i];
			cont++;
		}
	}
	
	<? if(!$_SESSION["boAtivNaoPagas2009"]) { ?>
	if((classificacaoEscola != "R") && cont < 3) {
		alert("Devem ser cadastradas atividades com no mínimo 3 macrocampos diferentes.");
		return;
	}
	<? } ?>
	
	
	<? if(!$_SESSION["boAtivNaoPagas2009"]) { ?>	
			// Verifica se foram cadastradas as atividades 'BANDA FANFARRA, HIP HOP ou RÁDIO ESCOLAR' no ano anterior,
			// e em caso afirmativo, obriga o usuário a cadastrá-las no ano atual.
			var ativBandaFanfarra = ativHipHop = ativRadioEscolar = false;
			 
			<?
				$sql = "SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1);
				$memid_ano_anterior = $db->pegaUm($sql);
				
				if($memid_ano_anterior) {
					$sql = "SELECT
								mta.mtaid as atividade
							FROM
								pdeescola.meatividade mea
							INNER JOIN
								pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
											AND mta.mtasituacao = 'A' AND mta.mtaanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)."
							WHERE
								mea.memid = ".$memid_ano_anterior." AND
								mea.meaano = ".((integer)$_SESSION["exercicio"] - 1);

					$dadosAtividades = $db->carregar($sql);
					
					for($i=0; $i<count($dadosAtividades); $i++) {
						if($dadosAtividades[$i]["atividade"] == 40)
							echo "ativBandaFanfarra = true; ";
							
						if($dadosAtividades[$i]["atividade"] == 42)
							echo "ativHipHop = true; ";
							
						if($dadosAtividades[$i]["atividade"] == 49)
							echo "ativRadioEscolar = true; ";
					}
				}
			?>
			
			var existeBandaFanfarra = existeHipHop = existeRadioEscolar = false;
			for(var i=0; i<atividade.length; i++) {
				if(atividade[i].value == 152)
					existeBandaFanfarra = true;
					
				if(atividade[i].value == 153)
					existeHipHop = true;
					
				if(atividade[i].value == 197)
					existeRadioEscolar = true;
			}
			
			if(ativBandaFanfarra) {
				if(!existeBandaFanfarra) {
					alert("Como a atividade 'Banda Fanfarra' foi cadastrada nas atividades do ano anterior\n     você deve cadastrá-la nas atividades do ano atual.");
					return;
				}
			}
			if(ativHipHop) {
				if(!existeHipHop) {
					alert("Como a atividade 'Hip Hop' foi cadastrada nas atividades do ano anterior\n     você deve cadastrá-la nas atividades do ano atual.");
					return;
				}
			}
			if(ativRadioEscolar) {
				if(!existeRadioEscolar) {
					alert("Como a atividade 'Rádio Escolar' foi cadastrada nas atividades do ano anterior\n     você deve cadastrá-la nas atividades do ano atual.");
					return;
				}
			}
	<? } ?>
	
	var alunadoParticipante = document.getElementsByName("alunado_participante[]");
	for(var i=0; i<alunadoParticipante.length; i++) {
		alunadoParticipante[i].disabled = false;
	}
	
	var form 		= document.getElementById("formAtividadesAnoAtual");
	var btSalvar	= document.getElementById("bt_salvar");
	var btVoltar 	= document.getElementById("bt_voltar");
	
	btSalvar.disabled = true;
	btVoltar.disabled = true;
	form.submit();
}

/**
 * Função que valida os valores da tabela de 'Alunado participante' e...
 *
 * @return void
 *
 * Since: 16/04/2009
 */
function informaAlunadoParticipante() {
	var alunadoParticipante 	= document.getElementsByName("alunado_participante[]");
	var alunadoCenso			= document.getElementsByName("alunado_censo[]");
	var divInstrucoesAlunado 	= document.getElementById("div_instrucoes_alunado");
	var divBotaoAlunado 		= document.getElementById("div_botao_alunado");
	var divTabelaAtividades		= document.getElementById("div_tabela_atividades");
	var divValoresPagos			= document.getElementById("div_valores_pagos");
	var botaoAlunado			= document.getElementById("botao_alunado");
	var liberaValidacaoAlunado	= document.getElementById("libera_validacao_alunado");

	
	//verifica alunado
	botaoAlunado.disabled = true;
	liberaValidacaoAlunado.value = "1";
	
	var quantidadeAlunos = 0;
	var quantidadeCenso = 0;
	
	for(var i=0; i<alunadoParticipante.length; i++) {
		// Ensino Fundamental
		if(modalidadeEnsino == "F") {
			if((i>=0) && (i<=8)) {
				if(alunadoParticipante[i].value == "") {
					/*alert("O Alunado de todas as séries do Ensino Fundamental deve ser informado corretamente.");
					alunadoParticipante[i].focus();
					botaoAlunado.disabled = false;
					return;*/
					alunadoParticipante[i].value = 0;
				}
				
				verificaAlunadoCenso('meajax.php', 'tipo=verifica_censo&serie=' + Number(i + 1) + '&valor=' + alunadoParticipante[i].value + '');
				if(liberaValidacaoAlunado.value == "0") {
					botaoAlunado.disabled = false;
					return;
				}
				
				quantidadeAlunos = Number(quantidadeAlunos) + Number(alunadoParticipante[i].value);
				quantidadeCenso = Number(quantidadeCenso) + Number(alunadoCenso[i].value);

				//verifica se o alunado participante for menos que o alunado do censo.
				<?//if($classificacaoEscola == 'R'){?>
					/*
					if(alunadoParticipante[i].value != alunadoCenso[i].value){
						alert("O alunado participante deve ser igual ao alunado do censo.");
						alunadoParticipante[i].focus();
						botaoAlunado.disabled = false;
						return;
					}
					*/
				<?//}?>
				
			}
		}
		// Ensino Médio
		else if(modalidadeEnsino == "M") {
			if((i>=9) && (i<=11)) {
				if(alunadoParticipante[i].value == "") {
					/*alert("O Alunado de todas as séries do Ensino Médio deve ser informado corretamente.");
					alunadoParticipante[i].focus();
					botaoAlunado.disabled = false;
					return;*/
					alunadoParticipante[i].value = 0;
				}
				
				verificaAlunadoCenso('meajax.php', 'tipo=verifica_censo&serie=' + Number(i + 11) + '&valor=' + alunadoParticipante[i].value + '');
				if(liberaValidacaoAlunado.value == "0") {
					botaoAlunado.disabled = false;
					return;
				}
				
				quantidadeAlunos = Number(quantidadeAlunos) + Number(alunadoParticipante[i].value);
				quantidadeCenso = Number(quantidadeCenso) + Number(alunadoCenso[i].value);
			}
		}
		// Ensino Fundamental e Médio
		else {
			if(alunadoParticipante[i].value == "") {
				/*alert("O Alunado de todas as séries do Ensino Fundamental e Médio deve ser informado corretamente.");
				alunadoParticipante[i].focus();
				botaoAlunado.disabled = false;
				return;*/
				alunadoParticipante[i].value = 0;
			}
			var vSerie = ((i>=0) && (i<=8)) ? Number(i + 1) : Number(i + 11);
			
			verificaAlunadoCenso('meajax.php', 'tipo=verifica_censo&serie=' + vSerie + '&valor=' + alunadoParticipante[i].value + '');
			if(liberaValidacaoAlunado.value == "0") {
				botaoAlunado.disabled = false;
				return;
			}
			
			quantidadeAlunos = Number(quantidadeAlunos) + Number(alunadoParticipante[i].value);
			quantidadeCenso = Number(quantidadeCenso) + Number(alunadoCenso[i].value);

		}
		
	}
	
	//verifica escolas rurais com menos de 30 alunos é obrigatório a participação do alunado máximo do Censo
	<?if($classificacaoEscola == 'R'){?>
		if(quantidadeCenso < 30 && quantidadeCenso != quantidadeAlunos){
			alert("Escolas de Campo com menos de 30 alunos é obrigatório a participação do alunado máximo do Censo.");
			botaoAlunado.disabled = false;
			return;
		}
	<?}?>

	
	document.getElementById("quantidade_alunos").value = quantidadeAlunos;
	
	for(var i=0; i<alunadoParticipante.length; i++) {
		// Ensino Fundamental
		if(modalidadeEnsino == "F") {
			if((i>=0) && (i<=8))
				alunadoParticipante[i].disabled = true;
		}
		// Ensino Médio
		else if(modalidadeEnsino == "M") {
			if((i>=9) && (i<=11))
				alunadoParticipante[i].disabled = true;
		}
		// Ensino Fundamental e Médio
		else {
			alunadoParticipante[i].disabled = true;
		}
	}
	
	divInstrucoesAlunado.style.display 	= "none";
	divBotaoAlunado.style.display 		= "none";
	botaoAlunado.disabled 				= false;
	divTabelaAtividades.style.display	= "block";
	divValoresPagos.style.display		= "block";
	
	exibeEditarAlunado();
}

/**
 * Função que calcula o total da tabela de 'Alunado participante'.
 *
 * @return void
 *
 * Since: 16/04/2009
 */
function calculaTotalAlunado() {
	var alunadoParticipante			= document.getElementsByName("alunado_participante[]");
	var alunadoCenso				= document.getElementsByName("alunado_censo[]");
	var totalAlunadoParticipante	= document.getElementById("total_alunado_participante");
	
	var somaTotal = 0; 
	
	for(var i=0; i<alunadoParticipante.length; i++) {
		<?if($classificacaoEscola == 'R'){?>
			/*
			if(alunadoParticipante[i].value > 20){
				alert("O valor digitado não pode ser maior que 20!");
				alunadoParticipante[i].value = "";
			}
			*/
		<?}?>

		if(parseFloat(alunadoParticipante[i].value) > parseFloat(alunadoCenso[i].value)){
			alert("O valor digitado não pode ser maior que o valor do Censo: "+alunadoCenso[i].value);
			alunadoParticipante[i].value = "";
		}
		
		if(alunadoParticipante[i].value != "") {
			somaTotal += Number(alunadoParticipante[i].value);
		}
	}
	
	totalAlunadoParticipante.value = somaTotal;
}


/**
 * Função que calcula o total da tabela de 'Atividades'.
 *
 * @return void
 *
 * Since: 22/04/2009
 */
function calculaTotalAtividade() {
	var numAlunos 		= document.getElementsByName("num_alunos[]");
	var totalNumAlunos	= document.getElementById("total_num_alunos");
	
	var somaTotal = 0; 
	
	for(var i=0; i<numAlunos.length; i++) {
		if(numAlunos[i].value != "") {
			somaTotal += Number(numAlunos[i].value);
		}
	}
	
	totalNumAlunos.value = somaTotal;
}


/**
 * Função que insere uma nova atividade na tabela de 'Atividades'.
 *
 * @return void
 *
 * Since: 22/04/2009
 */
function insereAtividade() {

// Declaração das variáveis a serem utilizadas.
	var macroCampo 		= document.getElementById("macrocampo");
	var atividade 		= document.getElementById("atividade");
	var localizacao		= document.getElementById("localizacao_atividade");
	var numAlunos 		= document.getElementsByName("num_alunos[]");
	var arrAtividades	= document.getElementsByName("mtatividade[]");
	var totalNumAlunos	= document.getElementById("total_num_alunos");
	var tabela 			= document.getElementById("tabela_atividades");
	var segundoTempo 	= document.getElementById('segundo_tempo');
	var bgColor;
	
	var idMacroCampo 	= macroCampo.options[macroCampo.selectedIndex].value;
	var textMacroCampo 	= macroCampo.options[macroCampo.selectedIndex].text;
		
	var idAtividade 	= atividade.options[atividade.selectedIndex].value;
	var textAtividade 	= atividade.options[atividade.selectedIndex].text;
	
	
	var mtaid 			= document.getElementsByName("mtaid[]");
	var vEsporteLazer	= false;
	
	for(var i=0; i<mtaid.length; i++) {
		if(Number(mtaid[i].value) == 884) vEsporteLazer = true;
	}
	
	if(vEsporteLazer == false && idAtividade==884) vEsporteLazer = true;

	var classificacaoEscola = "<?=$classificacaoEscola?>";

	if(classificacaoEscola == 'U'){
		<?if((integer)$possuiAnoAnterior > 0) {?>
			if(vEsporteLazer == true) {
				if(contAtividades == 5) {
					alert("Não é possível cadastrar mais Atividades. (Máximo de 5 Atividades)");
					return;
				}
			}
			else{
				if(contAtividades == 4) {
					alert("Não é possível cadastrar mais Atividades. (Máximo de 4 Atividades). \n\nCaso queira optar por uma 5ª atividade, essa será, obrigatoriamente, (ESPORTE E LAZER - Esporte na Escola/ Atletismo e múltiplas vivências esportivas (basquete, futebol,futsal,handebol,voleibol,xadrez) ).");
					return;
				}
			}
		<?}else{?>
			if(contAtividades == 6) {
				alert("Não é possível cadastrar mais Atividades. (Máximo de 6 Atividades)");
				return;
			}
		<?}?>
	}
	else{
		
		if(contAtividades == 4) {
			alert("Não é possível cadastrar mais Atividades. (Máximo de 4 Atividades)");
			return;
		}
		
	}
	
	// Realiza a validação dos campos.
	if(!validaInsercaoAtividade()) return;
	
	if(totalNumAlunos.value == 0){
		alert("Soma total de alunos das séries deve ser maior que zero.");
		return;
	}
	
	
	
	if(classificacaoEscola == 'U'){
		<?if((integer)$possuiAnoAnterior > 0){?>
			if( (idAtividade != 877) && (totalNumAlunos.value < 25) ) {
				alert("Soma total de alunos das séries deve ser maior ou igual a 25 alunos para esta atividade.");
				return;
			}
		<?}?>
		
		if(textMacroCampo.search(/ESPORTE E LAZER/gi) != -1 && segundoTempo.value == "S") {
			if(contAtividadesPST == 2) {
				alert("Não é possível cadastrar mais Atividades do macrocampo 'ESPORTE E LAZER'.");
				return;
			} else {
				contAtividadesPST++;
			}
		}
		
	}

	if(classificacaoEscola == 'R'){
		<?if((integer)$possuiAnoAnterior > 0){?>
			/*if( (idAtividade != 783) && (totalNumAlunos.value < 15) ) {
				alert("Soma total de alunos das séries deve ser maior ou igual a 15 alunos para esta atividade.");
				return;
			}*/
		<?}?>

		if(textMacroCampo.search(/AGROECOLOGIA/gi) != -1) {
			if(ctMacroCampoAgroecologia == 2) {
				alert("Não é possível cadastrar mais Atividades do macrocampo 'AGROECOLOGIA'.");
				return;
			} else {
				ctMacroCampoAgroecologia++;
			}
		}
	}
	
	

	if(classificacaoEscola == 'U'){
		<?//if( (integer)$_SESSION["exercicio"] < 2013){ ?>
			// validação para não permitir mais de 4 macrocampos.
			/*
			if( !in_array(macroCampo.value, arrMacroCampos) ) {
				if(contMacroCampos == 4) {
					alert("Não é possível cadastrar mais Macrocampos. (Máximo de 4 Macrocampos)");
					return;
				} else {
					contMacroCampos++;
				}
			}
			*/
		<?//}?>
	}

	//verifica se existe no maximo 2 atividades: Tecnologias Educacioanis
	if(textAtividade == 'Tecnologias Educacionais'){ 

		if(ctAtividadeTecEduc == 2) {
			alert("Não é possível cadastrar mais Atividades: Tecnologias Educacioanis. (Máximo de 2)");
			return;
		}
		else{
			ctAtividadeTecEduc++;
		}
	
	}
	
		
	// Recupera a cor da linha a ser inserida.
	if(contAtividades == 0) {
		bgColor = "#FAFAFA";
		macroCampo.disabled = false;
	} else {
		/* rgb(250, 250, 250) => Firefox; 
		   			 #fafafa  => Internet Explorer */ 
		if(tabela.rows[3].style.backgroundColor == "rgb(250, 250, 250)" || tabela.rows[3].style.backgroundColor == "#fafafa")
			bgColor = "#E8E8E8";
		else
			bgColor = "#FAFAFA";
	}
	
	// Cria a nova linha.
	var linha = tabela.insertRow(3);
	linha.style.backgroundColor = bgColor;
	
	contAtividades++;
	arrMacroCampos.push(macroCampo.value);
	
	<?if( (integer)$_SESSION["exercicio"] < 2013){ ?>
	
			var celula	= new Array();
			for(var i=0; i<16; i++) {
				celula[i]			= linha.insertCell(i);
				celula[i].vAlign 	= "middle";
				celula[i].align 	= "center";
			}
			
			celula[0].innerHTML = '<input type="hidden" name="mtmid[]" value="' + idMacroCampo + '" />' +
								  '<input type="hidden" name="mtaid[]" value="' + idAtividade + '" />' +
								  '<input type="hidden" name="mtatividade[]" value="' + textAtividade + '" />' +
								  textMacroCampo + ' / ' + textAtividade;
								  
			atividade.disabled  = false;
			macroCampo.disabled = false;
			
			macroCampo.selectedIndex = 0;
			atividade.selectedIndex = 0;
			
			for(var i=0; i<numAlunos.length; i++) {
				// Ensino Fundamental
				if(modalidadeEnsino == "F") {
					if((i>=0) && (i<=8)) {
						celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
					}
					else
						celula[i+1].innerHTML = '-<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="" />';
				}
				// Ensino Médio
				else if(modalidadeEnsino == "M") {
					if((i>=9) && (i<=11))
						celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
					else
						celula[i+1].innerHTML = '-<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="" />';
				}
				// Ensino Fundamental e Médio
				else {
					celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
				}
				
				numAlunos[i].value = "";
			}
			
			celula[13].innerHTML = totalNumAlunos.value;
			totalNumAlunos.value = "";
			
			var idLocalizacao 	= localizacao.options[localizacao.selectedIndex].value;
			var textLocalizacao = localizacao.options[localizacao.selectedIndex].text;
			
			celula[14].innerHTML = textLocalizacao + '<input type="hidden" name="loc_atividade[]" value="' + idLocalizacao + '" />';
			localizacao.selectedIndex = 0;
			
			celula[15].innerHTML = '<img src="/imagens/alterar.gif" style="cursor:pointer;" onclick="alteraAtividade(this.parentNode.parentNode.rowIndex, ' + idMacroCampo + ', ' + idAtividade + ', \'' + idLocalizacao + '\', \''+textAtividade+'\');" title="Alterar a atividade">&nbsp;' +
								   '<img src="/imagens/excluir.gif" style="cursor:pointer;" onclick="excluiAtividade(this.parentNode.parentNode.rowIndex, '+idMacroCampo+',\''+textAtividade+'\');" title="Excluir a atividade">' +
								   '<input type="hidden" name="meaid[]" value="novo" />';
	<?}else{?>
	
			var celula	= new Array();
			for(var i=0; i<13; i++) {
				celula[i]			= linha.insertCell(i);
				celula[i].vAlign 	= "middle";
				celula[i].align 	= "center";
			}
			
			celula[0].innerHTML = '<input type="hidden" name="mtmid[]" value="' + idMacroCampo + '" />' +
								  '<input type="hidden" name="mtaid[]" value="' + idAtividade + '" />' +
								  '<input type="hidden" name="mtatividade[]" value="' + textAtividade + '" />' +
								  textMacroCampo + ' / ' + textAtividade;
								  
			atividade.disabled  = false;
			macroCampo.disabled = false;
			
			macroCampo.selectedIndex = 0;
			atividade.selectedIndex = 0;
			
			for(var i=0; i<numAlunos.length; i++) {
				// Ensino Fundamental
				if(modalidadeEnsino == "F") {
					if((i>=0) && (i<=8)) {
						celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
					}
					else
						celula[i+1].innerHTML = '-<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="" />';
				}
				// Ensino Médio
				else if(modalidadeEnsino == "M") {
					if((i>=9) && (i<=11))
						celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
					else
						celula[i+1].innerHTML = '-<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="" />';
				}
				// Ensino Fundamental e Médio
				else {
					celula[i+1].innerHTML = numAlunos[i].value + '<input type="hidden" name="num_alunos_atividade[' + idAtividade + '][]" value="' + numAlunos[i].value + '" />';
				}
				
				numAlunos[i].value = "";
			}
			
			celula[10].innerHTML = totalNumAlunos.value;
			totalNumAlunos.value = "";
			
			var idLocalizacao 	= localizacao.options[localizacao.selectedIndex].value;
			var textLocalizacao = localizacao.options[localizacao.selectedIndex].text;
			
			celula[11].innerHTML = textLocalizacao + '<input type="hidden" name="loc_atividade[]" value="' + idLocalizacao + '" />';
			localizacao.selectedIndex = 0;
			
			celula[12].innerHTML = '<img src="/imagens/alterar.gif" style="cursor:pointer;" onclick="alteraAtividade(this.parentNode.parentNode.rowIndex, ' + idMacroCampo + ', ' + idAtividade + ', \'' + idLocalizacao + '\', \''+textAtividade+'\');" title="Alterar a atividade">&nbsp;' +
								   '<img src="/imagens/excluir.gif" style="cursor:pointer;" onclick="excluiAtividade(this.parentNode.parentNode.rowIndex, '+idMacroCampo+',\''+textAtividade+'\');" title="Excluir a atividade">' +
								   '<input type="hidden" name="meaid[]" value="novo" />';
	<?}?>
		
	// Se não existir a linha com os totais, cria.
	/*var trTotalAtividades = document.getElementById('total_atividades');
	if(!trTotalAtividades) {
		// Cria a nova linha.
		var linhaTotalAtividades = tabela.insertRow(4);
		linhaTotalAtividades.style.backgroundColor = "#C0C0C0";
		linhaTotalAtividades.id = "total_atividades";
		
		var celulaTotalAtividades = new Array();
		for(var i=0; i<16; i++) {
			celulaTotalAtividades[i] 		= linhaTotalAtividades.insertCell(i);
			celulaTotalAtividades[i].vAlign	= "middle";
			
			if(i==0)
				celulaTotalAtividades[i].align = "right";
			else
				celulaTotalAtividades[i].align = "center";
		}
		
		celulaTotalAtividades[0].innerHTML = "<b>Total:</b>";
		celulaTotalAtividades[14].innerHTML = "-";
		celulaTotalAtividades[15].innerHTML = "-";
	}*/
	
	//atualizaTotalAtividades();
	exibeEditarAlunado();
}

/**
 * Função para atualizar a linha com os totais do número de alunos das atividades.
 *
 * @return void
 *
 * Since: 28/04/2009
 */
function atualizaTotalAtividades() {
	var mtaid				= document.getElementsByName("mtaid[]");
	var trTotalAtividades 	= document.getElementById("total_atividades");
	var totalAlunos			= new Array();
	var contAlunos;
	
	for(var i=0; i<mtaid.length; i++) {
		var numAlunos = document.getElementsByName("num_alunos_atividade[" + mtaid[i].value + "][]");
		
		contAlunos = 0;
		
		for(var z=0; z<numAlunos.length; z++) {
			if(numAlunos[z].value != "") {
				if(isNaN(totalAlunos[z])) totalAlunos[z] = 0;
				
				totalAlunos[z] += Number(numAlunos[z].value);
				contAlunos += Number(numAlunos[z].value);
			}
			else {
				totalAlunos[z] = "-";
			}
		}
		
		if(isNaN(totalAlunos[12])) totalAlunos[12] = 0;
		
		totalAlunos[12] += contAlunos;
	}
	
	for(var i=0; i<totalAlunos.length; i++) {
		trTotalAtividades.cells[(i+1)].innerHTML = totalAlunos[i];
	}
}

/**
 * Função para validação dos campos de uma nova atividade.
 *
 * @return boolean
 *
 * Since: 23/04/2009
 */
function validaInsercaoAtividade() {
	var macroCampo 		= document.getElementById("macrocampo");
	var atividade 		= document.getElementById("atividade");
	var numAlunos 		= document.getElementsByName("num_alunos[]");
	var localizacao		= document.getElementById("localizacao_atividade");
	
	var alunadoParticipante = document.getElementsByName("alunado_participante[]");
	
	// Valida preenchimento do Macrocampo
	if(macroCampo.value == "") {
		alert("O 'Macrocampo' deve ser informado.");
		macroCampo.focus();
		return false;
	}
	
	// Valida preenchimento da Atividade
	if(atividade.value == "") {
		alert("A 'Atividade' deve ser informada.");
		atividade.focus();
		return false;
	}
	
	// Verifica se já existe na lista a atividade que está sendo incluída.
	var mtaid 		= document.getElementsByName("mtaid[]");
	var idAtividade = atividade.options[atividade.selectedIndex].value;

	if(contAtividades > 0) {
	
		for(var i=0; i<mtaid.length; i++) {
			if(mtaid[i].value == idAtividade) {
				var tabela 		= document.getElementById("tabela_atividades");
				var indexLinha 	= mtaid[i].parentNode.parentNode.rowIndex;
				var corLinha 	= tabela.rows[indexLinha].style.backgroundColor;
		
				alert("Não é possível incluir. Já existe um registro na lista com esta Atividade.");
				alertaLinhaTabela(indexLinha, corLinha);
				atividade.focus();
				return false;
			}
		}
		
	}
	
	// Valida preenchimento do número de alunos das séries do Ensino Fundamental e/ou Médio
	for(var i=0; i<numAlunos.length; i++) {
	
		// Ensino Fundamental
		if(modalidadeEnsino == "F") {
			if((i>=0) && (i<=8)) {
				//verifica se macrocampo ACOMPANHAMENTO PEDAGÓGICO (OBRIGATÓRIA), o número de alunos informado deve ser igual ao valor referente na tabela de 'Alunado Participante'.
				if( macroCampo.value == 55 && Number(numAlunos[i].value) != Number(alunadoParticipante[i].value) ){
					alert("O número de alunos informado deve ser igual ao valor referente da tabela de Alunado Participante.");
					numAlunos[i].select();
					return false;
				}
				// Verifica se algum campo está vazio.
				if( numAlunos[i].value == "" ) {
					/*alert("O número de alunos de todas as séries do Ensino Fundamental deve ser informado corretamente.");
					numAlunos[i].focus();
					return false;*/
					numAlunos[i].value = 0;
				}
				// Verifica se o número de alunos informado é maior que o valor referente na tabela de 'Alunado Participante'.
				if( Number(numAlunos[i].value) > Number(alunadoParticipante[i].value) ) {
					alert("O número de alunos informado não pode ser maior que o valor referente da tabela de Alunado Participante.");
					numAlunos[i].select();
					return false;
				}
			}
		}
		
		// Ensino Médio
		else if(modalidadeEnsino == "M") {
			if((i>=9) && (i<=11)) {
				// Verifica se algum campo está vazio.
				if(numAlunos[i].value == "") {
					/*alert("O número de alunos de todas as séries do Ensino Médio deve ser informado corretamente.");
					numAlunos[i].focus();
					return false;*/
					numAlunos[i].value = 0;
				}
				// Verifica se o número de alunos informado é maior que o valor referente na tabela de 'Alunado Participante'.
				if( Number(numAlunos[i].value) > Number(alunadoParticipante[i].value) ) {
					alert("O número de alunos informado não pode ser maior que o valor\n          referente da tabela de Alunado Participante.");
					numAlunos[i].select();
					return false;
				}
			}
		}
		
		// Ensino Fundamental e Médio
		else {
			// Verifica se algum campo está vazio.
			if(numAlunos[i].value == "") {
				/*alert("O número de alunos de todas as séries do Ensino Fundamental e Médio deve ser informado corretamente.");
				numAlunos[i].focus();
				return false;*/
				numAlunos[i].value = 0;
			}
			// Verifica se o número de alunos informado é maior que o valor referente na tabela de 'Alunado Participante'.
			if( Number(numAlunos[i].value) > Number(alunadoParticipante[i].value) ) {
				alert("O número de alunos informado não pode ser maior que o valor\n          referente da tabela de Alunado Participante.");
				numAlunos[i].select();
				return false;
			}
		}
		
	}
	
	calculaTotalAtividade();
	
	// Valida preenchimento da Localização
	if(localizacao.value == "") {
		alert("A 'Localização da Atividade' deve ser informada.");
		localizacao.focus();
		return false;
	}
	
	return true;
}

function mensagemSegundoTempo() {
	displayMessage('?modulo=meprincipal/obssegundotempo&acao=A');
}

function popupMensagem(id) {
	if(id == 'EsporteLazer'){
		displayMessage('?modulo=meprincipal/popupMensagem&acao=A&id=EsporteLazer');
	}
	else if(id == 'PromocaoSaude'){
		displayMessage('?modulo=meprincipal/popupMensagem&acao=A&id=PromocaoSaude');
	}
}

/**
 * Função para carregar a combo de Atividades à partir do Macrocampo selecionado.
 *
 * @return void
 *
 * Since: 20/04/2009
 */
function comboAtividade() {
	var quantidadeAlunos = document.getElementById('quantidade_alunos');

	<?php if( (integer)$_SESSION['exercicio'] < 2013 ): ?>
		testaRequisitosPST('meajax.php', 'tipo=testa_requisitos2&qtd_alunos='+quantidadeAlunos.value);
	<?php else: ?>
		testaRequisitosPST('meajax.php', 'tipo=testa_requisitos&qtd_alunos='+quantidadeAlunos.value);
	<?php endif; ?>

	
	var selMacroCampo 	= document.getElementById('macrocampo');
	var requisitosPST 	= document.getElementById('requisitos_pst');
	var segundoTempo 	= document.getElementById('segundo_tempo');
	
	var classificacaoEscola = "<?=$classificacaoEscola?>";

	var idMacroCampo, textMacroCampo, mtapst;
	
	idMacroCampo 	=	selMacroCampo.options[selMacroCampo.selectedIndex].value;
	textMacroCampo	=	selMacroCampo.options[selMacroCampo.selectedIndex].text;

	<?php if( (integer)$_SESSION['exercicio'] == 2013 ): ?>
		if( idMacroCampo == 56 ){
			//alert('Atenção – Na organização das atividades deste macrocampo são prioritárias as temáticas de “Educação em Direitos Humanos”, “Promoção da Saúde” e temas relacionados à Ética e Cidadania. Em relação a essas temáticas, serão disponibilizados materiais de referência na página do Ministério da Educação.');
		}
		if( idMacroCampo == 59 ){
			//popupMensagem('EsporteLazer');
		}
		if( idMacroCampo == 65 ){
			popupMensagem('PromocaoSaude');
		}
	<?php endif; ?>
	
	<?php if( (integer)$_SESSION['exercicio'] < 2013 ){ ?>
		if( (textMacroCampo.search(/ESPORTE E LAZER/gi) != -1) && (requisitosPST.value == "1") && (segundoTempo.value == "B") && (classificacaoEscola == "U") ) {
			mensagemSegundoTempo();
		}
		else {
			if(segundoTempo.value == "S" && textMacroCampo.search(/ESPORTE E LAZER/gi) != -1){
			//if(requisitosPST.value == "1" && textMacroCampo.search(/ESPORTE E LAZER/gi) != -1)
				//mtaatividadepst = 'false';
				mtaatividadepst = 'true';
			}else{
				//mtaatividadepst = 'true';
				mtaatividadepst = 'false';
			}
	
			carregaComboAtividade('meajax.php', 'tipo=carrega_atividades&id='+idMacroCampo+'&mtaatividadepst='+mtaatividadepst+'&classificacaoEscola='+classificacaoEscola);
		}
	<?}else{?>
		if(segundoTempo.value == "S" && textMacroCampo.search(/ESPORTE E LAZER/gi) != -1){
		//if(requisitosPST.value == "1" && textMacroCampo.search(/ESPORTE E LAZER/gi) != -1)
			//mtaatividadepst = 'false';
			mtaatividadepst = 'true';
		}else{
			//mtaatividadepst = 'true';
			mtaatividadepst = 'false';
		}

		carregaComboAtividade('meajax.php', 'tipo=carrega_atividades&id='+idMacroCampo+'&mtaatividadepst='+mtaatividadepst+'&classificacaoEscola='+classificacaoEscola);
	<?}?>
}

function desfazerEscolhaAdesao() {
	if(confirm("Esta opção irá apagar todas as informações cadastradas referentes as atividades. Deseja continuar?")) {
		desfazerEscolhaAdesaoAjax('meajax.php','tipo=desfazer_escolha_adesao');
	}
}

// função que simula o 'in_array' do PHP
function in_array(needle,haystack) {
	return new RegExp('(^|\,)'+needle+'(\,|$)','gi').test(haystack);
}

function excluirAtividade(){
	if( !confirm("Ao Excluir as atividades, não será possivel recuperá-las novamente.\nDeseja realmente excluir os registros?") ){
		return false;	
	}
	var req = new Ajax.Request('pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A', {
							        method:     'post',
							        parameters: '&ajaxExclui=t',							         
							        onComplete: function (res)
							        {		 
										alert('Operação realizada com sucesso!');
										window.location.href = "pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A";
							        }
							        
							  });
}

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}


function alertAtividade(){
	//macrocampo: ACOMPANHAMENTO PEDAGOGICO - atividade: ciencias
	if(document.getElementById('atividade').value == 1016){
		alert("No caso da opção por Ciências, a escola receberá também recursos para o desenvolvimento de trabalhos relacionados a laboratórios, feiras e projetos científicos.");
	}
	if(document.getElementById('atividade').value == 879){
		alert("Na escolha desta atividade a escola deverá disponibilizar, como contrapartida, uma sala para o desenvolvimento da atividade. Para isto receberá recursos de custeio e capital para aquisição de equipamentos e materiais.");
	}
	if(document.getElementById('atividade').value == 884){
		popupMensagem('EsporteLazer');
	}
}

--></script>