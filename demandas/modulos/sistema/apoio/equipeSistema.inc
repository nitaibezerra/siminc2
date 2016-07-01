<?php

	//unset($_SESSION['eqsid']);
	//unset($_SESSION['op4']);

function exibeMembros($membros){
	global $db;
		$sql = " SELECT u.usunome 
			FROM demandas.equipemembros AS e 
			LEFT JOIN seguranca.usuario AS u ON u.usucpf = e.usucpf 
		WHERE e.eqsid = {$membros}";
		print "<div style=\"position:absolute;backgroundcolor:#FFFFFF;5px;border:#000000 solid 2px;\"><table cellSpacing=\"0\" cellPadding=\"0\" >";
		$res = pg_query($sql);
		$c = 1;
		while ($i = pg_fetch_array($res)){
			if($c % 2){
				print "<tr bgcolor=\"#FFFFFF\"><td>".$i['usunome']."</td></tr>";
			}
			else{
				print "<tr bgcolor=\"#CCCCCC\" ><td>".$i['usunome']."</td></tr>";
			}
			$c++;
		}
		print "</table></div>";
}

function exibeSistemas($sistemas){
	global $db;
		$sql = " SELECT d.siddescricao
			FROM demandas.sistemaequipe AS s 
			LEFT JOIN demandas.sistemadetalhe AS d ON s.sidid = d.sidid  
		WHERE s.eqsid = {$sistemas}";
		print "<div style=\"position:absolute;backgroundcolor:#FFFFFF;5px;border:#000000 solid 2px;\"><table cellSpacing=\"0\" cellPadding=\"0\" >";
		$res = pg_query($sql);
		$c = 1;
		while ($i = pg_fetch_array($res)){
			if($c % 2){
				print "<tr bgcolor=\"#FFFFFF\"><td>".$i['siddescricao']."</td></tr>";
			}
			else{
				print "<tr bgcolor=\"#CCCCCC\" ><td>".$i['siddescricao']."</td></tr>";
			}
			$c++;
		}
		print "</table></div>";
}

function inserirEquipe(){
	global $db;
	$sql = " INSERT INTO demandas.equipesistemas
			(eqsnome) VALUES
			('".$_POST['eqsnome']."') RETURNING eqsid";
	$eqsid = $db->pegaUm($sql);
	foreach ($_REQUEST['membrosequipe'] as $membrosequipe ) {
					$sql = "INSERT INTO demandas.equipemembros 
					( eqsid, usucpf) values ( '".$eqsid."', '".$membrosequipe."' )";
					$db->executar( $sql );
				}
	foreach ($_REQUEST['equipesistema'] as $equipesistema ) {
					$sql = "INSERT INTO demandas.sistemaequipe 
					( eqsid, sidid) values ( '".$eqsid."', '".$equipesistema."' )";
					$db->executar( $sql );
				}
	$db->commit();
	//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	//exit();
}

function alterarEquipe(){
	global $db;
	$sql = "UPDATE demandas.equipesistemas SET 
			eqsnome = '".$_POST['eqsnome']."'  
				WHERE eqsid = ".$_POST['eqsid']." RETURNING eqsid ";
	$eqsid2 = $db->pegaUm($sql);
	$sql = " DELETE FROM demandas.equipemembros  
				WHERE eqsid = $eqsid2";
	$db->executar($sql);
	foreach ($_REQUEST['membrosequipe'] as $membrosequipe ) {
					$sql = "INSERT INTO demandas.equipemembros 
					( eqsid, usucpf) values ( $eqsid2, '".$membrosequipe."' )";
					$db->executar( $sql );
				}
	
	$sql = " DELETE FROM demandas.sistemaequipe 
				WHERE eqsid = $eqsid2";
	$db->executar($sql);
	foreach ($_REQUEST['equipesistema'] as $equipesistema ) {
					$sql = "INSERT INTO demandas.sistemaequipe 
					( eqsid, sidid) values ( $eqsid2, '".$equipesistema."' )";
					$db->executar( $sql );
				}
	$db->commit();
	//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	//exit();
}

function deletarEquipe(){
	global $db;
	$sql = "DELETE FROM demandas.equipemembros 
			WHERE eqsid = '".$_SESSION['eqsid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM demandas.sistemaequipe  
			WHERE eqsid = '".$_SESSION['eqsid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM demandas.equipesistemas  
			WHERE eqsid = '".$_SESSION['eqsid']."'";
	$db->executar($sql);
	$db->commit();
	//exit();
}

function selecionarEquipe(){
	global $db;
	$sql = "SELECT e.eqsid, e.eqsnome  
			FROM  demandas.equipesistemas AS e     
			WHERE e.eqsid = '".$_SESSION['eqsid']."'
	";
	return $db->carregar($sql);
}

if($_POST['equipesistema'] && $_POST['membrosequipe'] && !$_POST['eqsid']){
	inserirEquipe();
	unset($_SESSION['eqsid']);
	unset($_SESSION['op4']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	//exit();
	
}

if($_POST['equipesistema'] && $_POST['membrosequipe'] && $_POST['eqsid']){
	alterarEquipe();
	unset($_SESSION['eqsid']);
	unset($_SESSION['op4']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	//exit();
	
}

if($_GET['eqsid'] && $_GET['op4']){
	session_start();
	$_SESSION['eqsid'] = $_GET['eqsid'];
	$_SESSION['op4'] = $_GET['op4'];
	header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	exit();
}

if($_SESSION['eqsid'] && $_SESSION['op4']){
	if($_SESSION['op4'] == 'delete'){
		deletarEquipe();	
		unset($_SESSION['eqsid']);
		unset($_SESSION['op4']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
	}
	if($_SESSION['op4'] == 'update'){
		$dados = selecionarEquipe();	
		unset($_SESSION['eqsid']);
		unset($_SESSION['op4']);
	}

}

if ($_REQUEST['membrosAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeMembros($_POST['membrosAjax']);
	exit;
}

if ($_REQUEST['sistemasAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeSistemas($_POST['sistemasAjax']);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Equipes de Sistemas', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formEquipes" name="formEquipes" action="" method="post" onsubmit="processaCombosPopup();return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="eqsid" id="eqsid" value="<? echo $dados[0]['eqsid']; ?>" />
	<tr>
	<td align='right' width="25%" class="SubTituloDireita">Nome da Equipe</td>
	<td>
	<? $eqsnome = $dados[0]['eqsnome'];  ?>
		<?= campo_texto( 'eqsnome', 'N', 'S', '', 94, 250, '', '','','','','',''); echo obrigatorio();?>
	</td>
	</tr>
	<tr>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Membros da Equipe:</td>
	<td>                   
        <?php
					$sqlequipe = "SELECT
						 e.usucpf AS codigo,
						 u.usunome AS descricao
						FROM
							demandas.equipemembros AS e 
						LEFT JOIN seguranca.usuario AS u ON u.usucpf = e.usucpf 
						WHERE 
						 e.eqsid = ".$dados[0]['eqsid']."";
					
					$sql_combo1 = "SELECT
									 u.usucpf as codigo,
									 u.usucpf || ' - ' || u.usunome as descricao
									FROM
									 seguranca.usuario u
									WHERE
									 u.usustatus = 'A' OR
									 u.usustatus IS NULL
									ORDER BY
									 u.usunome";

			if($dados[0]['eqsid'] != '' ){
			               $membrosequipe = $db->carregar($sqlequipe);
			            }
					combo_popup(
						'membrosequipe',
						$sql_combo1,
						'Selecione o(s) Membro(s) da Equipe',
						'400x400',
						0,
						array(),
						'',
						'S',
						true,
						false,
						5
					);
				echo obrigatorio();?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Sistemas da Equipe:</td>
	<td>                   
        <?php
					$sqlsis = "SELECT
										 d.sidid AS codigo,
										 s.siddescricao AS descricao
										FROM 
										 demandas.sistemaequipe AS d
										
									LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid 
										 WHERE d.eqsid = ".$dados[0]['eqsid']."
										  ";
					
					$sql_combo2 = "SELECT
										 sidid AS codigo,
										 siddescricao AS descricao
										FROM 
										 demandas.sistemadetalhe
										WHERE
										 sidstatus = 'A' 
										ORDER BY 
										 siddescricao";

			if($dados[0]['eqsid'] != '' ){
			               $equipesistema = $db->carregar($sqlsis);
			            }
					combo_popup(
						'equipesistema',
						$sql_combo2,
						'Selecione o(s) Sistema(s) da Equipe',
						'400x400',
						0,
						array(),
						'',
						'S',
						true,
						false,
						5
					);
				echo obrigatorio(); ?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "SELECT
	'<a href=\"demandas.php?modulo=sistema/apoio/equipeSistema&acao=A&eqsid=' || e.eqsid || '&op4=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/equipeSistema&acao=A&eqsid=' || e.eqsid || '&op4=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>', 
				e.eqsnome, 
				'<a style=\"cursor:pointer;padding-right:30px;\"  onMouseOut=\"return esconde_membros(' || e.eqsid || ');\" onMouseOver=\"return exibe_membros(' || e.eqsid || ');\">' ||(SELECT COUNT(*) FROM demandas.equipemembros  AS a WHERE a.eqsid = e.eqsid) || '</a><div style=\"position:relative;margin-left:35px;top:-20px;\" id=\"mem' || e.eqsid || '\"></div>' AS membros ,
				'<a style=\"cursor:pointer;padding-right:30px;\"  onMouseOut=\"return esconde_sistemas(' || e.eqsid || ');\" onMouseOver=\"return exibe_sistemas(' || e.eqsid || ');\">' ||(SELECT COUNT(*) FROM demandas.sistemaequipe  AS b WHERE b.eqsid = e.eqsid) || '</a><div style=\"position:relative;margin-left:35px;top:-20px;\" id=\"sis' || e.eqsid || '\"></div>' AS sistemas 
				 FROM demandas.equipesistemas AS e
		  		ORDER BY 
				 e.eqsid";
				$cabecalho = array( "Opções","Equipe","Qtd. Membros","Qtd. Sistemas");
				$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
				?>
<script type="text/javascript">
function validaForm(){
	if(document.formEquipes.eqsnome.value == ''){
		alert ('O campo Nome da Equipe deve ser preenchido.');
		document.formEquipes.eqsnome.focus();
		return false;
	}
	if(document.formEquipes.membrosequipe.value == ''){
		alert ('Informe o(s) Membro(s) da Equipe.');
		document.formEquipes.membrosequipe.focus();
		return false;
	}
	if(document.formEquipes.equipesistema.value == ''){
		alert ('Informe o(s) Sistema(s) da Equipe.');
		document.formEquipes.equipesistema.focus();
		return false;
	}
}

function exclusao(url) {
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}

function exibe_membros(m) {
	var x = m;
	
	if(x){
	div = document.getElementById('mem'+x);
	
	// Faz uma requisição ajax
	var req = new Ajax.Request('demandas.php?modulo=sistema/apoio/equipeSistema&acao=A', {
							        method:     'post',
							        parameters: '&membrosAjax=' + x,
							        onComplete: function (res)
							        {			
										div.innerHTML = res.responseText;
										div.style.display = '';

							        }
							  });
	}
}

function esconde_membros(m) {
	var x = m;
	div = document.getElementById('mem'+x).style.display='none';
}

function exibe_sistemas(m) {
	var x = m;
	
	if(x){
	div = document.getElementById('sis'+x);
	
	// Faz uma requisição ajax
	var req = new Ajax.Request('demandas.php?modulo=sistema/apoio/equipeSistema&acao=A', {
							        method:     'post',
							        parameters: '&sistemasAjax=' + x,
							        onComplete: function (res)
							        {			
										div.innerHTML = res.responseText;
										div.style.display = '';

							        }
							  });
	}
}

function esconde_sistemas(m) {
	var x = m;
	div = document.getElementById('sis'+x).style.display='none';
}

function processaCombosPopup(){
	selectAllOptions( document.getElementById( 'membrosequipe' ) );
	selectAllOptions( document.getElementById( 'equipesistema' ) );	
}

</script>
</body>